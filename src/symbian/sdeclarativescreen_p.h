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

#ifndef SDECLARATIVESCREEN_P_H
#define SDECLARATIVESCREEN_P_H

#include "sdeclarativescreen.h"
#include <QtCore/qpointer.h>
#include <QtGui/qgraphicsview.h>

QT_FORWARD_DECLARE_CLASS(QDeclarativeEngine)

class SDeclarativeScreenPrivate
{
    Q_DECLARE_PUBLIC(SDeclarativeScreen)
public:
    SDeclarativeScreenPrivate(SDeclarativeScreen *qq, QDeclarativeEngine *engine);
    ~SDeclarativeScreenPrivate();

    void updateOrientationAngle();
    void _q_initView(const QSize &);
    void _q_updateScreenSize(const QSize &);
    void _q_desktopResized(int);
    bool isLandscapeScreen() const;
    QSize currentScreenSize() const;
    QSize adjustedSize(const QSize &size) const;
    bool portraitAllowed() const;
    bool landscapeAllowed() const;

public:
    SDeclarativeScreen *q_ptr;
    SDeclarativeScreen::Orientation currentOrientation;
    SDeclarativeScreen::Orientations allowedOrientations;
    qreal dpi;
    QSize screenSize;  // "logical" screen
    QSize displaySize; // "physical" display
    bool settingDisplay;
    QPointer<QGraphicsView> gv;
    bool initCalled;
    bool initDone;
    QDeclarativeEngine *engine;

    static SDeclarativeScreenPrivate *d_ptr(SDeclarativeScreen *screen) {
        Q_ASSERT(screen);
        return screen->d_func();
    }
};

#endif // SDECLARATIVESCREEN_P_H
