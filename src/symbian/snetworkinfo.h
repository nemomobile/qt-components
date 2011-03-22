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

#ifndef SNETWORKINFO_H
#define SNETWORKINFO_H

#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarativeitem.h>

class SNetworkInfoPrivate;

class SNetworkInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(NetworkMode networkMode READ networkMode NOTIFY networkModeChanged)
    Q_PROPERTY(NetworkStatus networkStatus READ networkStatus NOTIFY networkStatusChanged)
    Q_PROPERTY(int networkSignalStrength READ networkSignalStrength NOTIFY networkSignalStrengthChanged)

    Q_ENUMS(NetworkMode)
    Q_ENUMS(NetworkStatus)

public:
    explicit SNetworkInfo(QObject *parent = 0);

    // Direct match to QSystemNetworkInfo::NetworkMode
    enum NetworkMode {
        UnknownMode=0,
        GsmMode,
        CdmaMode,
        WcdmaMode,
        WlanMode,
        EthernetMode,
        BluetoothMode,
        WimaxMode
    };

    // Direct match to QSystemNetworkInfo::NetworkStatus
    enum NetworkStatus {
        UndefinedStatus = 0,
        NoNetworkAvailable,
        EmergencyOnly,
        Searching,
        Busy,
        Connected,
        HomeNetwork,
        Denied,
        Roaming
    };

    NetworkMode networkMode() const;
    NetworkStatus networkStatus() const;
    int networkSignalStrength() const;

Q_SIGNALS:
    void networkModeChanged(NetworkMode mode);
    void networkStatusChanged(NetworkMode mode, NetworkStatus status);
    void networkSignalStrengthChanged(NetworkMode mode, int strength);

protected:
    QScopedPointer<SNetworkInfoPrivate> d_ptr;

private:
#ifdef HAVE_MOBILITY
    Q_PRIVATE_SLOT(d_func(), void _q_networkModeChanged(QSystemNetworkInfo::NetworkMode))
    Q_PRIVATE_SLOT(d_func(), void _q_networkStatusChanged(QSystemNetworkInfo::NetworkMode, QSystemNetworkInfo::NetworkStatus))
    Q_PRIVATE_SLOT(d_func(), void _q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int))
#endif

    Q_DISABLE_COPY(SNetworkInfo)
    Q_DECLARE_PRIVATE(SNetworkInfo)
};

QML_DECLARE_TYPE(SNetworkInfo)

#endif // SNETWORKINFO_H
