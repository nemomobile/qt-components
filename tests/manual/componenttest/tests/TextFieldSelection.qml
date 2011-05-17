/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

import QtQuick 1.0
import com.nokia.symbian 1.0

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

        Rectangle {
            x: textField.positionToRectangle(textField.selectionStart).x
            y: textField.positionToRectangle(textField.selectionStart).y
            height: parent.height; width: 5; opacity:0.7; color: "lightblue"
            visible: textField.selectionStart != textField.selectionEnd
        }

        Rectangle {
            x: textField.positionToRectangle(textField.selectionEnd).x
            y: textField.positionToRectangle(textField.selectionEnd).y
            height: parent.height; width: 5; opacity:0.7; color: "lightpink"
            visible: textField.selectionStart != textField.selectionEnd
        }
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

            Text {
                text: "Selection start"
                color: "white"
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

            Text {
                text: "Selection end"
                color: "white"
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

        Text {
            text: "Cursor position"
            color: "white"
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
