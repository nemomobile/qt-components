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

#include "sdeclarativemaskedimage.h"
#include "sdeclarativemaskedimage_p.h"
#include "sframepool.h"

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createImagePixmap()
{
    Q_Q(SDeclarativeMaskedImage);

    QPixmap imageMap;
    QString imageFileName = QString(":/graphics/%1.svg").arg(imageName);

    if (!imageMap.load(imageFileName))
        qWarning() << "Fail to load Image: " << imageFileName;

    QSize imageTileSize = imageMap.size();
    imageTileSize.scale(q->width(), q->height(), Qt::KeepAspectRatio);

    tileSize = imageTileSize.width();

    imagePixmap = QPixmap(q->width(), q->height());
    imagePixmap.fill(Qt::transparent);
    QPainter painter(&imagePixmap);
    painter.fillRect(QRect(0, 0, q->width(), q->height()), QBrush(imageMap));
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createNonImagePixmap()
{
    QString imageFileName = QString(":/graphics/%1.svg").arg(imageName);
    imagePixmap.load(imageFileName);

    tileSize = imagePixmap.width();
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeMaskedImagePrivate::createMask()
{
    Q_Q(SDeclarativeMaskedImage);

    // paint the mask from its frame graphics to the mask pixmap
    imageMask = QBitmap(q->width(), q->height());
    imageMask.fill(Qt::white);
    QPainter painter(&imageMask);

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

        painter.drawPixmap(QRect(0, 0, borderWidth, q->height()), maskLeft);
        painter.drawPixmap(QRect(borderWidth, 0, q->width() - 2 * borderWidth, q->height()), maskCenter);
        painter.drawPixmap(QRect(q->width() - borderWidth, 0, borderWidth, q->height()), maskRight);
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

        painter.drawPixmap(QRect(0, 0, q->width(), borderHeight), maskTop);
        painter.drawPixmap(QRect(0, borderHeight, q->width(), q->height() - 2 * borderHeight), maskCenter);
        painter.drawPixmap(QRect(0, q->height() - borderHeight, q->width(), borderHeight), maskBottom);
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
        painter.drawPixmap(QRect(borderWidth, 0, q->width() - 2 * borderWidth, borderHeight), maskTop);
        painter.drawPixmap(QRect(q->width() - borderWidth, 0, borderWidth, borderHeight), maskTopRight);
        painter.drawPixmap(QRect(0, borderHeight, borderWidth, q->height() - borderHeight), maskLeft);
        painter.drawPixmap(QRect(borderWidth, borderHeight, q->width() - 2 * borderWidth, q->height() - borderHeight), maskCenter);
        painter.drawPixmap(QRect(q->width() - borderWidth, borderHeight, borderWidth, q->height() - borderHeight), maskRight);
        painter.drawPixmap(QRect(0, q->height() - borderHeight, borderWidth, borderHeight), maskBottomLeft);
        painter.drawPixmap(QRect(borderWidth, q->height() - borderHeight, q->width() - 2 * borderWidth, borderHeight), maskBottom);
        painter.drawPixmap(QRect(q->width() - borderWidth, q->height() - borderHeight, borderWidth, borderHeight), maskBottomRight);
        break;
    }
    default:
        break;
    }

    painter.end();
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
        d->pixmapsCreated = false;
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
        d->pixmapsCreated = false;
    }
}

SDeclarativeFrame::FrameType SDeclarativeMaskedImage::maskType() const
{
    Q_D(const SDeclarativeMaskedImage);
    return d->maskType;
}

void SDeclarativeMaskedImage::setMaskType(SDeclarativeFrame::FrameType type)
{
    Q_D(SDeclarativeMaskedImage);
    if (d->maskType != type) {
        d->maskType = type;
        d->pixmapsCreated = false;
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

void SDeclarativeMaskedImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_D(SDeclarativeMaskedImage);
    QSize rectSize(width(), height());

    // If size has changed, need to recreate pixmaps
    if (d->createdSize != rectSize)
        d->pixmapsCreated = false;

    // Create pixmap and mask if not done yet
    if (!d->pixmapsCreated) {
        if (d->tiled)
            d->createImagePixmap();
        else
            d->createNonImagePixmap();

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

    p.drawPixmap(QPointF(0, 0), d->imagePixmap, QRectF(QPointF(offsetX, -d->offset.y()), rectSize));
    p.end(); // Can't set mask when painter's active

    // Set the mask in the frame
    if (!d->imageMask.isNull())
        imageFrame.setMask(d->imageMask);

    // Update the frame in the screen
    painter->drawPixmap(QPointF(0, 0), imageFrame);
}
