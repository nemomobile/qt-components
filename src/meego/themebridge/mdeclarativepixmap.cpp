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

#include "mdeclarativepixmap.h"

#include <QPixmap>
#include <QPainter>
#include <MWidgetStyle>

Q_DECLARE_METATYPE(const QPixmap *)

MDeclarativePixmap::MDeclarativePixmap(QDeclarativeItem *parent) :
    MDeclarativePrimitive(parent), m_pixmap(0)
{
}

MDeclarativePixmap::~MDeclarativePixmap()
{
}

QString MDeclarativePixmap::imageProperty() const
{
    return m_imageProperty;
}

void MDeclarativePixmap::setImageProperty(const QString &imageProperty)
{
    if (m_imageProperty == imageProperty)
        return;

    m_imageProperty = imageProperty;
    updateStyleData();
}

void MDeclarativePixmap::clearStyleData()
{
    m_pixmap = 0;
    setImplicitWidth(0);
    setImplicitHeight(0);
}

void MDeclarativePixmap::fetchStyleData(const MStyle *style)
{
    const QVariant imageVariant = style->property(m_imageProperty.toAscii());
    m_pixmap = imageVariant.value<const QPixmap *>();

    if (m_pixmap) {
        setImplicitWidth(m_pixmap->width());
        setImplicitHeight(m_pixmap->height());
    } else {
        setImplicitWidth(0);
        setImplicitHeight(0);
    }
}

void MDeclarativePixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_pixmap)
        return;

    QRect rect = boundingRect().toRect();
    if (rect.isEmpty())
        return;

    painter->drawPixmap(rect, *m_pixmap);
}

