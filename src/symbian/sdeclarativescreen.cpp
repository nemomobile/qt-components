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

#include "sdeclarativescreen.h"
#include "sdeclarativescreen_p.h"
#include <QApplication>
#include <QResizeEvent>
#include <QDesktopWidget>

#ifdef Q_OS_SYMBIAN
#include <aknappui.h>
#include <eikenv.h>
#include <eikaufty.h>
#include <eikspane.h>
#include <avkon.rsg>
#include <hal.h>
#endif

#define Q_DEBUG_SCREEN
#ifdef Q_DEBUG_SCREEN
#include <QDebug>
#endif

static const qreal DEFAULT_TWIPS_PER_INCH = 1440.0;
static const qreal DEFAULT_DPI = 211.7;
static const int DEFAULT_WIDTH = 360;
static const int DEFAULT_HEIGHT = 640;

SDeclarativeScreenPrivate::SDeclarativeScreenPrivate(SDeclarativeScreen *qq) :
    q_ptr(qq),
    orientation(SDeclarativeScreen::Automatic),
    dpi(DEFAULT_DPI),
    screenSize(),
    settingDisplay(false),
    statusPaneChanged(false)
{
    Q_Q(SDeclarativeScreen);
#ifdef Q_OS_SYMBIAN
    QCoreApplication::setAttribute(Qt::AA_S60DontConstructApplicationPanes);
    Q_UNUSED(DEFAULT_WIDTH)
    Q_UNUSED(DEFAULT_HEIGHT)
    QSize initScreenSize = currentScreenSize();
#ifndef __WINS__
    int twipsHor(0);
    int twipsVer(0);

    if (HAL::Get(HALData::EDisplayXTwips, twipsHor) == KErrNone
        && HAL::Get(HALData::EDisplayYTwips, twipsVer) == KErrNone) {
        dpi = 0.5 * ((initScreenSize.width() / (twipsHor / DEFAULT_TWIPS_PER_INCH)) +
                     (initScreenSize.height() / (twipsVer / DEFAULT_TWIPS_PER_INCH)));
    }
#endif // __WINS__
#else
    QSize initScreenSize = QSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
#endif // Q_OS_SYMBIAN
    foreach (QWidget *w, QApplication::allWidgets()) {
        QGraphicsView *graphicsView = qobject_cast<QGraphicsView *>(w);
        if (graphicsView) {
            gv = graphicsView;

#ifndef Q_OS_SYMBIAN
            // Init view asynchronously
            QMetaObject::invokeMethod(q, "_q_initView", Qt::QueuedConnection, Q_ARG(QSize, initScreenSize));
#else
            // Install event filter for observing graphics view resizes
            gv->installEventFilter(q);
            // Observe desktop resizes
            q->connect(QApplication::desktop(), SIGNAL(resized(int)), SLOT(_q_desktopResized(int)), Qt::QueuedConnection);
            // Set initial scene rect
            gv->setSceneRect(0, 0, gv->width(), gv->height());
            // Update screen size asynchronously
            QMetaObject::invokeMethod(q, "_q_updateScreenSize", Qt::QueuedConnection, Q_ARG(QSize, initScreenSize));
#endif
            break;
        }
    }
}

SDeclarativeScreenPrivate::~SDeclarativeScreenPrivate()
{
}

void SDeclarativeScreenPrivate::updateOrientationAngle()
{
    Q_Q(SDeclarativeScreen);
    SDeclarativeScreen::Orientation oldOrientation = orientation;
    SDeclarativeScreen::Orientation newOrientation(SDeclarativeScreen::Portrait);

    if (isLandscapeScreen())
        newOrientation = SDeclarativeScreen::Landscape;

    orientation = newOrientation;
    if (orientation != oldOrientation)
        emit q->orientationChanged();
}

void SDeclarativeScreenPrivate::_q_updateScreenSize(const QSize &size)
{
    Q_Q(SDeclarativeScreen);
#ifdef Q_OS_SYMBIAN
    Q_UNUSED(size);
    QSize newSize(currentScreenSize());
#else
    QSize newSize(size);
    if (newSize.isEmpty())
        newSize = screenSize;
#endif

    if (newSize == screenSize)
        return;

    screenSize = newSize;
    if (!settingDisplay)
        emit q->displayChanged();
    updateOrientationAngle();
}

#ifndef Q_OS_SYMBIAN
void SDeclarativeScreenPrivate::_q_initView(const QSize &size)
{
    Q_Q(SDeclarativeScreen);
    // Install event filter for observing graphics view resizes
    gv->installEventFilter(q);
    // Emulate the resizing done by the system
    gv->resize(adjustedSize(size, orientation));
}
#endif

void SDeclarativeScreenPrivate::_q_desktopResized(int screen)
{
    Q_UNUSED(screen)

#ifdef Q_OS_SYMBIAN
    if (screen == QApplication::desktop()->primaryScreen()) {
        QSize current = currentScreenSize();
        // Move view's scene rect to origin
        if (gv)
            gv->setSceneRect(0, 0, gv->width(), gv->height());

        _q_updateScreenSize(current);
    }
#endif
}

bool SDeclarativeScreenPrivate::isLandscapeScreen() const
{
    return (screenSize.width() > screenSize.height());
}

QSize SDeclarativeScreenPrivate::currentScreenSize() const
{
#ifdef Q_OS_SYMBIAN
    TPixelsTwipsAndRotation params;
    CWsScreenDevice *screenDevice = CEikonEnv::Static()->ScreenDevice();
    int mode = screenDevice->CurrentScreenMode();
    screenDevice->GetScreenModeSizeAndRotation(mode, params);
    QSize nSize(params.iPixelSize.iWidth, params.iPixelSize.iHeight);
    return nSize;
#else
    return screenSize;
#endif
}

QSize SDeclarativeScreenPrivate::adjustedSize(const QSize &size, SDeclarativeScreen::Orientation o) const
{
    int shortEdge = qMin(size.width(), size.height());
    int longEdge = qMax(size.width(), size.height());
    QSize newSize(shortEdge, longEdge);
    if (o == SDeclarativeScreen::Landscape || o == SDeclarativeScreen::LandscapeInverted)
        newSize.transpose();
    return newSize;
}

#ifdef Q_OS_SYMBIAN
void SDeclarativeScreenPrivate::setStatusPaneLayout()
{
    Q_Q(SDeclarativeScreen);
    // Change to flat status pane layout to make more space for QML view.
    if (!statusPaneChanged) {
        MEikAppUiFactory *eikAppUiFactory = CEikonEnv::Static()->AppUiFactory();
        if (eikAppUiFactory) {
            CEikStatusPane *eikStatusPane = eikAppUiFactory->StatusPane();
            if (eikStatusPane && eikStatusPane->CurrentLayoutResId()
                    == R_AVKON_STATUS_PANE_LAYOUT_USUAL_EXT) {
                QT_TRAP_THROWING(eikStatusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL_FLAT));
                statusPaneChanged = true;
                emit q->statusPaneChanged();
            }
        }
    }
}
#endif

SDeclarativeScreen::SDeclarativeScreen(QDeclarativeItem *parent)
    : QObject(parent), d_ptr(new SDeclarativeScreenPrivate(this))
{
}

SDeclarativeScreen::~SDeclarativeScreen()
{
}

SDeclarativeScreen::Orientation SDeclarativeScreen::orientation() const
{
    Q_D(const SDeclarativeScreen);
    return d->orientation;
}

void SDeclarativeScreen::setOrientation(Orientation orientation)
{
    Q_D(SDeclarativeScreen);
#ifdef Q_OS_SYMBIAN
    switch (orientation) {
    case SDeclarativeScreen::Portrait:
    case SDeclarativeScreen::PortraitInverted:
        TRAP_IGNORE(iAvkonAppUi->SetOrientationL(CAknAppUiBase::EAppUiOrientationPortrait));
        break;
    case SDeclarativeScreen::Landscape:
    case SDeclarativeScreen::LandscapeInverted:
        TRAP_IGNORE(iAvkonAppUi->SetOrientationL(CAknAppUiBase::EAppUiOrientationLandscape));
        break;
    case SDeclarativeScreen::Automatic:
        // dynamic property for RnD purposes
        TRAP_IGNORE(iAvkonAppUi->SetOrientationL(CAknAppUiBase::EAppUiOrientationAutomatic));
        break;
    default:
        break;
    }
#else
    if (!d->gv) {
        // View not initialized yet. Just store the orientation enum.
        d->orientation = orientation;
        emit orientationChanged();
        return;
    }
    QSize newSize(d->adjustedSize(d->screenSize, orientation));
    if (newSize != d->screenSize) {
        if (d->gv)
            d->gv->resize(newSize); // emulate the resizing done by the system
    }
#endif
}

int SDeclarativeScreen::rotation() const
{
    Q_D(const SDeclarativeScreen);
    int angle = 0;
    switch (d->orientation) {
    case Portrait:
        angle = 0;
        break;
    case Landscape:
        angle = 270;
        break;
    case PortraitInverted:
        angle = 180;
        break;
    case LandscapeInverted:
        angle = 90;
        break;
    case Automatic:
        break;
    default:
        break;
    }
    return angle;
}

int SDeclarativeScreen::width() const
{
    Q_D(const SDeclarativeScreen);
    return d->screenSize.width();
}

int SDeclarativeScreen::height() const
{
    Q_D(const SDeclarativeScreen);
    return d->screenSize.height();
}

qreal SDeclarativeScreen::dpi() const
{
    Q_D(const SDeclarativeScreen);
    return d->dpi;
}

void SDeclarativeScreen::setDisplay(int width, int height, qreal dpi)
{
#ifdef Q_OS_SYMBIAN
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(dpi);
#else
    Q_D(SDeclarativeScreen);
    d->settingDisplay = true;
    d->dpi = dpi;
    if (d->gv)
        d->gv->resize(width, height); // emulate the resizing done by the system
    emit displayChanged();
    d->settingDisplay = false;
#endif
}

bool SDeclarativeScreen::eventFilter(QObject *obj, QEvent *event)
{
    Q_D(SDeclarativeScreen);
    if (event->type() == QEvent::Resize) {
#ifdef Q_OS_SYMBIAN
        // Statuspane layout cannot be changed in the constructor because the statuspane
        // has not been created yet.
        d->setStatusPaneLayout();
#endif
        QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);
        if (resizeEvent->size() == d->screenSize)
            return false;
        if (d->gv)
            d->gv->setSceneRect(0, 0, d->gv->width(), d->gv->height());

        d->_q_updateScreenSize(resizeEvent->size());
        return false;
    } else {
        return QObject::eventFilter(obj, event);
    }
}

#include "moc_sdeclarativescreen.cpp"
