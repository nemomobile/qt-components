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

#ifndef MSTYLEWRAPPER_H
#define MSTYLEWRAPPER_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QVariant>

#include <mwidgetstyle.h>

#define M_STYLE_PROPERTY(type, name, propertyName, defaultValue)       \
inline type name () const {                                                \
    if (!m_stylecontainer)                                                 \
        return defaultValue ;                                              \
                                                                           \
    return (*m_stylecontainer)->property( propertyName ).value< type >(); \
}

class MWidgetStyleContainer;

class MStyleWrapper : public QObject
{
    Q_OBJECT

    Q_ENUMS(StyleMode)
    Q_ENUMS(StyleType)

    // To be set from QML
    Q_PROPERTY(StyleMode mode READ mode WRITE setMode)
    Q_PROPERTY(StyleType styleType READ styleType WRITE setStyleType)

    // To expose data from current StyleContainer
    Q_PROPERTY(int preferredWidth READ preferredWidth NOTIFY modeChanged)
    Q_PROPERTY(int preferredHeight READ preferredHeight NOTIFY modeChanged)
    Q_PROPERTY(QColor textColor READ textColor NOTIFY modeChanged)
    Q_PROPERTY(QColor promptColor READ promptColor NOTIFY modeChanged)
    Q_PROPERTY(QColor selectionTextColor READ selectionTextColor NOTIFY modeChanged)
    Q_PROPERTY(QColor selectionBackgroundColor READ selectionBackgroundColor NOTIFY modeChanged)
    Q_PROPERTY(QString maskString READ maskString NOTIFY modeChanged)

    Q_PROPERTY(int marginLeft READ marginLeft NOTIFY modeChanged)
    Q_PROPERTY(int marginTop READ marginTop NOTIFY modeChanged)
    Q_PROPERTY(int marginRight READ marginRight NOTIFY modeChanged)
    Q_PROPERTY(int marginBottom READ marginBottom NOTIFY modeChanged)
    Q_PROPERTY(int paddingLeft READ paddingLeft NOTIFY modeChanged)
    Q_PROPERTY(int paddingTop READ paddingTop NOTIFY modeChanged)
    Q_PROPERTY(int paddingRight READ paddingRight NOTIFY modeChanged)
    Q_PROPERTY(int paddingBottom READ paddingBottom NOTIFY modeChanged)

    Q_PROPERTY(QFont font READ font NOTIFY modeChanged)

    // XXX Were not "MStyleContainer::currentStyle()" private, we could consider
    // replacing the above properties by the single one below
    // Q_PROPERTY(QObject * internalStyle READ internalStyle);

public:
    MStyleWrapper(QObject *parent = 0);
    virtual ~MStyleWrapper();

    enum StyleMode {
        DefaultMode = 0,
        PressedMode,
        SelectedMode
    };

    enum StyleType {
        None = 0,
        Button,
        GroupButton,
        CheckBox,
        Slider,
        NavigationBar,
        HomeButton,
        TextEdit,
        Label,
        Page,
        Switch
    };

    StyleMode mode() const;
    void setMode(const StyleMode mode);

    StyleType styleType() const;
    void setStyleType(const StyleType styleType);

    int preferredWidth() const;
    int preferredHeight() const;
    QColor textColor() const;

    const MWidgetStyleContainer *styleContainer() const;

    M_STYLE_PROPERTY(QColor, promptColor, "promptColor", QColor() )
    M_STYLE_PROPERTY(QColor, selectionTextColor, "selectionTextColor", QColor() )
    M_STYLE_PROPERTY(QColor, selectionBackgroundColor, "selectionBackgroundColor", QColor() )
    M_STYLE_PROPERTY(QFont, font, "font", QFont() )
    M_STYLE_PROPERTY(QString, maskString, "maskString", "*")

    M_STYLE_PROPERTY(int, marginLeft, "marginLeft", 0)
    M_STYLE_PROPERTY(int, marginTop, "marginTop", 0)
    M_STYLE_PROPERTY(int, marginRight, "marginRight", 0)
    M_STYLE_PROPERTY(int, marginBottom, "marginBottom", 0)
    M_STYLE_PROPERTY(int, paddingLeft, "paddingLeft", 0)
    M_STYLE_PROPERTY(int, paddingTop, "paddingTop", 0)
    M_STYLE_PROPERTY(int, paddingRight, "paddingRight", 0)
    M_STYLE_PROPERTY(int, paddingBottom, "paddingBottom", 0)

    // Needs patch in libmeegotouch
    // QObject *internalStyle();

Q_SIGNALS:
    void modeChanged(const StyleMode newMode);

protected Q_SLOTS:
    void notifyProperties();

protected:
    void updateStyleMode();

    StyleMode m_mode;
    StyleType m_styletype;
    MWidgetStyleContainer *m_stylecontainer;
};

#endif // MSTYLEWRAPPER_H
