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
#include "sdeclarativemagnifier.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QImageReader>
#include <QPixmapCache>

class SDeclarativeMagnifierPrivate
{

    Q_DECLARE_PUBLIC(SDeclarativeMagnifier)

public:
    SDeclarativeMagnifierPrivate(SDeclarativeMagnifier *qq):
        mScaleFactor(1), q_ptr(qq) {}
    ~SDeclarativeMagnifierPrivate() {}

    void init();
    void preparePixmaps();

    QRectF mSourceRect;
    qreal mScaleFactor;
    QPixmap mMask;
    QPixmap mOverlay;

    static QPixmap *mSource;
    static QString mOverlayFileName;
    static QString mMaskFileName;
    static const QString mMaskKey;
    static const QString mOverlayKey;

    SDeclarativeMagnifier *q_ptr;
};

QPixmap *SDeclarativeMagnifierPrivate::mSource = 0;
QString SDeclarativeMagnifierPrivate::mOverlayFileName = QString();
QString SDeclarativeMagnifierPrivate::mMaskFileName = QString();
const QString SDeclarativeMagnifierPrivate::mMaskKey = "sdmagnifierprivate_mmask";
const QString SDeclarativeMagnifierPrivate::mOverlayKey = "sdmagnifierprivate_moverlay";

void SDeclarativeMagnifierPrivate::init()
{
    Q_Q(SDeclarativeMagnifier);
    q->setFlag(QGraphicsItem::ItemHasNoContents, false);
}

void SDeclarativeMagnifierPrivate::preparePixmaps()
{
    QSize sourceSize = mSourceRect.size().toSize();

    if (mOverlay.size() != sourceSize
        && !mOverlayFileName.isEmpty()
        && (!QPixmapCache::find(mOverlayKey, &mOverlay) || mOverlay.size() != sourceSize))
    {
        QImageReader overlayReader(mOverlayFileName);
        overlayReader.setScaledSize(sourceSize);
        if (overlayReader.canRead())
            mOverlay = QPixmap::fromImage(overlayReader.read());
        QPixmapCache::insert(mOverlayKey, mOverlay);
    }

    if (mMask.size() != sourceSize
        && !mMaskFileName.isEmpty()
        && (!QPixmapCache::find(mMaskKey, &mMask) || mMask.size() != sourceSize))
    {
        QImageReader maskReader(mMaskFileName);
        maskReader.setScaledSize(sourceSize);
        if (maskReader.canRead())
            mMask = QPixmap::fromImage(maskReader.read());
        QPixmapCache::insert(mMaskKey, mMask);
    }

    if (!mSource || mSource->size() != sourceSize)
    {
        delete mSource;
        mSource = new QPixmap(sourceSize);
        mSource->fill(Qt::transparent);
    }
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
    Q_D(SDeclarativeMagnifier);
    delete d->mSource;
    d->mSource = 0;
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
        d->mOverlay = QPixmap();
        QPixmapCache::remove(d->mOverlayKey);
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
        d->mMask = QPixmap();
        QPixmapCache::remove(d->mMaskKey);
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

    // Cancel painting if source rectangle has not been set
    if (d->mSourceRect.size().toSize().isEmpty()) return;

    // Do not paint a magnifier inside a magnifier
    static bool inPaint = false;
    if (!inPaint) {
        inPaint = true;
        d->preparePixmaps();

        QPainter sourcePainter(d->mSource);
        QRectF targetRect = QRectF(QPointF(0, 0), d->mSourceRect.size());

        scene()->render(&sourcePainter, targetRect, d->mSourceRect);
        sourcePainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        sourcePainter.drawPixmap(0, 0, d->mMask);
        sourcePainter.setCompositionMode(QPainter::CompositionMode_Multiply);
        sourcePainter.drawPixmap(0, 0, d->mOverlay);
        sourcePainter.end();

        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->drawPixmap(0, 0, boundingRect().width(), boundingRect().height(), *d->mSource);
        inPaint = false;
    }
}


