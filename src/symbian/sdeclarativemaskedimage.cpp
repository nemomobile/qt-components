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

#include <qpainter.h>
#include <qapplication.h>
#include <qbitmap.h>
#include <qmargins.h>

#include "sdeclarativemaskedimage.h"
#include "sdeclarativemaskedimage_p.h"
#include "sframepool.h"
#include "siconpool.h"
#include "sdeclarative.h"

SDeclarativeMaskedImagePrivate::~SDeclarativeMaskedImagePrivate()
{
    releasePixmaps();
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createTiledPixmap()
{
    Q_Q(SDeclarativeMaskedImage);

    const int width = static_cast<int>(q->width());
    const int height = static_cast<int>(q->height());

    IconParams params;
    params.fileName = SDeclarative::resolveIconFileName(imageName);
    params.size = QSize(width, height);

    QPixmap tile = SIconPool::get(params.fileName, params.size);
    tileSize = tile.width();

    pixmap = QPixmap(width, height);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    // Fill using tiling
    painter.fillRect(QRect(0, 0, width, height), QBrush(tile));
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createNonTiledPixmap()
{
    Q_Q(SDeclarativeMaskedImage);

    IconParams params;
    params.fileName = SDeclarative::resolveIconFileName(imageName);
    params.size = QSize(q->width(), q->height());

    pixmap = SIconPool::get(params.fileName, params.size);
    requestedFromIconPool.append(params);
    tileSize = pixmap.width();
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createMask()
{
    Q_Q(SDeclarativeMaskedImage);

    const int width = static_cast<int>(q->width());
    const int height = static_cast<int>(q->height());

    // paint the mask from its frame graphics to the mask pixmap
    mask = QBitmap(width, height);
    mask.fill(Qt::white);
    QPainter painter(&mask);

    // If margins is set, draw the mask like QML BorderImage draws the image.
    if (topMargin >= 0 && bottomMargin >= 0 && leftMargin >= 0 && rightMargin >= 0) {
        IconParams params;
        params.fileName = SDeclarative::resolveIconFileName(maskName);
        params.size = QSize();

        QPixmap mask = SIconPool::get(params.fileName, params.size);
        requestedFromIconPool.append(params);

        QMargins margins(leftMargin, topMargin, rightMargin, bottomMargin);
        QTileRules rules(Qt::StretchTile, Qt::StretchTile);
        qDrawBorderPixmap(&painter, QRect(0, 0, width, height), margins, mask, mask.rect(), margins, rules);
    }

    else {
        // TODO: this whole branch is deprecated functionality, removed soon.

        switch (maskType) {
        case SDeclarativeFrame::OnePiece: {
            QPixmap mask;
            QString maskFileName = QString(":/graphics/%1.svg").arg(maskName);

            if (!mask.load(maskFileName))
                qWarning() << "Fail to load icon: " << maskFileName;

            painter.drawPixmap(QRect(0, 0, mask.width(), mask.height()), mask);
            break;
        }
        case SDeclarativeFrame::ThreePiecesHorizontal: {
            QPixmap maskLeft;
            QPixmap maskCenter;
            QPixmap maskRight;

            QString maskLeftFileName = QString(":/graphics/%1_l.svg").arg(maskName);
            QString maskCenterFileName = QString(":/graphics/%1_c.svg").arg(maskName);
            QString maskRightFileName = QString(":/graphics/%1_r.svg").arg(maskName);

            if (!maskLeft.load(maskLeftFileName))
                qWarning() << "Fail to load icon: " << maskLeftFileName;
            if (!maskCenter.load(maskCenterFileName))
                qWarning() << "Fail to load icon: " << maskCenterFileName;
            if (!maskRight.load(maskRightFileName))
                qWarning() << "Fail to load icon: " << maskRightFileName;

            const int borderWidth = maskLeft.width();

            painter.drawPixmap(QRect(0, 0, borderWidth, height), maskLeft);
            painter.drawPixmap(QRect(borderWidth, 0, width - 2 * borderWidth, height), maskCenter);
            painter.drawPixmap(QRect(width - borderWidth, 0, borderWidth, height), maskRight);
            break;
        }
        case SDeclarativeFrame::ThreePiecesVertical: {
            QPixmap maskTop;
            QPixmap maskCenter;
            QPixmap maskBottom;

            QString maskTopFileName = QString(":/graphics/%1_t.svg").arg(maskName);
            QString maskCenterFileName = QString(":/graphics/%1_c.svg").arg(maskName);
            QString maskBottomFileName = QString(":/graphics/%1_b.svg").arg(maskName);

            if (!maskTop.load(maskTopFileName))
                qWarning() << "Fail to load icon: " << maskTopFileName;
            if (!maskCenter.load(maskCenterFileName))
                qWarning() << "Fail to load icon: " << maskCenterFileName;
            if (!maskBottom.load(maskBottomFileName))
                qWarning() << "Fail to load icon: " << maskBottomFileName;

            const int borderHeight = maskTop.height();

            painter.drawPixmap(QRect(0, 0, width, borderHeight), maskTop);
            painter.drawPixmap(QRect(0, borderHeight, width, height - 2 * borderHeight), maskCenter);
            painter.drawPixmap(QRect(0, height - borderHeight, width, borderHeight), maskBottom);
            break;
        }
        case SDeclarativeFrame::NinePieces: {
            QPixmap maskTopLeft;
            QPixmap maskTop;
            QPixmap maskTopRight;
            QPixmap maskLeft;
            QPixmap maskCenter;
            QPixmap maskRight;
            QPixmap maskBottomLeft;
            QPixmap maskBottom;
            QPixmap maskBottomRight;

            QString maskTopLeftFileName = QString(":/graphics/%1_tl.svg").arg(maskName);
            QString maskTopFileName = QString(":/graphics/%1_t.svg").arg(maskName);
            QString maskTopRightFileName = QString(":/graphics/%1_tr.svg").arg(maskName);
            QString maskLeftFileName = QString(":/graphics/%1_l.svg").arg(maskName);
            QString maskCenterFileName = QString(":/graphics/%1_c.svg").arg(maskName);
            QString maskRightFileName = QString(":/graphics/%1_r.svg").arg(maskName);
            QString maskBottomLeftFileName = QString(":/graphics/%1_bl.svg").arg(maskName);
            QString maskBottomFileName = QString(":/graphics/%1_b.svg").arg(maskName);
            QString maskBottomRightFileName = QString(":/graphics/%1_br.svg").arg(maskName);

            if (!maskTopLeft.load(maskTopLeftFileName))
                qWarning() << "Fail to load icon: " << maskTopLeftFileName;
            if (!maskTop.load(maskTopFileName))
                qWarning() << "Fail to load icon: " << maskTopFileName;
            if (!maskTopRight.load(maskTopRightFileName))
                qWarning() << "Fail to load icon: " << maskTopRightFileName;
            if (!maskLeft.load(maskLeftFileName))
                qWarning() << "Fail to load icon: " << maskLeftFileName;
            if (!maskCenter.load(maskCenterFileName))
                qWarning() << "Fail to load icon: " << maskCenterFileName;
            if (!maskRight.load(maskRightFileName))
                qWarning() << "Fail to load icon: " << maskRightFileName;
            if (!maskBottomLeft.load(maskBottomLeftFileName))
                qWarning() << "Fail to load icon: " << maskBottomLeftFileName;
            if (!maskBottom.load(maskBottomFileName))
                qWarning() << "Fail to load icon: " << maskBottomFileName;
            if (!maskBottomRight.load(maskBottomRightFileName))
                qWarning() << "Fail to load icon: " << maskBottomRightFileName;

            const int borderWidth = maskLeft.width();
            const int borderHeight = maskTop.height();

            painter.drawPixmap(QRect(0, 0, borderWidth, borderHeight), maskTopLeft);
            painter.drawPixmap(QRect(borderWidth, 0, width - 2 * borderWidth, borderHeight), maskTop);
            painter.drawPixmap(QRect(width - borderWidth, 0, borderWidth, borderHeight), maskTopRight);
            painter.drawPixmap(QRect(0, borderHeight, borderWidth, height - borderHeight), maskLeft);
            painter.drawPixmap(QRect(borderWidth, borderHeight, width - 2 * borderWidth, height - borderHeight), maskCenter);
            painter.drawPixmap(QRect(width - borderWidth, borderHeight, borderWidth, height - borderHeight), maskRight);
            painter.drawPixmap(QRect(0, height - borderHeight, borderWidth, borderHeight), maskBottomLeft);
            painter.drawPixmap(QRect(borderWidth, height - borderHeight, width - 2 * borderWidth, borderHeight), maskBottom);
            painter.drawPixmap(QRect(width - borderWidth, height - borderHeight, borderWidth, borderHeight), maskBottomRight);
            break;
        }
        default:
            break;
        }
    }

    painter.end();
}

void SDeclarativeMaskedImagePrivate::releasePixmaps()
{
    foreach(const IconParams &params, requestedFromIconPool)
        SIconPool::release(params.fileName, params.size);

    requestedFromIconPool.clear();
    pixmapsCreated = false;
}

SDeclarativeMaskedImage::SDeclarativeMaskedImage(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), d_ptr(new SDeclarativeMaskedImagePrivate(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

SDeclarativeMaskedImage::~SDeclarativeMaskedImage()
{
}

QString SDeclarativeMaskedImage::imageName() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->imageName;
}

void SDeclarativeMaskedImage::setImageName(const QString &name)
{
    Q_D(SDeclarativeMaskedImage);
    if (d->imageName != name) {
        d->imageName = name;
        d->releasePixmaps();
    }
}

QString SDeclarativeMaskedImage::maskName() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->maskName;
}

void SDeclarativeMaskedImage::setMaskName(const QString &name)
{
    Q_D(SDeclarativeMaskedImage);
    if (d->maskName != name) {
        d->maskName = name;
        d->releasePixmaps();
    }
}

SDeclarativeFrame::FrameType SDeclarativeMaskedImage::maskType() const
{
    Q_D(const SDeclarativeMaskedImage);
    qWarning() << "WARNING: MaskedImage.maskType is deprecated and will be removed soon.";
    return d->maskType;
}

void SDeclarativeMaskedImage::setMaskType(SDeclarativeFrame::FrameType type)
{
    Q_D(SDeclarativeMaskedImage);
    qWarning() << "WARNING: MaskedImage.maskType is deprecated and will be removed soon.";
    if (d->maskType != type) {
        d->maskType = type;
        d->releasePixmaps();
    }
}

QPoint SDeclarativeMaskedImage::offset() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->offset;
}

void SDeclarativeMaskedImage::setOffset(const QPoint &offset)
{
    Q_D(SDeclarativeMaskedImage);
    if (d->offset != offset) {
        d->offset = offset;
        update();
    }
}

bool SDeclarativeMaskedImage::isTiled() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->tiled;
}

void SDeclarativeMaskedImage::setTiled(bool tiled)
{
    Q_D(SDeclarativeMaskedImage);
    if (tiled != d->tiled) {
        d->tiled = tiled;
        update();
    }
}

int SDeclarativeMaskedImage::topMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->topMargin;
}

void SDeclarativeMaskedImage::setTopMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->topMargin) {
        d->topMargin = margin;
        d->releasePixmaps();
        update();
    }
}

int SDeclarativeMaskedImage::bottomMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->bottomMargin;
}

void SDeclarativeMaskedImage::setBottomMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->bottomMargin) {
        d->bottomMargin = margin;
        d->releasePixmaps();
        update();
    }
}

int SDeclarativeMaskedImage::leftMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->leftMargin;
}

void SDeclarativeMaskedImage::setLeftMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->leftMargin) {
        d->leftMargin = margin;
        d->releasePixmaps();
        update();
    }
}

int SDeclarativeMaskedImage::rightMargin() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->rightMargin;
}

void SDeclarativeMaskedImage::setRightMargin(int margin)
{
    Q_D(SDeclarativeMaskedImage);
    if (margin != d->rightMargin) {
        d->rightMargin = margin;
        d->releasePixmaps();
        update();
    }
}

void SDeclarativeMaskedImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_D(SDeclarativeMaskedImage);
    QSize rectSize(width(), height());

    // If size has changed, need to recreate pixmaps
    if (d->createdSize != rectSize)
        d->releasePixmaps();

    // Create pixmap and mask if not done yet
    if (!d->pixmapsCreated) {
        if (d->tiled)
            d->createTiledPixmap();
        else
            d->createNonTiledPixmap();

        d->createMask();
        d->pixmapsCreated = true;
        d->createdSize = rectSize;
    }

    // draw the frame masked with the mask
    QPixmap imageFrame(rectSize);
    imageFrame.fill(Qt::transparent);
    QPainter p(&imageFrame);

    int offsetX = QApplication::layoutDirection() == Qt::RightToLeft ?
        d->tileSize + d->offset.x() : -d->offset.x();

    p.drawPixmap(QPointF(0, 0), d->pixmap, QRectF(QPointF(offsetX, -d->offset.y()), rectSize));
    p.end(); // Can't set mask when painter's active

    // Set the mask in the frame
    if (!d->mask.isNull())
        imageFrame.setMask(d->mask);

    // Update the frame in the screen
    painter->drawPixmap(QPointF(0, 0), imageFrame);
}
