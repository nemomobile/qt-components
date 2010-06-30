/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QDECLARATIVERUNTIME_P_H
#define QDECLARATIVERUNTIME_P_H

#include <qobject.h>
#include "deviceorientation_p.h"


class QWindowObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isActiveWindow READ isActiveWindow NOTIFY isActiveWindowChanged)
    Q_PROPERTY(DeviceOrientation::Orientation orientation READ orientation NOTIFY orientationChanged)

public:
    static QWindowObject* instance()
    {
        static QWindowObject *instance = 0;
        if (!instance)
            instance = new QWindowObject;
        return instance;
    }

    bool isActiveWindow() const { return activeWindow; }
    void setActiveWindow(bool active)
    {
        if (active == activeWindow)
            return;
        activeWindow = active;
        emit isActiveWindowChanged();
    }

    DeviceOrientation::Orientation orientation() const { return DeviceOrientation::instance()->orientation(); }

Q_SIGNALS:
    void isActiveWindowChanged();
    void orientationChanged();

private:
    QWindowObject(QObject *parent=0) : QObject(parent), activeWindow(false)
    {
        connect(DeviceOrientation::instance(), SIGNAL(orientationChanged()),
                this, SIGNAL(orientationChanged()));
    }

    bool activeWindow;
};

#endif
