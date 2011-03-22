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

#include "sbatteryinfo.h"

#ifdef HAVE_MOBILITY
#include <QSystemDeviceInfo>
using namespace QtMobility;
#endif

class SBatteryInfoPrivate
{
    Q_DECLARE_PUBLIC(SBatteryInfo)

public:
    SBatteryInfoPrivate(SBatteryInfo *qq) : q_ptr(qq) {}

#ifdef HAVE_MOBILITY
    void _q_batteryLevelChanged(int level);
    void _q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus status);
    void _q_powerStateChanged(QSystemDeviceInfo::PowerState state);

    QSystemDeviceInfo deviceInfo;
#endif

private:
    SBatteryInfo *q_ptr;
};

#ifdef HAVE_MOBILITY
void SBatteryInfoPrivate::_q_batteryLevelChanged(int level)
{
    qDebug() << "_q_batteryLevelChanged():" << level;
    emit q_ptr->batteryLevelChanged(level);
}

void SBatteryInfoPrivate::_q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus status)
{
    qDebug() << "_q_batteryStatusChanged():" << status;
    emit q_ptr->batteryStatusChanged(static_cast<SBatteryInfo::BatteryStatus>(status));
}

void SBatteryInfoPrivate::_q_powerStateChanged(QSystemDeviceInfo::PowerState state)
{
    qDebug() << "_q_powerStateChanged():" << state;
    emit q_ptr->powerStateChanged(static_cast<SBatteryInfo::PowerState>(state));
}
#endif

SBatteryInfo::SBatteryInfo(QObject *parent) :
    QObject(parent), d_ptr(new SBatteryInfoPrivate(this))
{
#ifdef HAVE_MOBILITY
    connect(&d_ptr->deviceInfo, SIGNAL(batteryLevelChanged(int)),
            this, SLOT(_q_batteryLevelChanged(int)));
    connect(&d_ptr->deviceInfo, SIGNAL(batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)),
            this, SLOT(_q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus)));
    connect(&d_ptr->deviceInfo, SIGNAL(powerStateChanged(QSystemDeviceInfo::PowerState)),
            this, SLOT(_q_powerStateChanged(QSystemDeviceInfo::PowerState)));
#endif
}

int SBatteryInfo::batteryLevel() const
{
#ifdef HAVE_MOBILITY
    return d_ptr->deviceInfo.batteryLevel();
#else
    return 0;
#endif
}

SBatteryInfo::BatteryStatus SBatteryInfo::batteryStatus() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SBatteryInfo::BatteryStatus>(d_ptr->deviceInfo.batteryStatus());
#else
    return NoBatteryLevel;
#endif
}

SBatteryInfo::PowerState SBatteryInfo::powerState() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SBatteryInfo::PowerState>(d_ptr->deviceInfo.currentPowerState());
#else
    return UnknownPower;
#endif
}

#include "moc_sbatteryinfo.cpp"
