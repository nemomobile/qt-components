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

#include "sdeclarativescreen.h"
#include "sdeclarativescreen_p.h"
#include "sdeclarativescreen_p_sensor.h"
#include <QResizeEvent>
#include <QDeclarativeEngine>

#ifdef Q_OS_SYMBIAN
#include <QApplication>
#include <qsymbianevent.h>
#include <w32std.h>
#include <coemain.h>
#include <bitstd.h>
#endif

#ifdef Q_DEBUG_SCREEN
#include <QDebug>
#endif // Q_DEBUG_SCREEN

#ifdef Q_OS_SYMBIAN
const TInt KEikDynamicLayoutVariantSwitch = 0x101F8121;
OrientationListener *OrientationListener::instance = 0;
QCoreApplication::EventFilter OrientationListener::previousEventFilter = 0;
int OrientationListener::userCount = 0;

OrientationListener::OrientationListener()
{
    previousEventFilter = QApplication::instance()->setEventFilter(symbianEventFilter);
}

OrientationListener::~OrientationListener()
{
    QApplication::instance()->setEventFilter(previousEventFilter);
    previousEventFilter = 0;
}

OrientationListener *OrientationListener::getCountedInstance()
{
    if(!instance)
        instance = new OrientationListener();
    userCount++;
    return instance;
}

void OrientationListener::deleteCountedInstance()
{
    userCount--;
    if(userCount < 1) {
        delete instance;
        instance = 0;
    }
}

bool OrientationListener::symbianEventFilter(void *message, long *result)
{
    if (instance) {
        QSymbianEvent *symbianEvent = static_cast<QSymbianEvent *>(message);

        if (symbianEvent) {
            // Make the Screen sync its orientation with system orientation on two events:
            // layout switch and app gaining foreground.
            // The layout switch event is gained when the application rect has changed. If
            // the application is in background, this is not necessarily in sync with the
            // changes of "global" system orientation. For this reason, the system orientation
            // needs to be checked also when foreground is gained.
            if (symbianEvent->type() == QSymbianEvent::ResourceChangeEvent
                && symbianEvent->resourceChangeType() == KEikDynamicLayoutVariantSwitch) {
                instance->emit orientationEvent();
            } else if (symbianEvent->type() == QSymbianEvent::WindowServerEvent) {
                const TWsEvent *wsEvent = symbianEvent->windowServerEvent();
                if (wsEvent && wsEvent->Type() == EEventWindowVisibilityChanged) {
                    const TWsVisibilityChangedEvent *visEvent = wsEvent->VisibilityChanged();
                    if (visEvent && visEvent->iFlags & TWsVisibilityChangedEvent::EFullyVisible) {
                        instance->emit orientationEvent();
                    }
                }
            }
        }
    }

    bool returnValue = false;
    // Use the previous event filter to provide us with a return value, if it exists.
    if (previousEventFilter)
        returnValue = previousEventFilter(message, result);
    return returnValue;
}
#endif

SDeclarativeScreenPrivateSensor::SDeclarativeScreenPrivateSensor(SDeclarativeScreen *qq, QDeclarativeEngine *engine, QDeclarativeView *view)
    : SDeclarativeScreenPrivate(qq, engine, view)
    , m_animate(0)
    , m_hasWindow(0)
{
    if (m_view) {
        m_view->installEventFilter(this);
        connect(m_view, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(viewStatusChanged(QDeclarativeView::Status)));

        bool landscapeLock = false;
#ifdef Q_OS_SYMBIAN
        landscapeLock = deviceSupportsOnlyLandscape();
#endif

        //In case the orientation lock was set in the cpp side
        if (m_view->testAttribute(Qt::WA_LockLandscapeOrientation) || landscapeLock) {
#ifdef Q_DEBUG_SCREEN
            qDebug() << "SDeclarativeScreenPrivateSensor - Locking LandscapeOrientation";
#endif
            setAllowedOrientations(SDeclarativeScreen::Landscape);
        } else if (m_view->testAttribute(Qt::WA_LockPortraitOrientation)) {
#ifdef Q_DEBUG_SCREEN
            qDebug() << "SDeclarativeScreenPrivateSensor - Locking PortraitOrientation";
#endif
            setAllowedOrientations(SDeclarativeScreen::Portrait);
        }
    }

#ifdef Q_OS_SYMBIAN
    connect(OrientationListener::getCountedInstance(), SIGNAL(orientationEvent()), this, SLOT(syncOrientationWithSystem()));
#endif

}

SDeclarativeScreenPrivateSensor::~SDeclarativeScreenPrivateSensor()
{
#if defined(Q_OS_SYMBIAN)
    OrientationListener::deleteCountedInstance();
#endif
}

void SDeclarativeScreenPrivateSensor::setAllowedOrientations(SDeclarativeScreen::Orientations orientations)
{
#ifdef Q_DEBUG_SCREEN
    qDebug() << "SDeclarativeScreenPrivateSensor::setAllowedOrientations";
#endif

#if defined(Q_OS_SYMBIAN)
    if((orientations != SDeclarativeScreen::Landscape) && deviceSupportsOnlyLandscape())
        return;
#endif

    SDeclarativeScreenPrivate::setAllowedOrientations(orientations);

    if (!m_initialized)
        return;

    if (portraitAllowed() && landscapeAllowed()) {
        if (m_view)
            m_view->setAttribute(Qt::WA_AutoOrientation, true);
#ifdef Q_OS_SYMBIAN
        privateSetOrientation(systemOrientation());
#else
        privateSetOrientation(portraitDisplay() ? SDeclarativeScreen::Portrait : SDeclarativeScreen::Landscape);
#endif
    } else if (portraitAllowed() && !landscapeAllowed()) {
        if (m_view)
            m_view->setAttribute(Qt::WA_LockPortraitOrientation, true);
            privateSetOrientation(SDeclarativeScreen::Portrait);
    } else if (!portraitAllowed() && landscapeAllowed()) {
        if (m_view)
            m_view->setAttribute(Qt::WA_LockLandscapeOrientation, true);    
        privateSetOrientation(SDeclarativeScreen::Landscape);
    }
}

void SDeclarativeScreenPrivateSensor::privateSetOrientation(int orientation)
{
#ifdef Q_DEBUG_SCREEN
    qDebug() << "SDeclarativeScreenPrivateSensor::privateSetOrientation";
#endif
    Q_Q(SDeclarativeScreen);

    if (orientation == SDeclarativeScreen::All)
        return;

    if (m_allowedOrientations != SDeclarativeScreen::Default && !(m_allowedOrientations & orientation))
        return;

    int rotation = 0;

    if(orientation == SDeclarativeScreen::Default)
        orientation = portraitDisplay() ? SDeclarativeScreen::Portrait : SDeclarativeScreen::Landscape;

    if (orientation == SDeclarativeScreen::Portrait)
        rotation = portraitDisplay() ? 0 : 90;
    else if (orientation == SDeclarativeScreen::Landscape)
        rotation = portraitDisplay() ? 90 : 0;
    else if (orientation == SDeclarativeScreen::LandscapeInverted)
        rotation = portraitDisplay() ? -90 : -180;
    else if (orientation == SDeclarativeScreen::PortraitInverted)
        rotation = portraitDisplay() ? -180: -90;

    setCurrentOrientation(static_cast<SDeclarativeScreen::Orientation>(orientation), !m_hasWindow);

    // starts orientation change in Window.qml
    if (m_hasWindow) {
        q->emit privateAboutToChangeOrientation(rotation, m_animate);
        if(m_view)
            m_animate = !m_view->property("sensorRotationAnimationDisabled").toBool();
        else
            m_animate = 1;
    }

    if (!m_animate || !m_hasWindow) switchGeometry();
}

bool SDeclarativeScreenPrivateSensor::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);

#ifdef Q_OS_SYMBIAN
    Q_Q(SDeclarativeScreen);
    if (event->type() == QEvent::Show)
        QMetaObject::invokeMethod(q, "privateSetOrientation", Q_ARG(int, m_currentOrientation));
#else
    if (m_view && m_view == obj && m_initialized && event->type() == QEvent::Resize) {
        QSize size = static_cast<QResizeEvent*>(event)->size();
        m_animate = 0;
        setDisplay(size.width(), size.height(), m_dpi);
    }
#endif
    return QObject::eventFilter(obj, event);
}

#ifdef Q_OS_SYMBIAN
void SDeclarativeScreenPrivateSensor::syncOrientationWithSystem()
{
    Q_Q(SDeclarativeScreen);

    SDeclarativeScreen::Orientation orientation = systemOrientation();
    if (orientation == m_currentOrientation)
        return;

    QMetaObject::invokeMethod(q, "privateSetOrientation", Q_ARG(int, orientation));
}

SDeclarativeScreen::Orientation SDeclarativeScreenPrivateSensor::systemOrientation()
{
    TPixelsTwipsAndRotation params = screenParams();

    if (params.iRotation == CFbsBitGc::EGraphicsOrientationNormal)
        return portraitDisplay() ? SDeclarativeScreen::Portrait : SDeclarativeScreen::Landscape;
    else if (params.iRotation == CFbsBitGc::EGraphicsOrientationRotated90)
        return portraitDisplay() ? SDeclarativeScreen::Landscape : SDeclarativeScreen::PortraitInverted;
    else if (params.iRotation == CFbsBitGc::EGraphicsOrientationRotated180)
        return portraitDisplay() ? SDeclarativeScreen::PortraitInverted : SDeclarativeScreen::LandscapeInverted;
    else if (params.iRotation == CFbsBitGc::EGraphicsOrientationRotated270)
        return portraitDisplay() ? SDeclarativeScreen::LandscapeInverted : SDeclarativeScreen::Portrait;

    return portraitDisplay() ? SDeclarativeScreen::Portrait : SDeclarativeScreen::Landscape;
}
#endif

void SDeclarativeScreenPrivateSensor::switchGeometry()
{
    QSize newScreenSize = m_displaySize;

    bool portraitTranspose = (m_currentOrientation == SDeclarativeScreen::Portrait
                          || m_currentOrientation == SDeclarativeScreen::PortraitInverted)
                          && !portraitDisplay();

    bool landscapeTranpose = (m_currentOrientation == SDeclarativeScreen::Landscape
                          || m_currentOrientation == SDeclarativeScreen::LandscapeInverted)
                          && portraitDisplay();

    if(portraitTranspose || landscapeTranpose)
        newScreenSize.transpose();

    setScreenSize(newScreenSize);
}

void SDeclarativeScreenPrivateSensor::viewStatusChanged(QDeclarativeView::Status status)
{
    Q_Q(SDeclarativeScreen);

    if (m_view.isNull())
        return;

    if (status == QDeclarativeView::Ready) {
        QGraphicsObject* window = m_view->rootObject();

        // if window and needed signals found
        if (window
         && window->property("privateWindow").isValid()
         && window->property("inPortrait").isValid()) {
            connect(window, SIGNAL(orientationChangeFinished()), q, SIGNAL(currentOrientationChanged()));
            connect(window, SIGNAL(privateSwitchGeometry()), this, SLOT(switchGeometry()));
            m_hasWindow = 1;
        }
    } else
        m_hasWindow = 0;
}
