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
import Qt.labs.components 1.1
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

        Label {
            id: imageLabel
            width: common.leftColumnWidth
            height: imageButton.height
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
                model: ["<none>", "list1.svg", "list2.svg", "list3.svg", "list4.svg", "list5.svg",
                        "list6.svg", "list7.svg", "list8.svg", "list9.svg", "list10.svg",
                        "list11.svg", "list12.svg", "list13.svg", "list14.svg", "list15.svg"]
            }
        }

        Label {
            id: textLabel
            width: common.leftColumnWidth
            height: buttonTextEditor.height
            font.pixelSize: platformStyle.fontSizeSmall
            text: "Text:"
        }

        TextField {
            id: buttonTextEditor
            objectName: "buttonTextEditor"
            width: common.rightColumnWidth
            text: "Hello QML"
        }

        Label {
            id: fontLabel
            width: common.leftColumnWidth
            height: fontButton.height
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

        Label {
            id: fontSizeLabel
            width: common.leftColumnWidth
            height: labelSizeSlider.height
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

        Label {
            id: longPressLabel
            width: common.leftColumnWidth
            height: longPressButton.height
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
                var menu = Qt.createQmlObject('import QtQuick 1.1; Rectangle { id: tempMenu; color: "lightsteelblue"; width: 200; height: 100; anchors.centerIn: parent; opacity: 0.8; Text { id: text1; text: "Click to close"; font.pointSize: 12; anchors.top: parent.top;}  MouseArea { anchors.fill: parent; onClicked: {tempMenu.destroy(); } } }', scalableButton.parent, "menu");
            }
        }

        Label {
            id: repeatLabel
            width: common.leftColumnWidth
            height: repeatButton.height
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
