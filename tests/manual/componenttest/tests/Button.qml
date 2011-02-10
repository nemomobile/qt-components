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

import QtQuick 1.0
import com.nokia.symbian 1.0

Item {

    function isLandscape() {
        if ( screen.width > screen.height )
            return true;
        else
            return false;
    }

    Button {
        id: button1

        anchors { top: parent.top; topMargin: 50; left: parent.left; leftMargin: 50 }
        anchors { right: isLandscape() ? buttonTextEditor.left : parent.right; rightMargin: isLandscape() ? 15 : 50 }
        anchors { bottom: button2.top; bottomMargin: 15 }
        text: buttonTextEditor.text
        iconSource: ""

        Rectangle {
            width: parent.implicitWidth
            height: parent.implicitHeight
            anchors.centerIn: parent
            color: "#00000000"
            border.color: "blue"
        }
    }

    Button {
        id: button2

        anchors { left: parent.left; leftMargin: 50; right: button1.right }
        anchors { bottom: isLandscape() ? parent.bottom : buttonTextEditor.top; bottomMargin: 50 }
        width: 150
        height: 35
        text: buttonTextEditor.text
        iconSource: ""
        checkable: true
    }

    Button {
        id: button3

        anchors { top: button2.bottom; left: parent.left; leftMargin: 50 }
        longPress: true
        width: 150
        height: 35
        text: "longPress"

        onClicked: {
            rect.x += 2;
            if (rect.x > parent.width)
                rect.x = 0;
        }

        onPressAndHold: {
            var menu = Qt.createQmlObject('import Qt 4.7; Rectangle {id: tempMenu; color: "lightsteelblue"; width: 200; height: 100; anchors.centerIn: parent; opacity: 0.8; Text {id: text1; text: "Click to close"; font.pointSize: 12; anchors.top: parent.top;}  MouseArea{anchors.fill: parent; onClicked: {tempMenu.destroy();} } }', button1.parent, "menu");
        }
    }

    CheckBox {
        id: checkBox

        anchors { top: button2.bottom; left: button3.right }
        width: 100
        height: 50
        text: "toggle autorepeat"
        checked: false

        onClicked: {
            if (checked) {
                button3.autoRepeat = true;
                button3.longPress = false;
                button3.text = "autoRepeat"
            }
            else {
                button3.longPress = true;
                button3.autoRepeat = false;
                button3.text = "longPress"
            }
        }
    }

    Rectangle {
        id: rect

        anchors.top: button3.bottom
        x: 0
        width: 50
        height: 15
        color: "red"
    }

    TextField {
        id: buttonTextEditor

        anchors { right: parent.right; rightMargin: 50; bottom: buttonIconEditor.top; bottomMargin: 15 }
        width: 150
        height: 40
        text: "Button text"
        clip:true
    }

    TextField {
         id: buttonIconEditor

         anchors { left: parent.left; bottom: parent.bottom; bottomMargin: 50; right: parent.right; rightMargin: 150 }
         height: 40
         text: "image://theme/:/list1.png"
         clip:true
     }

    Button {
        anchors { left: buttonIconEditor.right; verticalCenter: buttonIconEditor.verticalCenter}
        text:"Load file!"
        width: 150
        height: 50

        onClicked: {
            button1.iconSource = buttonIconEditor.text
            button2.iconSource = buttonIconEditor.text
        }
    }
}
