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

/// Primitive to draw a Pixmap from MeeGo Touch theme but painted
/// using the QPainter::drawPie() -- for usage see Spinner component
/// which is the motivation for this primitive.

#include "mdeclarativepiepixmap.h"

#include <QPainter>
#include <QPixmap>

MDeclarativePiePixmap::MDeclarativePiePixmap(QDeclarativeItem *parent) :
    MDeclarativePixmap(parent), m_pieBrush(Qt::NoBrush), m_piePen(Qt::NoPen),
    m_startAngle(0.0), m_spanAngle(90.0)
{
}

MDeclarativePiePixmap::~MDeclarativePiePixmap()
{
}

qreal MDeclarativePiePixmap::startAngle() const
{
    return m_startAngle;
}

void MDeclarativePiePixmap::setStartAngle(qreal startAngle)
{
    if (m_startAngle == startAngle)
        return;

    m_startAngle = startAngle;
    update();
    emit startAngleChanged(startAngle);
}

qreal MDeclarativePiePixmap::spanAngle() const
{
    return m_spanAngle;
}

void MDeclarativePiePixmap::setSpanAngle(qreal spanAngle)
{
    if (m_spanAngle == spanAngle)
        return;

    m_spanAngle = spanAngle;
    update();
    emit spanAngleChanged(spanAngle);
}

bool MDeclarativePiePixmap::hasPendingPixmap() {
    // Assumption is that hasPendingPixmap() will return false only once and
    // not be called anymore unless there's a possibility of a new Pixmap.
    // This is how MDeclarativePrimitive works at the moment.
    if (!MDeclarativePixmap::hasPendingPixmap()) {
        m_pieBrush.setTexture(*m_pixmap);
        return false;
    }

    // Invalidate previous brush, since we don't have a valid pixmap to use as texture
    // anymore.
    m_pieBrush.setStyle(Qt::NoBrush);
    return true;
}

void MDeclarativePiePixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rectf = boundingRect();
    if (rectf.isEmpty())
        return;

    if (m_pieBrush.style() != Qt::NoBrush) {
        painter->setBrush(m_pieBrush);
        painter->setPen(m_piePen);
        painter->drawPie(rectf.toRect(), m_startAngle * 16, m_spanAngle * 16);
    }
}
