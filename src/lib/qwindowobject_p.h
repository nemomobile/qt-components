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

#ifndef QWINDOWOBJECT_P_H
#define QWINDOWOBJECT_P_H

#include <qobject.h>
class QDeclarativeWindow;

class QWindowObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isActiveWindow READ isActiveWindow NOTIFY isActiveWindowChanged)
    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(bool orientationLocked READ isOrientationLocked WRITE setOrientationLocked NOTIFY orientationLockedChanged)

    Q_ENUMS(Orientation)

public:
    QWindowObject(QDeclarativeWindow *parent);

    bool isActiveWindow() const { return activeWindow; }
    void setActiveWindow(bool active)
    {
        if (active == activeWindow)
            return;
        activeWindow = active;
        emit isActiveWindowChanged();
    }

    enum Orientation {
        UnknownOrientation,
        Portrait,
        Landscape,
        PortraitInverted,
        LandscapeInverted
    };
    Orientation orientation() const;
    void setOrientation(Orientation orientation);

    bool isOrientationLocked() const;
    void setOrientationLocked(bool locked);

Q_SIGNALS:
    void isActiveWindowChanged();
    void orientationChanged();
    void orientationLockedChanged();

private:
    bool activeWindow;
    QDeclarativeWindow *window;
};

#endif
