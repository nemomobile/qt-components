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

Item {

    Connections {
        target: platformPopupManager
        onPopupStackDepthChanged: console.log("Popup stack depth: ", platformPopupManager.popupStackDepth)
    }

    Dialog {
        id: customDialog
        width: 400
        height: 250

        title: Text {
            text: "Custom layout and font"
            font { bold: true; pixelSize: 22 }
            color: "yellow"
            anchors.fill: parent
            anchors.leftMargin: 10
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        buttons: Row {
            LayoutMirroring.enabled: false
            LayoutMirroring.childrenInherit: true

            height: 60
            width: parent.width
            Button {
                text: "Ok"
                width: parent.width / 2
                height: parent.height
                onClicked: customDialog.accept()
            }
            Button {
                text: "Cancel"
                width: parent.width / 2
                height: parent.height
                onClicked: customDialog.reject()
            }
        }
        content: Column {
            width: parent.width
            Text {
                text: "Popup stack depth: "+platformPopupManager.popupStackDepth
                font.bold: true
                color: "white"
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Open dialog"
                onClicked: dialogWithProgressBar.open()
            }
        }
    }

    CommonDialog {
        id: dialogWithProgressBar
        titleText: "Dialog with Progress"

        buttons: ToolBar {
            id: buttons
            width: parent.width
            height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall

            tools: ToolButton {
                id: acceptButton
                anchors.centerIn: parent
                width: Math.round((privateStyle.dialogMaxSize - 3 * platformStyle.paddingMedium) / 2)
                text: "Ok"
                onClicked: dialogWithProgressBar.accept()
            }
        }
        content: Column {
            width: parent.width
            ProgressBar {
                id: horizontalBarWithAnimation
                anchors.left: parent.left
                value: 1
                indeterminate: true
                height: 30
            }

            ProgressBar {
                id: horizontalBar
                anchors.left: parent.left
                value: 75 // Default minimum and maximum are 0 - 100
                height: 30
            }
            Button {
                anchors.left: parent.left
                text: "Open dialog"
                onClicked: dialogWithScrollBar.open()
            }
            Text {
                anchors.left: parent.left
                text: "Popup stack depth: "+platformPopupManager.popupStackDepth
                horizontalAlignment: Text.AlignLeft
                color: "white"
                font.pointSize: 10
            }
        }
    }

    CommonDialog {
        id: dialogWithScrollBar
        titleText: "Dialog with scrolling"
        privateCloseIcon: true

        content: Item {
            id: page

            height: dialogWithScrollBar.platformContentMaximumHeight
            width: dialogWithScrollBar.platformContentMaximumWidth

            Flickable {
                id: flickableArea

                anchors.fill: parent
                contentWidth: colorfulSquare.width
                contentHeight: colorfulSquare.height
                clip: true

                Rectangle {
                    id: colorfulSquare

                    width: 1000
                    height: 1000
                    opacity: 0.5
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "blue" }
                        GradientStop { position: 1.0; color: "yellow" }
                    }
                }
                Rectangle {
                    height: colorfulSquare.height
                    width: colorfulSquare.width
                    opacity: 0.5
                    rotation: -90
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "red" }
                        GradientStop { position: 1.0; color: "green" }
                    }
                }
                Text {
                    anchors.centerIn: parent
                    text: "Flick around!"
                    color: "white"
                    font.pixelSize: 50
                    opacity: 0.85
                }
            }
            ScrollBar {
                id: verticalScrollbar

                orientation: Qt.Vertical
                flickableItem: flickableArea
                anchors { top: flickableArea.top; right: flickableArea.right }
            }
            ScrollBar {
                orientation: Qt.Horizontal
                flickableItem: flickableArea
                anchors {
                    bottom: flickableArea.bottom
                    left: flickableArea.left
                    rightMargin: verticalScrollbar.width
                }
            }
            Text {
                text: "Popup stack depth: "+platformPopupManager.popupStackDepth
                color: "white"
                font.pointSize: 10
                horizontalAlignment: Text.AlignLeft
                width: page.width
            }
        }
    }

    SelectionDialog {
        id: selectionDialog
        titleText: "Dialog Header #1 123456789012345678901234567890"
        selectedIndex: 1

        model: ListModel {
            ListElement { name: "ListTitle #1" }
            ListElement { name: "ListTitle #2" }
            ListElement { name: "ListTitle #3" }
            ListElement { name: "ListTitle #4" }
            ListElement { name: "ListTitle #5. 123456789012345678901234567890" }
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: platformStyle.paddingSmall
        spacing: platformStyle.paddingSmall

        Button {
            width: button4.width
            text: "Dialog 1"
            onClicked: dialogWithProgressBar.open()
        }

        Button {
            width: button4.width
            text: "Dialog 2"
            onClicked: dialogWithScrollBar.open()
        }

        Button {
            width: button4.width
            text: "Custom dialog"
            onClicked: customDialog.open()
        }

        Button {
            id: button4
            text: "Selection Dialog"
            onClicked: selectionDialog.open()
        }
    }
}
