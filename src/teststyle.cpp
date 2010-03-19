/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "teststyle.h"

#include <QtGui/QBrush>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QPainter>


TestStyle::TestStyle(QObject *parent)
    : Style("test", parent)
{
}


class RectWidget : public QGraphicsWidget
{
public:
    RectWidget(int border, QColor color, QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent), m_color(color), m_border(border) { }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->fillRect(rect().adjusted(m_border, m_border, -m_border, -m_border), m_color);
    }

private:
    QColor m_color;
    int m_border;
};


const char *TestComponentPopulator::styleName = "test";

QGraphicsWidget *TestComponentPopulator::createRectangle(int border, const QColor &color, QGraphicsItem *parent)
{
    return new RectWidget(border, color, parent);
}

