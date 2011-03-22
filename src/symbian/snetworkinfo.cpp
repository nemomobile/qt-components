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

#include "snetworkinfo.h"

#ifdef HAVE_MOBILITY
#include <QSystemNetworkInfo>
using namespace QtMobility;
#endif

class SNetworkInfoPrivate
{
    Q_DECLARE_PUBLIC(SNetworkInfo)

public:
    SNetworkInfoPrivate(SNetworkInfo *qq) : q_ptr(qq) {}

#ifdef HAVE_MOBILITY
    void _q_networkModeChanged(QSystemNetworkInfo::NetworkMode mode);
    void _q_networkStatusChanged(QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status);
    void _q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode mode, int strength);

    QSystemNetworkInfo networkInfo;
#endif

private:
    SNetworkInfo *q_ptr;
};

#ifdef HAVE_MOBILITY
void SNetworkInfoPrivate::_q_networkModeChanged(QSystemNetworkInfo::NetworkMode mode)
{
    emit q_ptr->networkModeChanged(static_cast<SNetworkInfo::NetworkMode>(mode));
}

void SNetworkInfoPrivate::_q_networkStatusChanged(QSystemNetworkInfo::NetworkMode mode, QSystemNetworkInfo::NetworkStatus status)
{
    emit q_ptr->networkStatusChanged(static_cast<SNetworkInfo::NetworkMode>(mode), static_cast<SNetworkInfo::NetworkStatus>(status));
}

void SNetworkInfoPrivate::_q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode mode, int strength)
{
    emit q_ptr->networkSignalStrengthChanged(static_cast<SNetworkInfo::NetworkMode>(mode), strength);
}
#endif

SNetworkInfo::SNetworkInfo(QObject *parent) :
    QObject(parent), d_ptr(new SNetworkInfoPrivate(this))
{
#ifdef HAVE_MOBILITY
    connect(&d_ptr->networkInfo, SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)),
            this, SLOT(_q_networkModeChanged(QSystemNetworkInfo::NetworkMode)));
    connect(&d_ptr->networkInfo, SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode, QSystemNetworkInfo::NetworkStatus)),
            this, SLOT(_q_networkStatusChanged(QSystemNetworkInfo::NetworkMode, QSystemNetworkInfo::NetworkStatus)));
    connect(&d_ptr->networkInfo, SIGNAL(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)),
            this, SLOT(_q_networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)));
#endif
}

SNetworkInfo::NetworkMode SNetworkInfo::networkMode() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SNetworkInfo::NetworkMode>(d_ptr->networkInfo.currentMode());
#else
    return UnknownMode;
#endif
}

SNetworkInfo::NetworkStatus SNetworkInfo::networkStatus() const
{
#ifdef HAVE_MOBILITY
    return static_cast<SNetworkInfo::NetworkStatus>(d_ptr->networkInfo.networkStatus(d_ptr->networkInfo.currentMode()));
#else
    return UndefinedStatus;
#endif
}

int SNetworkInfo::networkSignalStrength() const
{
#ifdef HAVE_MOBILITY
    return QSystemNetworkInfo::networkSignalStrength(d_ptr->networkInfo.currentMode());
#else
    return 0;
#endif
}

#include "moc_snetworkinfo.cpp"
