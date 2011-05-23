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
#include "sdeclarativemagnifier.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>


class SDeclarativeMagnifierPrivate
{

    Q_DECLARE_PUBLIC(SDeclarativeMagnifier)

public:
    SDeclarativeMagnifierPrivate(SDeclarativeMagnifier *qq):
        mScaleFactor(1), q_ptr(qq) {}
    ~SDeclarativeMagnifierPrivate() {}

    void init();
    QPixmap ovelay();
    void drawOverlay(QPainter *painter);
    QPixmap mask();
    void drawMask(QPainter *painter);
    void drawContent(QPainter *painter);

    QRectF mSourceRect;
    qreal mScaleFactor;
    QPixmap mMask;
    QPixmap mOverlay;
    QString mOverlayFileName;
    QString mMaskFileName;

    SDeclarativeMagnifier *q_ptr;
};


void SDeclarativeMagnifierPrivate::init()
{
    Q_Q(SDeclarativeMagnifier);
    q->setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QPixmap SDeclarativeMagnifierPrivate::ovelay()
{
    Q_Q(SDeclarativeMagnifier);
    QSize magnifierSize = q->boundingRect().size().toSize();

    if (mOverlay.size() == magnifierSize)
        return mOverlay;

    mOverlay = mOverlay.scaled(magnifierSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return mOverlay;
}

void SDeclarativeMagnifierPrivate::drawOverlay(QPainter *painter)
{
    painter->setCompositionMode(QPainter::CompositionMode_Darken);
    painter->drawPixmap(0, 0, ovelay());
}

QPixmap SDeclarativeMagnifierPrivate::mask()
{
    Q_Q(SDeclarativeMagnifier);
    QSize magnifierSize = q->boundingRect().size().toSize();

    if (mMask.size() == magnifierSize)
        return mMask;

    mMask = mMask.scaled(magnifierSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return mMask;
}

void SDeclarativeMagnifierPrivate::drawMask(QPainter *painter)
{
    painter->setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter->drawPixmap(0, 0, mask());
}

void SDeclarativeMagnifierPrivate::drawContent(QPainter *painter)
{
    Q_Q(SDeclarativeMagnifier);

    QPixmap sourcePixmap(mSourceRect.size().toSize());
    sourcePixmap.fill(Qt::black);
    QPainter sourcePainter(&sourcePixmap);

    QRectF targetRect = QRectF(QPointF(0, 0), mSourceRect.size());
    q->scene()->render(&sourcePainter, targetRect, mSourceRect);
    sourcePainter.end();

    painter->save();
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);

    QTransform transform;
    transform.scale(mScaleFactor, mScaleFactor);
    painter->setTransform(transform);
    painter->drawPixmap(0, 0, sourcePixmap);
    painter->restore();
}


SDeclarativeMagnifier::SDeclarativeMagnifier(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    d_ptr(new SDeclarativeMagnifierPrivate(this))
{
    Q_D(SDeclarativeMagnifier);
    d->init();
}

SDeclarativeMagnifier::~SDeclarativeMagnifier()
{
}

void SDeclarativeMagnifier::setSourceRect(const QRectF &rect)
{
    Q_D(SDeclarativeMagnifier);

    if (rect != d->mSourceRect) {
        d->mSourceRect = rect;
        update();
        emit sourceRectChanged();
    }
}

QRectF SDeclarativeMagnifier::sourceRect() const
{
    Q_D(const SDeclarativeMagnifier);
    return d->mSourceRect;
}

void SDeclarativeMagnifier::setScaleFactor(qreal scaleFactor)
{
    Q_D(SDeclarativeMagnifier);

    if (scaleFactor != d->mScaleFactor) {
        d->mScaleFactor = scaleFactor;
        update();
        emit scaleFactorChanged();
    }
}

qreal SDeclarativeMagnifier::scaleFactor() const
{
    Q_D(const SDeclarativeMagnifier);

    return d->mScaleFactor;
}

void SDeclarativeMagnifier::setOverlayFileName(const QString &overlayFileName)
{
    Q_D(SDeclarativeMagnifier);

    if (overlayFileName != d->mOverlayFileName) {
        d->mOverlayFileName = overlayFileName;
        d->mOverlay.load(d->mOverlayFileName);
        update();
        emit overlayFileNameChanged();
    }
}

QString SDeclarativeMagnifier::overlayFileName() const
{
    Q_D(const SDeclarativeMagnifier);

    return d->mOverlayFileName;
}

void SDeclarativeMagnifier::setMaskFileName(const QString &maskFileName)
{
    Q_D(SDeclarativeMagnifier);

    if (maskFileName != d->mMaskFileName) {
        d->mMaskFileName = maskFileName;
        d->mMask.load(d->mMaskFileName);
        update();
        emit maskFileNameChanged();
    }
}

QString SDeclarativeMagnifier::maskFileName() const
{
    Q_D(const SDeclarativeMagnifier);

    return d->mMaskFileName;
}

void SDeclarativeMagnifier::paint(QPainter *painter, const QStyleOptionGraphicsItem *styleOption, QWidget *widget)
{
    Q_UNUSED(styleOption)
    Q_UNUSED(widget)

    Q_D(SDeclarativeMagnifier);

    static bool inPaint = false;
    if (!inPaint) {
        inPaint = true;

        QPixmap contentPixmap(boundingRect().size().toSize());
        contentPixmap.fill(Qt::transparent);
        QPainter pixmapPainter(&contentPixmap);

        d->drawContent(&pixmapPainter);
        d->drawMask(&pixmapPainter);
        d->drawOverlay(&pixmapPainter);

        pixmapPainter.end();
        painter->drawPixmap(0, 0, contentPixmap);
        inPaint = false;
    }
}


