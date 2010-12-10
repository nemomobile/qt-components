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

#ifndef MDeclarativeStatusBar_H
#define MDeclarativeStatusBar_H

#include <qdeclarativeitem.h>
#include <mdeclarativescreen.h>
#include <qglobal.h>

class QPixmap;
class QDBusServiceWatcher;
class QDBusPendingCallWatcher;

#ifndef Q_WS_MAC
   #define HAVE_DBUS
   #define HAVE_XDAMAGE
#endif

class MDeclarativeStatusBar : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(MDeclarativeScreen::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

public:
    MDeclarativeStatusBar(QDeclarativeItem *parent = 0);
    virtual ~MDeclarativeStatusBar();

    void setOrientation(MDeclarativeScreen::Orientation o);
    MDeclarativeScreen::Orientation orientation() const;

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
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

    MDeclarativeScreen::Orientation mOrientation;

Q_SIGNALS:
    void orientationChanged();

public Q_SLOTS:
    void enablePixmapUpdates();
    void disablePixmapUpdates();
    void sharedPixmapHandleReceived(QDBusPendingCallWatcher * call);
    void handlePixmapProviderOnline();
    void handlePixmapProviderOffline();
};

QML_DECLARE_TYPE(MDeclarativeStatusBar)
#endif
