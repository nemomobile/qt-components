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

#include "mdeclarativestatusbar.h"

#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QGraphicsSceneMouseEvent>
#include <QCoreApplication>
#include <qpainter.h>
#include <qx11info_x11.h>
#include <qgraphicsscene.h>
#include <qdebug.h>

#ifdef HAVE_DBUS
   #include <QDBusInterface>
   #include <QDBusServiceWatcher>
   #include <QDBusConnectionInterface>
#endif

#ifdef HAVE_XDAMAGE
   #include <X11/extensions/Xdamage.h>
#endif

static const int STATUSBAR_HEIGHT = 30;

#ifdef HAVE_DBUS
static const QLatin1String PIXMAP_PROVIDER_DBUS_SERVICE("com.meego.core.MStatusBar");
static const QLatin1String PIXMAP_PROVIDER_DBUS_PATH("/statusbar");
static const QLatin1String PIXMAP_PROVIDER_DBUS_INTERFACE("com.meego.core.MStatusBar");
static const QLatin1String PIXMAP_PROVIDER_DBUS_SHAREDPIXMAP_CALL("sharedPixmapHandle");

static const QLatin1String STATUS_INDICATOR_MENU_DBUS_SERVICE("com.meego.core.MStatusIndicatorMenu");
static const QLatin1String STATUS_INDICATOR_MENU_DBUS_PATH("/statusindicatormenu");
static const QLatin1String STATUS_INDICATOR_MENU_DBUS_INTERFACE("com.meego.core.MStatusIndicatorMenu");
#endif

#ifdef HAVE_XDAMAGE
// for XDamage events
static int xDamageEventBase = 0;
static int xDamageErrorBase = 0;
#endif

static bool filterRegistered = false;
QCoreApplication::EventFilter oldFilter = 0;
#ifdef HAVE_XDAMAGE
static QHash<Damage, MDeclarativeStatusBar *> damageMap;
#endif

#ifdef HAVE_XDAMAGE
static int handleXError(Display *, XErrorEvent *)
{
    return 0;
}
#endif

static bool x11EventFilter(void *message, long *)
{
#ifdef HAVE_XDAMAGE
    XEvent *event = (XEvent *)message;

    if (event->type == xDamageEventBase + XDamageNotify) {
        XDamageNotifyEvent *xevent = (XDamageNotifyEvent *) event;

        // It is possible that the Damage has already been destroyed so register an error handler to suppress X errors
        XErrorHandler errh = XSetErrorHandler(handleXError);
        XDamageSubtract(QX11Info::display(), xevent->damage, None, None);
        XSetErrorHandler(errh);

        // notify status bar
        MDeclarativeStatusBar *statusBar = damageMap.value(xevent->damage);
        if (statusBar) {
            statusBar->update();
            return true;
        }
    }
#endif
    return false;
}



MDeclarativeStatusBar::MDeclarativeStatusBar(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    updatesEnabled(true),
    mOrientation(MDeclarativeScreen::Portrait)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setAcceptedMouseButtons(Qt::LeftButton);
    // higher than TitleBar
    setZValue(1010);
    setImplicitHeight(STATUSBAR_HEIGHT);

    if (!filterRegistered) {
        ::oldFilter = QCoreApplication::instance()->setEventFilter(x11EventFilter);
#ifdef HAVE_XDAMAGE
        XDamageQueryExtension(QX11Info::display(), &xDamageEventBase, &xDamageErrorBase);
#endif
        filterRegistered = true;
    }

#ifdef HAVE_DBUS
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(PIXMAP_PROVIDER_DBUS_SERVICE))
        isPixmapProviderOnline = true;
    else
#endif
        isPixmapProviderOnline = false;

#ifdef HAVE_DBUS
    dbusWatcher = new QDBusServiceWatcher( PIXMAP_PROVIDER_DBUS_SERVICE , QDBusConnection::sessionBus(),
                                           QDBusServiceWatcher::WatchForRegistration|QDBusServiceWatcher::WatchForUnregistration,
                                           this );

    connect(dbusWatcher, SIGNAL(serviceRegistered(QString)),
            this, SLOT(handlePixmapProviderOnline()));
    connect(dbusWatcher, SIGNAL(serviceUnregistered(QString)),
            this, SLOT(handlePixmapProviderOffline()));

#endif
    querySharedPixmapFromProvider();
}

MDeclarativeStatusBar::~MDeclarativeStatusBar()
{
    destroyXDamageForSharedPixmap();
}

void MDeclarativeStatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (sharedPixmap.isNull()) {
        MDeclarativeStatusBar *view = const_cast<MDeclarativeStatusBar *>(this);
        view->querySharedPixmapFromProvider();
    }

    if (sharedPixmap.isNull()) {
        painter->fillRect(boundingRect(), Qt::black);
        return;
    }

    QRectF sourceRect;
    if (mOrientation == MDeclarativeScreen::Landscape) {
        sourceRect.setX(0);
        sourceRect.setY(0);
        sourceRect.setWidth(width());
        sourceRect.setHeight(height());
    } else {
        sourceRect.setX(0);
        sourceRect.setY(height());
        sourceRect.setWidth(width());
        sourceRect.setHeight(height());
    }

    painter->drawPixmap(QPointF(0.0, 0.0), sharedPixmap, sourceRect);
}

void MDeclarativeStatusBar::updateSharedPixmap()
{
    destroyXDamageForSharedPixmap();
    if ((!updatesEnabled)||(!isPixmapProviderOnline))
        return;

    if (!sharedPixmap.isNull()) {
        setupXDamageForSharedPixmap();
    }
}

void MDeclarativeStatusBar::setupXDamageForSharedPixmap()
{
#ifdef HAVE_XDAMAGE
    Q_ASSERT(!sharedPixmap.isNull());
    pixmapDamage = XDamageCreate(QX11Info::display(), sharedPixmap.handle(), XDamageReportNonEmpty);
    damageMap.insert(pixmapDamage, this);
#endif
}

void MDeclarativeStatusBar::destroyXDamageForSharedPixmap()
{
#ifdef HAVE_XDAMAGE
    if (pixmapDamage) {
        damageMap.remove(pixmapDamage);
        XDamageDestroy(QX11Info::display(), pixmapDamage);
        pixmapDamage = 0;
    }
#endif
}

void MDeclarativeStatusBar::enablePixmapUpdates()
{
    updatesEnabled = true;
    querySharedPixmapFromProvider();
}

void MDeclarativeStatusBar::disablePixmapUpdates()
{
    updatesEnabled = false;
    destroyXDamageForSharedPixmap();
}

void MDeclarativeStatusBar::querySharedPixmapFromProvider()
{
    if (!updatesEnabled || !isPixmapProviderOnline)
        return;
#ifdef HAVE_DBUS
    QDBusInterface interface(PIXMAP_PROVIDER_DBUS_SERVICE, PIXMAP_PROVIDER_DBUS_PATH, PIXMAP_PROVIDER_DBUS_INTERFACE,
                             QDBusConnection::sessionBus());
    QDBusPendingCall asyncCall =  interface.asyncCall(PIXMAP_PROVIDER_DBUS_SHAREDPIXMAP_CALL);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(asyncCall, this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(sharedPixmapHandleReceived(QDBusPendingCallWatcher*)));
#endif
}

void MDeclarativeStatusBar::sharedPixmapHandleReceived(QDBusPendingCallWatcher * call)
{
#ifdef HAVE_DBUS
    QDBusPendingReply<quint32> reply = *call;
    if (reply.isError()) {
        qWarning() << "MDeclarativeStatusBar" << reply.error().message();
        return;
    }
    quint32 tmp = reply;
    sharedPixmap = QPixmap::fromX11Pixmap(tmp, QPixmap::ExplicitlyShared);
    setImplicitWidth(sharedPixmap.size().width());
    updateSharedPixmap();
    call->deleteLater();
    scene()->update();
#endif
}

void MDeclarativeStatusBar::handlePixmapProviderOnline()
{
    isPixmapProviderOnline = true;
    querySharedPixmapFromProvider();
}

void MDeclarativeStatusBar::handlePixmapProviderOffline()
{
    isPixmapProviderOnline = false;
    destroyXDamageForSharedPixmap();
}

void MDeclarativeStatusBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    firstPos = event->pos();
    playHapticsFeedback();
}

void MDeclarativeStatusBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(firstPos.y() /* ### + style()->swipeThreshold()*/ + 25 < event->pos().y())
        showStatusIndicatorMenu();
}

void MDeclarativeStatusBar::showStatusIndicatorMenu()
{
#ifdef HAVE_DBUS
    QDBusInterface interface(STATUS_INDICATOR_MENU_DBUS_SERVICE, STATUS_INDICATOR_MENU_DBUS_PATH, STATUS_INDICATOR_MENU_DBUS_INTERFACE, QDBusConnection::sessionBus());
    interface.call(QDBus::NoBlock, "open");
#endif
}

void MDeclarativeStatusBar::playHapticsFeedback()
{
//    style()->pressFeedback().play();
}



void MDeclarativeStatusBar::setOrientation(MDeclarativeScreen::Orientation o)
{
    if (mOrientation != o) {
        mOrientation = o;
        update();
        emit orientationChanged();
    }
}

MDeclarativeScreen::Orientation MDeclarativeStatusBar::orientation() const
{
    return mOrientation;
}
