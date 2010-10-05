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

    property string text
    property bool  multiLine: false
    property int   documentMargin: 4
    property alias promptText: prompt.text
    property alias styleType: meegostyle.styleType

    // Inherited from text items
    property variant echoMode: TextInput.Normal
    property variant wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
    property variant inputElement: loader.item
    property variant activeFocusOnPress: true
    property variant horizontalAlignment: TextEdit.AlignLeft
    property variant color: meegostyle.current.get("textColor")
    property variant selectedTextColor: meegostyle.current.get("selectionTextColor")
    property variant selectionColor: meegostyle.current.get("selectionBackgroundColor")
    property variant font: meegostyle.current.get("font")
    property variant passwordCharacter: meegostyle.current.get("maskString")

    property real preferredWidth: inputElement.textModel.width +
            meegostyle.current.get("paddingLeft") +
            meegostyle.current.get("paddingRight") +
            2 * documentMargin

    property real preferredHeight: inputElement.textModel.height +
            meegostyle.current.get("paddingTop") +
            meegostyle.current.get("paddingBottom") +
            2 * documentMargin

    implicitWidth: Math.max(meegostyle.preferredWidth, preferredWidth)
    implicitHeight: preferredHeight

    Style {
        id: meegostyle
        styleClass: "MTextEditStyle"
        mode: inputElement.activeFocus ? "selected" : "default"
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle
    }

    // XXX Having an additional text element and duplicating all the text
    //     rendering logic is ugly and not efficient. I would expect textInput
    //     to expose the implicit sizes so I could use that as a parameter to
    //     control the size of my own component.
    //     Something like, preferredWidth = textInput.implicitWidth + margins


    Loader {
        id: loader

        anchors.fill:parent
        anchors.leftMargin: documentMargin + meegostyle.current.get("paddingLeft")
        anchors.rightMargin: documentMargin + meegostyle.current.get("paddingRight")
        anchors.topMargin: documentMargin + meegostyle.current.get("paddingTop")
        anchors.bottomMargin: documentMargin + meegostyle.current.get("paddingBottom")

        sourceComponent: multiLine ? multilineComponent : singleLineComponent

        clip:true

        Text {
            id: prompt
            anchors.fill: parent
            visible: !inputElement.activeFocus && !inputElement.text && prompt.text
            color: meegostyle.current.get("promptColor")
            font: root.font
        }
    }

    Component {
        id:multilineComponent
        TextEdit {
            id: textEdit
            anchors.fill: parent
            text:root.text
            onTextChanged: root.text = text
            visible: multiLine ? true : false
            font: root.font
            color: root.color
            wrapMode: root.wrapMode
            selectByMouse: true
            horizontalAlignment: root.horizontalAlignment
            selectedTextColor: root.selectedTextColor
            selectionColor: root.selectionColor
            property variant textModel : Text {
                font: root.font;
                text: root.text;
                visible: false
                wrapMode: root.wrapMode;
                width: textEdit.width
            }
        }
    }

    Component {
        id: singleLineComponent
        TextInput {
            id: textInput
            anchors.fill: parent
            text:root.text
            onTextChanged: root.text = text
            visible: multiLine ? false : true
            passwordCharacter: root.passwordCharacter
            echoMode: root.echoMode
            font: root.font
            color: root.color
            selectByMouse: true
            horizontalAlignment: root.horizontalAlignment
            selectedTextColor: root.selectedTextColor
            selectionColor: root.selectionColor
            property variant textModel: Text {
                font: root.font;
                text: root.text;
                visible: false
            }
        }
    }
}
