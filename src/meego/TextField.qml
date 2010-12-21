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

    // Common public API
    property string text
    property alias placeholderText: prompt.text

    property alias inputMethodHints: textInput.inputMethodHints
    property alias cursorPosition: textInput.cursorPosition
    property alias readOnly: textInput.readOnly
    property alias acceptableInput: textInput.acceptableInput
    property alias inputMask: textInput.inputMask
    property alias validator: textInput.validator

    property alias selectedText: textInput.selectedText
    property alias selectionStart: textInput.selectionStart
    property alias selectionEnd: textInput.selectionEnd

    property variant font: meegostyle.current.get("font")
    property alias echoMode: textInput.echoMode // ### TODO: declare own enum { Normal, Password }

    function copy() {
        textInput.copy()
    }

    function paste() {
        textInput.paste()
    }

    function cut() {
        textInput.cut()
    }

    function select(start, end) {
        textInput.select(start, end)
    }

    function selectAll() {
        textInput.selectAll()
    }

    function selectWord() {
        textInput.selectWord()
    }

    function positionAt(x) {
        // ### TODO: remove the left margins from x
        return textInput.positionAt(x)
    }

    function positionToRectangle(pos) {
        // ### TODO: translate rect to TextField coord
        return textInput.positionToRectangle(pos)
    }

    // API extensions
    property real preferredWidth: textInput.textModel.width +
            meegostyle.current.get("paddingLeft") +
            meegostyle.current.get("paddingRight") +
            2 * __documentMargin

    property real preferredHeight: textInput.textModel.height +
            meegostyle.current.get("paddingTop") +
            meegostyle.current.get("paddingBottom") +
            2 * __documentMargin

    implicitWidth: Math.max(meegostyle.preferredWidth, preferredWidth)
    implicitHeight: preferredHeight

    // private
    property variant __passwordCharacter: meegostyle.current.get("maskString")
    property int __documentMargin: 4


    Style {
        id: meegostyle
        styleClass: "MTextEditStyle"
        mode: textInput.activeFocus ? "selected" : "default"
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


    anchors.leftMargin: __documentMargin + meegostyle.current.get("paddingLeft")
    anchors.rightMargin: __documentMargin + meegostyle.current.get("paddingRight")
    anchors.topMargin: __documentMargin + meegostyle.current.get("paddingTop")
    anchors.bottomMargin: __documentMargin + meegostyle.current.get("paddingBottom")

    clip: true

    Text {
        id: prompt
        anchors.fill: parent
        visible: !textInput.activeFocus && !textInput.text && prompt.text
        color: meegostyle.current.get("promptColor")
        font: root.font
    }

    TextInput {
        id: textInput
        x: root.anchors.leftMargin
        y: root.anchors.topMargin
        width: root.width - x - root.anchors.rightMargin
        height: root.height - y - root.anchors.bottomMargin
        text: root.text
        onTextChanged: root.text = text
        passwordCharacter: root.__passwordCharacter
        font: root.font
        color: meegostyle.current.get("textColor")
        selectByMouse: true
        selectedTextColor: meegostyle.current.get("selectionTextColor")
        selectionColor: meegostyle.current.get("selectionBackgroundColor")
        property variant textModel: Text {
            font: root.font;
            text: root.text;
            visible: false
        }
        MouseArea {
            anchors.fill: parent
            onReleased: {
                parent.focus = true;
                screen.sendClicked(mouseX,mouseY,parent.positionAt(mouseX));
            }
        }
    }
}
