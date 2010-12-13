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

#include <mdeclarativescreen.h>
#include <mdeviceprofile.h>
#include <minputmethodstate.h>

#include <qapplication.h>
#include <qpointer.h>
#include <qtimer.h>
#include <qrect.h>
#include <QMouseEvent>
#include <QInputContext>

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

class MDeclarativeScreenPrivate
{
public:
    MDeclarativeScreenPrivate(MDeclarativeScreen *qq);
    ~MDeclarativeScreenPrivate();

    void initContextSubscriber();
    void _q_isCoveredChanged();
    void _q_updateOrientationAngle();

    void _q_setOrientationHelper();

    void _q_sipChanged(const QRect &);
    void _q_checkMicroFocusHint();

    MDeclarativeScreen *q;

//    QPointer<QDeclarativeItem> window;

    MDeclarativeScreen::Orientation orientation;
    bool orientationLocked;
    bool isCovered;
    bool keyboardOpen;
    bool sipVisible;

    QSize resolution;
    QRectF microFocus;
    QTimer checkMicroFocusHintTimer;

#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty topEdgeProperty;
    ContextProperty isCoveredProperty;
    ContextProperty keyboardOpenProperty;
#endif
};

MDeclarativeScreenPrivate::MDeclarativeScreenPrivate(MDeclarativeScreen *qq)
    : q(qq)
//        , window(0)
    , orientation(MDeclarativeScreen::Landscape)
    , orientationLocked(false)
    , isCovered(false)
    , keyboardOpen(false)
    , sipVisible(false)
#ifdef HAVE_CONTEXTSUBSCRIBER
    , topEdgeProperty("Screen.TopEdge")
    , isCoveredProperty("Screen.IsCovered")
    , keyboardOpenProperty("/maemo/InternalKeyboard/Open")
#endif
{
    QObject::connect(MInputMethodState::instance(), SIGNAL(inputMethodAreaChanged(const QRect &)),
                     q, SLOT(_q_sipChanged(const QRect &)));

    checkMicroFocusHintTimer.setInterval(200);
    QObject::connect(&checkMicroFocusHintTimer, SIGNAL(timeout()), q, SLOT(_q_checkMicroFocusHint()));
    checkMicroFocusHintTimer.stop();
    resolution = MDeviceProfile::instance()->resolution();
}

MDeclarativeScreenPrivate::~MDeclarativeScreenPrivate()
{
//    delete window;
}


void MDeclarativeScreenPrivate::initContextSubscriber()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    //waiting for properties to synchronize
    topEdgeProperty.waitForSubscription();
    isCoveredProperty.waitForSubscription();
    keyboardOpenProperty.waitForSubscription();

    QObject::connect(&topEdgeProperty, SIGNAL(valueChanged()),
            q, SLOT(_q_updateOrientationAngle()));
    QObject::connect(&isCoveredProperty, SIGNAL(valueChanged()),
            q, SLOT(_q_isCoveredChanged()));
    QObject::connect(&keyboardOpenProperty, SIGNAL(valueChanged()),
            q, SLOT(_q_updateOrientationAngle()));


    //initiating the variables to current orientation
    _q_updateOrientationAngle();
    if (orientation == MDeclarativeScreen::Portrait)
        // make sure we notify the input method
        _q_setOrientationHelper();
    _q_isCoveredChanged();
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

void MDeclarativeScreenPrivate::_q_updateOrientationAngle()
{
    if (orientationLocked)
        return;

#ifdef HAVE_CONTEXTSUBSCRIBER
    MDeclarativeScreen::Orientation newOrientation = MDeclarativeScreen::Portrait;
    QString edge = topEdgeProperty.value().toString();
    bool open = keyboardOpenProperty.value().toBool();

    if (open) {
        newOrientation = MDeclarativeScreen::Landscape;
    } else if (edge == "top") {
        newOrientation = MDeclarativeScreen::Landscape;
    } else if (edge == "left") {
        newOrientation = MDeclarativeScreen::Portrait;
    } else if (edge == "right") {
        newOrientation = MDeclarativeScreen::PortraitInverted;
    } else if (edge == "bottom") {
        newOrientation = MDeclarativeScreen::LandscapeInverted;
    }

    if (open != keyboardOpen) {
        keyboardOpen = open;
        emit q->keyboardOpenChanged();
    }

    if (newOrientation != orientation) {
        orientation = newOrientation;
        _q_setOrientationHelper();
        emit q->orientationChanged();
    }
#endif
}

void MDeclarativeScreenPrivate::_q_setOrientationHelper()
{
    M::OrientationAngle angle = M::Angle0;
    switch(orientation) {
    case MDeclarativeScreen::Landscape:
        angle = M::Angle0;
        break;
    case MDeclarativeScreen::PortraitInverted:
        angle = M::Angle90;
        break;
    case MDeclarativeScreen::LandscapeInverted:
        angle = M::Angle180;
        break;
    case MDeclarativeScreen::Portrait:
        angle = M::Angle270;
        break;
    }

    // MThemeBridge and MStyleWrapper watch this property and will update
    // theme resources when orientation changes.
    MInputMethodState::instance()->setActiveWindowOrientationAngle(angle);
}

void MDeclarativeScreenPrivate::_q_sipChanged(const QRect &rect)
{
    fprintf(stderr, "sip changed %d %d %d/%d\n", rect.x(), rect.y(), rect.width(), rect.height());
    bool visible = !rect.isEmpty();
    if (visible != sipVisible) {
        if (visible)
            checkMicroFocusHintTimer.start();
        else
            checkMicroFocusHintTimer.stop();
        sipVisible = visible;
    }

    emit q->softwareInputPanelVisibleChanged();
}

void MDeclarativeScreenPrivate::_q_checkMicroFocusHint()
{
    if (!sipVisible)
        return;
    if (QWidget *widget = QApplication::focusWidget()) {
        QVariant v = widget->inputMethodQuery(Qt::ImMicroFocus);
        if (!v.toRectF().isValid())
            return;
        QRectF mf = v.toRectF();
        if (mf != microFocus) {
            microFocus = mf;
            fprintf(stderr, "microfocus changed %f %f %f/%f\n", mf.x(), mf.y(), mf.width(), mf.height());
            emit q->microFocusChanged();
        }
    }
}

MDeclarativeScreen::MDeclarativeScreen(QDeclarativeItem *parent)
        : QObject(parent),
        d(new MDeclarativeScreenPrivate(this))
{
    d->initContextSubscriber();
}

MDeclarativeScreen::~MDeclarativeScreen()
{
    delete d;
}

void MDeclarativeScreen::setOrientation(Orientation o)
{
    if (d->orientation == o)
        return;

    d->orientation = o;
    d->_q_setOrientationHelper();
    emit orientationChanged();
}

MDeclarativeScreen::Orientation MDeclarativeScreen::orientation() const
{
    return d->orientation;
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
    }
    return QString::fromLatin1(s);
}

int MDeclarativeScreen::rotation() const
{
    int angle = 0;
    switch (d->orientation) {
    case Landscape:
        angle = 0;
        break;
    case Portrait:
        angle = 270;
        break;
    case LandscapeInverted:
        angle = 180;
        break;
    case PortraitInverted:
        angle = 90;
        break;
    }
    return angle;
}


bool MDeclarativeScreen::isOrientationLocked() const
{
    return d->orientationLocked;
}

void MDeclarativeScreen::setOrientationLocked(bool locked)
{
    if (d->orientationLocked == locked)
        return;

    d->orientationLocked = locked;
    emit orientationLockedChanged();
}

bool MDeclarativeScreen::isCovered() const
{
    return d->isCovered;
}

bool MDeclarativeScreen::isKeyboardOpen() const
{
    return d->keyboardOpen;
}

bool MDeclarativeScreen::softwareInputPanelVisible() const
{
    qWarning() << "IM visible" << !MInputMethodState::instance()->inputMethodArea().isEmpty() << d->sipVisible;
    return d->sipVisible;
}

QRect MDeclarativeScreen::softwareInputPanelRect() const
{
    qWarning() << "IM rect" << MInputMethodState::instance()->inputMethodArea();
    return MInputMethodState::instance()->inputMethodArea();
}

QRectF MDeclarativeScreen::microFocus() const
{
    return d->microFocus;
}


bool MDeclarativeScreen::isMinimized() const
{
    QWidget *top = QApplication::activeWindow();
    if (!top)
        return true;
    return top->windowState() & Qt::WindowMinimized;
}

void MDeclarativeScreen::setMinimized(bool minimized)
{
    Q_UNUSED(minimized);

    QWidget *top = QApplication::activeWindow();
    if (!top)
        return;
    top->setWindowState(Qt::WindowMinimized);
}

int MDeclarativeScreen::width() const
{
    return d->resolution.width();
}

int MDeclarativeScreen::height() const
{
    return d->resolution.height();
}

void MDeclarativeScreen::sendClicked(int x, int y, int position) const
{
    QInputContext *ic = qApp->inputContext();

    if (ic != NULL) {
        QEvent::Type mouseEventType(QEvent::MouseButtonRelease);
        QPoint mousePoint(x,y);
        QMouseEvent mouseEvent(mouseEventType, mousePoint, Qt::LeftButton,
                               Qt::LeftButton, Qt::NoModifier);
        int preeditOffset = 0;
        ic->mouseHandler(preeditOffset, &mouseEvent);
    }
}

#include "moc_mdeclarativescreen.cpp"
