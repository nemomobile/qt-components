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
            Text {
                text: "Popup stack depth: "+platformPopupManager.popupStackDepth
                font.bold: true
                color: "white"
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Button {
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
                width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                text: "Ok"
                onClicked: dialogWithProgressBar.accept()
            }
        }
        content: Column {
            ProgressBar {
                id: horizontalBarWithAnimation
                value: 1
                indeterminate: true
                height: 30
            }

            ProgressBar {
                id: horizontalBar
                value: 75 // Default minimum and maximum are 0 - 100
                height: 30
            }
            Button {
                text: "Open dialog"
                onClicked: dialogWithScrollBar.open()
            }
            Text {
                text: "Popup stack depth: "+platformPopupManager.popupStackDepth
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
