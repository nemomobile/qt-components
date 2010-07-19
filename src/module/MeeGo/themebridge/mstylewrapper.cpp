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

#include "mstylewrapper.h"

#include <QVariant>
#include <mwidgetstyle.h>
#include <mbuttonstyle.h>

MStyleWrapper::MStyleWrapper(QObject *parent)
  : QObject(parent), m_mode("default"), m_styletype(None), m_stylecontainer(0)
{
}

MStyleWrapper::~MStyleWrapper()
{
    delete m_stylecontainer;
}

QString MStyleWrapper::mode() const
{
    return m_mode;
}

void MStyleWrapper::setMode(const QString &mode)
{
    if (mode == m_mode)
        return;

    m_mode = mode;
    updateStyleMode();
}

void MStyleWrapper::updateStyleMode()
{
    if (!m_stylecontainer)
        return;

    // XXX should use the protected method setMode(QString mode)
    if (m_mode == "default")
        m_stylecontainer->setModeDefault();
    else if (m_mode == "pressed")
        m_stylecontainer->setModePressed();
    else if (m_mode == "selected")
        m_stylecontainer->setModeSelected();
    else
        m_stylecontainer->setModeDefault();

    emit modeChanged(m_mode);
}

MStyleWrapper::StyleType MStyleWrapper::styleType() const
{
    return m_styletype;
}

void MStyleWrapper::setStyleType(const StyleType styletype)
{
    if (styletype == m_styletype)
        return;

    m_styletype = styletype;

    MStyleContainer *oldStyleContainer = m_stylecontainer;

    m_stylecontainer = 0;

    switch (m_styletype) {
    case Button:
        m_stylecontainer = new MButtonStyleContainer();
        m_stylecontainer->initialize("", "", 0);
        break;
    case GroupButton:
        m_stylecontainer = new MButtonStyleContainer();
        m_stylecontainer->initialize("", "group", 0);
        break;
    default:
        m_stylecontainer = new MWidgetStyleContainer();
        m_stylecontainer->initialize("", "", 0);
    }

    updateStyleMode();
    delete oldStyleContainer;
}

QColor MStyleWrapper::textColor() const
{
    if (!m_stylecontainer)
        return QColor();

    return (*m_stylecontainer)->property("textColor").value<QColor>();
}

const MWidgetStyleContainer *MStyleWrapper::styleContainer() const
{
    return m_stylecontainer;
}

// Needs patch in libmeegotouch
// QObject *MStyleWrapper::internalStyle()
// {
//     return m_stylecontainer ? const_cast<MStyle *>(m_stylecontainer->currentStyle()) : 0;
// }
