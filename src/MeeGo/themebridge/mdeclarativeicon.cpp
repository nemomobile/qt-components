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

#include "mdeclarativeicon.h"

#include "mthemebridge.h"

#include <QPixmap>
#include <QPainter>
#include <MWidgetStyle>
#include <mtheme.h>

MDeclarativeIcon::MDeclarativeIcon(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), m_icon(0)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    MThemeBridge::instance()->registerIcon(this);
}

MDeclarativeIcon::~MDeclarativeIcon()
{
    MThemeBridge::instance()->unregisterIcon(this);
    setIconId(QLatin1String(""));
}

QString MDeclarativeIcon::iconId() const
{
    return m_iconId;
}

void MDeclarativeIcon::setIconId(const QString &iconId)
{
    if (m_iconId == iconId)
        return;

    m_iconId = iconId;
    refreshPixmap();
    emit iconIdChanged(m_iconId);
}

void MDeclarativeIcon::refreshPixmap()
{
    if (m_icon) {
        MTheme::instance()->releasePixmap(m_icon);
        m_icon = 0;
    }

    if (!m_iconId.isEmpty()) {
        // By setting iconSize to (0, 0) we let MTheme server provide us with
        // and icon of default size
        QSize iconSize(0, 0);

        if (widthValid() && heightValid()) {
            // If we were explicitly given a valid size, then ask the MTheme
            // server to give us a pixmap rendered with the correct size
            // Note: MTheme::pixmap() does not support sizes that have only
            //       one non-zero dimension
            iconSize.setWidth(width());
            iconSize.setHeight(height());
        }

        m_icon = MTheme::instance()->pixmap(m_iconId, iconSize);
    }

    if (m_icon) {
        setImplicitWidth(m_icon->width());
        setImplicitHeight(m_icon->height());
    } else {
        setImplicitWidth(0);
        setImplicitHeight(0);
    }

    update();
}

void MDeclarativeIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // If no icon available (no icon ID) then skip
    if (!m_icon)
        return;

    // If our size changes, we want to re-render the pixmap. However, to avoid
    // excessive rendering, we postpone that to the paint event.
    if ((widthValid() && heightValid() && (m_icon->size() != QSize(width(), height()))))
        refreshPixmap();

    const bool oldSmooth = painter->testRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(boundingRect().toRect(), *m_icon);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, oldSmooth);
}
