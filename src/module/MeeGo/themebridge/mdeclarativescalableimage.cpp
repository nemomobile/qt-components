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

#include "mdeclarativescalableimage.h"
#include "mstylewrapper.h"

#include <MScalableImage>
#include <MWidgetStyle>

Q_DECLARE_METATYPE(const MScalableImage *)

MDeclarativeScalableImage::MDeclarativeScalableImage(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), m_style(0), m_image(0)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

MDeclarativeScalableImage::~MDeclarativeScalableImage()
{
}

QString MDeclarativeScalableImage::imageProperty() const
{
    return m_imageProperty;
}

void MDeclarativeScalableImage::setImageProperty(const QString &imageProperty)
{
    if (m_imageProperty == imageProperty)
        return;

    m_imageProperty = imageProperty;

    if (m_style)
        updateImage();
}

MStyleWrapper *MDeclarativeScalableImage::style() const
{
    return m_style;
}

void MDeclarativeScalableImage::setStyle(MStyleWrapper *style)
{
    if (style == m_style)
        return;

    if (m_style) {
        m_style->disconnect(this);
        m_image = 0;
    }

    m_style = style;

    if (m_style) {
        connect(m_style, SIGNAL(modeChanged(const QString &)), SLOT(updateImage()));
        updateImage();
    }
}

void MDeclarativeScalableImage::updateImage()
{
    m_image = 0;

    const MWidgetStyleContainer *styleContainer = m_style->styleContainer();
    if (!styleContainer)
        return;

    const QVariant imageVariant = (*styleContainer)->property(m_imageProperty.toAscii());
    m_image = imageVariant.value<const MScalableImage *>();

    update();
}

void MDeclarativeScalableImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_image)
        return;

    // XXX respect minimum size here...
    m_image->draw(boundingRect().toRect(), painter);
}

