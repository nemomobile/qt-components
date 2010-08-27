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

import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: root

    property alias text: textInput.text
    property alias promptText: prompt.text
    property alias echoMode: textInput.echoMode
    property alias activeFocusOnPress: textInput.activeFocusOnPress
    property alias horizontalAlignment: textInput.horizontalAlignment

    implicitWidth: Math.max(meegostyle.preferredWidth, textSizeModel.preferredWidth)
    implicitHeight: textSizeModel.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MTextEditStyle"
        mode: textInput.focus ? "selected" : "default"
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle
    }

    Text {
        id: prompt
        anchors.fill: textInput
        visible: !textInput.focus && !textInput.text && prompt.text

        clip: true
        color: meegostyle.current.get("promptColor")
        font: meegostyle.current.get("font")
    }

    TextInput {
        id: textInput

        property int documentMargin: 4

        anchors.fill: parent
        anchors.leftMargin: textInput.documentMargin + meegostyle.current.get("paddingLeft")
        anchors.rightMargin: textInput.documentMargin + meegostyle.current.get("paddingRight")
        anchors.topMargin: textInput.documentMargin + meegostyle.current.get("paddingTop")
        anchors.bottomMargin: textInput.documentMargin + meegostyle.current.get("paddingBottom")

        clip: true
        selectByMouse: true
        color: meegostyle.current.get("textColor")
        selectedTextColor: meegostyle.current.get("selectionTextColor")
        selectionColor: meegostyle.current.get("selectionBackgroundColor")
        font: meegostyle.current.get("font")
        passwordCharacter: meegostyle.current.get("maskString")
    }

    // XXX Having an additional text element and duplicating all the text
    //     rendering logic is ugly and not efficient. I would expect textInput
    //     to expose the implicit sizes so I could use that as a parameter to
    //     control the size of my own component.
    //     Something like, preferredWidth = textInput.implicitWidth + margins
    TextInput {
        id: textSizeModel
        font: textInput.font
        text: textInput.text
        visible: false

        property real preferredWidth
        property real preferredHeight

        preferredWidth: textSizeModel.width + meegostyle.current.get("paddingLeft") +
            meegostyle.current.get("paddingRight") + 2 * textInput.documentMargin

        preferredHeight: textSizeModel.height + meegostyle.current.get("paddingTop") +
            meegostyle.current.get("paddingBottom") + 2 * textInput.documentMargin
    }
}
