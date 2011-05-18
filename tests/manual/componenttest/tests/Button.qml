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
import Qt.labs.components 1.0

Item {
    id: root
    anchors.fill: parent

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
            right: isLandscape() ? parent.horizontalCenter : parent.right
            margins: 10
        }
        spacing: isLandscape() ? 1 : 5

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
                    iconChoicelist.currentIndex = 0
                }
            }
        }

        Grid {
            columns: 2
            spacing: 5

            Text {
                color: platformStyle.colorNormalLight
                font.pointSize: platformStyle.fontSizeSmall
                text: "Button text:"
            }

            TextField {
                id: buttonTextEditor
                text: "Hello QML"
            }

            Text {
                color: platformStyle.colorNormalLight
                font.pointSize: platformStyle.fontSizeSmall
                text: "Label font:"
            }

            Button {
                id: fontButton
                text: scalableButton.font.family
                property Item fontSelectionDialog: null
                onClicked: {
                    if (fontSelectionDialog == null) {
                        var component = Qt.createComponent("qrc:/FontSelectionDialog.qml")
                        if (component.status == Component.Ready) {
                            fontSelectionDialog = component.createObject(fontButton);
                        } else {
                            console.log(component.errorString())
                            return
                        }
                    }
                    fontSelectionDialog.open()
                }

                Binding { target: fontButton; property: "text"; value: fontButton.fontSelectionDialog != null ? fontButton.fontSelectionDialog.fontFamily: ""; when: fontButton.fontSelectionDialog != null }
                Binding { target: scalableButton; property: "font.family"; value: fontButton.text; when: fontButton.fontSelectionDialog != null }
            }

            Text {
                color: platformStyle.colorNormalLight
                font.pointSize: platformStyle.fontSizeSmall
                text: "Label font size:"
            }

            Slider {
                id: "labelSizeSlider"
                minimumValue: 1; maximumValue: 50; stepSize: 1;
                valueIndicatorVisible: true
                value: scalableButton.font.pixelSize
                onValueChanged: if (pressed) scalableButton.font.pixelSize = value
            }
        }

        Button {
            id: checkableButton
            text: "Auto repeat / Long press"
            checkable: true
            onCheckedChanged: {
                if (checked) {
                    mixedButton.text = "Auto repeat"
                    mixedButton.platformLongPress = false;
                    mixedButton.platformAutoRepeat = true;
                } else {
                    mixedButton.text = "Long press"
                    mixedButton.platformLongPress = true;
                    mixedButton.platformAutoRepeat = false;
                }
            }
        }

        Button {
            id: mixedButton
            platformLongPress: true
            text: "Long press"

            onClicked: {
                rect.x += 2;
                if (rect.x + rect.width > parent.width)
                    rect.x = mixedButton.x + mixedButton.width + 10;
            }

            onPlatformPressAndHold: {
                var menu = Qt.createQmlObject('import QtQuick 1.0; Rectangle { id: tempMenu; color: "lightsteelblue"; width: 200; height: 100; anchors.centerIn: parent; opacity: 0.8; Text { id: text1; text: "Click to close"; font.pointSize: 12; anchors.top: parent.top;}  MouseArea { anchors.fill: parent; onClicked: {tempMenu.destroy(); } } }', scalableButton.parent, "menu");
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

        Button {
            id: disabledButton
            enabled: false
            text: "Disabled button"
            checkable: true // not really checkable since disabled
            onPressedChanged: text = "Error: onPressedChanged" // should never be seen
            onCheckedChanged: text = "Error: onCheckedChanged" // should never be seen
        }
    }

    Button {
        id: scalableButton

        anchors {
            top: isLandscape() ? parent.top : common.bottom;
            left: isLandscape() ? parent.horizontalCenter : parent.left;
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
