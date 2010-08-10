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
    MDeclarativePrimitive(parent), m_image(0), m_mask(0)
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
    setImplicitWidth(0);
    setImplicitHeight(0);
}

void MDeclarativeMaskedImage::fetchStyleData(const MWidgetStyleContainer &styleContainer)
{
    const QVariant imageVariant = styleContainer->property(m_imageProperty.toAscii());
    m_image = imageVariant.value<const MScalableImage *>();

    const QVariant maskVariant = styleContainer->property(m_maskProperty.toAscii());
    m_mask = maskVariant.value<const MScalableImage *>();

    setImplicitWidth(0);
    setImplicitHeight(0);
}

bool MDeclarativeMaskedImage::hasPendingPixmap()
{
    // Note that we assume that a 1x1 pixmap means an unloaded pixmap. This will fail if there
    // are actual 1x1 pixmaps in the theme.
    const bool hasPendingMask = m_mask && (m_mask->pixmap()->size() == QSize(1, 1));
    const bool hasPendingImage = m_image && (m_image->pixmap()->size() == QSize(1, 1));

    if (hasPendingMask || hasPendingImage)
        return true;

    if (m_mask) {
        setImplicitWidth(m_mask->pixmap()->width());
        setImplicitHeight(m_mask->pixmap()->height());
    }

    return false;
}

void MDeclarativeMaskedImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_image || !m_mask)
        return;

    QPixmap buffer(QSize(width(), height()));
    buffer.fill(Qt::transparent);

    // XXX We assume this primitive is being used for the switch component which is not
    // performance critical. We could consider allocating the buffer only once, or try
    // to compose on the destination buffer directly.
    QPainter p(&buffer);
    m_mask->draw(boundingRect().toRect(), &p);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);

    // XXX Image is painted using its standard size since this fits the need we have
    // for the switch component. Were this component to be more general, we could export
    // the imageSize as a public property.
    m_image->draw(QRect(m_imageOffset, m_image->pixmap()->size()), &p);

    painter->drawPixmap(QPoint(), buffer);
}

