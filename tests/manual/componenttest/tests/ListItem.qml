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
                        width: parent.width // Need to set parent width to support mirroring in right-to-left layout
                        mode: listItem2.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText2
                        width: parent.width
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
                        width: parent.width
                        mode: listItem3.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText3
                        width: parent.width
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
                        width: parent.width
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
                        right: listItem6.paddingItem.right
                    }

                    ListItemText {
                        id: titleText6
                        width: parent.width
                        mode: listItem6.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText6
                        width: parent.width
                        anchors.right: parent.right
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
                        width: parent.width
                        mode: listItem10.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText10
                        width: parent.width
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
                        width: parent.width
                        mode: listItem11.mode
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText11
                        width: parent.width
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

            SelectionListItem {
                id: keyHandlingListItem
                title: activeFocus ? "Press cursor keys" : "Click to set focus"
                subTitle: "No key pressed yet"

                onClicked: focus = true

                Keys.onPressed:  {
                    if (event.isAutoRepeat)
                        return
                    switch (event.key) {
                        case Qt.Key_Up:
                            subTitle = "You pressed up"
                            break
                        case Qt.Key_Down:
                            subTitle = "You pressed down"
                            break
                        case Qt.Key_Left:
                            subTitle = "You pressed left"
                            break
                        case Qt.Key_Right:
                            subTitle = "You pressed right"
                            break
                        default:
                            subTitle = "You pressed a non-cursor key"
                            break
                    }
                }
            }
        }
    }

    ScrollBar {
        id: scrollBar
        anchors { top: flick.top; right: flick.right }
    }
}
