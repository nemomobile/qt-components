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
#include "mstylewrapper.h"

#include <QPainter>
#include <MTheme>
#include <MScalableImage>
#include <MWidgetStyle>

MDeclarativeBackground::MDeclarativeBackground(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), m_style(0), m_image(0), m_tiles(0), m_pendingPixmap(0)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

MDeclarativeBackground::~MDeclarativeBackground()
{
}

MStyleWrapper *MDeclarativeBackground::style() const
{
    return m_style;
}

void MDeclarativeBackground::setStyle(MStyleWrapper *style)
{
    if (style == m_style)
        return;

    if (m_style) {
        m_style->disconnect(this);
        clearOldStyleData();
    }

    m_style = style;

    if (m_style) {
        connect(m_style, SIGNAL(modeChanged(const QString &)), SLOT(updateStyleData()));
        updateStyleData();
    }
}

void MDeclarativeBackground::clearOldStyleData()
{
    m_image = 0;
    m_tiles = 0;
    m_color = QColor();
    m_opacity = 1;

    if (m_pendingPixmap) {
        MTheme::instance()->disconnect(this);
        m_pendingPixmap = 0;
    }
}

void MDeclarativeBackground::updateStyleData()
{
    Q_ASSERT(m_style);

    const MWidgetStyleContainer *styleContainer = m_style->styleContainer();
    if (!styleContainer) {
        clearOldStyleData();
        return;
    }

    // Since it's kind of easy to get info from styleContainer in our case
    // we may consider leaving this task to the paint() method instead and
    // avoid caching the info.

    if ((*styleContainer)->backgroundTiles().isValid())
        m_tiles = (*styleContainer)->backgroundTiles()[/* layout position */ M::HorizontalLeftPosition];
    else
        m_tiles = 0;

    m_image = (*styleContainer)->backgroundImage();
    m_color = (*styleContainer)->backgroundColor();
    m_opacity = (*styleContainer)->backgroundOpacity();

    checkPendingPixmap();
}

void MDeclarativeBackground::checkPendingPixmap()
{
    // In MeeGo the themeserver may run in a separate process. In that case MScalableImages
    // may be created without a proper pixmap, instead a gray 1x1 pixmap is provided.
    // We must account for that situation and then listen for the pixmapRequestFinished signal
    // in order to repaint the primitive.
    // Note that we assume that a 1x1 pixmap means an unloaded pixmap. This will fail if there
    // are actual 1x1 pixmaps in the theme.

    const bool imagePixmapPending = m_image && (m_image->pixmap()->size() == QSize(1, 1));
    const bool tilesPixmapPending = m_tiles && (m_tiles->pixmap()->size() == QSize(1, 1));

    if (imagePixmapPending || tilesPixmapPending) {
        if (!m_pendingPixmap) {
            // If not yet connected to MTheme, connect and wait for update
            connect(MTheme::instance(), SIGNAL(pixmapRequestsFinished()), SLOT(checkPendingPixmap()));
            m_pendingPixmap = 1;
        }
    } else {
        if (m_pendingPixmap) {
            // If still connected to MTheme, disconnect.
            MTheme::instance()->disconnect(this);
            m_pendingPixmap = 0;
        }
        update();
    }
}

void MDeclarativeBackground::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    const qreal oldOpacity = painter->opacity();
    painter->setOpacity(m_opacity * oldOpacity);

    if (m_tiles) {
        // XXX No size hint so boundingRect may be smaller than minimum image size...
        m_tiles->draw(0, 0, boundingRect().width(), boundingRect().height(), painter);
    } else if (m_image) {
        // XXX No size hint so boundingRect may be smaller than minimum image size...
        m_image->draw(0, 0, boundingRect().width(), boundingRect().height(), painter);
    } else if (m_color.isValid()) {
        painter->fillRect(boundingRect(), QBrush(m_color));
    }

    painter->setOpacity(oldOpacity);
}

