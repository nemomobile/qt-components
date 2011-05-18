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

#include "sdeclarativestatuspanedatasubscriber.h"

// #define Q_DEBUG_SUBSCRIBER

#ifdef Q_DEBUG_SUBSCRIBER
#include <QDebug>
#endif // Q_DEBUG_SUBSCRIBER

const TUint32 KAknStatusPaneSystemData = 0x10000000;
const TUid KPSUidAvkonInternal = { 0x10207218 };


CSDeclarativeStatusPaneSubscriber::CSDeclarativeStatusPaneSubscriber(
    MSDeclarativeStatusPaneSubscriberObverver& aObserver )
: CActive( CActive::EPriorityStandard ), iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    iInitialized = EFalse;

    // initialize the array to 'empty'
    for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators; ++i )
        {
        iIndicatorState.visibleIndicators[i] = KIndicatorEmptySpot;
        }
    }

CSDeclarativeStatusPaneSubscriber* CSDeclarativeStatusPaneSubscriber::NewL(
    MSDeclarativeStatusPaneSubscriberObverver& aObserver )
    {
    CSDeclarativeStatusPaneSubscriber* self =
        new ( ELeave ) CSDeclarativeStatusPaneSubscriber( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CSDeclarativeStatusPaneSubscriber::ConstructL()
    {
    User::LeaveIfError( iProperty.Attach( KPSUidAvkonInternal, KAknStatusPaneSystemData ) );
    DoSubscribe();
    }

CSDeclarativeStatusPaneSubscriber::~CSDeclarativeStatusPaneSubscriber()
    {
    Cancel();
    iProperty.Close();
    }

void CSDeclarativeStatusPaneSubscriber::EnsureData()
    {
    if ( !iInitialized )
        {
        TAknStatusPaneStateData::TAknStatusPaneStateDataPckg statusPaneStateDataPckg( iStateData );
        TInt err = iProperty.Get( KPSUidAvkonInternal, KAknStatusPaneSystemData, statusPaneStateDataPckg );
        if ( err == KErrNone )
            {
            iIndicatorState = iStateData.iIndicatorState;
            iSignalState = iStateData.iSignalState;
            iInitialized = ETrue;
            }
#ifdef Q_DEBUG_SUBSCRIBER
        else
            {
            qDebug() << "CSDeclarativeStatusPaneSubscriber::IndicatorState() cannot read status pane data";
            }
#endif // Q_DEBUG_SUBSCRIBER
        }
    }

TAknIndicatorState CSDeclarativeStatusPaneSubscriber::IndicatorState() const
    {
    const_cast<CSDeclarativeStatusPaneSubscriber*>(this)->EnsureData();
    return iIndicatorState;
    }

TAknSignalState CSDeclarativeStatusPaneSubscriber::SignalState() const
    {
    const_cast<CSDeclarativeStatusPaneSubscriber*>(this)->EnsureData();
    return iSignalState;
    }

void CSDeclarativeStatusPaneSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }


void CSDeclarativeStatusPaneSubscriber::DoSubscribe()
    {
    if ( !IsActive() )
        {
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    }

TBool IndicatorStatesEqual( const TAknIndicatorState& state1, const TAknIndicatorState& state2 )
    {
    TBool same = ETrue;
    for ( TInt i = 0; i < TAknIndicatorState::EMaxVisibleIndicators && same; ++i ) {
        if ( state1.visibleIndicators[i] != state2.visibleIndicators[i] )
            {
            same = EFalse;
            }
        else if ( state1.visibleIndicators[i] != KIndicatorEmptySpot )
            {
            if ( state1.visibleIndicatorStates[i] != state2.visibleIndicatorStates[i] )
                {
                same = EFalse;
                }
            }
        }
    return same;
    }

TBool SignalStatesEqual( const TAknSignalState& state1, const TAknSignalState& state2 )
    {
    return state1.iIconState == state2.iIconState && state1.iSignalStrength == state2.iSignalStrength;
    }

void CSDeclarativeStatusPaneSubscriber::RunL()
    {
    if ( iStatus != KErrNone )
        {
#ifdef Q_DEBUG_SUBSCRIBER
        qDebug() << "CSDeclarativeStatusPaneSubscriber::RunL error in status pane data change, iStatus: " << iStatus;
#endif // Q_DEBUG_SUBSCRIBER
        return;
        }

    // resubscribe before processing new value to prevent missing updates
    DoSubscribe();

    TAknStatusPaneStateData::TAknStatusPaneStateDataPckg statusPaneStateDataPckg( iStateData );
    iProperty.Get( KPSUidAvkonInternal, KAknStatusPaneSystemData, statusPaneStateDataPckg );

    int changeFlags = MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneUndefined;
    if ( !iInitialized || !IndicatorStatesEqual( iStateData.iIndicatorState, iIndicatorState ) )
        {
        iIndicatorState = iStateData.iIndicatorState;
        changeFlags += MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneIndicatorsState;
        }

    if ( !iInitialized || !SignalStatesEqual( iStateData.iSignalState, iSignalState ) )
        {
        iSignalState = iStateData.iSignalState;
        changeFlags |= MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneSignalState;
        }

    iInitialized = ETrue;

    if ( changeFlags != MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneUndefined ) {
        iObserver.StatusPaneStateChanged(
            MSDeclarativeStatusPaneSubscriberObverver::TStatusPaneChangeFlags( changeFlags ) );
        }
    }

//  End of File
