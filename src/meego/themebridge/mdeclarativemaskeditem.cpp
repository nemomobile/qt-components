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

#include "mdeclarativemaskeditem.h"

#include <QDeclarativeComponent>
#include <QPainter>

MDeclarativeMaskedItem::MDeclarativeMaskedItem(QDeclarativeItem *parent)
  : QDeclarativeItem(parent), m_maskComponent(0), m_visualItemComponent(0),
    m_mask(0), m_visualItem(0), m_buffer(), m_imageOffset()
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

MDeclarativeMaskedItem::~MDeclarativeMaskedItem()
{
}

QDeclarativeComponent *MDeclarativeMaskedItem::mask() const
{
    return m_maskComponent;
}

void MDeclarativeMaskedItem::setMask(QDeclarativeComponent *maskComponent)
{
    if (maskComponent == m_maskComponent)
        return;

    QDeclarativeItem *mask = 0;
    if (maskComponent) {
        QObject *maskObject = maskComponent->create(maskComponent->creationContext());
        mask = qobject_cast<QDeclarativeItem*>(maskObject);

        if (!mask)
            qWarning("MaskedItem: the mask property isn't being set with a Component.");
        else if (!mask->childItems().isEmpty())
            qWarning("MaskedItem: the mask property has children. Due to current limitation, it won't be painted.");
    }

    m_mask.reset(mask);

    if (m_mask && !m_buffer.isNull())
        paintMaskInBuffer();

    m_maskComponent = maskComponent;
    emit maskChanged(maskComponent);
    update();
}

QDeclarativeComponent *MDeclarativeMaskedItem::visualItem() const
{
    return m_visualItemComponent;
}

void MDeclarativeMaskedItem::setVisualItem(QDeclarativeComponent *visualItemComponent)
{
    if (visualItemComponent == m_visualItemComponent)
        return;

    QDeclarativeItem *visualItem = 0;
    if (visualItemComponent) {
        QObject *itemObject = visualItemComponent->create(visualItemComponent->creationContext());
        visualItem = qobject_cast<QDeclarativeItem*>(itemObject);

        if (!visualItem)
            qWarning("MaskedItem: the visualItem property isn't being set with a Component.");
        else if (!visualItem->childItems().empty())
            qWarning("MaskedItem: the visualItem property has children. Due to current limitation, it won't be painted.");
    }

    m_visualItem.reset(visualItem);
    m_visualItemComponent = visualItemComponent;
    emit visualItemChanged(visualItemComponent);
    update();
}

QPoint MDeclarativeMaskedItem::imageOffset() const
{
    return m_imageOffset;
}

void MDeclarativeMaskedItem::setImageOffset(const QPoint &imageOffset)
{
    if (m_imageOffset == imageOffset)
        return;

    m_imageOffset = imageOffset;
    emit imageOffsetChanged(imageOffset);
    update();
}

void MDeclarativeMaskedItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // If masked buffer is not ready, try to create it. If resources are missing
    // or current size is empty, then no buffer is created
    if (m_buffer.isNull() && !initializeMaskedBuffer())
        return;

    QPainter p(&m_buffer);
    p.translate(m_imageOffset);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);

    // Painting item
    m_visualItem->paint(&p, 0, 0);

    // Blit offscreen buffer on the screen
    painter->drawImage(QPoint(), m_buffer);
}

void MDeclarativeMaskedItem::geometryChanged(const QRectF &newGeometry,
                                              const QRectF &oldGeometry)
{
    QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

    if (oldGeometry.size() != newGeometry.size())
        m_buffer = QImage();
}

bool MDeclarativeMaskedItem::initializeMaskedBuffer()
{
    if (!m_mask || !m_visualItem)
        return false;

    // This primitive:
    // 1) allocates a QImage buffer
    // 2) paints the mask item (m_mask) scaled to the primitive size
    // 3) paints the image on top of that mask using the specified offset and
    //    the original size of the image

    // ### Alternatives to the QImage buffer could be
    // 1) Use a QPixmap buffer, but it seems to be broken a some X11 systems
    // 2) Try to compose the images using "painter" directly, without the offscreen
    //    buffer. But not all platforms support that either.

    const QSize size(width(), height());
    if (size.isEmpty())
        return false;

    m_buffer = QImage(size, QImage::Format_ARGB32_Premultiplied);
    paintMaskInBuffer();

    return true;
}

void MDeclarativeMaskedItem::paintMaskInBuffer()
{
    m_buffer.fill(Qt::transparent);

    QPainter p(&m_buffer);
    m_mask->paint(&p, 0 , 0);
}
