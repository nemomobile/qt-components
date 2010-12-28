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

#include <QApplication>

#ifdef Q_OS_SYMBIAN
#include <displaylayoutmetrics.cdl.h>
#include <aknappui.h>
#endif

#define Q_DEBUG_SCREEN
#ifdef Q_DEBUG_SCREEN
#include <QDebug>
#endif

static const qreal MM_PER_INCH = 25.4;
static const qreal PPI = 229;
static const qreal UNIT = 6.75;

class SDeclarativeScreenPrivate
{
    Q_DECLARE_PUBLIC(SDeclarativeScreen)

public:
    SDeclarativeScreenPrivate(SDeclarativeScreen *qq);
    ~SDeclarativeScreenPrivate();

    void _q_updateOrientationAngle();
    void _q_updateScreenSize(const QSize&);
    bool isLandscapeScreen() const;
    QSize currentScreenSize() const;

    SDeclarativeScreen *q_ptr;

    SDeclarativeScreen::Orientation orientation;

    qreal ppi;
    qreal ppmm;
    qreal unit;
    QSizeF physicalSize;

    QSize screenSize;
    int activeDisplayProfile;
    bool activatingDisplayProfile;
};

SDeclarativeScreenPrivate::SDeclarativeScreenPrivate(SDeclarativeScreen *qq)
    : q_ptr(qq)
    ,orientation(SDeclarativeScreen::Automatic)
    ,ppi(PPI)
    ,ppmm(PPI / MM_PER_INCH)
    ,unit(UNIT)
    ,screenSize()
    ,activeDisplayProfile(0)
    ,activatingDisplayProfile(false)
{
#ifdef Q_OS_SYMBIAN
    screenSize = currentScreenSize();
    unit = Display_Layout_Metrics::UnitValue();
#endif

#ifdef Q_DEBUG_SCREEN
    qDebug() << "SDeclarativeScreenPrivate constructed with size: " << screenSize;
#endif
    physicalSize = QSizeF(screenSize.width(), screenSize.height());
    physicalSize /= ppmm;
}

SDeclarativeScreenPrivate::~SDeclarativeScreenPrivate()
{
}

void SDeclarativeScreenPrivate::_q_updateOrientationAngle()
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
    if (!newSize.isValid())
        newSize = screenSize;
#endif
    screenSize = newSize;
    physicalSize = screenSize/ppmm;

#ifdef Q_DEBUG_SCREEN
    qDebug() << "_q_updateScreenSize new screen size is: " << screenSize;
#endif

    emit q->sizeChanged();

    _q_updateOrientationAngle();
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

QString SDeclarativeScreen::orientationString() const
{
    Q_D(const SDeclarativeScreen);
    int index = metaObject()->indexOfEnumerator("Orientation");
    Q_ASSERT(index != -1);
    QMetaEnum enumerator = metaObject()->enumerator(index);
    return QLatin1String(enumerator.valueToKey(d->orientation));
}

void SDeclarativeScreen::setOrientation(Orientation orientation)
{
    Q_D(SDeclarativeScreen);
    if (orientation != SDeclarativeScreen::Automatic) {
        d->orientation = orientation;
    }

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
#endif

    emit orientationChanged();
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

bool SDeclarativeScreen::isMinimized() const
{
    QWidget *top = QApplication::activeWindow();
    return !top || top->isMinimized();
}

void SDeclarativeScreen::setMinimized(bool minimized)
{
    QWidget *top = QApplication::activeWindow();
    if (!top)
        return;
    if (minimized)
        top->setWindowState(top->windowState() | Qt::WindowMinimized);
    else
        top->setWindowState(top->windowState() & ~Qt::WindowMinimized);
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

qreal SDeclarativeScreen::ppi() const
{
    Q_D(const SDeclarativeScreen);
    return d->ppi;
}

qreal SDeclarativeScreen::ppmm() const
{
    Q_D(const SDeclarativeScreen);
    return d->ppmm;
}

qreal SDeclarativeScreen::unit() const
{
    Q_D(const SDeclarativeScreen);
    return d->unit;
}

QSizeF SDeclarativeScreen::physicalSize() const
{
    Q_D(const SDeclarativeScreen);
    return d->physicalSize;
}

#include "moc_sdeclarativescreen.cpp"
