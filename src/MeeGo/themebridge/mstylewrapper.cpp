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

#include "mthemebridge.h"

#include <mtheme.h>
#include <mapplication.h>
#include <mapplicationwindow.h>
#include <minputmethodstate.h>

// to be able to qvariant_cast MFeedback
Q_DECLARE_METATYPE(MFeedback)

static M::Orientation currentOrientation()
{
    const M::OrientationAngle angle = \
        MInputMethodState::instance()->activeWindowOrientationAngle();
    return (angle == M::Angle0 || angle == M::Angle180) ? M::Landscape : M::Portrait;
}

MStyleWrapper::MStyleWrapper(QObject *parent)
    : QObject(parent), m_mode("default"), m_styleClass(), m_styleType()
{
    m_currentStyle[0] = 0;
    m_currentStyle[1] = 0;
    MThemeBridge::instance()->registerStyleWrapper(this);
}

MStyleWrapper::~MStyleWrapper()
{
    invalidateStyle();
    MThemeBridge::instance()->unregisterStyleWrapper(this);
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

    // Since we keep a cache of different modes, changing a mode will not
    // invalidate our cache, just reset the current style and emit the
    // styleChanged signal. The getter for current style will load the style
    // new mode -- possibly from our cache.
    m_currentStyle[0] = 0;
    m_currentStyle[1] = 0;
    emit currentStyleChanged();
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

QString MStyleWrapper::styleParentClass() const
{
    return m_styleParentClass;
}

void MStyleWrapper::setStyleParentClass(const QString &styleParentClass)
{
    if (styleParentClass == m_styleParentClass)
        return;
    m_styleParentClass = styleParentClass;

    updateStyle();
    emit styleParentClassChanged();
}

const MStyle *MStyleWrapper::currentStyle() const
{
    M::Orientation orientation = currentOrientation();

    if (m_currentStyle[orientation]) {
        return m_currentStyle[orientation];
    }

    // ### This function does lazy initialization, so we break the const
    // assumption here to do the actual initialization. We could avoid this
    // here and do this cast in other functions, like preferredWidth().
    MStyleWrapper *wrapper = const_cast<MStyleWrapper *>(this);

    const MStyle *style = 0;
    QHash<QString, const MStyle *>::iterator it = wrapper->m_cachedStyles[orientation].find(m_mode);
    if (it != wrapper->m_cachedStyles[orientation].end()) {
        style = it.value();
    } else {
        QList<QStringList> parentClassNames;
        if (!m_styleParentClass.isEmpty()) {
            // ### Works for simple cases in themes we use. For complete coverage
            // each string list need to have the entire hierarchy of the class. We
            // don't have dynamic access to that information (we don't want to create
            // controllers for libmeegotouch widgets), so if we need to implement
            // that, we'll need a map class -> hierarchy list.
            parentClassNames.append(QStringList(m_styleParentClass));
        }

        style = MTheme::style(m_styleClass.toAscii().constData(), m_styleObjectName,
                              m_mode, m_styleType, orientation, parentClassNames, QString());

        if (style) {
            wrapper->m_cachedStyles[orientation].insert(m_mode, style);
        } else {
            qWarning("MStyleWrapper::currentStyle: could not find style class '%s'.",
                     m_styleClass.toAscii().constData());
        }
    }

    wrapper->m_currentStyle[orientation] = style;
    return style;
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
        QHash<QString, const MStyle *>::iterator it = m_cachedStyles[i].begin();
        while (it != m_cachedStyles[i].end()) {
            MTheme::releaseStyle(it.value());
            ++it;
        }
        m_cachedStyles[i].clear();
        m_currentStyle[i] = 0;
    }
}

void MStyleWrapper::feedback(const QByteArray& feedbackFunctionName)
{
    const MWidgetStyle *style = qobject_cast<const MWidgetStyle *>(currentStyle());
    if (style && style->property(feedbackFunctionName.constData()).canConvert<MFeedback>())
        qvariant_cast<MFeedback>(style->property(feedbackFunctionName.constData())).play();
}
