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

#include "sdeclarativeprogressbaranimation.h"

#include <QPainter>
#include <QApplication>
#include <QBitmap>

class SDeclarativeProgressBarAnimationPrivate
{
    Q_DECLARE_PUBLIC(SDeclarativeProgressBarAnimation)

public:
    SDeclarativeProgressBarAnimationPrivate(SDeclarativeProgressBarAnimation *qq) :
      q_ptr(qq),
      animationOffset(0),
      pixmapsCreated(false),
      tileSize(0)
      {}

    void createRepeatPixmap();
    void createMask();

    SDeclarativeProgressBarAnimation *q_ptr;
    QString animationIconName;
    QString animationMaskName;
    int animationOffset;

    bool pixmapsCreated;
    QSize createdSize;
    QPixmap repeatPixmap; // tiled from animation graphic, little wider than actual rect for being able to animate
    QBitmap animationMask;
    int tileSize;
};


// Assumes that the bounding rect of this item has been set
void SDeclarativeProgressBarAnimationPrivate::createRepeatPixmap()
{
    Q_Q(SDeclarativeProgressBarAnimation);

    QPixmap animMap;

    QString animFileName = QString(":/graphics/%1.svg").arg(animationIconName);

    if (!animMap.load(animFileName))
        qWarning() << "Fail to load icon: " << animFileName;

    QSize animTileSize = animMap.size();
    animTileSize.scale(q->width(), q->height(), Qt::KeepAspectRatio);

    // Tile the animation graphic to the pixmap
    // Add size for one extra piece in the animation pixmap for being able to animate it
    int usedWidth = q->width() + animTileSize.width();
    tileSize = animTileSize.width();

    repeatPixmap = QPixmap(q->width() + animTileSize.width(), q->height());
    repeatPixmap.fill(Qt::transparent);
    QPainter painter(&repeatPixmap);
    if (animTileSize.width() > 0) {
        for (int i = 0; i < usedWidth; i += animTileSize.width())
            painter.drawPixmap(QRect(i, 0, animTileSize.width(), animTileSize.height()), animMap);
    }
}

// Assumes that the bounding rect of this item has been set
void SDeclarativeProgressBarAnimationPrivate::createMask()
{
    Q_Q(SDeclarativeProgressBarAnimation);

    // paint the mask from its frame graphics to the mask pixmap
    animationMask = QBitmap(q->width(), q->height());
    animationMask.fill(Qt::white);
    QPainter painter(&animationMask);

    QPixmap maskLeft;
    QPixmap maskCenter;
    QPixmap maskRight;

    QString maskLeftFileName = QString(":/graphics/%1_l.svg").arg(animationMaskName);
    QString maskCenterFileName = QString(":/graphics/%1_c.svg").arg(animationMaskName);
    QString maskRightFileName = QString(":/graphics/%1_r.svg").arg(animationMaskName);

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
}

SDeclarativeProgressBarAnimation::SDeclarativeProgressBarAnimation(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), d_ptr(new SDeclarativeProgressBarAnimationPrivate(this))
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

SDeclarativeProgressBarAnimation::~SDeclarativeProgressBarAnimation()
{
}

QString SDeclarativeProgressBarAnimation::animationIcon() const
{
    Q_D(const SDeclarativeProgressBarAnimation);
    return d->animationIconName;
}

void SDeclarativeProgressBarAnimation::setAnimationIcon(const QString &name)
{
    Q_D(SDeclarativeProgressBarAnimation);
    if (d->animationIconName != name) {
        d->animationIconName = name;
        d->pixmapsCreated = false;
    }
}

QString SDeclarativeProgressBarAnimation::animationMask() const
{
    Q_D(const SDeclarativeProgressBarAnimation);
    return d->animationMaskName;
}

void SDeclarativeProgressBarAnimation::setAnimationMask(const QString &name)
{
    Q_D(SDeclarativeProgressBarAnimation);
    if (d->animationMaskName != name) {
        d->animationMaskName = name;
        d->pixmapsCreated = false;
    }
}

int SDeclarativeProgressBarAnimation::animationOffset() const
{
    Q_D(const SDeclarativeProgressBarAnimation);
    return d->animationOffset;
}

void SDeclarativeProgressBarAnimation::setAnimationOffset(int offset)
{
    Q_D(SDeclarativeProgressBarAnimation);
    int oldOffset = d->animationOffset;
    d->animationOffset = d->tileSize ? offset % d->tileSize : offset;

    if (d->animationOffset != oldOffset)
        update();
}

void SDeclarativeProgressBarAnimation::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_D(SDeclarativeProgressBarAnimation);
    QSize rectSize(width(), height());

    // If size has changed, need to recreate pixmaps
    if (d->createdSize != rectSize)
        d->pixmapsCreated = false;

    // Create tiled animation pixmap and mask if not done yet
    if (!d->pixmapsCreated) {
        d->createRepeatPixmap();
        d->createMask();
        d->pixmapsCreated = true;
        d->createdSize = rectSize;
    }

    // draw the animation frame masked with the mask
    QPixmap animFrame(rectSize);
    animFrame.fill(Qt::transparent);
    QPainter p(&animFrame);

    // Horizontal, take layout direction in account to decide the direction of the animation
    int offset = QApplication::layoutDirection() == Qt::RightToLeft ?
        d->tileSize - d->animationOffset : d->animationOffset;

    p.drawPixmap(QPointF(0, 0), d->repeatPixmap, QRectF(QPointF(offset, 0), rectSize));
    p.end(); // Can't set mask when painter's active

    // Set the mask in the animation frame
    if (!d->animationMask.isNull())
        animFrame.setMask(d->animationMask);

    // Update the animation frame in the screen
    painter->drawPixmap(QPointF(0, 0), animFrame);
}
