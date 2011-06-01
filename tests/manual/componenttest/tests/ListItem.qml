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

    Rectangle {
        id: notification
        property alias text: notifyText.text
        color: "gray"; opacity: 0.5
        height: 80; width: parent.width

        Text {
            id: notifyText
            text: "Click or press-and-hold on an item"
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font { bold: true; pointSize: 10 }
            anchors.fill: parent

            Behavior on text {
                SequentialAnimation {
                    PropertyAction { target: notification; property: "opacity"; value: 1 }
                    PropertyAnimation { target: notification; property: "opacity"; to: 0.5; duration: 600 }
                }
            }
        }
    }

    Flickable {
        id: flick
        anchors.top: notification.bottom
        clip: true
        width: parent.width
        height: parent.height - notification.height
        contentWidth: flick.width
        contentHeight: itemColumn.height
        Component.onCompleted: scrollBar.flickableItem = flick

        Column {
            id: itemColumn

            ListItem {
                id: listItem1
                objectName: "listItem1"
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem2
                objectName: "listItem2"

                Column {
                    anchors.fill: listItem2.paddingItem

                    ListItemText {
                        id: titleText2
                        mode: listItem2.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText2
                        mode: listItem2.mode
                        role: "SubTitle"
                        text: "SubTitle text"
                    }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem3
                objectName: "listItem3"

                Image {
                    id: image3
                    anchors { left: listItem3.left; top: listItem3.top }
                    sourceSize.width: platformStyle.graphicSizeLarge
                    sourceSize.height: platformStyle.graphicSizeLarge
                    source: "image://theme/:/list1.svg"
                }

                Column {
                    anchors {
                        top: listItem3.paddingItem.top
                        left: image3.right
                        leftMargin: platformStyle.paddingMedium
                        right: listItem3.paddingItem.right
                    }

                    ListItemText {
                        id: titleText3
                        mode: listItem3.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText3
                        mode: listItem3.mode
                        role: "SubTitle"
                        text: "SubTitle text"
                    }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem4
                objectName: "listItem4"

                Image {
                    id: image4
                    anchors { left: listItem4.left; top: listItem4.top }
                    sourceSize.width: platformStyle.graphicSizeLarge
                    sourceSize.height: platformStyle.graphicSizeLarge
                    source: "image://theme/:/list1.svg"
                }

                Column {
                    anchors {
                        top: listItem4.paddingItem.top
                        left: image4.right
                        leftMargin: platformStyle.paddingMedium
                        right: listItem4.paddingItem.right
                    }

                    ListItemText {
                        id: titleText4
                        mode: listItem4.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ProgressBar {
                        id: progressBar4
                        width: parent.width
                        value: 0
                        maximumValue: 100
                        state: "normal"
                        onValueChanged: {
                            if (value >= progressBar4.maximumValue)
                                timer.running = false
                        }

                        Timer {
                            id: timer
                            interval: 50
                            running: true
                            repeat: true
                            onTriggered: progressBar4.value++
                        }
                    }
                }
                onClicked: {
                    progressBar4.value = 0
                    timer.running = true
                    notification.text = "Clicked " + objectName
                }
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem5
                objectName: "listItem5"
                height: listItem5.paddingItem.anchors.topMargin + listItem5.paddingItem.anchors.bottomMargin + titleText5.height

                Row {
                    anchors.fill: listItem5.paddingItem
                    spacing: platformStyle.paddingMedium

                    Image {
                        id: image5
                        source: "image://theme/:/list1.svg"
                        sourceSize.width: platformStyle.graphicSizeSmall
                        sourceSize.height: platformStyle.graphicSizeSmall
                    }

                    ListItemText {
                        id: titleText5
                        mode: listItem5.mode
                        role: "Title"
                        text: "Title text"
                    }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem6
                objectName: "listItem6"

                Image {
                    id: image6a
                    anchors { left: listItem6.left; top: listItem6.top }
                    source: "image://theme/:/list1.svg"
                    sourceSize.width: platformStyle.graphicSizeLarge
                    sourceSize.height: platformStyle.graphicSizeLarge
                }

                Column {
                    anchors {
                        top: listItem6.paddingItem.top
                        left: image6a.right
                        leftMargin: platformStyle.paddingMedium
                    }

                    ListItemText {
                        id: titleText6
                        mode: listItem6.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText6
                        mode: listItem6.mode
                        role: "SubTitle"
                        text: "SubTitle text"
                    }
                }

                Image {
                    id: image6b
                    anchors { right: listItem6.paddingItem.right; top: listItem6.paddingItem.top }
                    source: "image://theme/:/list1.svg"
                    sourceSize.width: platformStyle.graphicSizeSmall
                    sourceSize.height: platformStyle.graphicSizeSmall
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem7
                objectName: "listItem7"

                Image {
                    id: image7a
                    anchors { left: listItem7.left; top: listItem7.top }
                    source: "image://theme/:/list1.svg"
                    sourceSize.width: platformStyle.graphicSizeLarge
                    sourceSize.height: platformStyle.graphicSizeLarge
                }

                ListItemText {
                    id: titleText7
                    mode: listItem7.mode
                    role: "Title"
                    text: "Title text"
                    anchors {
                        left: image7a.right
                        leftMargin: platformStyle.paddingMedium
                        verticalCenter: parent.verticalCenter
                    }
                }

                Image {
                    id: image7b
                    source: "image://theme/:/list1.svg"
                    sourceSize.width: platformStyle.graphicSizeSmall
                    sourceSize.height: platformStyle.graphicSizeSmall
                    anchors { verticalCenter: parent.verticalCenter; right: listItem7.paddingItem.right }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem8
                objectName: "listItem8"

                Rectangle {
                    id: rect8
                    anchors { left: listItem8.paddingItem.left; top: listItem8.paddingItem.top }
                    width: height
                    height: listItem8.paddingItem.height
                    color: "Blue"
                    opacity: 0.5
                    radius: 4

                    Text {
                        id: rectText8
                        anchors.fill: parent
                        text: "8"
                        font.pixelSize: 30
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                ListItemText {
                    id: titleText8
                    mode: listItem8.mode
                    role: "Title"
                    text: "Title text"
                    anchors {
                        left: rect8.right
                        leftMargin: platformStyle.paddingMedium
                        verticalCenter: parent.verticalCenter
                    }
                }

                Image {
                    id: image8b
                    source: "image://theme/:/list1.svg"
                    sourceSize.width: platformStyle.graphicSizeSmall
                    sourceSize.height: platformStyle.graphicSizeSmall
                    anchors { verticalCenter: parent.verticalCenter; right: listItem8.paddingItem.right }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem9
                objectName: "listItem9"

                Rectangle {
                    id: rect9
                    anchors { left: listItem9.paddingItem.left; top: listItem9.paddingItem.top }
                    width: height
                    height: listItem9.paddingItem.height
                    color: "Blue"
                    opacity: 0.5
                    radius: 4

                    Text {
                        id: rectText9
                        anchors.fill: parent
                        text: "9"
                        font.pixelSize: 30
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                ListItemText {
                    id: titleText9
                    mode: listItem9.mode
                    role: "Title"
                    text: "Title text"
                    anchors {
                        left: rect9.right
                        leftMargin: platformStyle.paddingMedium
                        verticalCenter: parent.verticalCenter
                    }
                }

                CheckBox {
                    id: checkbox9
                    anchors { right: listItem9.paddingItem.right; verticalCenter: listItem9.paddingItem.verticalCenter }
                }
                onClicked: {
                    notification.text = "Clicked " + objectName
                }
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem10
                objectName: "listItem10"
                enabled: false

                Column {
                    anchors.fill: listItem10.paddingItem


                    ListItemText {
                        id: titleText10
                        mode: listItem10.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText10
                        mode: listItem10.mode
                        role: "SubTitle"
                        text: "SubTitle text"
                    }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem11
                objectName: "listItem11"
                subItemIndicator: true

                Column {
                    anchors.fill: listItem11.paddingItem

                    ListItemText {
                        id: titleText11
                        mode: listItem11.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText11
                        mode: listItem11.mode
                        role: "SubTitle"
                        text: "SubTitle text"
                    }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            SelectionListItem {
                id: listItem12
                objectName: "listItem12"
                title: selectionDialog12.selectedIndex < 0 ? "Unselected" : selectionDialog12.model.get(selectionDialog12.selectedIndex).name

                onClicked: selectionDialog12.open()
                onPressAndHold: selectionDialog12.open()

                SelectionDialog {
                    id: selectionDialog12
                    titleText: "Select one of the values"
                    selectedIndex: -1
                    model: ListModel {
                        ListElement { name: "Zero" }
                        ListElement { name: "One" }
                        ListElement { name: "Two" }
                    }
                    // Clear existing selection
                    onRejected: selectedIndex = -1
                }
            }

            SelectionListItem {
                id: listItem13
                objectName: "listItem13"
                title: "Selected value"
                subTitle: selectionDialog13.model.get(selectionDialog13.selectedIndex).name

                onClicked: selectionDialog13.open()
                onPressAndHold: selectionDialog13.open()

                SelectionDialog {
                    id: selectionDialog13
                    titleText: "Select one of the values"
                    selectedIndex: 0
                    model: ListModel {
                        ListElement { name: "Zero" }
                        ListElement { name: "One" }
                        ListElement { name: "Two" }
                        ListElement { name: "Three" }
                        ListElement { name: "Four" }
                    }
                }
            }

            SelectionListItem {
                id: listItem14
                objectName: "listItem14"
                title: "Title"
                subTitle: "Subtitle"
                enabled: false

                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            SelectionListItem {
                id: listItem15
                objectName: "listItem15"
                title: "Static item with too long text to fit in list item"
                subTitle: "Strings should elide nicely from the end"
                enabled: true
            }
        }
    }

    ScrollBar {
        id: scrollBar
        anchors { top: flick.top; right: flick.right }
    }
}
