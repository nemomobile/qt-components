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

#ifndef SBATTERYINFO_H
#define SBATTERYINFO_H

#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarativeitem.h>

class SBatteryInfoPrivate;

class SBatteryInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(BatteryStatus batteryStatus READ batteryStatus NOTIFY batteryStatusChanged)
    Q_PROPERTY(PowerState powerState READ powerState NOTIFY powerStateChanged)

    Q_ENUMS(BatteryStatus)
    Q_ENUMS(PowerState)

public:
    explicit SBatteryInfo(QObject *parent = 0);

    // Direct match to QSystemDeviceInfo::BatteryStatus
    enum BatteryStatus {
        NoBatteryLevel = 0,
        BatteryCritical,
        BatteryVeryLow,
        BatteryLow,
        BatteryNormal
    };

    // Direct match to QSystemDeviceInfo::PowerState
    enum PowerState {
        UnknownPower = 0,
        BatteryPower,
        WallPower,
        WallPowerChargingBattery
    };

    int batteryLevel() const;
    BatteryStatus batteryStatus() const;
    PowerState powerState() const;

Q_SIGNALS:
    void batteryLevelChanged(int level);
    void batteryStatusChanged(BatteryStatus status);
    void powerStateChanged(PowerState state);

protected:
    QScopedPointer<SBatteryInfoPrivate> d_ptr;

private:
#ifdef HAVE_MOBILITY
    Q_PRIVATE_SLOT(d_func(), void _q_batteryLevelChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_batteryStatusChanged(QSystemDeviceInfo::BatteryStatus))
    Q_PRIVATE_SLOT(d_func(), void _q_powerStateChanged(QSystemDeviceInfo::PowerState))
#endif

    Q_DISABLE_COPY(SBatteryInfo)
    Q_DECLARE_PRIVATE(SBatteryInfo)
};

QML_DECLARE_TYPE(SBatteryInfo)

#endif // SBATTERYINFO_H
