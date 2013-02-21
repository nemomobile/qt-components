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

#include <QApplication>
#include <QDebug>
#include <QDeclarativeItem>
#include <QWindowStateChangeEvent>

#include "mdeclarativescreen.h"
#include "mdeclarativeinputcontext.h"
#include "mwindowstate.h"
#include <math.h>

#ifdef HAVE_CONTEXTSUBSCRIBER
# include "contextproperty.h"
# include "contextpropertyinfo.h"
# include "mservicelistener.h"
#endif

#ifdef Q_WS_X11
  // These includes conflict with some of Qt's types, so should be kept last
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

#ifdef HAVE_XRANDR
#include <X11/extensions/Xrandr.h>
#endif

static const qreal CATEGORY_SMALL_LIMIT  = 3.2;
static const qreal CATEGORY_MEDIUM_LIMIT = 4.5;
static const qreal CATEGORY_LARGE_LIMIT  = 7.0;
static const qreal DENSITY_SMALL_LIMIT   = 140.0;
static const qreal DENSITY_MEDIUM_LIMIT  = 180.0;
static const qreal DENSITY_LARGE_LIMIT   = 270.0;

class MDeclarativeScreenPrivate
{
public:
    MDeclarativeScreenPrivate(MDeclarativeScreen *qq);
    ~MDeclarativeScreenPrivate();

    void updateX11OrientationAngleProperty();
    void initContextSubscriber();

    void _q_isCoveredChanged();
    void _q_updateOrientationAngle();
    void _q_updateIsTvConnected();
    void _q_windowAnimationChanged();

    void updateScreenSize();

    qreal dpi() const;
    int rotation() const;

    MDeclarativeScreen::Orientation physicalOrientation() const;

    MDeclarativeScreen *q;

    MDeclarativeScreen::Orientation orientation;
    MDeclarativeScreen::Orientation finalOrientation;
    MDeclarativeScreen::Orientations allowedOrientations;
    MDeclarativeScreen::Direction rotationDirection;

    bool isCovered;
    bool keyboardOpen;
    bool isTvConnected;

    QPointer<QWidget> topLevelWidget;

    QCoreApplication::EventFilter oldEventFilter;

    QSize displaySize;
    QSize screenSize;

    bool allowSwipe;

    void setMinimized(bool);
    bool isMinimized() const;
    bool isRemoteScreenPresent() const;
    QString topEdgeValue() const;

    MDeclarativeScreen::Orientations physicalDisplayOrientation() const { return _physicalDisplayOrientation; }

#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty topEdgeProperty;
    ContextProperty remoteTopEdgeProperty;
    ContextProperty isCoveredProperty;
    ContextProperty keyboardOpenProperty;
    ContextProperty videoRouteProperty;
    MServiceListener remoteTopEdgeListener;
#endif
private:
    void initPhysicalDisplayOrientation();

private:
    bool minimized;
    MDeclarativeScreen::Orientations _physicalDisplayOrientation;
};

MDeclarativeScreen* MDeclarativeScreen::instance()
{
    static MDeclarativeScreen *self = 0;

    if (!self)
        self = new MDeclarativeScreen();
    return self;
}

MDeclarativeScreenPrivate::MDeclarativeScreenPrivate(MDeclarativeScreen *qq)
    : q(qq)
#ifdef __ARMEL__
    , orientation(MDeclarativeScreen::Landscape)
    , finalOrientation(MDeclarativeScreen::Landscape)
#else
    , orientation(MDeclarativeScreen::Portrait)
    , finalOrientation(MDeclarativeScreen::Portrait)
#endif
    , allowedOrientations(MDeclarativeScreen::Landscape | MDeclarativeScreen::Portrait)
    , rotationDirection(MDeclarativeScreen::NoDirection)
    , isCovered(false)
    , keyboardOpen(false)
    , isTvConnected(false)
    , topLevelWidget(0)
    , oldEventFilter(0)
    , allowSwipe(true)
#ifdef HAVE_CONTEXTSUBSCRIBER
    , topEdgeProperty("Screen.TopEdge")
    , remoteTopEdgeProperty("RemoteScreen.TopEdge")
    , isCoveredProperty("Screen.IsCovered")
    , keyboardOpenProperty("/maemo/InternalKeyboard/Open")
    , videoRouteProperty("com.nokia.policy.video_route")
    , remoteTopEdgeListener(remoteTopEdgeProperty.info()->providerDBusType()
    , remoteTopEdgeProperty.info()->providerDBusName())
#endif
    , minimized(false)
{
#ifdef __ARMEL__
    displaySize = QSize(854, 480);
#else
    // TODO: Could use QDesktopWidget, but what about on host PC?
    displaySize = QSize(480, 854);
#endif

    initPhysicalDisplayOrientation();
}

MDeclarativeScreenPrivate::~MDeclarativeScreenPrivate()
{
}

void MDeclarativeScreenPrivate::initPhysicalDisplayOrientation()
{
    if(displaySize.isValid()) {
        if(displaySize.height() > displaySize.width()) {
            _physicalDisplayOrientation = MDeclarativeScreen::Portrait;
            _physicalDisplayOrientation |= MDeclarativeScreen::PortraitInverted;
        } else {
            _physicalDisplayOrientation = MDeclarativeScreen::Landscape;
            _physicalDisplayOrientation |= MDeclarativeScreen::LandscapeInverted;
        }
        updateScreenSize();
    }
}

void MDeclarativeScreenPrivate::initContextSubscriber()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    //waiting for properties to synchronize
    topEdgeProperty.waitForSubscription();
    isCoveredProperty.waitForSubscription();
    keyboardOpenProperty.waitForSubscription();
    videoRouteProperty.waitForSubscription();
    remoteTopEdgeListener.startListening(true);

    QObject::connect(&topEdgeProperty, SIGNAL(valueChanged()),
                     q, SLOT(_q_updateOrientationAngle()));
    QObject::connect(&remoteTopEdgeProperty, SIGNAL(valueChanged()),
                     q, SLOT(_q_updateOrientationAngle()));
    QObject::connect(&keyboardOpenProperty, SIGNAL(valueChanged()),
                     q, SLOT(_q_updateOrientationAngle()));
    QObject::connect(&isCoveredProperty, SIGNAL(valueChanged()),
                     q, SLOT(_q_isCoveredChanged()));
    QObject::connect(&videoRouteProperty, SIGNAL(valueChanged()),
                     q, SLOT(_q_updateIsTvConnected()));
    QObject::connect(&remoteTopEdgeListener, SIGNAL(nameAppeared()),
                     q, SLOT(_q_updateOrientationAngle()));
    QObject::connect(&remoteTopEdgeListener, SIGNAL(nameDisappeared()),
                     q, SLOT(_q_updateOrientationAngle()));
#endif
    //initiating the variables to current orientation
    _q_updateOrientationAngle();
    _q_isCoveredChanged();
    updateX11OrientationAngleProperty();
    _q_updateIsTvConnected();

    QObject::connect(MWindowState::instance(), SIGNAL(animatingChanged()),
                     q, SLOT(_q_windowAnimationChanged()));
}

void MDeclarativeScreenPrivate::updateScreenSize() {
    // Update screen width / height properties
    if(orientation & physicalDisplayOrientation()) {
        screenSize.setWidth(displaySize.width());
        screenSize.setHeight(displaySize.height());
    } else {
        screenSize.setWidth(displaySize.height());
        screenSize.setHeight(displaySize.width());
    }
    emit q->platformWidthChanged();
    emit q->platformHeightChanged();
}

void MDeclarativeScreenPrivate::updateX11OrientationAngleProperty()
{
#ifdef Q_WS_X11
    if (!topLevelWidget)
        return;

    // This writes the orientation angle of into the X11 window property,
    // which makes OS dialogs coming on top follow the app orientation
    Display *display = QX11Info::display();
    Atom orientationAngleAtom = XInternAtom(display, "_MEEGOTOUCH_ORIENTATION_ANGLE", False);

    int angle = q->rotation();

    XChangeProperty(display, topLevelWidget->effectiveWinId(), orientationAngleAtom, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char*)&angle, 1);
#endif
}

void MDeclarativeScreenPrivate::_q_isCoveredChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    bool covered = isCoveredProperty.value().toBool();

    if (isCovered != covered) {
        qDebug() << "MDeclarativeScreenPrivate" << "Covered:" << covered;

        isCovered = covered;
        emit q->coveredChanged();
    }
#endif
}

void MDeclarativeScreenPrivate::_q_updateIsTvConnected()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    QString value = videoRouteProperty.value().toString();

    isTvConnected = (value == "tvout" ||
                     value == "builtinandtvout");
    _q_updateOrientationAngle();
#endif
}

qreal MDeclarativeScreenPrivate::dpi() const
{
    static qreal dpi = 0;
#if defined(Q_WS_X11) && defined(HAVE_XRANDR)
    if (!dpi) {
        int physicalHeight = 0;
        XRRScreenResources *sr;
        sr = XRRGetScreenResources(QX11Info::display(), RootWindow(QX11Info::display(), 0));
        if (sr->noutput) {
            XRROutputInfo *output = XRRGetOutputInfo(QX11Info::display(),sr,sr->outputs[0]);
            if (output->crtc) {
               physicalHeight = output->mm_height;
            }
            XRRFreeOutputInfo(output);
        }
        XRRFreeScreenResources(sr);
        dpi = QDesktopWidget().screenGeometry().height() / (physicalHeight / 25.4);
    }
#else
    if (!dpi)
        dpi = QDesktopWidget().physicalDpiX();
#endif
    return dpi;
}

int MDeclarativeScreenPrivate::rotation() const
{
    int angle = 0;

    if(_physicalDisplayOrientation & MDeclarativeScreen::Landscape) {
        switch (orientation) {
        case MDeclarativeScreen::Landscape:
            angle = 0;
            break;
        case MDeclarativeScreen::Portrait:
        case MDeclarativeScreen::Default:  //handle default as portrait
            angle = 270;
            break;
        case MDeclarativeScreen::LandscapeInverted:
            angle = 180;
            break;
        case MDeclarativeScreen::PortraitInverted:
            angle = 90;
            break;
        default:
            qCritical() << "MDeclarativeScreen hast invalid orientation set.";
        }
    } else {
        switch (orientation) {
        case MDeclarativeScreen::Landscape:
            angle = 90;
            break;
        case MDeclarativeScreen::Portrait:
        case MDeclarativeScreen::Default:  //handle default as portrait
            angle = 0;
            break;
        case MDeclarativeScreen::LandscapeInverted:
            angle = 270;
            break;
        case MDeclarativeScreen::PortraitInverted:
            angle = 180;
            break;
        default:
            qCritical() << "MDeclarativeScreen hast invalid orientation set.";
        }
    }

    return angle;
}

MDeclarativeScreen::Orientation MDeclarativeScreenPrivate::physicalOrientation() const {
    MDeclarativeScreen::Orientation o = MDeclarativeScreen::Default;
#ifdef HAVE_CONTEXTSUBSCRIBER
    QString topEdge = topEdgeValue();

    if (topEdge == "top") {
        o = MDeclarativeScreen::Landscape;
    } else if (topEdge == "left") {
        o = MDeclarativeScreen::Portrait;
    } else if (topEdge == "right") {
        o = MDeclarativeScreen::PortraitInverted;
    } else if (topEdge == "bottom") {
        o = MDeclarativeScreen::LandscapeInverted;
    }
#endif
    return o;
}

void MDeclarativeScreenPrivate::_q_updateOrientationAngle()
{
    MDeclarativeScreen::Orientation newOrientation = MDeclarativeScreen::Default;

#ifdef HAVE_CONTEXTSUBSCRIBER
    if (isMinimized())
        return; // ignore sensor changes when minimized, we'll fix window orientation when it is restored

    QString edge = topEdgeValue();
    bool open = keyboardOpenProperty.value().toBool();

    //HW Keyboard open or TV connected causes a switch to landscape, but only if this is allowed
    if ((open || isTvConnected) && allowedOrientations & MDeclarativeScreen::Landscape) {
        newOrientation = MDeclarativeScreen::Landscape;
    } else if (edge == "top" && (allowedOrientations & MDeclarativeScreen::Landscape)) {
        newOrientation = MDeclarativeScreen::Landscape;
    } else if (edge == "left" && (allowedOrientations & MDeclarativeScreen::Portrait)) {
        newOrientation = MDeclarativeScreen::Portrait;
    } else if (edge == "right" && (allowedOrientations & MDeclarativeScreen::PortraitInverted)) {
        newOrientation = MDeclarativeScreen::PortraitInverted;
    } else if (edge == "bottom" && (allowedOrientations & MDeclarativeScreen::LandscapeInverted)) {
        newOrientation = MDeclarativeScreen::LandscapeInverted;
    }

    if (open != keyboardOpen) {
        keyboardOpen = open;
        emit q->keyboardOpenChanged();
    }
#endif

    //only set the new orientation if it is a valid one
    if(newOrientation != MDeclarativeScreen::Default) {
        q->setOrientation(newOrientation);
    }
}

void MDeclarativeScreenPrivate::_q_windowAnimationChanged()
{
    if (!MWindowState::instance()->animating() && finalOrientation != orientation)
        q->setOrientation(finalOrientation);
}

void MDeclarativeScreenPrivate::setMinimized(bool m) {
    if(minimized == m)
        return;

    minimized = m;
    emit q->minimizedChanged();
}

bool MDeclarativeScreenPrivate::isMinimized() const {
    return minimized;
}

bool MDeclarativeScreenPrivate::isRemoteScreenPresent() const {
    bool present = false;
#ifdef HAVE_CONTEXTSUBSCRIBER
    QString remoteTopEdge = remoteTopEdgeProperty.value().toString();
    bool remoteTopEdgePresent = (remoteTopEdgeListener.isServicePresent() == MServiceListener::Present);
    present = remoteTopEdgePresent && !remoteTopEdge.isEmpty();
#endif
    return present;
}

QString MDeclarativeScreenPrivate::topEdgeValue() const {
    QString top; // Empty string simulates MDeclarativeScreen::Default orientation
#ifdef HAVE_CONTEXTSUBSCRIBER
    QString topEdge = topEdgeProperty.value().toString();
    QString remoteTopEdge = remoteTopEdgeProperty.value().toString();
    top = isRemoteScreenPresent() ? remoteTopEdge : topEdge;
#endif
    return top;
}

MDeclarativeScreen::MDeclarativeScreen(QObject *parent)
        : QObject(parent),
        d(new MDeclarativeScreenPrivate(this))
{
    d->initContextSubscriber();

    qApp->installEventFilter(this);
    emit physicalDisplayChanged();
}

MDeclarativeScreen::~MDeclarativeScreen()
{
    delete d;
}

bool MDeclarativeScreen::eventFilter(QObject *o, QEvent *e) {
    if (e->type() != QEvent::WindowStateChange)
        goto out;

    // TODO: technically this is all sorts of stupid if we want to support
    // multiple top level windows
    d->topLevelWidget = qobject_cast<QWidget*>(o);
    if (!d->topLevelWidget) {
        qCritical() << Q_FUNC_INFO << "State change event from foreign window";
        goto out;
    }

    if (d->topLevelWidget->parent() != NULL)
        goto out;

    d->setMinimized(d->topLevelWidget->windowState() & Qt::WindowMinimized);
    if (!d->isMinimized()) {
        //if the current sensor's value is allowed, switch to it
        if(d->physicalOrientation() & allowedOrientations())
            setOrientation(d->physicalOrientation());
    }

    // TODO: technically we should only do this on initial show
    d->updateX11OrientationAngleProperty();
out:
    return QObject::eventFilter(o, e);
}

MDeclarativeScreen::Orientations MDeclarativeScreen::platformPhysicalDisplayOrientation() const
{
    return d->physicalDisplayOrientation();
}

void MDeclarativeScreen::setOrientation(Orientation o)
{
    d->finalOrientation = o;
    MDeclarativeScreen::Direction oldDirection = d->rotationDirection;

    if (d->orientation == o || MWindowState::instance()->animating())
        return;

    if ( (d->orientation == MDeclarativeScreen::LandscapeInverted && o == MDeclarativeScreen::Portrait) ||
         (d->orientation == MDeclarativeScreen::PortraitInverted && o == MDeclarativeScreen::LandscapeInverted) ||
         (d->orientation == MDeclarativeScreen::Landscape && o == MDeclarativeScreen::PortraitInverted) ||
         (d->orientation == MDeclarativeScreen::Portrait && o == MDeclarativeScreen::Landscape) ) {
         d->rotationDirection = MDeclarativeScreen::CounterClockwise;
    }
    else {
         d->rotationDirection = MDeclarativeScreen::Clockwise;
    }
    if (oldDirection != d->rotationDirection)
        emit rotationDirectionChanged();

    Orientation newOrientation = Default;
    //if keyboard is open always set landscape and ignore allowed orientations
#ifdef HAVE_CONTEXTSUBSCRIBER
    if(d->keyboardOpenProperty.value().toBool()) {
        newOrientation = Landscape;
    } else {
#endif
        if (!(d->allowedOrientations & o))
            return;

        newOrientation = o;
#ifdef HAVE_CONTEXTSUBSCRIBER
    }
#endif
    d->orientation = newOrientation;
    d->updateX11OrientationAngleProperty();

    d->updateScreenSize();


    emit widthChanged();
    emit heightChanged();

    MDeclarativeInputContext::setKeyboardOrientation(o);
    emit currentOrientationChanged();
}

MDeclarativeScreen::Orientation MDeclarativeScreen::currentOrientation() const
{
    return d->orientation;
}

void MDeclarativeScreen::setAllowedOrientations(Orientations orientation) {
    if (d->allowedOrientations == orientation)
        return;

    d->allowedOrientations = orientation;

    // Check if physical orientation fits allowed orientations
    if(d->physicalOrientation() != d->orientation) {
        if(d->physicalOrientation() & d->allowedOrientations) {
            setOrientation(d->physicalOrientation());
        }
    }

    // Check if current orientation still fits allowed
    if(!(d->orientation & d->allowedOrientations)) {
        if(d->allowedOrientations & MDeclarativeScreen::Portrait) {
            setOrientation(MDeclarativeScreen::Portrait);
            return;
        } else if(d->allowedOrientations & MDeclarativeScreen::Landscape) {
            setOrientation(MDeclarativeScreen::Landscape);
            return;
        } else if(d->allowedOrientations & MDeclarativeScreen::LandscapeInverted) {
            setOrientation(MDeclarativeScreen::LandscapeInverted);
            return;
        } else if(d->allowedOrientations & MDeclarativeScreen::PortraitInverted) {
            setOrientation(MDeclarativeScreen::PortraitInverted);
            return;
        }
    }
    emit allowedOrientationsChanged();
}

MDeclarativeScreen::Orientations MDeclarativeScreen::allowedOrientations() const {
    return d->allowedOrientations;
}

QString MDeclarativeScreen::orientationString() const
{
    const char *s = 0;
    switch (d->orientation) {
    case Portrait:
        s = "Portrait";
        break;
    case PortraitInverted:
        s = "PortraitInverted";
        break;
    case Landscape:
        s = "Landscape";
        break;
    case LandscapeInverted:
        s = "LandscapeInverted";
        break;
    default:
        qCritical() << "MDeclarativeScreen has invalid orientation set.";
        break;
    }
    return QString::fromLatin1(s);
}

int MDeclarativeScreen::rotation() const
{
    return d->rotation();
}

MDeclarativeScreen::Direction MDeclarativeScreen::rotationDirection() const
{
    return d->rotationDirection;
}

bool MDeclarativeScreen::isCovered() const
{
    return d->isCovered;
}

bool MDeclarativeScreen::isKeyboardOpen() const
{
    return d->keyboardOpen;
}

bool MDeclarativeScreen::isMinimized() const
{
    return d->isMinimized();
}

void MDeclarativeScreen::setMinimized(bool minimized)
{
    if(minimized == d->isMinimized())
        return;

    if(d->topLevelWidget) {
        d->topLevelWidget->setWindowState(minimized ? Qt::WindowMinimized : Qt::WindowMaximized);
        d->setMinimized(minimized);
    } else {
        qCritical() << "No top level widget set";
    }
}

int MDeclarativeScreen::width() const
{
#ifdef USE_DEPRECATED_SCREEN_WIDTH_HEIGHT
    qWarning() << "The semantics of screen.width property is deprecated, see QTCOMPONENTS-521. Please use screen.displayWidth to query native screen width.";
    return d->displaySize.width();
#else
    //Enable these after applications have been changed
    return d->screenSize.width();
#endif
}

int MDeclarativeScreen::height() const
{
#ifdef USE_DEPRECATED_SCREEN_WIDTH_HEIGHT
    qWarning() << "The semantics of screen.height property is deprecated, see QTCOMPONENTS-521. Please use screen.displayHeight to query native screen height.";
    return d->displaySize.height();
#else
    // Enable these after applications have been changed
    return d->screenSize.height();
#endif
}

int MDeclarativeScreen::platformWidth() const
{
    return d->screenSize.width();
}

int MDeclarativeScreen::platformHeight() const
{
    return d->screenSize.height();
}


int MDeclarativeScreen::displayWidth() const
{
    return d->displaySize.width();
}

int MDeclarativeScreen::displayHeight() const
{
    return d->displaySize.height();
}

MWindowState * MDeclarativeScreen::windowState() const
{
    qWarning() << "Warning: screen.windowState() is deprecated, use platformWindow property instead";
    return MWindowState::instance();
}

int MDeclarativeScreen::dpi() const {
    return d->dpi();
}

MDeclarativeScreen::DisplayCategory MDeclarativeScreen::displayCategory() const {
    const int w = QApplication::desktop()->screenGeometry().width();
    const int h = QApplication::desktop()->screenGeometry().height();
    const qreal diagonal = sqrt(static_cast<qreal>(w * w + h * h)) / dpi();
    if (diagonal < CATEGORY_SMALL_LIMIT)
        return Small;
    else if (diagonal < CATEGORY_MEDIUM_LIMIT)
        return Normal;
    else if (diagonal < CATEGORY_LARGE_LIMIT)
        return Large;
    else
        return ExtraLarge;
}

MDeclarativeScreen::Density MDeclarativeScreen::density() const {
    if (dpi() < DENSITY_SMALL_LIMIT)
        return Low;
    else if (dpi() < DENSITY_MEDIUM_LIMIT)
        return Medium;
    else if (dpi() < DENSITY_LARGE_LIMIT)
        return High;
    else
        return ExtraHigh;
}

void MDeclarativeScreen::updatePlatformStatusBarRect(QDeclarativeItem * statusBar)
{
    Q_UNUSED(statusBar);

#ifdef Q_WS_X11
    QWidget * activeWindow = QApplication::activeWindow();
    if(!activeWindow) {
        foreach (QWidget *widget, QApplication::topLevelWidgets()) {
            if (widget->effectiveWinId() != 0 &&
                widget->windowType() == Qt::Window) {
                activeWindow = widget;
                break;
            } 
        }

        if (!activeWindow)
            return;
    }

    QRectF rect(statusBar->mapRectToScene(0, 0, (qreal)statusBar->width(), (qreal)statusBar->height()));
    unsigned long data[4] = {0};

    if(statusBar->y() >= 0) {
        data[0] = 0;
        data[1] = 0;
        data[2] = rect.width();
        data[3] = rect.height();
    }

    Display *dpy = QX11Info::display();
    Atom a = XInternAtom(dpy, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY", False);
    Window w = activeWindow->effectiveWinId();
    if(data[3] == 0)
        XDeleteProperty(dpy, w, a);
    else
        XChangeProperty(dpy, w, a, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)data, 4);
#endif
}

bool MDeclarativeScreen::allowSwipe() const
{
    return d->allowSwipe;
}

bool MDeclarativeScreen::isPortrait() const
{
    return platformHeight() > platformWidth();
}

bool MDeclarativeScreen::isDisplayLandscape() const {
    return platformPhysicalDisplayOrientation() & Landscape;
}

void MDeclarativeScreen::setAllowSwipe(bool enabled)
{
    if (enabled != d->allowSwipe) {
#ifdef Q_WS_X11
        QWidget * activeWindow = QApplication::activeWindow();
        if(!activeWindow) {
            return;
        }
        Display       *dpy = QX11Info::display();
        Window w = activeWindow->effectiveWinId();

        unsigned long val = (enabled) ? 0 : 1;

        Atom atom = XInternAtom(dpy, "_MEEGOTOUCH_CANNOT_MINIMIZE", false);
        if (!atom) {
            qWarning("Unable to obtain _MEEGOTOUCH_CANNOT_MINIMIZE. This example will only work "
                     "with the MeeGo Compositor!");
            return;
        }

        XChangeProperty (dpy,
                w,
                atom,
                XA_CARDINAL,
                32,
                PropModeReplace,
                reinterpret_cast<unsigned char *>(&val),
                1);

        d->allowSwipe = enabled;
        emit allowSwipeChanged();
#endif
    }
}


#include "moc_mdeclarativescreen.cpp"
