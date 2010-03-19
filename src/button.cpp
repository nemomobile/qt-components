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

#include "button.h"

#include "teststyle.h"


Button::Button(QColor color, QDeclarativeItem *parent)
    : QDeclarativeItem(parent), m_color(color)
{
    // ### knowing style vs. Widget::populate()
    Style::instance()->populate(this, 0);
}

QML_DECLARE_TYPE(Button);


void ButtonTestPopulator::populate(QGraphicsObject *component, QObject *model)
{
    // This code should be able to assume the correct types for model and widget.

    // ### usually we would like to rely on the model instead of the
    // component, since different components might want to use the
    // same model (think of a QGW and a QDI both using the same model, and wanting to
    // use the same style).
    Q_UNUSED(model);

    // ### we would have to actually bind or connect the color property or others
    // from the model, and if needed even from the widget

    // Note that we use "common style primitives" defined in TestComponentPopulator
    QGraphicsWidget *r = createRectangle(20, QColor(255, 0, 0, 128), component);
    r = createRectangle(10, QColor(0, 255, 0, 128), component);
}

STYLE_REGISTER_COMPONENT_POPULATOR(Button, ButtonTestPopulator);
