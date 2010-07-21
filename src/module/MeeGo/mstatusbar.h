/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef MDeclarativeStatusBar_H
#define MDeclarativeStatusBar_H

#include <qdeclarativeitem.h>
#include <qglobal.h>

class QPixmap;
class QDBusServiceWatcher;
class QDBusPendingCallWatcher;

#define HAVE_DBUS
#define HAVE_XDAMAGE

class MDeclarativeStatusBar : public QDeclarativeItem
{
    Q_OBJECT

public:
    MDeclarativeStatusBar(QDeclarativeItem *parent = 0);
    virtual ~MDeclarativeStatusBar();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    Q_DISABLE_COPY(MDeclarativeStatusBar)

    bool updatesEnabled;
    /*!
     * Displays the status indicator menu.
     */
    void showStatusIndicatorMenu();

    // Perform the haptic feedback
    void playHapticsFeedback();

    //! position of mouse button press(firstPos) and position of last point of mouse move(lastPos)
    QPointF firstPos, lastPos;

    void updateSharedPixmap();
    bool isPixmapProviderOnline;
    QDBusServiceWatcher *dbusWatcher;
    void querySharedPixmapFromProvider();
    void setupXDamageForSharedPixmap();
    void destroyXDamageForSharedPixmap();

    QPixmap sharedPixmap;
    Qt::HANDLE pixmapDamage;

public Q_SLOTS:
    void handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                 unsigned short &width, unsigned short &height);

    void enablePixmapUpdates();
    void disablePixmapUpdates();
    void sharedPixmapHandleReceived(QDBusPendingCallWatcher * call);
    void handlePixmapProviderOnline();
    void handlePixmapProviderOffline();
};

#endif
