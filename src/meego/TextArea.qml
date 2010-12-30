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

    property alias cursorPosition: textEdit.cursorPosition
    property alias readOnly: textEdit.readOnly

    property alias horizontalAlignment: textEdit.horizontalAlignment
    property alias verticalAlignment: textEdit.verticalAlignment
    property variant font: meegostyle.current.get("font")

    property alias selectedText: textEdit.selectedText
    property alias selectionStart: textEdit.selectionStart
    property alias selectionEnd: textEdit.selectionEnd

    property alias wrapMode: textEdit.wrapMode // ### was TextEdit.WrapAtWordBoundaryOrAnywhere
    property alias textFormat: textEdit.textFormat

    function copy() {
        textEdit.copy()
    }

    function paste() {
        textEdit.paste()
    }

    function cut() {
        textEdit.cut()
    }

    function select(start, end) {
        textEdit.select(start, end)
    }

    function selectAll() {
        textEdit.selectAll()
    }

    function selectWord() {
        textEdit.selectWord()
    }

    function positionAt(x) {
        // ### TODO: remove the left margins from x
        return textEdit.positionAt(x)
    }

    function positionToRectangle(pos) {
        // ### TODO: translate rect to TextArea coord
        return textEdit.positionToRectangle(pos)
    }

    // API extensions
    property real preferredWidth: textEdit.textModel.width +
            meegostyle.current.get("paddingLeft") +
            meegostyle.current.get("paddingRight") +
            2 * __documentMargin

    property real preferredHeight: textEdit.textModel.height +
            meegostyle.current.get("paddingTop") +
            meegostyle.current.get("paddingBottom") +
            2 * __documentMargin

    implicitWidth: Math.max(meegostyle.preferredWidth, preferredWidth)
    implicitHeight: preferredHeight

    // private
    property int __documentMargin: 4

    Style {
        id: meegostyle
        styleClass: "MTextEditStyle"
        mode: textEdit.activeFocus ? "selected" : "default"
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle
    }

    // XXX Having an additional text element and duplicating all the text
    //     rendering logic is ugly and not efficient. I would expect textEdit
    //     to expose the implicit sizes so I could use that as a parameter to
    //     control the size of my own component.
    //     Something like, preferredWidth = textEdit.implicitWidth + margins


    anchors.leftMargin: __documentMargin + meegostyle.current.get("paddingLeft")
    anchors.rightMargin: __documentMargin + meegostyle.current.get("paddingRight")
    anchors.topMargin: __documentMargin + meegostyle.current.get("paddingTop")
    anchors.bottomMargin: __documentMargin + meegostyle.current.get("paddingBottom")

    clip: true

    Text {
        id: prompt

        anchors.fill: parent
        anchors.leftMargin: __documentMargin + meegostyle.current.get("paddingLeft")
        anchors.rightMargin: __documentMargin + meegostyle.current.get("paddingRight")
        anchors.topMargin: __documentMargin + meegostyle.current.get("paddingTop")
        anchors.bottomMargin: __documentMargin + meegostyle.current.get("paddingBottom")

        visible: !textEdit.activeFocus && !textEdit.text && prompt.text
        color: meegostyle.current.get("promptColor")
        font: root.font
    }

    TextEdit {
        id: textEdit

        anchors.fill: parent
        anchors.leftMargin: __documentMargin + meegostyle.current.get("paddingLeft")
        anchors.rightMargin: __documentMargin + meegostyle.current.get("paddingRight")
        anchors.topMargin: __documentMargin + meegostyle.current.get("paddingTop")
        anchors.bottomMargin: __documentMargin + meegostyle.current.get("paddingBottom")

        text: root.text
        onTextChanged: root.text = text
        font: root.font
        color: meegostyle.current.get("textColor")
        selectByMouse: true
        selectedTextColor: meegostyle.current.get("selectionTextColor")
        selectionColor: meegostyle.current.get("selectionBackgroundColor")
        property variant textModel: Text {
            font: root.font;
            text: root.text;
            visible: false
            wrapMode: textEdit.wrapMode;
        }
        MouseArea {
            anchors.fill: parent
            onReleased: {
                parent.focus=true;
                screen.sendClicked(mouseX,mouseY,parent.positionAt(mouseX,mouseY));
            }
        }
    }
}
