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
#include <QTimer>
#if defined(Q_WS_X11)
   #include <qx11info_x11.h>
#endif
#include <qgraphicsscene.h>
#include <qdebug.h>
#include "mwindowstate.h"
#include "feedbackplayer.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
   #include <QtQuick/qsgnode.h>
   #include <QtQuick/qsggeometry.h>
   #include <QtQuick/qsgtexturematerial.h>
   #include <QtQuick/qsgflatcolormaterial.h>
   #include <QtQuick/qquickcanvas.h>
   #include <qguiapplication.h>
   #include <QtGui/qpa/qplatformnativeinterface.h>
   #if defined(QT_OPENGL_ES_2)
      #include <EGL/egl.h>
      #include <EGL/eglext.h>
      #include <GLES2/gl2ext.h>
   #endif
#endif

#ifdef HAVE_DBUS
   #include <QDBusMessage>
   #include <QDBusServiceWatcher>
   #include <QDBusConnectionInterface>
#endif

#ifdef HAVE_XDAMAGE
   #include <X11/extensions/Xdamage.h>

   #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
      #include <X11/Xlib-xcb.h>
      #include <xcb/xcb.h>
      #include <xcb/damage.h>
   #endif
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>

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
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    xcb_generic_event_t *event = (xcb_generic_event_t *)message;
    if ((event->response_type & ~0x80) == xDamageEventBase + XDamageNotify) {
        xcb_damage_notify_event_t *xevent = (xcb_damage_notify_event_t *) event;

        // notify status bar
        MDeclarativeStatusBar *statusBar = damageMap.value(xevent->damage);
        if (statusBar) {
            statusBar->update();
            return true;
        }
    }
#else
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
#endif
#else
    Q_UNUSED(message);
#endif

    return false;
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && defined(HAVE_XLIB) && !defined(QT_OPENGL_ES_2)
static QImage qimageFromXImage(XImage* xi)
{
    QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    if (xi->depth == 24)
        format = QImage::Format_RGB32;
    else if (xi->depth == 16)
        format = QImage::Format_RGB16;

    QImage image = QImage(reinterpret_cast<uchar*>(xi->data), xi->width, xi->height, xi->bytes_per_line, format).copy();

    // we may have to swap the byte order
    if ((QSysInfo::ByteOrder == QSysInfo::LittleEndian && xi->byte_order == MSBFirst)
        || (QSysInfo::ByteOrder == QSysInfo::BigEndian && xi->byte_order == LSBFirst)) {

        for (int i = 0; i < image.height(); i++) {
            if (xi->depth == 16) {
                ushort* p = reinterpret_cast<ushort*>(image.scanLine(i));
                ushort* end = p + image.width();
                while (p < end) {
                    *p = ((*p << 8) & 0xff00) | ((*p >> 8) & 0x00ff);
                    p++;
                }
            } else {
                uint* p = reinterpret_cast<uint*>(image.scanLine(i));
                uint* end = p + image.width();
                while (p < end) {
                    *p = ((*p << 24) & 0xff000000) | ((*p << 8) & 0x00ff0000)
                         | ((*p >> 8) & 0x0000ff00) | ((*p >> 24) & 0x000000ff);
                    p++;
                }
            }
        }
    }

    // fix-up alpha channel
    if (format == QImage::Format_RGB32) {
        QRgb* p = reinterpret_cast<QRgb*>(image.bits());
        for (int y = 0; y < xi->height; ++y) {
            for (int x = 0; x < xi->width; ++x)
                p[x] |= 0xff000000;
            p += xi->bytes_per_line / 4;
        }
    }

    return image;
}
#endif

MDeclarativeStatusBar::MDeclarativeStatusBar(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    updatesEnabled(true),
    mousePressed(false),
    feedbackDelay(false),
    swipeGesture(false),
    sharedPixmapHandle(0),
    pixmapDamage(0),
    mOrientation(MDeclarativeScreen::Portrait),
    feedbackPlayer(new FeedbackPlayer(this))
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    , updateSharedTexture(false)
#endif
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setAcceptedMouseButtons(Qt::LeftButton);
    // higher than TitleBar
    setZValue(1010);
    setImplicitHeight(STATUSBAR_HEIGHT);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if (!filterRegistered) {
        ::oldEventFilter = QCoreApplication::instance()->setEventFilter(x11EventFilter);
#ifdef HAVE_XDAMAGE
        MDeclarativeScreen* screen = MDeclarativeScreen::instance();
        XDamageQueryExtension(screen->display(), &xDamageEventBase, &xDamageErrorBase);
#endif
        filterRegistered = true;
    }
#endif

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
    connect(this, SIGNAL(visibleChanged()), this, SLOT(updateXdamageEventSubscription()));

    if (!feedbackPlayer->init("qt-components")) {
        delete feedbackPlayer;
        feedbackPlayer = 0;
    }
}

MDeclarativeStatusBar::~MDeclarativeStatusBar()
{
    MWindowState * windowState = MWindowState::instance();
    disconnect(windowState, SIGNAL(activeChanged()), this, SLOT(updateXdamageEventSubscription()));

    destroyXDamageForSharedPixmap();
    delete feedbackPlayer;
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
QSGNode* MDeclarativeStatusBar::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(oldNode);
    if (!node) {
        node = new QSGGeometryNode;
        node->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial | QSGNode::OwnsOpaqueMaterial);
        node->setGeometry(new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4));
    }

    if (!sharedPixmapHandle) {
        node->setMaterial(new QSGFlatColorMaterial);
        node->setOpaqueMaterial(new QSGFlatColorMaterial);
        node->markDirty(QSGNode::DirtyMaterial);
        return node;
    }

    if (!sharedTexture || updateSharedTexture) {
        node->setMaterial(new QSGTextureMaterial);
        node->setOpaqueMaterial(new QSGTextureMaterial);

#if defined(HAVE_XLIB)
        MDeclarativeScreen* screen = MDeclarativeScreen::instance();

#if defined(QT_OPENGL_ES_2)
        static PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");
        static PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) eglGetProcAddress("eglCreateImageKHR");
        static PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) eglGetProcAddress("eglDestroyImageKHR");
        typedef EGLBoolean (EGLAPIENTRY *eglQueryImageNOKFunc)(EGLDisplay, EGLImageKHR, EGLint, EGLint*);
        static eglQueryImageNOKFunc eglQueryImageNOK = (eglQueryImageNOKFunc)eglGetProcAddress("eglQueryImageNOK");

        const EGLint attribs[] = { EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE };

        EGLDisplay eglDpy = eglGetDisplay((EGLNativeDisplayType)screen->display());
        EGLImageKHR img = eglCreateImageKHR(eglDpy, EGL_NO_CONTEXT, EGL_NATIVE_PIXMAP_KHR, (EGLClientBuffer)sharedPixmapHandle, attribs);

        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)img);

        GLint width = 0;
        GLint height = 0;
        eglQueryImageNOK(eglDpy, img, EGL_WIDTH, &width);
        eglQueryImageNOK(eglDpy, img, EGL_HEIGHT, &height);

        sharedTexture.reset(canvas()->createTextureFromId(textureId, QSize(width, height), QQuickCanvas::TextureOwnsGLTexture));

        eglDestroyImageKHR(eglDpy, img);
#else
        Display* dpy = screen->display();
        Window dummy1;
        int x, y;
        unsigned int width, height, borderwidth, depth;
        XGetGeometry(dpy, sharedPixmapHandle, &dummy1, &x, &y, &width, &height, &borderwidth, &depth);
        XImage* xi = XGetImage(dpy, sharedPixmapHandle, 0, 0, width, height, ULONG_MAX, ZPixmap);
        QImage img = qimageFromXImage(xi);
        XDestroyImage(xi);
        sharedTexture.reset(canvas()->createTextureFromImage(img));
#endif
#endif // HAVE_XLIB

        static_cast<QSGTextureMaterial*>(node->material())->setTexture(sharedTexture.data());
        static_cast<QSGOpaqueTextureMaterial*>(node->opaqueMaterial())->setTexture(sharedTexture.data());
        node->markDirty(QSGNode::DirtyMaterial);
        updateSharedTexture = false;
    }

    QRectF sourceRect;
    sourceRect = QRectF(0, 0, width(), height());
    if (mOrientation == MDeclarativeScreen::Portrait || mOrientation == MDeclarativeScreen::PortraitInverted)
        sourceRect.moveTop(height());
    sourceRect = sharedTexture.data()->convertToNormalizedSourceRect(sourceRect);

    QRect targetRect(x(), y(), width(), height());

    QSGGeometry::updateTexturedRectGeometry(node->geometry(), targetRect, sourceRect);
    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}
#else
void MDeclarativeStatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QT_TRY {
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

        if (feedbackDelay || mousePressed) {
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
#endif

void MDeclarativeStatusBar::updateXdamageEventSubscription()
{
    MWindowState * windowState = MWindowState::instance();
    if(windowState->active() && isVisible()) {
        // Subscribe to xdamage events only if there is a need
        if(pixmapDamage == 0 && !sharedPixmap.isNull())
            setupXDamageForSharedPixmap();
    } else {
        destroyXDamageForSharedPixmap();
    }
}

void MDeclarativeStatusBar::updateSharedPixmap()
{
    destroyXDamageForSharedPixmap();

    if (updatesEnabled && sharedPixmapHandle)
        setupXDamageForSharedPixmap();
}

void MDeclarativeStatusBar::setupXDamageForSharedPixmap()
{
#ifdef HAVE_XDAMAGE
    Q_ASSERT(sharedPixmapHandle);
    MDeclarativeScreen* screen = MDeclarativeScreen::instance();
    pixmapDamage = XDamageCreate(screen->display(this), (Drawable)sharedPixmapHandle, XDamageReportRawRectangles);
    damageMap.insert(pixmapDamage, this);
#endif
}

void MDeclarativeStatusBar::destroyXDamageForSharedPixmap()
{
#ifdef HAVE_XDAMAGE
    if (pixmapDamage) {
        damageMap.remove(pixmapDamage);
        MDeclarativeScreen* screen = MDeclarativeScreen::instance();
        XDamageDestroy(screen->display(), pixmapDamage);
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
    if (!updatesEnabled)
        return;

#ifdef HAVE_DBUS
    QDBusMessage message = QDBusMessage::createMethodCall(PIXMAP_PROVIDER_DBUS_SERVICE, PIXMAP_PROVIDER_DBUS_PATH,
                            PIXMAP_PROVIDER_DBUS_INTERFACE, PIXMAP_PROVIDER_DBUS_SHAREDPIXMAP_CALL);
    QDBusPendingCall asyncCall = QDBusConnection::sessionBus().asyncCall(message);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(asyncCall, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(sharedPixmapHandleReceived(QDBusPendingCallWatcher*)));
#endif
}

void MDeclarativeStatusBar::sharedPixmapHandleReceived(QDBusPendingCallWatcher * call)
{
#ifdef HAVE_DBUS
    bool pixmapWasNull = sharedPixmap.isNull();
    QDBusPendingReply<quint32> reply = *call;
    if (reply.isError()) {
        qWarning() << "MDeclarativeStatusBar" << reply.error().message();
        return;
    }

    sharedPixmapHandle = reply;
    setImplicitWidth(sharedPixmap.size().width());
    updateSharedPixmap();
    call->deleteLater();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    updateSharedTexture = true;
    update();
#else
    // Fix for NB276546
    if (scene()) {
      scene()->update();
    } else {
      qWarning() << "MDeclarativeStatusBar::sharedPixmapHandleReceived: scene is NULL!";
    }
#else
    Q_UNUSED(call)
#endif
}

bool MDeclarativeStatusBar::hasPixmap() const
{
    return !sharedPixmap.isNull();
}

void MDeclarativeStatusBar::handlePixmapProviderOnline()
{
    querySharedPixmapFromProvider();
}

void MDeclarativeStatusBar::handlePixmapProviderOffline()
{
    destroyXDamageForSharedPixmap();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void MDeclarativeStatusBar::mousePressEvent(QMouseEvent *event)
{
    handleMousePressEvent(event->localPos());
}

void MDeclarativeStatusBar::mouseMoveEvent(QMouseEvent *event)
{
    handleMouseMoveEvent(event->localPos());
}

void MDeclarativeStatusBar::mouseReleaseEvent(QMouseEvent *event)
{
    handleMouseReleaseEvent(event->localPos());
}
#else
void MDeclarativeStatusBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    handleMousePressEvent(event->pos());
}

void MDeclarativeStatusBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    handleMouseMoveEvent(event->pos());
}

void MDeclarativeStatusBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    handleMouseReleaseEvent(event->pos());
}
#endif

void MDeclarativeStatusBar::handleMousePressEvent(const QPointF& pos)
{
    firstPos = pos;

    if (feedbackPlayer)
        feedbackPlayer->sendPlaybackRequest(FeedbackPlayer::Press);

    if (!mousePressed) {
        mousePressed = true;
        update();
    }
    // When the status bar is being touch at the top edge, in the area that is also active for swipe
    // the mouse press and release events are delivered directly after each other.
    // Ensure the visual feedback is there long enough for the user to notice
    feedbackDelay = true;
    QTimer::singleShot(200, this, SLOT(disablePressedFeedback()));
}

void MDeclarativeStatusBar::handleMouseMoveEvent(const QPointF& pos)
{
    if(swipeGesture && firstPos.y() /* ### + style()->swipeThreshold()*/ + 25 < pos.y())
        showStatusIndicatorMenu();
}

void MDeclarativeStatusBar::handleMouseReleaseEvent(const QPointF& pos)
{
    if (swipeGesture || !mousePressed) {
        return;
    }

    if (feedbackPlayer)
        feedbackPlayer->sendPlaybackRequest(FeedbackPlayer::Release);

    mousePressed = false;
    update();

    QRectF rect = boundingRect();
    rect.adjust(-30, -30, 30, 30);
    if(rect.contains(pos)) {
        showStatusIndicatorMenu();
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void MDeclarativeStatusBar::itemChange(ItemChange change, const ItemChangeData& changeData)
{
#if 0
    if (change == ItemSceneChange) {
        if (!filterRegistered) {
            QPlatformNativeInterface* iface = QGuiApplication::platformNativeInterface();
            ::oldEventFilter = iface->setEventFilter(QByteArrayLiteral("xcb_generic_event_t"), x11EventFilter);
#ifdef HAVE_XDAMAGE
            MDeclarativeScreen* screen = MDeclarativeScreen::instance();
            if (!XDamageQueryExtension(screen->display(this), &xDamageEventBase, &xDamageErrorBase))
                qCritical() << "Could not find X damage extension on display. Status bar updates will not work.";
#endif
            filterRegistered = true;
        }
    }
#warning "status bar updates broken in Qt 5 port"
#endif
    QQuickItem::itemChange(change, changeData);
}
#endif

void MDeclarativeStatusBar::disablePressedFeedback()
{
    feedbackDelay = false;
    update();
}

void MDeclarativeStatusBar::showStatusIndicatorMenu()
{
#ifdef HAVE_DBUS
    QDBusMessage message = QDBusMessage::createMethodCall(STATUS_INDICATOR_MENU_DBUS_SERVICE, STATUS_INDICATOR_MENU_DBUS_PATH,
                            STATUS_INDICATOR_MENU_DBUS_INTERFACE, "open");
    QDBusConnection::sessionBus().asyncCall(message);
#endif
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
