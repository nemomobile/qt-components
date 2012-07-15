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
#include <QApplication>
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <qmath.h>
#include <qnamespace.h>

#ifdef Q_OS_SYMBIAN
#include <aknappui.h>
#include <hal.h>
#include <featmgr.h>
#endif

#ifdef Q_WS_SIMULATOR
#include <cmath>
#endif

#ifdef Q_DEBUG_SCREEN
#include <QDebug>
#endif // Q_DEBUG_SCREEN

static const qreal DEFAULT_TWIPS_PER_INCH = 1440.0;
static const qreal DEFAULT_DPI = 211.7;
static const int DEFAULT_WIDTH = 360;
static const int DEFAULT_HEIGHT = 640;

// Matches QOrientationReading
enum OrientationReading {
    Undefined = 0,	// The orientation is unknown.
    TopUp =     1,	// The Top edge of the device is pointing up.
    TopDown =   2,	// The Top edge of the device is pointing down.
    LeftUp =    3,	// The Left edge of the device is pointing up.
    RightUp =   4,	// The Right edge of the device is pointing up.
    FaceUp =    5,	// The Face of the device is pointing up.
    FaceDown = 	6	// The Face of the device is pointing down.
};

SDeclarativeScreenPrivate::SDeclarativeScreenPrivate( SDeclarativeScreen *qq, QDeclarativeEngine *engine, QDeclarativeView *view)
    : m_currentOrientation(SDeclarativeScreen::Default)
    , m_allowedOrientations(SDeclarativeScreen::Default)
    , m_displaySize(QSize(DEFAULT_WIDTH, DEFAULT_HEIGHT))
    , m_screenSize()
    , m_dpi(DEFAULT_DPI)
    , m_initialized(0)
    , m_view(view)
    , m_engine(engine)
    , q_ptr(qq)
#ifdef Q_OS_SYMBIAN
    , m_screenDevice(NULL)
#endif

{
#ifdef Q_OS_SYMBIAN
    initDisplaySize();
    if (m_view)
        m_view->setWindowState(view->windowState() | Qt::WindowFullScreen);
#elif defined(Q_WS_SIMULATOR)
    initDisplaySize();
    if (m_view)
        m_view->setWindowState(view->windowState() | Qt::WindowFullScreen);
    connect(QApplication::desktop(), SIGNAL(resized(int)), SLOT(desktopResized(int)));
#endif

    initScreenSize();

    // Init view async
    QMetaObject::invokeMethod(this, "initView", Qt::QueuedConnection);
}

SDeclarativeScreenPrivate::~SDeclarativeScreenPrivate()
{
}

SDeclarativeScreen::Orientation SDeclarativeScreenPrivate::currentOrientation() const
{
    return m_currentOrientation;
}

void SDeclarativeScreenPrivate::setCurrentOrientation(SDeclarativeScreen::Orientation orientation, bool emitSignal)
{
#ifdef Q_DEBUG_SCREEN
    qDebug() << "SDeclarativeScreenPrivate::setCurrentOrientation";
#endif
    Q_Q(SDeclarativeScreen);
    if (m_currentOrientation == orientation)
        return;

    m_currentOrientation = orientation;
    if (emitSignal) q->emit currentOrientationChanged();
}

SDeclarativeScreen::Orientations SDeclarativeScreenPrivate::allowedOrientations() const
{
    return m_allowedOrientations;
}

void SDeclarativeScreenPrivate::setAllowedOrientations(SDeclarativeScreen::Orientations orientations)
{
    Q_Q(SDeclarativeScreen);
    if(m_allowedOrientations == orientations)
        return;

    m_allowedOrientations = orientations;
    q->emit allowedOrientationsChanged();
}

QSize SDeclarativeScreenPrivate::screenSize() const
{
    return m_screenSize;
}

void SDeclarativeScreenPrivate::setScreenSize(QSize size)
{
    Q_Q(SDeclarativeScreen);
    if (m_screenSize == size)
        return;

    QSize oldSize = m_screenSize;
    m_screenSize = size;

    if (oldSize.width() != m_screenSize.width())
        q->emit widthChanged();

    if (oldSize.height() != m_screenSize.height())
        q->emit heightChanged();
}

QSize SDeclarativeScreenPrivate::displaySize() const
{
    return m_displaySize;
}

void SDeclarativeScreenPrivate::setDisplay(int width, int height, qreal dpi)
{
#ifdef Q_OS_SYMBIAN
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(dpi);
#else
    Q_Q(SDeclarativeScreen);

    bool displayChanged = false;

    if (m_displaySize.width() != width) {
        m_displaySize.setWidth(width);
        displayChanged = true;
    }

    if (m_displaySize.height() != height) {
        m_displaySize.setHeight(height);
        displayChanged = true;
    }

    if (m_dpi != dpi) {
        m_dpi = dpi;
        displayChanged = true;
    }

    if (displayChanged) {
#ifdef Q_WS_SIMULATOR
        privateSetOrientation(portraitDisplay() ? SDeclarativeScreen::Portrait : SDeclarativeScreen::Landscape);
#else
        privateSetOrientation(m_currentOrientation);
#endif
        q->emit displayChanged();
    }
#endif
}

qreal SDeclarativeScreenPrivate::dpi() const
{
    return m_dpi;
}

void SDeclarativeScreenPrivate::initView()
{
    m_initialized = true;
    setAllowedOrientations(m_allowedOrientations);
}

void SDeclarativeScreenPrivate::privateSetOrientation(int orientation)
{
    Q_UNUSED(orientation);
}

bool SDeclarativeScreenPrivate::privateSensorOrientationMethod() const
{
    return m_view && m_view->testAttribute(Qt::WA_SymbianNoSystemRotation);
}

#ifdef Q_WS_SIMULATOR
void SDeclarativeScreenPrivate::desktopResized(int screen)
{
    if( screen != QApplication::desktop()->primaryScreen())
        return;

     // Move view's scene rect to origin
    initDisplaySize();
    if (m_view) m_view->setSceneRect(0, 0, m_view->width(), m_view->height());
}
#endif

void SDeclarativeScreenPrivate::initScreenSize()
{
#ifdef Q_OS_SYMBIAN

    TPixelsTwipsAndRotation params = screenParams();
    m_screenSize = QSize(params.iPixelSize.iWidth, params.iPixelSize.iHeight);

#elif defined(Q_WS_SIMULATOR)

    m_screenSize = QApplication::desktop()->screenGeometry().size();

#endif
}

bool SDeclarativeScreenPrivate::portraitAllowed() const
{
    return m_allowedOrientations == SDeclarativeScreen::Default
        || m_allowedOrientations & (SDeclarativeScreen::Portrait | SDeclarativeScreen::PortraitInverted);
}

bool SDeclarativeScreenPrivate::landscapeAllowed() const
{
    return m_allowedOrientations == SDeclarativeScreen::Default
        || m_allowedOrientations & (SDeclarativeScreen::Landscape | SDeclarativeScreen::LandscapeInverted);
}

bool SDeclarativeScreenPrivate::portraitDisplay() const
{
    return m_displaySize.height() > m_displaySize.width();
}

#ifdef Q_OS_SYMBIAN
void SDeclarativeScreenPrivate::initDisplaySize()
{
#ifndef __WINS__
    int twipsX(0);
    int twipsY(0);
    int pixelsX(0);
    int pixelsY(0);

    if (HAL::Get(HALData::EDisplayXTwips, twipsX) == KErrNone
        && HAL::Get(HALData::EDisplayYTwips, twipsY) == KErrNone
        && HAL::Get(HALData::EDisplayXPixels, pixelsX) == KErrNone
        && HAL::Get(HALData::EDisplayYPixels, pixelsY) == KErrNone) {
        m_dpi = 0.5 * ((pixelsX / (twipsX / DEFAULT_TWIPS_PER_INCH)) +
                      (pixelsY / (twipsY / DEFAULT_TWIPS_PER_INCH)));
        m_displaySize = QSize(pixelsX, pixelsY);
    }
#endif // __WINS__
#ifdef S60_V54_OR_HIGHER
    iAvkonAppUi->SetExtendedOrientationCompatible(ETrue);
#endif
}

TPixelsTwipsAndRotation SDeclarativeScreenPrivate::screenParams()
{
    TPixelsTwipsAndRotation params;
    if (screenDevice())
        screenDevice()->GetScreenModeSizeAndRotation(screenDevice()->CurrentScreenMode(), params);
    return params;
}

CWsScreenDevice *SDeclarativeScreenPrivate::screenDevice()
{
    if (m_screenDevice)
        return m_screenDevice;

    CCoeEnv* coeEnv = CCoeEnv::Static();
    if (coeEnv)
        m_screenDevice = coeEnv->ScreenDevice();

    return m_screenDevice;
}

bool SDeclarativeScreenPrivate::deviceSupportsOnlyLandscape() {
    // Do not allow other orientations than landscape for VGA touch devices.
    if (FeatureManager::FeatureSupported(FfNativeLandscapeDisplayOrientation))
        return true;

    return false;
}

#elif defined(Q_WS_SIMULATOR)

void SDeclarativeScreenPrivate::initDisplaySize()
{
    int width = QApplication::desktop()->screenGeometry().size().width();
    int height = QApplication::desktop()->screenGeometry().size().height();
    qreal physicalDisplaySize = 0.0;

    // Guessing the display size because we have no means to figure it out in Qt Simulator
    if (width*height == 640*360)
        physicalDisplaySize = 3.5;
    else if (width*height == 640*480)
        physicalDisplaySize = 2.46;
    else if (width*height == 320*240)
        physicalDisplaySize = 2.8;
    else if (width*height == 800*480)
        physicalDisplaySize = 3.5;
    else
        physicalDisplaySize = 3.5;

    qreal width_r = width;
    qreal height_r = height;
    qreal dpi = sqrt(width_r*width_r + height_r*height_r) / physicalDisplaySize;

    setDisplay(width, height, dpi);
}
#endif
