/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include "sdeclarativeindicatordatahandler.h"
#include "sdeclarativestatuspanedatasubscriber.h"
#include "sdeclarativeindicatordata.h"
#include "sdeclarativeindicator.h"
#include "sdeclarativeindicatorcontainer.h"

#include <avkon.hrh> // EAknIndicatorStateOff
#include <avkon.rsg> // R_AVKON_STATUS_PANE_INDICATOR_DEFAULT
#include <barsread.h> // TResourceReader
#include <coemain.h> // CCoeEnv

CSDeclarativeIndicatorDataHandler* CSDeclarativeIndicatorDataHandler::NewL(
    SDeclarativeIndicatorContainer* aContainer )
    {
    CSDeclarativeIndicatorDataHandler* self = new CSDeclarativeIndicatorDataHandler( aContainer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CSDeclarativeIndicatorDataHandler::ConstructL()
    {
    // subsciber reads the status pane data
    iSubscriber = CSDeclarativeStatusPaneSubscriber::NewL( *this );

    TCallBack callback( InitializeCallBack, this );
    iInitializer = new (ELeave) CAsyncCallBack( callback, CActive::EPriorityLow );
    iInitializer->CallBack(); // shoot
    }

CSDeclarativeIndicatorDataHandler::~CSDeclarativeIndicatorDataHandler()
    {
    delete iInitializer;
    delete iSubscriber;

    while (iIndicatorItems.count())
        delete iIndicatorItems.takeLast();

    while (iIndicatorsData.count())
        delete iIndicatorsData.take( iIndicatorsData.keys().at(0) );

    }

CSDeclarativeIndicatorDataHandler::CSDeclarativeIndicatorDataHandler(
    SDeclarativeIndicatorContainer* aContainer )
: iContainer( aContainer )
    {
    }

TInt CSDeclarativeIndicatorDataHandler::InitializeCallBack( TAny* aAny )
    {
    CSDeclarativeIndicatorDataHandler* thisPtr = static_cast<CSDeclarativeIndicatorDataHandler*>( aAny );
    if ( !thisPtr )
        {
        return KErrGeneral;
        }

    if ( !thisPtr->iIndicatorItems.count() )
        {
        TBool needsUpdating = EFalse;
        const TAknIndicatorState& indicatorState = thisPtr->iSubscriber->IndicatorState();
        for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators && !needsUpdating; ++i )
            {
            if ( indicatorState.visibleIndicators[i] != KIndicatorEmptySpot )
                {
                needsUpdating = ETrue;
                }
            }

        if ( needsUpdating )
            {
            thisPtr->UpdateIndicators();
            }
        }

    delete thisPtr->iInitializer;
    thisPtr->iInitializer = NULL;
    return KErrNone;
    }

void CSDeclarativeIndicatorDataHandler::StatusPaneStateChanged( TStatusPaneChangeFlags aChangeFlags )
    {
    if ( aChangeFlags&MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneIndicatorsState )
        {
        UpdateIndicators();
        }
    }

void CSDeclarativeIndicatorDataHandler::LoadIndicatorDataL()
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_AVKON_STATUS_PANE_INDICATOR_DEFAULT );

    TInt count = reader.ReadInt16();
    for ( TInt i = 0; i < count; ++i )
        {
        CSDeclarativeIndicatorData* indicatorData = CSDeclarativeIndicatorData::NewL( reader );
        iIndicatorsData.insert( indicatorData->Uid(), indicatorData );
        }

    CleanupStack::PopAndDestroy( 1 ); // Resource reader
    }

CSDeclarativeIndicatorData *CSDeclarativeIndicatorDataHandler::Data( TInt aUid ) const
    {
    if ( !iIndicatorsData.count() )
        {
        TRAP_IGNORE(const_cast<CSDeclarativeIndicatorDataHandler*>(this)->LoadIndicatorDataL());
        }

    if ( iIndicatorsData.contains( aUid ) )
        {
        return iIndicatorsData.value( aUid );
        }

    return 0;
    }

SDeclarativeIndicator *CSDeclarativeIndicatorDataHandler::Indicator( TInt aUid ) const
    {
    foreach ( SDeclarativeIndicator *indicator, iIndicatorItems )
        {
        if ( indicator->uid() == aUid )
            {
            return indicator;
            }
        }
    return 0;
    }

void CSDeclarativeIndicatorDataHandler::UpdateIndicators()
    {
    const TAknIndicatorState& indicatorState = iSubscriber->IndicatorState();
    bool changed = false;
    QList<SDeclarativeIndicator*> newIndicatorItems;

    // go through the visible ones
    for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators; ++i )
        {
        const TInt visibleUid = indicatorState.visibleIndicators[i];
        if ( visibleUid != KIndicatorEmptySpot )
            {
            // check if there is an existing istance
            SDeclarativeIndicator* indicator = Indicator( visibleUid );
            if ( indicator )
                {
                // remove from old list
                iIndicatorItems.removeOne( indicator );
                indicator->setParentItem( 0 ); // temporarily remove so we can re-arrange
                }
            else
                {
                // create new one
                CSDeclarativeIndicatorData *data = Data( visibleUid );
                indicator = new SDeclarativeIndicator( data, 0 );
                indicator->setColor( iContainer->indicatorColor() );
                QObject::connect( iContainer, SIGNAL(indicatorColorChanged(const QColor&)),
                                  indicator, SLOT(setColor(const QColor&)) );
                changed = true;
                }
            newIndicatorItems.append( indicator );
            changed |= indicator->setState( indicatorState.visibleIndicatorStates[i] );
            }
        }

    // delete old list
    while( iIndicatorItems.count() )
        {
        SDeclarativeIndicator *deletedIndicator = iIndicatorItems.at( iIndicatorItems.count() - 1 );
        deletedIndicator->setState( EAknIndicatorStateOff );
        iIndicatorItems.removeLast();
        delete deletedIndicator;
        changed = true;
        }

    // take the new one into use
    iIndicatorItems = newIndicatorItems;

    if ( changed )
        {
        PrioritizeIndicators();
        }

    // set parent in priority order
    foreach ( SDeclarativeIndicator *indicator, iIndicatorItems )
        {
        indicator->setParentItem( iContainer );
        }

    iContainer->layoutChildren();
    }

bool IndicatorPriorityLessThan( SDeclarativeIndicator *i1, SDeclarativeIndicator *i2 )
     {
     return i1->priority() < i2->priority();
     }

void CSDeclarativeIndicatorDataHandler::PrioritizeIndicators()
    {
    if ( iIndicatorItems.count() >= 2 )
        {
        qSort( iIndicatorItems.begin(), iIndicatorItems.end(), IndicatorPriorityLessThan );
        }
    }

//  End of File



