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
#include <mapplication.h>
#include <mapplicationwindow.h>
#include <mclassfactory.h>


MStyleWrapper::MStyleWrapper(QObject *parent)
    : QObject(parent), m_mode("default"), m_styleClass(), m_styleType()
{
    m_currentStyle[0] = 0;
    m_currentStyle[1] = 0;
    MStyleWrapperManager::instance()->registerStyleWrapper(this);
}

MStyleWrapper::~MStyleWrapper()
{
    invalidateStyle();
    MStyleWrapperManager::instance()->unregisterStyleWrapper(this);
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

    updateStyle();
    emit modeChanged();
}

QString MStyleWrapper::styleClass() const
{
    return m_styleClass;
}

void MStyleWrapper::setStyleClass(const QString &styleClass)
{
    if (styleClass == m_styleClass)
        return;
    m_styleClass = styleClass;

    updateStyle();
    emit styleClassChanged();
}

QString MStyleWrapper::styleType() const
{
    return m_styleType;
}

void MStyleWrapper::setStyleType(const QString &styleType)
{
    if (styleType == m_styleType)
        return;
    m_styleType = styleType;

    updateStyle();
    emit styleTypeChanged();
}

QString MStyleWrapper::styleObjectName() const
{
    return m_styleObjectName;
}

void MStyleWrapper::setStyleObjectName(const QString &styleObjectName)
{
    if (styleObjectName == m_styleObjectName)
        return;
    m_styleObjectName = styleObjectName;

    updateStyle();
    emit styleObjectNameChanged();
}

static M::Orientation orientationHelper()
{
    M::Orientation orientation = M::Landscape;

    const MWindow *activeWindow = MApplication::activeWindow();
    if (activeWindow)
        orientation = activeWindow->orientation();

    return orientation;
}

const MStyle *MStyleWrapper::currentStyle() const
{
    M::Orientation orientation = orientationHelper();

    // ### We can keep a cache "per-mode" like MStyleContainer from MeeGo Touch does.

    if (!m_currentStyle[orientation]) {
        // ### This function does lazy initialization, so we break the const
        // assumption here to do the actual initialization. We could avoid this
        // here and do this cast in other functions, like preferredWidth().
        MStyleWrapper *wrapper = const_cast<MStyleWrapper *>(this);

        // MTheme::style() assumes that the name of the style class will actually exist, so
        // we need to check.
        if (MClassFactory::instance()->styleMetaObject(m_styleClass.toAscii().constData())) {
            wrapper->m_currentStyle[orientation] = MTheme::style(m_styleClass.toAscii().constData(), m_styleObjectName, m_mode, m_styleType, orientation, 0);
        } else {
            qWarning("MStyleWrapper::currentStyle: could not find style class '%s'.", m_styleClass.toAscii().constData());
            wrapper->m_currentStyle[orientation] = 0;
        }
    }

    return m_currentStyle[orientation];
}

QObject *MStyleWrapper::currentStyleAsObject()
{
    return this;
}

int MStyleWrapper::preferredWidth() const
{
    const MWidgetStyle *style = qobject_cast<const MWidgetStyle *>(currentStyle());
    if (!style)
        return 0;

    const int min = style->minimumSize().width();
    const int pref = style->preferredSize().width();
    const int max = style->maximumSize().width();

    return qBound(min, pref, max);
}

int MStyleWrapper::preferredHeight() const
{
    const MWidgetStyle *style = qobject_cast<const MWidgetStyle *>(currentStyle());
    if (!style)
        return 0;

    const int min = style->minimumSize().height();
    const int pref = style->preferredSize().height();
    const int max = style->maximumSize().height();

    return qBound(min, pref, max);
}

// ### This property getter is a workaround to the following situation:
//
// - QML warns when when we bind to properties that doesn't have either a
//   NOTIFY signal or a CONSTANT mark.
//
// - libmeegotouch MStyle classes have properties with the data we want, but do not
//   have NOTIFY signal (they don't need since they write only once), neither
//   CONSTANT mark (because they have WRITE, which they use to dynamically fill the
//   properties from a set of CSS files).
//
// - Since MStyle is a QObject we do want to export it to QML instead of having to
//   create wrapper for each proper as we did before.
//
// This workaround avoids the warning by accessing the property in C++ (via the get()
// method). But we still need some signal to notify the properties that use data from
// get() that something change. The trick is to make the MStyleWrapper have a property
// that points to itself, and NOTIFY change when the style changes (what we want).
QVariant MStyleWrapper::get(const QString &propertyName)
{
    if (!currentStyle())
        return QVariant();

    return currentStyle()->property(propertyName.toAscii());
}

void MStyleWrapper::updateStyle()
{
    invalidateStyle();
    emit currentStyleChanged();
}

void MStyleWrapper::invalidateStyle()
{
    for (int i = 0; i < 2; i++) {
        if (m_currentStyle[i]) {
            MTheme::releaseStyle(m_currentStyle[i]);
            m_currentStyle[i] = 0;
        }
    }
}


MStyleWrapperManager *MStyleWrapperManager::m_self = 0;

MStyleWrapperManager::MStyleWrapperManager() : QObject()
{
}

MStyleWrapperManager::~MStyleWrapperManager()
{
}

MStyleWrapperManager *MStyleWrapperManager::instance()
{
    if (!m_self) {
        m_self = new MStyleWrapperManager();
    }

    return m_self;
}

void MStyleWrapperManager::registerStyleWrapper(MStyleWrapper *wrapper)
{
    Q_ASSERT(!m_registeredStyleWrappers.contains(wrapper));
    m_registeredStyleWrappers.append(wrapper);

    // If it's the first style wrapper, start watching for theme change.
    if (m_registeredStyleWrappers.size() == 1) {
        connect(MTheme::instance(), SIGNAL(themeIsChanging()), SLOT(updateStyleWrappers()));
    }
}

void MStyleWrapperManager::unregisterStyleWrapper(MStyleWrapper *wrapper)
{
    Q_ASSERT(m_registeredStyleWrappers.contains(wrapper));
    m_registeredStyleWrappers.removeOne(wrapper);

    // If we don't have more style wrappers, stop watching for theme change.
    if (m_registeredStyleWrappers.empty()) {
        disconnect(MTheme::instance(), SIGNAL(themeIsChanging()));
    }
}

void MStyleWrapperManager::updateStyleWrappers()
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
