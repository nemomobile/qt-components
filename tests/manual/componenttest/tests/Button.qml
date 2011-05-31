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
import "../components"

Item {
    id: root
    anchors.fill: parent

    function isLandscape() {
        if ( screen.width > screen.height )
            return true;
        else
            return false;
    }

    Grid {
        id: common

        property real leftColumnWidth: disabledButton.width
        property real rightColumnWidth: width - spacing - leftColumnWidth

        anchors {
            top: parent.top
            left: parent.left
            right: isLandscape() ? parent.horizontalCenter : parent.right
            margins: 10
        }
        columns: isLandscape() ? 4 : 2
        spacing: 5

        Text {
            id: imageLabel
            width: common.leftColumnWidth
            height: imageButton.height
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.colorNormalLight
            font.pixelSize: platformStyle.fontSizeSmall
            text: "Image:"
        }

        Button {
            id: imageButton
            objectName: "imageButton"
            text: imageDialog.model[imageDialog.selectedIndex]
            iconSource: imageDialog.currentImage
            onClicked: imageDialog.open()

            SelectionDialog {
                id: imageDialog
                titleText: "Select image"
                selectedIndex: 0
                property string currentImage
                currentImage: {
                    if (selectedIndex <= 0)
                        return ""
                    return "qrc:/" + model[selectedIndex]
                }
                model: ["<none>", "list1.png", "list2.png", "list3.png", "list4.png", "list5.png",
                        "list6.png", "list7.png", "list8.png", "list9.png", "list10.png",
                        "list11.png", "list12.png", "list13.png", "list14.png", "list15.png"]
            }
        }

        Text {
            id: textLabel
            width: common.leftColumnWidth
            height: buttonTextEditor.height
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.colorNormalLight
            font.pixelSize: platformStyle.fontSizeSmall
            text: "Text:"
        }

        TextField {
            id: buttonTextEditor
            objectName: "buttonTextEditor"
            width: common.rightColumnWidth
            text: "Hello QML"
        }

        Text {
            id: fontLabel
            width: common.leftColumnWidth
            height: fontButton.height
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.colorNormalLight
            font.pixelSize: platformStyle.fontSizeSmall
            text: "Font:"
        }

        Button {
            id: fontButton
            objectName: "fontButton"
            width: common.rightColumnWidth
            text: scalableButton.font.family

            FontSelectionDialog {
                id: fontSelector
                onAccepted: scalableButton.font.family = fontSelector.fontFamily
            }

            onClicked: fontSelector.open()
        }

        Text {
            id: fontSizeLabel
            width: common.leftColumnWidth
            height: labelSizeSlider.height
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.colorNormalLight
            font.pixelSize: platformStyle.fontSizeSmall
            text: "Font size:"
        }

        Slider {
            id: labelSizeSlider
            objectName: "labelSizeSlider"
            width: common.rightColumnWidth
            minimumValue: 1; maximumValue: 50; stepSize: 1;
            valueIndicatorVisible: false
            value: scalableButton.font.pixelSize
            onValueChanged: if (pressed) scalableButton.font.pixelSize = value
        }

        Text {
            id: longPressLabel
            width: common.leftColumnWidth
            height: longPressButton.height
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.colorNormalLight
            font.pixelSize: platformStyle.fontSizeSmall
            text: "Long press:"
        }

        Button {
            id: longPressButton
            objectName: "longPressButton"
            width: common.rightColumnWidth
            checkable: true
            text: "Press me"
            onPlatformPressAndHold: {
                var menu = Qt.createQmlObject('import QtQuick 1.0; Rectangle { id: tempMenu; color: "lightsteelblue"; width: 200; height: 100; anchors.centerIn: parent; opacity: 0.8; Text { id: text1; text: "Click to close"; font.pointSize: 12; anchors.top: parent.top;}  MouseArea { anchors.fill: parent; onClicked: {tempMenu.destroy(); } } }', scalableButton.parent, "menu");
            }
        }

        Text {
            id: repeatLabel
            width: common.leftColumnWidth
            height: repeatButton.height
            verticalAlignment: Text.AlignVCenter
            color: platformStyle.colorNormalLight
            font.pixelSize: platformStyle.fontSizeSmall
            text: "Auto repeat:"
        }

        Button {
            id: repeatButton
            objectName: "repeatButton"
            width: common.rightColumnWidth
            platformAutoRepeat: true
            text: "Press me"

            onClicked: {
                width -= 2
                if (width < 10)
                    width = common.rightColumnWidth
            }
        }
    }

    Row {
        spacing: 5

        anchors {
            top: common.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }

        Button {
            id: disabledButton
            objectName: "disabledButton"
            text: "Disabled"
            checked: false
            checkable: true
        }

        Button {
            id: scalableButton
            objectName: "scalableButton"
            height: parent.height
            width: parent.width - parent.spacing - disabledButton.width
            text: buttonTextEditor.text
            iconSource: imageDialog.currentImage
            enabled: !disabledButton.checked

            Rectangle {
                width: parent.implicitWidth
                height: parent.implicitHeight
                anchors.centerIn: parent
                color: "transparent"
                border.color: "blue"
            }
        }
    }
}
