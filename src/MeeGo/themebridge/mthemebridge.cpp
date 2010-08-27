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

#include "mthemebridge.h"

#include "mstylewrapper.h"

#include <mtheme.h>


MThemeBridge *MThemeBridge::m_self = 0;

MThemeBridge::MThemeBridge() : QObject()
{
}

MThemeBridge::~MThemeBridge()
{
}

MThemeBridge *MThemeBridge::instance()
{
    if (!m_self) {
        m_self = new MThemeBridge();
    }

    return m_self;
}

void MThemeBridge::registerStyleWrapper(MStyleWrapper *wrapper)
{
    Q_ASSERT(!m_registeredStyleWrappers.contains(wrapper));
    m_registeredStyleWrappers.append(wrapper);

    // If it's the first style wrapper, start watching for theme change.
    if (m_registeredStyleWrappers.size() == 1) {
        connect(MTheme::instance(), SIGNAL(themeIsChanging()), SLOT(updateStyleWrappers()));
    }
}

void MThemeBridge::unregisterStyleWrapper(MStyleWrapper *wrapper)
{
    Q_ASSERT(m_registeredStyleWrappers.contains(wrapper));
    m_registeredStyleWrappers.removeOne(wrapper);

    // If we don't have more style wrappers, stop watching for theme change.
    if (m_registeredStyleWrappers.empty()) {
        disconnect(MTheme::instance(), SIGNAL(themeIsChanging()));
    }
}

void MThemeBridge::updateStyleWrappers()
{
    // First we make sure that the style wrappers invalidate all cached
    // styles they have.
    for (int i = 0; i < m_registeredStyleWrappers.size(); i++) {
        m_registeredStyleWrappers[i]->invalidateStyle();
    }

    // ...once they all are invalidated, we trigger a changed signal in
    // the styles themselves. From our tests, it is important to fully
    // cleanup the styles (not have any old style) before start to
    // asking for new styles. We used this two loops together, but had
    // situations when a new style was asked, and old one was received.
    for (int i = 0; i < m_registeredStyleWrappers.size(); i++) {
        emit m_registeredStyleWrappers[i]->currentStyleChanged();
    }
}
