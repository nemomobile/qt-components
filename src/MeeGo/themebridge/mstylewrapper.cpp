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

#include <mtheme.h>
#include <mbuttonstyle.h>
#include <mcheckboxstyle.h>
#include <msliderstyle.h>
#include <mnavigationbarstyle.h>
#include <mhomebuttonpanelstyle.h>
#include <mbuttonswitchstyle.h>
#include <mtexteditstyle.h>
#include <mlabelstyle.h>

MStyleWrapper::MStyleWrapper(QObject *parent)
  : QObject(parent), m_mode(DefaultMode), m_styletype(None), m_stylecontainer(0)
{
    // Emit notifier signals for all properties we export
    connect(MTheme::instance(), SIGNAL(themeChangeCompleted()), SLOT(notifyProperties()));
}

MStyleWrapper::~MStyleWrapper()
{
    delete m_stylecontainer;
}

MStyleWrapper::StyleMode MStyleWrapper::mode() const
{
    return m_mode;
}

void MStyleWrapper::setMode(const StyleMode mode)
{
    if (mode == m_mode)
        return;

    m_mode = mode;
    updateStyleMode();
}

void MStyleWrapper::notifyProperties()
{
    // Notify each of the properties we export directly and also any
    // primitives that might be accessing our data.
    // Currently this signal handles it all.
    emit modeChanged(m_mode);
}

void MStyleWrapper::updateStyleMode()
{
    if (!m_stylecontainer)
        return;

    switch (m_mode) {
    case DefaultMode:
        m_stylecontainer->setModeDefault();
        break;
    case PressedMode:
        m_stylecontainer->setModePressed();
        break;
    case SelectedMode:
        m_stylecontainer->setModeSelected();
        break;
    }

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

    const char *viewtype = "";

    switch (m_styletype) {
    case Button:
        m_stylecontainer = new MButtonStyleContainer();
        break;
    case GroupButton:
        m_stylecontainer = new MButtonStyleContainer();
        viewtype = "group";
        break;
    case CheckBox:
        m_stylecontainer = new MCheckboxStyleContainer();
        break;
    case Slider:
        m_stylecontainer = new MSliderStyleContainer();
        break;
    case NavigationBar:
        m_stylecontainer = new MNavigationBarStyleContainer();
        break;
    case HomeButton:
        m_stylecontainer = new MHomeButtonPanelStyleContainer();
        break;
    case Switch:
        m_stylecontainer = new MButtonSwitchStyleContainer();
        break;
    case TextEdit:
        m_stylecontainer = new MTextEditStyleContainer();
        break;
    case Label:
        m_stylecontainer = new MLabelStyleContainer();
        break;
    default:
        m_stylecontainer = new MWidgetStyleContainer();
    }
    m_stylecontainer->initialize("", viewtype, 0);

    updateStyleMode();
    delete oldStyleContainer;
}

int MStyleWrapper::preferredWidth() const
{
    if (!m_stylecontainer)
        return 0;

    const int min = (*m_stylecontainer)->minimumSize().width();
    const int pref = (*m_stylecontainer)->preferredSize().width();
    const int max = (*m_stylecontainer)->maximumSize().width();

    return qBound(min, pref, max);
}

int MStyleWrapper::preferredHeight() const
{
    if (!m_stylecontainer)
        return 0;

    const int min = (*m_stylecontainer)->minimumSize().height();
    const int pref = (*m_stylecontainer)->preferredSize().height();
    const int max = (*m_stylecontainer)->maximumSize().height();

    return qBound(min, pref, max);
}

QColor MStyleWrapper::textColor() const
{
    if (!m_stylecontainer)
        return QColor();

    const char *propertyName;

    switch (m_styletype) {
    case Label:
        propertyName = "color";
        break;
    default:
        propertyName = "textColor";
    }

    return (*m_stylecontainer)->property(propertyName).value<QColor>();
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
