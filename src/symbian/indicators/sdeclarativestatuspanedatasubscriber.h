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

#ifndef CSDECLARATIVESTATUSPANESUBSCRIBER_H
#define CSDECLARATIVESTATUSPANESUBSCRIBER_H

// SYMBIAN HEADERS
#include <e32base.h>
#include <e32property.h>
#include <aknstatuspanedata.h>

const TInt KIndicatorEmptySpot = 0xFFFFFFFF;

class MSDeclarativeStatusPaneSubscriberObverver
{
public:
    enum TStatusPaneChangeFlags {
        EStatusPaneUndefined        = 0x00,
        EStatusPaneIndicatorsState  = 0x01,
        EStatusPaneSignalState      = 0x02
    };
    virtual void StatusPaneStateChanged( TStatusPaneChangeFlags aChangeFlags ) = 0;
};

class CSDeclarativeStatusPaneSubscriber : public CActive
{
public:
    static CSDeclarativeStatusPaneSubscriber* NewL( MSDeclarativeStatusPaneSubscriberObverver& aObserver );
    virtual ~CSDeclarativeStatusPaneSubscriber();

    TAknIndicatorState IndicatorState() const;
    TAknSignalState SignalState() const;

private:
    CSDeclarativeStatusPaneSubscriber( MSDeclarativeStatusPaneSubscriberObverver& aObserver );
    void ConstructL();

    void DoSubscribe();
    void EnsureData();

    void RunL();
    void DoCancel();

private:
    RProperty iProperty;
    TAknStatusPaneStateData iStateData;
    MSDeclarativeStatusPaneSubscriberObverver& iObserver;
    TAknIndicatorState iIndicatorState;
    TAknSignalState iSignalState;
    TBool iInitialized;
};


#endif // CSDECLARATIVESTATUSPANESUBSCRIBER_H
