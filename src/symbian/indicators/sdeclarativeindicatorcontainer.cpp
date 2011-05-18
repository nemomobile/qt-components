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

#include "sdeclarativeindicatorcontainer.h"
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
#include "sdeclarativeindicatordatahandler.h"
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

class SDeclarativeIndicatorContainerPrivate
{
public:
    SDeclarativeIndicatorContainerPrivate();
    ~SDeclarativeIndicatorContainerPrivate();

#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    CSDeclarativeIndicatorDataHandler *dataHandler;
#else
    int *dataHandler; // dummy
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

    QColor indicatorColor;
    QSize indicatorSize;
    int indicatorPadding;
    int maxIndicatorCount;
    bool layoutRequestPending;

};

SDeclarativeIndicatorContainerPrivate::SDeclarativeIndicatorContainerPrivate()
    : dataHandler(0), layoutRequestPending(false)
{
}

SDeclarativeIndicatorContainerPrivate::~SDeclarativeIndicatorContainerPrivate()
{
    delete dataHandler;
}


SDeclarativeIndicatorContainer::SDeclarativeIndicatorContainer(QDeclarativeItem *parent)
    : SDeclarativeImplicitSizeItem(parent), d_ptr(new SDeclarativeIndicatorContainerPrivate)
{
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    QT_TRAP_THROWING(d_ptr->dataHandler = CSDeclarativeIndicatorDataHandler::NewL(this));
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

    connect(this, SIGNAL(indicatorSizeChanged()), this, SLOT(layoutChildren()));
    connect(this, SIGNAL(indicatorPaddingChanged(int)), this, SLOT(layoutChildren()));
    connect(this, SIGNAL(maxIndicatorCountChanged(int)), this, SLOT(layoutChildren()));
}

SDeclarativeIndicatorContainer::~SDeclarativeIndicatorContainer()
{
}

QColor SDeclarativeIndicatorContainer::indicatorColor() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorColor;
}

void SDeclarativeIndicatorContainer::setIndicatorColor(const QColor &color)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorColor != color) {
        d->indicatorColor = color;
        emit indicatorColorChanged(d->indicatorColor);
    }
}

int SDeclarativeIndicatorContainer::indicatorWidth() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorSize.width();
}

void SDeclarativeIndicatorContainer::setIndicatorWidth(int width)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorSize.width() != width) {
        d->indicatorSize.setWidth(width);
        emit indicatorSizeChanged();
    }
}

int SDeclarativeIndicatorContainer::indicatorHeight() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorSize.height();
}

void SDeclarativeIndicatorContainer::setIndicatorHeight(int height)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorSize.height() != height) {
        d->indicatorSize.setHeight(height);
        emit indicatorSizeChanged();
    }
}

int SDeclarativeIndicatorContainer::indicatorPadding() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->indicatorPadding;
}

void SDeclarativeIndicatorContainer::setIndicatorPadding(int padding)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->indicatorPadding != padding) {
        d->indicatorPadding = padding;
        emit indicatorPaddingChanged(d->indicatorPadding);
    }
}

int SDeclarativeIndicatorContainer::maxIndicatorCount() const
{
    Q_D(const SDeclarativeIndicatorContainer);
    return d->maxIndicatorCount;
}

void SDeclarativeIndicatorContainer::setMaxIndicatorCount(int maxCount)
{
    Q_D(SDeclarativeIndicatorContainer);
    if (d->maxIndicatorCount != maxCount) {
        d->maxIndicatorCount = maxCount;
        emit maxIndicatorCountChanged(d->maxIndicatorCount);
    }
}

void SDeclarativeIndicatorContainer::layoutChildren()
{
    Q_D(SDeclarativeIndicatorContainer);
    if (!d->layoutRequestPending) {
        d->layoutRequestPending = true;
        QMetaObject::invokeMethod(this, "doLayoutChildren", Qt::QueuedConnection);
    }
}

void SDeclarativeIndicatorContainer::doLayoutChildren()
{
    Q_D(SDeclarativeIndicatorContainer);

    int xPosition = 0;
    int itemsShown = 0;
    const QSize itemSize(d->indicatorSize);

    for (int i = 0; i < childItems().count(); i++) {
        QDeclarativeItem *child = qobject_cast<QDeclarativeItem *>(childItems().at(i)->toGraphicsObject());
        if (child && child->isVisible()) {
            if (itemsShown >= d->maxIndicatorCount && d->maxIndicatorCount >= 0) {
                child->setSize(QSize(0, 0));
                continue;
            }

            if (itemsShown++)
                xPosition += d->indicatorPadding;

            child->setPos(xPosition, 0);
            child->setSize(itemSize);

            xPosition += child->width();
        }
    }

    setImplicitWidthNotify(xPosition);
    setImplicitHeightNotify(itemSize.height());
    d->layoutRequestPending = false;
}

