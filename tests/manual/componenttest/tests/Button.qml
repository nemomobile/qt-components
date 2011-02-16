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
import com.nokia.symbian 1.0
import Qt.labs.components 1.0

Item {
    id: root

    function isLandscape() {
        if ( screen.width > screen.height )
            return true;
        else
            return false;
    }


    Column {
        id: common
        anchors {
            top: parent.top
            left: parent.left
            right: isLandscape() ? undefined : parent.right
            margins: 10
        }
        spacing: 10

        Row {
            ChoiceList {
                Component.onCompleted: {console.log("ChoiceList::onCompleted: " + currentValue + " " + currentIndex)}

                id: iconChoicelist
                width: 200
                onCurrentValueChanged: {
                    currentValue != "<none>" ? scalableButton.iconSource = "image://theme/:/" + currentValue :  scalableButton.iconSource = ""
                    currentValue != "<none>" ? mixedButton.iconSource = "image://theme/:/" + currentValue :  mixedButton.iconSource = ""
                }
                model: ["<none>", "list1.png", "list2.png", "list3.png", "list4.png", "list5.png",
                        "list6.png", "list7.png", "list8.png", "list9.png", "list10.png",
                        "list11.png", "list12.png", "list13.png", "list14.png", "list15.png"]
            }

            Button {
                text: "Unset"
                onClicked: {
                    scalableButton.iconSource = ""
                    mixedButton.iconSource = ""
                }
            }
        }

        Row {
            spacing: 10
            Text {
                color: "white"
                font.pointSize: 12
                text: "Button text:"
            }

            TextField {
                id: buttonTextEditor
                text: "Hello QML"
            }
        }

        Button {
            id: checkableButton
            text: "Auto repeat / Long press"
            checkable: true
            onCheckedChanged: {
                if (checked) {
                    mixedButton.text = "Auto repeat"
                    mixedButton.longPress = false;
                    mixedButton.autoRepeat = true;
                } else {
                    mixedButton.text = "Long press"
                    mixedButton.longPress = true;
                    mixedButton.autoRepeat = false;
                }
            }
        }

        Button {
            id: mixedButton
            longPress: true
            text: "Long press"

            onClicked: {
                rect.x += 2;
                if (rect.x + rect.width > parent.width)
                    rect.x = mixedButton.x + mixedButton.width + 10;
            }

            onPressAndHold: {
                var menu = Qt.createQmlObject('import Qt 4.7; Rectangle { id: tempMenu; color: "lightsteelblue"; width: 200; height: 100; anchors.centerIn: parent; opacity: 0.8; Text { id: text1; text: "Click to close"; font.pointSize: 12; anchors.top: parent.top;}  MouseArea { anchors.fill: parent; onClicked: {tempMenu.destroy(); } } }', scalableButton.parent, "menu");
            }

            Rectangle {
                id: rect
                x: mixedButton.x + mixedButton.width + 10
                width: 50
                height: mixedButton.height
                color: "red"
                radius: 10
            }
        }
    }

    Button {
        id: scalableButton

        anchors {
            top: isLandscape() ? parent.top : common.bottom;
            left: isLandscape() ? common.right : parent.left;
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }
        text: buttonTextEditor.text

        Rectangle {
            width: parent.implicitWidth
            height: parent.implicitHeight
            anchors.centerIn: parent
            color: "#00000000"
            border.color: "blue"
        }
    }
}
