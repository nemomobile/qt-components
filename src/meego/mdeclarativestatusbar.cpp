/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
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
#include "mwindowstate.h"

#ifdef HAVE_DBUS
   #include <QDBusInterface>
   #include <QDBusServiceWatcher>
   #include <QDBusConnectionInterface>
#endif

#ifdef HAVE_XDAMAGE
   #include <X11/extensions/Xdamage.h>
#endif

static const int STATUSBAR_HEIGHT = 36;

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
QCoreApplication::EventFilter oldEventFilter = 0;
#ifdef HAVE_XDAMAGE
static QHash<Damage, MDeclarativeStatusBar *> damageMap;
#endif

#ifdef HAVE_XDAMAGE
static int handleXError(Display *, XErrorEvent *)
{
    return 0;
}
#endif

static bool x11EventFilter(void *message, long *result)
{
#ifdef HAVE_XDAMAGE
    XEvent *event = (XEvent *)message;

    if (event->type == xDamageEventBase + XDamageNotify) {
        XDamageNotifyEvent *xevent = (XDamageNotifyEvent *) event;

        // notify status bar
        MDeclarativeStatusBar *statusBar = damageMap.value(xevent->damage);
        if (statusBar) {
            statusBar->update();
            return true;
        }
    }
#else
    Q_UNUSED(message);
#endif

    if (oldEventFilter) {
        return oldEventFilter(message, result);
    } else
        return false;
}

MDeclarativeStatusBar::MDeclarativeStatusBar(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    updatesEnabled(true),
    mousePressed(false),
    swipeGesture(false),
    mOrientation(MDeclarativeScreen::Portrait)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setAcceptedMouseButtons(Qt::LeftButton);
    // higher than TitleBar
    setZValue(1010);
    setImplicitHeight(STATUSBAR_HEIGHT);

    if (!filterRegistered) {
        ::oldEventFilter = QCoreApplication::instance()->setEventFilter(x11EventFilter);
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

    // XDamage event should come only when application is in foreground
    MWindowState * windowState = MWindowState::instance();
    connect(windowState, SIGNAL(activeChanged()), this, SLOT(updateXdamageEventSubscription()));
}

MDeclarativeStatusBar::~MDeclarativeStatusBar()
{
    MWindowState * windowState = MWindowState::instance();
    disconnect(windowState, SIGNAL(activeChanged()), this, SLOT(updateXdamageEventSubscription()));

    destroyXDamageForSharedPixmap();
}

void MDeclarativeStatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QT_TRY {

        if (sharedPixmap.isNull()) {
            MDeclarativeStatusBar *view = const_cast<MDeclarativeStatusBar *>(this);
            view->querySharedPixmapFromProvider();
        }

        if (sharedPixmap.isNull()) {
            painter->fillRect(boundingRect(), Qt::black);
            return;
        }

        QRectF sourceRect;
        if (mOrientation == MDeclarativeScreen::Landscape || mOrientation == MDeclarativeScreen::LandscapeInverted) {
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

        if (mousePressed) {
            painter->save();
            painter->setOpacity(0.6);
            painter->fillRect(QRectF(QPointF(0.0, 0.0), sourceRect.size()), Qt::black);
            painter->restore();
        }
    }
    QT_CATCH (...) {
        qDebug() << "MDeclarativeStatusBar::paint: Cannot draw sharedPixmap.";
    }

}

void MDeclarativeStatusBar::updateXdamageEventSubscription()
{
    MWindowState * windowState = MWindowState::instance();
    if(windowState->active()) {
        // Subscribe to xdamage events only if there is a need
        if(pixmapDamage == 0)
            setupXDamageForSharedPixmap();
    } else {
        destroyXDamageForSharedPixmap();
    }
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
    pixmapDamage = XDamageCreate(QX11Info::display(), sharedPixmap.handle(), XDamageReportRawRectangles);
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

#ifdef Q_WS_X11
    quint32 tmp = reply;
    sharedPixmap = QPixmap::fromX11Pixmap(tmp, QPixmap::ExplicitlyShared);
#endif

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

    if (!mousePressed) {
        mousePressed = true;
        update();
    }

}

void MDeclarativeStatusBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(swipeGesture && firstPos.y() /* ### + style()->swipeThreshold()*/ + 25 < event->pos().y())
        showStatusIndicatorMenu();
}

void MDeclarativeStatusBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    if (!mousePressed || swipeGesture)
        return;

    mousePressed = false;
    update();

    QRectF rect = boundingRect();
    rect.adjust(-30, -30, 30, 30);
    if(rect.contains(event->pos())) {
        showStatusIndicatorMenu();
    }
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
