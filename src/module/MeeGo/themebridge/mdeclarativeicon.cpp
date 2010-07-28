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

#include <QPixmap>
#include <QPainter>
#include <MWidgetStyle>
#include <mtheme.h>

MDeclarativeIcon::MDeclarativeIcon(QDeclarativeItem *parent) :
    MDeclarativePrimitive(parent), m_icon(0)
{
}

MDeclarativeIcon::~MDeclarativeIcon()
{
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
    updateStyleData();
}

void MDeclarativeIcon::clearStyleData()
{
    setImplicitWidth(0);
    setImplicitHeight(0);

    if (m_icon) {
        MTheme::instance()->releasePixmap(m_icon);
        m_icon = 0;
    }
}

void MDeclarativeIcon::fetchStyleData(const MWidgetStyleContainer &styleContainer)
{
    if (m_icon) {
        MTheme::instance()->releasePixmap(m_icon);
        m_icon = 0;
    }

    if (m_iconId.isEmpty())
        return;

    const QVariant pixmapSizeVariant = styleContainer->property("iconSize");
    if (pixmapSizeVariant.canConvert<QSize>()) {
        const QSize pixmapSize = pixmapSizeVariant.toSize();
        setImplicitWidth(pixmapSize.width());
        setImplicitHeight(pixmapSize.height());
    } else {
        setImplicitWidth(0);
        setImplicitHeight(0);
    }

    // Request the pixmap represented by the given iconID
    // XXX The second QSize(0, 0) argument is the size we want the returned pixmap
    // to be. However, if we specify that, we do not have a way of knowing whether
    // that pixmap is yet to be loaded (size -1, -1) or actually ready.
    // Thus we ask the default size (0, 0) and can then use a heuristic in "hasPendingPixmap"
    m_icon = MTheme::instance()->pixmap(m_iconId, QSize(0, 0));
}

bool MDeclarativeIcon::hasPendingPixmap()
{
    if (!m_icon || (m_icon->size() != QSize(-1, -1)))
        return false;

    return true;
}

void MDeclarativeIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_icon)
        return;

    painter->drawPixmap(boundingRect().toRect(), *m_icon);
}

