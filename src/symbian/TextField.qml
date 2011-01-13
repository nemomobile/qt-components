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
import com.nokia.symbian.themebridge 1.0

ImplicitSizeItem {
    id: root

    // Common Public API
    property alias placeholderText: placeholder.text
    property alias inputMethodHints: textInput.inputMethodHints
    property alias font: textInput.font
    property alias cursorPosition: textInput.cursorPosition
    property alias readOnly: textInput.readOnly
    // TODO FIX: when new enum available
    property alias echoMode: textInput.echoMode
    property alias acceptableInput: textInput.acceptableInput
    property alias inputMask: textInput.inputMask
    property alias validator: textInput.validator
    property alias selectedText: textInput.selectedText
    property alias selectionEnd: textInput.selectionEnd
    property alias selectionStart: textInput.selectionStart
    property alias text: textInput.text

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
    implicitWidth: style.current.get("paddingLeft") +
                   Math.max(style.textWidth(text, textInput.font), priv.minWidth) +
                   style.current.get("paddingRight")

    implicitHeight: style.current.get("paddingTop") +
                    style.fontHeight(textInput.font) +
                    style.current.get("paddingBottom")

    // Private data
    QtObject {
        id: priv
        // Minimum width is either placeholder text lenght or 5 spaces on current font.
        // Using placeholder text lenght as minimum width prevents implicit sized item from
        // shrinking on focus gain.
        property real minWidth: placeholder.text ? style.textWidth(placeholder.text, textInput.font)
                                                 : style.textWidth("     ", textInput.font)
    }

    Style {
        id: style
        mode: textInput.activeFocus ? "selected" : "default"
        styleClass: "TextField"
    }

    Frame {
        id: frame
        anchors.fill: parent
        frameName: style.current.get("frameName")
        frameType: style.current.get("frameType")
    }

    Item {
        id: container
        anchors {
            fill: parent
            leftMargin: style.current.get("paddingLeft"); rightMargin: style.current.get("paddingRight")
            topMargin: style.current.get("paddingTop"); bottomMargin: style.current.get("paddingBottom")
        }
        clip: true

        Text {
            id: placeholder
            anchors.fill: parent
            color: style.current.get("placeholderColor")
            font: style.current.get("font")
            visible: !textInput.activeFocus && !textInput.text && text
        }

        TextInput {
            id: textInput
            anchors.fill: parent
            color: style.current.get("textColor")
            font: style.current.get("font")
            // TODO: Use desktop text selection behaviour for now.
            selectByMouse: true
            selectedTextColor: style.current.get("selectionTextColor")
            selectionColor: style.current.get("selectionColor")
            // TODO: Enable when Qt Mobility gets desktop backends for haptick feedback
            //onFocusChanged: if (focus) style.play(ThemeEffect.Editor)
        }
    }
}
