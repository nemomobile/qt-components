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

#include "mdeclarativebackground.h"

#include <QPainter>
#include <MScalableImage>
#include <MWidgetStyle>

MDeclarativeBackground::MDeclarativeBackground(QDeclarativeItem *parent) :
    MDeclarativePrimitive(parent), m_image(0), m_tiles(0), m_tileposition(DefaultPosition)
{
}

MDeclarativeBackground::~MDeclarativeBackground()
{
}

MDeclarativeBackground::Position MDeclarativeBackground::tilePosition() const
{
    return m_tileposition;
}

void MDeclarativeBackground::setTilePosition(const Position tilePosition)
{
    if (m_tileposition == tilePosition)
        return;

    m_tileposition = tilePosition;
    updateStyleData();
}

void MDeclarativeBackground::clearStyleData()
{
    m_image = 0;
    m_tiles = 0;
    m_color = QColor();
    setOpacity(1.);
}

void MDeclarativeBackground::fetchStyleData(const MStyle *style)
{
    // We know it is a MWidgetStyle, this allows direct access to the properties...
    const MWidgetStyle *widgetStyle = qobject_cast<const MWidgetStyle *>(style);

    // ...if isn't, it is not a valid style for us
    if (!widgetStyle) {
        clearStyleData();
        return;
    }

    if (widgetStyle->backgroundTiles().isValid())
        m_tiles = widgetStyle->backgroundTiles()[static_cast<M::Position>(m_tileposition)];
    else
        m_tiles = 0;

    m_image = widgetStyle->backgroundImage();
    m_color = widgetStyle->backgroundColor();
    setOpacity(widgetStyle->backgroundOpacity());
}

void MDeclarativeBackground::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_tiles) {
        // XXX No size hint so boundingRect may be smaller than minimum image size...
        m_tiles->draw(0.0, 0.0, boundingRect().width(), boundingRect().height(), painter);
    } else if (m_image) {
        // XXX No size hint so boundingRect may be smaller than minimum image size...
        m_image->draw(0.0, 0.0, boundingRect().width(), boundingRect().height(), painter);
    } else if (m_color.isValid()) {
        painter->fillRect(boundingRect(), QBrush(m_color));
    }
}

