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
#include "sdeclarativescreen_p_resize.h"
#include "sdeclarativescreen_p_sensor.h"
#include <QApplication>
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <qmath.h>
#include <qnamespace.h>

static const qreal CATEGORY_SMALL_LIMIT = 3.2;
static const qreal CATEGORY_MEDIUM_LIMIT = 4.5;
static const qreal CATEGORY_LARGE_LIMIT = 7.0;
static const qreal DENSITY_SMALL_LIMIT = 140.0;
static const qreal DENSITY_MEDIUM_LIMIT = 180.0;
static const qreal DENSITY_LARGE_LIMIT = 270.0;

SDeclarativeScreen::SDeclarativeScreen(QDeclarativeEngine *engine, QObject *parent)
    : QObject(parent)
{
    // retrieve view pointer set in SymbianPlugin
    QDeclarativeView *declarativeView = NULL;
    QVariant declarativeViewPtr = parent->property("declarativeViewPtr");

    if (declarativeViewPtr.isValid())
        declarativeView  = qobject_cast<QDeclarativeView *>(declarativeViewPtr.value<QObject *>());

    if (declarativeView && declarativeView->testAttribute(Qt::WA_SymbianNoSystemRotation))
       d_ptr.reset(new SDeclarativeScreenPrivateSensor(this, engine, declarativeView));
    else
       d_ptr.reset(new SDeclarativeScreenPrivateResize(this, engine, declarativeView));
}

SDeclarativeScreen::~SDeclarativeScreen()
{
}

SDeclarativeScreen::Orientation SDeclarativeScreen::currentOrientation() const
{
    Q_D(const SDeclarativeScreen);
    return d->currentOrientation();
}

SDeclarativeScreen::Orientations SDeclarativeScreen::allowedOrientations() const
{
    Q_D(const SDeclarativeScreen);
    return d->allowedOrientations();
}

void SDeclarativeScreen::setAllowedOrientations(Orientations orientations)
{
    Q_D(SDeclarativeScreen);
    d->setAllowedOrientations(orientations);
}

int SDeclarativeScreen::rotation() const
{
    Q_D(const SDeclarativeScreen);

    int angle = 0;
    bool diplayPortrait = d->portraitDisplay();

    switch (d->currentOrientation()) {
    case Portrait:
        angle = diplayPortrait ? 0 : 90;
        break;
    case Landscape:
        angle = diplayPortrait ? 270 : 0;
        break;
    case PortraitInverted:
        angle = diplayPortrait ? 180 : 270;
        break;
    case LandscapeInverted:
        angle = diplayPortrait ? 90 : 180;
        break;
    default:
        break;
    }

    return angle;
}

int SDeclarativeScreen::width() const
{
    Q_D(const SDeclarativeScreen);
    return d->screenSize().width();
}

int SDeclarativeScreen::height() const
{
    Q_D(const SDeclarativeScreen);
    return d->screenSize().height();
}

int SDeclarativeScreen::displayWidth() const
{
    Q_D(const SDeclarativeScreen);
    return d->displaySize().width();
}

int SDeclarativeScreen::displayHeight() const
{
    Q_D(const SDeclarativeScreen);
    return d->displaySize().height();
}

qreal SDeclarativeScreen::dpi() const
{
    Q_D(const SDeclarativeScreen);
    return d->dpi();
}

SDeclarativeScreen::DisplayCategory SDeclarativeScreen::displayCategory() const
{
    Q_D(const SDeclarativeScreen);

    const int w = d->displaySize().width();
    const int h = d->displaySize().height();

    const qreal diagonal = qSqrt(static_cast<qreal>(w * w + h * h)) / d->dpi();

    if (diagonal < CATEGORY_SMALL_LIMIT)
        return Small;
    else if (diagonal < CATEGORY_MEDIUM_LIMIT)
        return Normal;
    else if (diagonal < CATEGORY_LARGE_LIMIT)
        return Large;
    else
        return ExtraLarge;
}

SDeclarativeScreen::Density SDeclarativeScreen::density() const
{
    Q_D(const SDeclarativeScreen);

    if (d->dpi() < DENSITY_SMALL_LIMIT)
        return Low;
    else if (d->dpi() < DENSITY_MEDIUM_LIMIT)
        return Medium;
    else if (d->dpi() < DENSITY_LARGE_LIMIT)
        return High;
    else
        return ExtraHigh;
}

void SDeclarativeScreen::privateSetDisplay(int width, int height, qreal dpi)
{
    Q_D(SDeclarativeScreen);
    d->setDisplay(width, height, dpi);
}

void SDeclarativeScreen::privateSetOrientation(int orientation)
{
    Q_D(SDeclarativeScreen);
    d->privateSetOrientation(orientation);
}

bool SDeclarativeScreen::privateSensorOrientationMethod() const
{
    Q_D(const SDeclarativeScreen);
    return d->privateSensorOrientationMethod();
}

#include "moc_sdeclarativescreen.cpp"
