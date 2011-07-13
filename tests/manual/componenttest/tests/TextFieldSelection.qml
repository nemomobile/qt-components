/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import com.nokia.symbian 1.1
import "../components"

Item {
    id: root
    anchors.fill: parent
    anchors.margins: privy.margin

    QtObject {
        id: privy
        property bool portrait:    screen.currentOrientation == Screen.Portrait
                                || screen.currentOrientation == Screen.PortraitInverted
        property int margin: 5
    }

    TextField {
        id: textField; objectName: "textField"
        anchors {
            left: privy.portrait ? parent.left : cursorAndButtons.right
            right: parent.right
            margins: privy.margin
        }
        text: "Lorem ipsum dolor sit amet."
        onCursorPositionChanged: cursorPosition.value = textField.cursorPosition
    }

    TextField {
        id: selectedText; objectName: "selectedText"
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: textField.bottom
            margins: privy.margin
        }
        text: textField.selectedText; enabled:false; readOnly: true
        placeholderText: "Selected Text"
    }

    Grid {
        id: selection
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors {
            left: privy.portrait ? parent.left : cursorAndButtons.right
            right: parent.right
            top: selectedText.bottom
            margins: privy.margin
        }
        columns: privy.portrait ? 1 : 2
        rows: privy.portrait ? 2 : 1
        height: parent.height * 1/3
        spacing: privy.margin

        Slider {
            id: start; objectName: "start"
            height: parent.h; width: parent.w;
            minimumValue: 0; maximumValue: textField.text.length
            stepSize: 1; valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.select(start.value, end.value)

            Label {
                text: "Selection start"
                font { pointSize: 5; bold: true }
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
            }
        }

        Slider {
            id: end; objectName: "end"
            height: parent.h; width: parent.w;
            minimumValue: 0; maximumValue: textField.text.length
            stepSize: 1; valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.select(start.value, end.value)

            Label {
                text: "Selection end"
                font { pointSize: 5; bold: true }
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
            }
        }
    }

    Slider {
        id: position; objectName: "position"
        anchors {
            left: privy.portrait ? parent.left : cursorAndButtons.right
            right: parent.right
            top: selection.bottom
            margins: privy.margin
        }
        height: parent.height * (privy.portrait ? 1/6 : 1/3)
        minimumValue: 0; maximumValue: textField.text.length
        stepSize: 1; valueIndicatorVisible: true
        value: textField.cursorPosition
        onValueChanged: if (pressed) textField.cursorPosition = value

        Label {
            text: "Cursor position"
            font { pointSize: 5; bold: true }
            anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
        }
    }

   Grid {
        id: cursorAndButtons
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors {
            left: parent.left
            top: privy.portrait ? position.bottom : parent.top
            bottom: parent.bottom
            margins: privy.margin
        }
        columns: privy.portrait ? 2 : 1
        rows: privy.portrait ? 2 : 4
        height: parent.height * (privy.portrait ? 1/6 : 1)
        width: parent.width * (privy.portrait ? 1 : 1/6)
        spacing: privy.margin

        Button {
            id: word; objectName: "word"
            height: parent.h; width: parent.w;
            text: "Word"
            onClicked: {
                textField.selectWord()
                start.value = textField.selectionStart
                end.value = textField.selectionEnd
            }
        }

        Button {
            id: all; objectName: "all"
            height: parent.h; width: parent.w;
            text: "All"
            onClicked: {
                textField.selectAll()
                start.value = textField.selectionStart
                end.value = textField.selectionEnd
            }
        }

        Button {
            id: select; objectName: "select"
            height: parent.h; width: parent.w;
            text: "Select"
            onClicked: textField.select(start.value, end.value)
        }

        Button {
            id: reset; objectName: "reset"
            height: parent.h; width: parent.w;
            text: "Reset"
            onClicked: {
                start.value = 0
                end.value = 0
                textField.select(start.value, end.value)
            }
        }
    }
}
