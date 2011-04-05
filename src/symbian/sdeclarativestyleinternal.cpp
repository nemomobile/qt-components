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

#include "sdeclarativestyleinternal.h"
#include "sdeclarative.h"
#include "sdeclarativescreen.h"
#include "sstyleengine.h"

#include <QObject>

#ifdef HAVE_MOBILITY
#include <QFeedbackEffect>
#endif //HAVE_MOBILITY


class SDeclarativeStyleInternalPrivate
{
public:
    SDeclarativeStyleInternalPrivate() {}

    SStyleEngine *engine;
};

SDeclarativeStyleInternal::SDeclarativeStyleInternal(SStyleEngine *engine, QObject *parent)
    : QObject(parent),
      d_ptr(new SDeclarativeStyleInternalPrivate())
{
    Q_D(SDeclarativeStyleInternal);
    d->engine = engine;
    QObject::connect(engine, SIGNAL(layoutParametersChanged()), this, SIGNAL(layoutParametersChanged()));
    QObject::connect(engine, SIGNAL(colorParametersChanged()), this, SIGNAL(colorParametersChanged()));
}

SDeclarativeStyleInternal::~SDeclarativeStyleInternal()
{
}

int SDeclarativeStyleInternal::statusBarHeight() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("status-bar-height"));
}

int SDeclarativeStyleInternal::tabBarHeightPortrait() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("tab-bar-height-portrait"));
}

int SDeclarativeStyleInternal::tabBarHeightLandscape() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("tab-bar-height-landscape"));
}

int SDeclarativeStyleInternal::toolBarHeightPortrait() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("tool-bar-height-portrait"));
}

int SDeclarativeStyleInternal::toolBarHeightLandscape() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("tool-bar-height-landscape"));
}

int SDeclarativeStyleInternal::scrollBarThickness() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("scroll-bar-thickness"));
}

int SDeclarativeStyleInternal::sliderThickness() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("slider-thickness"));
}

int SDeclarativeStyleInternal::menuItemHeight() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("menu-item-height"));
}

int SDeclarativeStyleInternal::dialogMinSize() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("dialog-min-size"));
}

int SDeclarativeStyleInternal::dialogMaxSize() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("dialog-max-size"));
}

int SDeclarativeStyleInternal::textFieldHeight() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("text-field-height"));
}

int SDeclarativeStyleInternal::switchButtonHeight() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("switch-button-height"));
}

int SDeclarativeStyleInternal::ratingIndicatorImageWidth() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("rating-image-width"));
}

int SDeclarativeStyleInternal::ratingIndicatorImageHeight() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("rating-image-height"));
}

QColor SDeclarativeStyleInternal::listItemSeparatorColor() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->colorParameter(QLatin1String("color-list-item-separator"));
}

int SDeclarativeStyleInternal::tumblerHeightPortrait() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("tumbler-height-portrait"));
}

int SDeclarativeStyleInternal::tumblerHeightLandscape() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("tumbler-height-landscape"));
}

int SDeclarativeStyleInternal::tumblerWidth() const
{
    Q_D(const SDeclarativeStyleInternal);
    return d->engine->layoutParameter(QLatin1String("tumbler-width"));
}

void SDeclarativeStyleInternal::play(int effect)
{
#ifdef HAVE_MOBILITY
    QtMobility::QFeedbackEffect::playThemeEffect(static_cast<QtMobility::QFeedbackEffect::ThemeEffect>(effect));
#else
    Q_UNUSED(effect);
#endif //HAVE_MOBILITY
}

int SDeclarativeStyleInternal::textWidth(const QString &text, const QFont &font) const
{
    QFontMetrics metrics(font);
    return metrics.width(text);
}

int SDeclarativeStyleInternal::fontHeight(const QFont &font) const
{
    QFontMetrics metrics(font);
    return metrics.height();
}

QString SDeclarativeStyleInternal::imagePath(const QString &path) const
{
    return QLatin1String("image://theme/") + path;
}
