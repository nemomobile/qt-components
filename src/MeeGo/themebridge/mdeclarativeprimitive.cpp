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

#include "mdeclarativeprimitive.h"
#include "mstylewrapper.h"

#include <MTheme>
#include <MWidgetStyle>

MDeclarativePrimitive::MDeclarativePrimitive(QDeclarativeItem *parent) :
    MDeclarativeImplicitSizeItem(parent), m_style(0)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

MDeclarativePrimitive::~MDeclarativePrimitive()
{
}

MStyleWrapper *MDeclarativePrimitive::style() const
{
    return m_style;
}

void MDeclarativePrimitive::setStyle(MStyleWrapper *style)
{
    if (style == m_style)
        return;

    if (m_style) {
        // Stop listening for mode changes in the old style
        disconnect(m_style, SIGNAL(currentStyleChanged()), this, SLOT(updateStyleData()));
        clearStyleData();
    }

    m_style = style;

    if (m_style) {
        // Listen for mode and/or styleType changes
        connect(m_style, SIGNAL(currentStyleChanged()), SLOT(updateStyleData()));
        updateStyleData();
    }

    update();
}

void MDeclarativePrimitive::updateStyleData()
{
    // This check is to protect from subclass calls to this method
    // We never call updateStyleData ourselves if we have no style set
    if (!m_style)
        return;

    const MStyle *style = m_style->currentStyle();

    if (style) {
        // Fill subclass-specific members with updated data from style
        fetchStyleData(style);
    } else {
        clearStyleData();
    }

    update();
}

void MDeclarativePrimitive::clearStyleData()
{

}

void MDeclarativePrimitive::fetchStyleData(const MStyle *style)
{
    Q_UNUSED(style);
}

