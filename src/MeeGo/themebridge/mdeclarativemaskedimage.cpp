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

#include "mdeclarativemaskedimage.h"

#include <MScalableImage>
#include <MWidgetStyle>
#include <QPainter>

Q_DECLARE_METATYPE(const MScalableImage *)

MDeclarativeMaskedImage::MDeclarativeMaskedImage(QDeclarativeItem *parent) :
    MDeclarativePrimitive(parent), m_image(0), m_mask(0), m_isTiled(false), m_isFullWidth(false)
{
}

MDeclarativeMaskedImage::~MDeclarativeMaskedImage()
{
}

int MDeclarativeMaskedImage::imageXOffset() const
{
    return m_imageOffset.x();
}

void MDeclarativeMaskedImage::setImageXOffset(int x)
{
    if (m_imageOffset.x() == x)
        return;

    m_imageOffset.setX(x);
    update();
}

int MDeclarativeMaskedImage::imageYOffset() const
{
    return m_imageOffset.y();
}

void MDeclarativeMaskedImage::setImageYOffset(int y)
{
    if (m_imageOffset.y() == y)
        return;

    m_imageOffset.setY(y);
    update();
}

QString MDeclarativeMaskedImage::imageProperty() const
{
    return m_imageProperty;
}

void MDeclarativeMaskedImage::setImageProperty(const QString &imageProperty)
{
    if (m_imageProperty == imageProperty)
        return;

    m_imageProperty = imageProperty;
    updateStyleData();
}

QString MDeclarativeMaskedImage::maskProperty() const
{
    return m_maskProperty;
}

bool MDeclarativeMaskedImage::isTiled() const
{
    return m_isTiled;
}

void MDeclarativeMaskedImage::setIsTiled(bool tiled)
{
    if (m_isTiled == tiled)
        return;

    m_isTiled = tiled;
    update();
}

bool MDeclarativeMaskedImage::isFullWidth() const
{
    return m_isFullWidth;
}

void MDeclarativeMaskedImage::setIsFullWidth(bool isFullWidth)
{
    if (m_isFullWidth == isFullWidth)
        return;

    m_isFullWidth = isFullWidth;
    update();
}

void MDeclarativeMaskedImage::setMaskProperty(const QString &maskProperty)
{
    if (m_maskProperty == maskProperty)
        return;

    m_maskProperty = maskProperty;
    updateStyleData();
}

void MDeclarativeMaskedImage::clearStyleData()
{
    m_image = 0;
    m_mask = 0;
    m_buffer.reset();
    setImplicitWidth(0);
    setImplicitHeight(0);
}

void MDeclarativeMaskedImage::fetchStyleData(const MStyle *style)
{
    const QVariant imageVariant = style->property(m_imageProperty.toAscii());
    m_image = imageVariant.value<const MScalableImage *>();

    const QVariant maskVariant = style->property(m_maskProperty.toAscii());
    m_mask = maskVariant.value<const MScalableImage *>();

    // New mask may be available, so lets invalidate the buffer
    m_buffer.reset();

    if (m_mask) {
        setImplicitWidthNotify(m_mask->pixmap()->width());
        setImplicitHeightNotify(m_mask->pixmap()->height());
    } else {
        setImplicitWidth(0);
        setImplicitHeight(0);
    }
}

void MDeclarativeMaskedImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // If masked buffer is not ready, try to create it. If resources are missing
    // or current size is empty, then no buffer is created
    if (m_buffer.isNull() && !initializeMaskedBuffer())
        return;

    QPainter p(m_buffer.data());
    QRect bufferRect(m_imageOffset, m_buffer.data()->size());
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    if (m_isTiled) {
        if (m_isFullWidth)
            p.drawTiledPixmap(QRect(QPoint(), m_buffer.data()->size()), *m_image->pixmap(), -m_imageOffset);
        else
            p.drawTiledPixmap(bufferRect, *m_image->pixmap());
    } else {
        m_image->draw(bufferRect, &p);
    }

    // Blit offscreen buffer on the screen
    painter->drawImage(QPoint(), *m_buffer);
}

void MDeclarativeMaskedImage::geometryChanged(const QRectF &newGeometry,
                                              const QRectF &oldGeometry)
{
    MDeclarativePrimitive::geometryChanged(newGeometry, oldGeometry);

    if (oldGeometry.size() != newGeometry.size())
        m_buffer.reset();
}

bool MDeclarativeMaskedImage::initializeMaskedBuffer()
{
    if (!m_mask || !m_image)
        return false;

    // This primitive:
    // 1) allocates a QImage buffer
    // 2) paints the mask image (m_mask) scaled to the primitive size
    // 3) paints the image on top of that mask using the specified offset and
    //    the original size of the image

    // ### Alternatives to the QImage buffer could be
    // 1) Use a QPixmap buffer, but it seems to be broken a some X11 systems
    // 2) Try to compose the images using "painter" directly, without the offscreen
    //    buffer. But not all platforms support that either.

    const QSize size(width(), height());
    if (size.isEmpty())
        return false;

    m_buffer.reset(new QImage(size, QImage::Format_ARGB32_Premultiplied));
    m_buffer->fill(Qt::transparent);

    QPainter p(m_buffer.data());
    m_mask->draw(boundingRect().toRect(), &p);

    return true;
}
