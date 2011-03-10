/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#define QT_NO_CAST_FROM_ASCII
#define QT_NO_CAST_TO_ASCII

#include "sdeclarativestyle.h"
#include "sstyleengine.h"
#include "sdeclarative.h"
#include "sdeclarativescreen.h"

#include <QObject>

class SDeclarativeStylePrivate
{
public:
    SDeclarativeStylePrivate() {}

    SStyleEngine *engine;
};

SDeclarativeStyle::SDeclarativeStyle(SStyleEngine *engine, QObject *parent)
    : QObject(parent),
      d_ptr(new SDeclarativeStylePrivate())
{
    Q_D(SDeclarativeStyle);
    d->engine = engine;
    QObject::connect(engine, SIGNAL(fontParametersChanged()), this, SIGNAL(fontParametersChanged()));
    QObject::connect(engine, SIGNAL(layoutParametersChanged()), this, SIGNAL(layoutParametersChanged()));
    QObject::connect(engine, SIGNAL(colorParametersChanged()), this, SIGNAL(colorParametersChanged()));
}

SDeclarativeStyle::~SDeclarativeStyle()
{
}

QString SDeclarativeStyle::fontFamilyRegular() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->fontFamilyParameter(QLatin1String("font-family-regular"));
}

int SDeclarativeStyle::fontSizeLarge() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("font-size-large"));
}

int SDeclarativeStyle::fontSizeMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("font-size-medium"));
}

int SDeclarativeStyle::fontSizeSmall() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("font-size-small"));
}

int SDeclarativeStyle::graphicSizeLarge() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-large"));
}

int SDeclarativeStyle::graphicSizeMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-medium"));
}

int SDeclarativeStyle::graphicSizeSmall() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-small"));
}

int SDeclarativeStyle::graphicSizeTiny() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("graphic-size-tiny"));
}

int SDeclarativeStyle::paddingLarge() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("padding-large"));
}

int SDeclarativeStyle::paddingMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("padding-medium"));
}

int SDeclarativeStyle::paddingSmall() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("padding-small"));
}

int SDeclarativeStyle::borderSizeMedium() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->layoutParameter(QLatin1String("border-size-medium"));
}

QColor SDeclarativeStyle::colorNormalLight() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-light"));
}

QColor SDeclarativeStyle::colorNormalMid() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-mid"));
}

QColor SDeclarativeStyle::colorNormalDark() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-dark"));
}

QColor SDeclarativeStyle::colorNormalLink() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-normal-link"));
}

QColor SDeclarativeStyle::colorPressed() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-pressed"));
}

QColor SDeclarativeStyle::colorChecked() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-checked"));
}

QColor SDeclarativeStyle::colorHighlighted() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-highlighted"));
}

QColor SDeclarativeStyle::colorDisabledLight() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-light"));
}

QColor SDeclarativeStyle::colorDisabledMid() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-mid"));
}

QColor SDeclarativeStyle::colorDisabledDark() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-disabled-dark"));
}

QColor SDeclarativeStyle::colorTextSelection() const
{
    Q_D(const SDeclarativeStyle);
    return d->engine->colorParameter(QLatin1String("color-text-selection"));
}
