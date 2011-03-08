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
                    anchors.fill: listItem2.padding
                    spacing: listItem2.verticalSpacing

                    ListItemText {
                        id: titleText2
                        style: listItem2.style
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText2
                        style: listItem2.style
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
                    anchors { left: listItem3.padding.left; top: listItem3.padding.top }
                    sourceSize.width: listItem3.preferredImageWidth(Symbian.Small)
                    sourceSize.height: listItem3.preferredImageHeight(Symbian.Small)
                    source: "image://theme/:/list1.png"
                }

                Column {
                    anchors {
                        top: listItem3.padding.top
                        left: image3.right
                        leftMargin: listItem3.horizontalSpacing
                        right: listItem3.padding.right
                    }
                    spacing: listItem3.verticalSpacing

                    ListItemText {
                        id: titleText3
                        style: listItem3.style
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText3
                        style: listItem3.style
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
                    anchors { left: listItem4.padding.left; top: listItem4.padding.top }
                    sourceSize.width: listItem3.preferredImageWidth(Symbian.Large)
                    sourceSize.height: listItem3.preferredImageHeight(Symbian.Large)
                    source: "image://theme/:/list1.png"
                }

                Column {
                    anchors {
                        top: listItem4.padding.top
                        left: image4.right
                        leftMargin: listItem4.horizontalSpacing
                        right: listItem4.padding.right
                    }
                    spacing: listItem4.verticalSpacing

                    ListItemText {
                        id: titleText4
                        style: listItem4.style
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
                                syncTimer.running = false
                        }

                        Timer {
                            id: syncTimer
                            interval: 50
                            running: true
                            repeat: true
                            onTriggered: progressBar4.value++
                        }
                    }
                }
                onClicked: {
                    progressBar4.value = 0
                    syncTimer.running = true
                    notification.text = "Clicked " + objectName
                }
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem5
                objectName: "listItem5"
                height: listItem5.padding.anchors.topMargin + listItem5.padding.anchors.bottomMargin + titleText5.height

                Row {
                    anchors.fill: listItem5.padding
                    spacing: listItem5.horizontalSpacing

                    Image {
                        id: image5
                        source: "image://theme/:/list1.png"
                        sourceSize.width: listItem3.preferredImageWidth(Symbian.Small)
                        sourceSize.height: listItem3.preferredImageHeight(Symbian.Small)
                    }

                    ListItemText {
                        id: titleText5
                        style: listItem5.style
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
                    anchors { left: listItem6.padding.left; top: listItem6.padding.top }
                    source: "image://theme/:/list1.png"
                    sourceSize.width: listItem3.preferredImageWidth(Symbian.Large)
                    sourceSize.height: listItem3.preferredImageHeight(Symbian.Large)
                }

                Column {
                    anchors {
                        top: listItem6.padding.top
                        left: image6a.right
                        leftMargin: listItem6.horizontalSpacing
                    }
                    spacing: listItem6.verticalSpacing

                    ListItemText {
                        id: titleText6
                        style: listItem6.style
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText6
                        style: listItem6.style
                        role: "SubTitle"
                        text: "SubTitle text"
                    }
                }

                Image {
                    id: image6b
                    anchors { right: listItem6.padding.right; top: listItem6.padding.top }
                    source: "image://theme/:/list1.png"
                    sourceSize.width: listItem3.preferredImageWidth(Symbian.Small)
                    sourceSize.height: listItem3.preferredImageHeight(Symbian.Small)
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem7
                objectName: "listItem7"

                Image {
                    id: image7a
                    anchors { left: listItem7.padding.left; top: listItem7.padding.top }
                    source: "image://theme/:/list1.png"
                    sourceSize.width: listItem3.preferredImageWidth(Symbian.Large)
                    sourceSize.height: listItem3.preferredImageHeight(Symbian.Large)
                }

                ListItemText {
                    id: titleText7
                    style: listItem7.style
                    role: "Title"
                    text: "Title text"
                    anchors {
                        left: image7a.right
                        leftMargin: listItem7.horizontalSpacing
                        verticalCenter: parent.verticalCenter
                    }
                }

                Image {
                    id: image7b
                    source: "image://theme/:/list1.png"
                    sourceSize.width: listItem3.preferredImageWidth(Symbian.Small)
                    sourceSize.height: listItem3.preferredImageHeight(Symbian.Small)
                    anchors { verticalCenter: parent.verticalCenter; right: listItem7.padding.right }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem8
                objectName: "listItem8"

                Rectangle {
                    id: rect8
                    anchors { left: listItem8.padding.left; top: listItem8.padding.top }
                    width: height
                    height: listItem8.padding.height
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
                    style: listItem8.style
                    role: "Title"
                    text: "Title text"
                    anchors {
                        left: rect8.right
                        leftMargin: listItem8.horizontalSpacing
                        verticalCenter: parent.verticalCenter
                    }
                }

                Image {
                    id: image8b
                    source: "image://theme/:/list1.png"
                    sourceSize.width: listItem3.preferredImageWidth(Symbian.Small)
                    sourceSize.height: listItem3.preferredImageHeight(Symbian.Small)
                    anchors { verticalCenter: parent.verticalCenter; right: listItem8.padding.right }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem9
                objectName: "listItem9"

                Rectangle {
                    id: rect9
                    anchors { left: listItem9.padding.left; top: listItem9.padding.top }
                    width: height
                    height: listItem9.padding.height
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
                    style: listItem9.style
                    role: "Title"
                    text: "Title text"
                    anchors {
                        left: rect9.right
                        leftMargin: listItem9.horizontalSpacing
                        verticalCenter: parent.verticalCenter
                    }
                }

                CheckBox {
                    id: checkbox9
                    anchors { right: listItem9.padding.right; top: listItem9.padding.top }
                }
                onClicked: {
                    checkbox9.checked = !checkbox9.checked
                    notification.text = "Clicked " + objectName
                }
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }

            ListItem {
                id: listItem10
                objectName: "listItem10"
                enabled: false

                Column {
                    anchors.fill: listItem10.padding
                    spacing: listItem10.verticalSpacing

                    ListItemText {
                        id: titleText10
                        style: listItem10.style
                        role: "Title"
                        text: "Title text"
                    }

                    ListItemText {
                        id: subTitleText10
                        style: listItem10.style
                        role: "SubTitle"
                        text: "SubTitle text"
                    }
                }
                onClicked: notification.text = "Clicked " + objectName
                onPressAndHold: notification.text = "Press-and-hold " + objectName
            }
        }
    }

    ScrollBar {
        id: scrollBar
        anchors { top: flick.top; right: flick.right }
    }
}
