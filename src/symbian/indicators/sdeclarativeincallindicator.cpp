/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "sdeclarativeincallindicator.h"

#include <AknIndicatorContainer.h>
#include <avkon.hrh>
#include <coecntrl.h>
#include <AknUtils.h>
#include <aknappui.h>
#include <layoutmetadata.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QApplication>
#include "sdeclarativescreen.h"

// #define Q_DEBUG_INCALL

#ifdef Q_DEBUG_INCALL
#include <QDebug>
#endif // Q_DEBUG_INCALL

_LIT( KGLOBALSEARCHMUTEXNAME, "IndicatorMutex*" );
_LIT( KGLOBALMUTEXNAME, "IndicatorMutex" );

class TSDeclarativeIncallIndicatorPrivate : public QObject
{
Q_OBJECT
public:
    TSDeclarativeIncallIndicatorPrivate(CSDeclarativeIncallIndicator* aIncallIndicator);
    ~TSDeclarativeIncallIndicatorPrivate() {}

public slots:
    void orientationChanged();
public:
    CSDeclarativeIncallIndicator* iIncallIndicator;
    CAknIndicatorContainer* iControl;
    CCoeControl* iIncallBubble;
    TInt iFlags;
    TBool iIsForeground;
    SDeclarativeScreen* iScreen;
};

TSDeclarativeIncallIndicatorPrivate::TSDeclarativeIncallIndicatorPrivate(CSDeclarativeIncallIndicator *aIncallIndicator)
: QObject(0), iIncallIndicator(aIncallIndicator), iControl(0), iIncallBubble(0), iFlags(0), iIsForeground(ETrue), iScreen(0)
{
    // Find screen context property
    QDeclarativeView *view = 0;
    const QWidgetList &widgets = QApplication::topLevelWidgets();
    for (int i = 0; i < widgets.count() && !view; i++) {
        QDeclarativeView *declarativeView = qobject_cast<QDeclarativeView *>(widgets.at(i));
        if (declarativeView && declarativeView->rootContext()->contextProperty("screen").isValid())
            view = declarativeView;
    }

    if (view) {
        QObject *screen = view->rootContext()->contextProperty("screen").value<QObject*>();
        iScreen = qobject_cast<SDeclarativeScreen*>(screen);
        QObject::connect(iScreen, SIGNAL(currentOrientationChanged()), this, SLOT(orientationChanged()));
    }
}

void TSDeclarativeIncallIndicatorPrivate::orientationChanged()
{
    if ( iIncallIndicator )
        iIncallIndicator->OrientationChanged();
}

// CSDeclarativeIncallIndicator
CSDeclarativeIncallIndicator* CSDeclarativeIncallIndicator::NewL()
    {
    CSDeclarativeIncallIndicator* self = new ( ELeave ) CSDeclarativeIncallIndicator();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CSDeclarativeIncallIndicator::CSDeclarativeIncallIndicator()
    : iMessageMonitorObserverAdded( EFalse )
    {
    }

void CSDeclarativeIncallIndicator::ConstructL()
    {
    iData.reset( new ( ELeave ) TSDeclarativeIncallIndicatorPrivate( this ) );
    User::LeaveIfNull( iData->iScreen );

    iCoeEnv = CCoeEnv::Static();
    User::LeaveIfNull( iCoeEnv );

    TBuf<256> mutexName;
    mutexName.Zero();
    TFindMutex findMutex( KGLOBALSEARCHMUTEXNAME );
    TInt error = findMutex.Next( mutexName );
#ifdef Q_DEBUG_INCALL
    qDebug() << "SDeclarativeIncallIndicator::ConstructL()";
    qDebug() << "Find mutex errorCode" << error;
#endif
    if ( error == KErrNotFound )
        {
        User::LeaveIfError( iMutex.CreateGlobal( KGLOBALMUTEXNAME ) );
        }
    else
        {
        User::LeaveIfError( iMutex.OpenGlobal( mutexName ) );
        }
    }

CSDeclarativeIncallIndicator::~CSDeclarativeIncallIndicator()
    {
    iCoeEnv->RemoveMessageMonitorObserver( *this );
    iMutex.Close();
    }

void CSDeclarativeIncallIndicator::CreateIncallControlL()
    {
    if (!iData->iControl)
        {
        // Creates CAknIndicatorContainer which internally creates the CIncallStatusBubble
        CAknIndicatorContainer* control = new ( ELeave ) CAknIndicatorContainer();
        CleanupStack::PushL( control );
        control->ConstructL();
        control->ActivateL();
        CleanupStack::Pop( control );
        iData->iControl = control;
        }

#ifdef Q_DEBUG_INCALL
    if (!iData->iControl)
        {
        qDebug() << "CSDeclarativeIncallIndicator::CreateIncallControlL() Cannot create CAknIndicatorContainer!";
        }
#endif
    }


CCoeControl* CSDeclarativeIncallIndicator::GetInCallBubbleL()
    {
    if(!iData->iControl)
        {
        return 0;
        }

    // When setting the visible flag on for the first time, the CIncallStatusBubble
    // instance in created. Internally it creates new window group and window.
    RWsSession& wsSession = iCoeEnv->WsSession();

    CArrayFixFlat<TInt>* oldWindowGroups = new ( ELeave ) CArrayFixFlat<TInt>( 2 );
    CleanupStack::PushL( oldWindowGroups );
    CArrayFixFlat<TInt>* newWindowGroups = new ( ELeave ) CArrayFixFlat<TInt>( 2 );
    CleanupStack::PushL( newWindowGroups );

    User::LeaveIfError( wsSession.WindowGroupList( oldWindowGroups ) );
    iData->iControl->SetIncallBubbleFlags( iData->iFlags|EAknStatusBubbleVisible );
    User::LeaveIfError( wsSession.WindowGroupList( newWindowGroups ) );

    CCoeControl *inCallBubble = 0;

    if ( newWindowGroups->Count() - 1 == oldWindowGroups->Count() )
        {
        // find the one that does not match - that is the new window group
        for (TInt newIndex = 0; newIndex < newWindowGroups->Count(); newIndex++)
            {
            const TInt id = newWindowGroups->At( newIndex );
            TBool found = EFalse;
            for (TInt oldIndex = 0; oldIndex < oldWindowGroups->Count() && !found; oldIndex++)
                {
                if (oldWindowGroups->At( oldIndex ) == id)
                    {
                    found = ETrue;
                    }
                }

            if (!found)
                {
                // found a window group which was created within the SetIncallBubbleFlags function
                // call
#ifdef Q_DEBUG_INCALL
                qDebug() << "CSDeclarativeIncallIndicator::CreateIncallControlL() found new wg id " << (int)id;
#endif
                RWindowGroup* wg = reinterpret_cast<RWindowGroup*>( wsSession.GetWindowGroupHandle( id ) );
                if ( wg && wg->Child() )
                    {
                    TUint32 child = wg->Child();
                    inCallBubble = reinterpret_cast<CCoeControl*>( child ) ;
#ifdef Q_DEBUG_INCALL
                    qDebug() << "CSDeclarativeIncallIndicator::CreateIncallControlL() found iIncallBubble " << (int)iData->iIncallBubble;
#endif
                    break;
                    }
                }
            }
        }
    else
        {
#ifdef Q_DEBUG_INCALL
        qDebug() << "CSDeclarativeIncallIndicator::CreateIncallControlL() mismatch old" << (int)oldWindowGroups->Count() << " new " << (int)newWindowGroups->Count();
#endif
        }
    CleanupStack::PopAndDestroy( newWindowGroups );
    CleanupStack::PopAndDestroy( oldWindowGroups );

    return inCallBubble;

    }

void CSDeclarativeIncallIndicator::MonitorWsMessage( const TWsEvent& aEvent )
    {
    switch ( aEvent.Type() )
        {
        case KAknFullOrPartialForegroundGained:
            iData->iIsForeground = ETrue;
            // Fetch bubble and make it visible in case it hasn't been because
            // it has been in background before.
            if( !iData->iIncallBubble )
                {
                iMutex.Wait();
                TRAP_IGNORE( iData->iIncallBubble = GetInCallBubbleL() );
                iMutex.Signal();
                }
            UpdateIncallBubbleVisibility();
            break;

        case KAknFullOrPartialForegroundLost:
            iData->iIsForeground = EFalse;
            UpdateIncallBubbleVisibility();
            break;

        case EEventScreenDeviceChanged: // flow through
        case KEikDynamicLayoutVariantSwitch: // flow through
        case EEventWindowVisibilityChanged:
#ifdef Q_DEBUG_INCALL
            qDebug() << "CSDeclarativeIncallIndicator::MonitorWsMessage() Avkon switch...";
#endif
            IncallBubbleSizeChanged();
            break;

        default:
            break;
        }
    }

void CSDeclarativeIncallIndicator::OrientationChanged()
    {
#ifdef Q_DEBUG_INCALL
    qDebug() << "CSDeclarativeIncallIndicator::OrientationChanged() QML switch...";
#endif
    IncallBubbleSizeChanged();
    }

void CSDeclarativeIncallIndicator::SetFlags( TInt aFlags )
    {
#ifdef Q_DEBUG_INCALL
    qDebug() << "CSDeclarativeIncallIndicator::SetFlags called by = " << this;
#endif

    iData->iIsForeground = static_cast<CAknAppUi*>( iCoeEnv->AppUi() )->IsForeground();
    iMutex.Wait();

#ifdef Q_DEBUG_INCALL
    qDebug() << "CSDeclarativeIncallIndicator::SetFlags() Locked Mutex ";
    qDebug() << "CSDeclarativeIncallIndicator::SetFlags() flags " << (int)aFlags;
#endif

    if ( aFlags != iData->iFlags )
        {
        iData->iFlags = aFlags;

        // Create controls and observer if bubble should be visible
        if ( !iData->iIncallBubble && (aFlags&EAknStatusBubbleVisible))
            {
            TRAP_IGNORE( CreateIncallControlL() );

            if( iData->iIsForeground )
                {
                TRAP_IGNORE( iData->iIncallBubble = GetInCallBubbleL() );
                }

            if( !iMessageMonitorObserverAdded )
                {
                TRAP_IGNORE( iCoeEnv->AddMessageMonitorObserverL( *this ) );
                iMessageMonitorObserverAdded = ETrue;
                }
            }

        if ( iData->iControl )
            {
            // Do not set visibility flag if bubble hasn't been found yet
            if( !iData->iIncallBubble )
                {
                aFlags = aFlags&(~EAknStatusBubbleVisible);
                }

            iData->iControl->SetIncallBubbleFlags( aFlags );

            // Set iIncallBubble to NULL if bubble shouldn't be visible
            if ( !(iData->iFlags&EAknStatusBubbleVisible) )
                {
#ifdef Q_DEBUG_INCALL
                qDebug() << "CSDeclarativeIncallIndicator::SetFlags() nulling iIncallBubble";
#endif
                iData->iIncallBubble = NULL;
                if (iMessageMonitorObserverAdded)
                    {
                    iCoeEnv->RemoveMessageMonitorObserver( *this );
                    iMessageMonitorObserverAdded = EFalse;
                    }
                }
            else
                {
                UpdateIncallBubbleVisibility();
                }
            }
        }
#ifdef Q_DEBUG_INCALL
    qDebug() << "CSDeclarativeIncallIndicator::SetFlags() Unlocked Mutex ";
#endif
    iMutex.Signal();
    }

void CSDeclarativeIncallIndicator::UpdateIncallBubbleVisibility()
    {
    if ( iData->iIncallBubble )
        {
        if ( (iData->iFlags&EAknStatusBubbleVisible) && iData->iIsForeground )
            {
#ifdef Q_DEBUG_INCALL
             qDebug() << "CSDeclarativeIncallIndicator::UpdateIncallBubbleVisibility() visible with new size";
#endif
            IncallBubbleSizeChanged();
            iData->iIncallBubble->MakeVisible( ETrue );
            }
        else
            {
#ifdef Q_DEBUG_INCALL
             qDebug() << "CSDeclarativeIncallIndicator::UpdateIncallBubbleVisibility() invisible";
#endif
             iData->iIncallBubble->MakeVisible( EFalse );
            }
        }
    }

void CSDeclarativeIncallIndicator::IncallBubbleSizeChanged()
    {
    if ( iData->iIncallBubble )
        {
        TAknLayoutRect layoutRect;
        TAknWindowLineLayout layout;
        TRect screenRect( TPoint(), TSize( (TInt)iData->iScreen->width(), (TInt)iData->iScreen->height() ) );
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
            {
            layout =
                AknLayoutScalable_Apps::popup_call_status_window( 6 ).LayoutLine();

            if ( AknLayoutUtils::LayoutMirrored() )
                {
                TInt tmp = layout.il;
                layout.il = layout.ir;
                layout.ir = tmp;
                }

            TAknWindowComponentLayout topLayout =
                AknLayoutScalable_Apps::area_top_pane( 2 );

            layoutRect.LayoutRect( screenRect, topLayout );
            layoutRect.LayoutRect( layoutRect.Rect(), layout );
            }
        else
            {
            layout =
                AknLayoutScalable_Apps::popup_call_status_window( 0 ).LayoutLine();
            layoutRect.LayoutRect( screenRect, layout );
            }

        // SizeChanged of incall indicator is heavyweight, so set size only if
        // necessary.
        TRect rect( layoutRect.Rect() );
        if ( rect != TRect( iData->iIncallBubble->Position(), iData->iIncallBubble->Size() ) )
            {
            iData->iIncallBubble->SetRect( rect );
            }
        }
    }

#include "sdeclarativeincallindicator.moc"


