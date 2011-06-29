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

#include "sbatteryinfo.h"

#include "indicators/sdeclarativestatuspanedatasubscriber.h"
#include <avkon.hrh>
#include <aknstatuspanedata.h>

class SBatteryInfoPrivate : public MSDeclarativeStatusPaneSubscriberObverver
{
    Q_DECLARE_PUBLIC(SBatteryInfo)

public:
    SBatteryInfoPrivate(SBatteryInfo *qq);
    ~SBatteryInfoPrivate();

    void StatusPaneStateChanged(TStatusPaneChangeFlags aChangeFlags);

    CSDeclarativeStatusPaneSubscriber *subscriber;
    bool powerSaveModeEnabled;
    bool charging;
    int batteryLevel;

private:
    SBatteryInfo *q_ptr;
};

SBatteryInfoPrivate::SBatteryInfoPrivate(SBatteryInfo *qq)
     : powerSaveModeEnabled(false), charging(false), batteryLevel(0), q_ptr(qq)
{
    QT_TRAP_THROWING(subscriber = CSDeclarativeStatusPaneSubscriber::NewL(*this));
    // Initialize property values
    const TAknBatteryState &batteryState = subscriber->BatteryState();
    powerSaveModeEnabled = (batteryState.iIconState != EAknBatteryIndicatorNormal);
    charging = batteryState.iRecharging;
    batteryLevel = batteryState.iBatteryStrength;
}

SBatteryInfoPrivate::~SBatteryInfoPrivate()
{
    delete subscriber;
}

void SBatteryInfoPrivate::StatusPaneStateChanged(TStatusPaneChangeFlags aChangeFlags)
{
    if (aChangeFlags & MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneBatteryState) {
        const TAknBatteryState &batteryState = subscriber->BatteryState();
        bool newPowerSaveModeEnabled(batteryState.iIconState != EAknBatteryIndicatorNormal);

        if (powerSaveModeEnabled != newPowerSaveModeEnabled) {
            powerSaveModeEnabled = newPowerSaveModeEnabled;
            emit q_ptr->powerSaveModeEnabledChanged(powerSaveModeEnabled);
        }
        if (charging != batteryState.iRecharging) {
            charging = batteryState.iRecharging;
            emit q_ptr->chargingChanged(charging);
        }
        if (batteryLevel != batteryState.iBatteryStrength) {
            batteryLevel = batteryState.iBatteryStrength;
            emit q_ptr->chargingChanged(batteryLevel);
        }
    }
}

SBatteryInfo::SBatteryInfo(QObject *parent) :
    QObject(parent), d_ptr(new SBatteryInfoPrivate(this))
{
}

int SBatteryInfo::batteryLevel() const
{
    return d_ptr->batteryLevel;
}

bool SBatteryInfo::charging() const
{
    return d_ptr->charging;
}

bool SBatteryInfo::powerSaveModeEnabled() const
{
    return d_ptr->powerSaveModeEnabled;
}

#include "moc_sbatteryinfo.cpp"
