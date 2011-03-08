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

Column {
    id: column

    property alias title: titleText.text
    property alias titleStyleColor: titleText.styleColor
    property alias titleColor: titleText.color

    Text {
        id: titleText
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        styleColor: "white"; color:"#333"; style: "Raised"
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Push me"
        width: parent.width - parent.spacing
    }

    TextField {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
    }

    TextArea {
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: "This is a\n multiline control."
        width: parent.width - parent.spacing; height: 280
    }

    Slider {
        anchors.horizontalCenter: parent.horizontalCenter
        value: 50
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: parent.spacing

        CheckableGroup { id: group }

        RadioButton {
            exclusiveGroup: group
        }

        RadioButton {
            exclusiveGroup: group
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: parent.spacing

        CheckBox {
        }

        CheckBox {
            checked: true
        }
    }

    ProgressBar {
        anchors.horizontalCenter: parent.horizontalCenter

        Timer {
            running: true
            repeat: true
            interval: 25
            onTriggered: parent.value = (parent.value + 1) % 1.1
        }
    }

    ProgressBar {
        anchors.horizontalCenter: parent.horizontalCenter
        indeterminate: true
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
        text: "Dialog"
        onClicked: dialog.open()
    }

    Dialog {
        id: dialog

        title: Text {
            text: "Dialog"
            font { bold: true; pixelSize: 16 }
            color: "white"
            anchors.fill: parent
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
                onClicked: dialog.accept()
            }

            Button {
                text: "Cancel"
                width: parent.width / 2
                height: parent.height
                onClicked: dialog.reject()
            }
        }
        content: Text {
            text: "This is the content"
            font { bold: true; pixelSize: 16 }
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
        text: "ContextMenu"
        onClicked: contextMenu.open()
    }

    ContextMenu {
        id: contextMenu

        content: MenuLayout {
            MenuItem { text: "White"; onClicked: { column.parent.color = "White" } }
            MenuItem { text: "Red"; onClicked: { column.parent.color = "Red" } }
            MenuItem { text: "LightBlue"; onClicked: { column.parent.color = "LightBlue" } }
            MenuItem { text: "LightGreen"; onClicked: { column.parent.color = "LightGreen" } }
        }
    }

    Button {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
        text: "Menu"
        onClicked: menu.open()
    }

    Menu {
        id: menu

        content: MenuLayout {
            MenuItem { text: "Blue"; onClicked: { column.parent.color = "Blue" } }
            MenuItem { text: "Green"; onClicked: { column.parent.color = "Green" } }
            MenuItem { text: "Yellow"; onClicked: { column.parent.color = "Yellow" } }
        }
    }

    ListView {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing; height: 120
        clip: true
        delegate: listDelegate
        model: listModel
        header: listHeading
    }

    ListModel {
        id: listModel

        ListElement {
            titleText: "Title"
            subTitleText: "SubTitle"
        }
    }

    Component {
        id: listHeading
        ListHeading {
            width: parent.width
            ListItemText {
                anchors.fill: parent.padding
                role: "Heading"
                text: "ListHeading"
            }
        }
    }

    Component {
        id: listDelegate
        ListItem {
            id: listItem
            Column {
                anchors.fill: listItem.padding
                spacing: listItem.verticalSpacing
                ListItemText {
                    anchors.fill: parent.padding
                    style: listItem.style
                    role: "Title"
                    text: titleText
                }
                ListItemText {
                    anchors.fill: parent.padding
                    style: listItem.style
                    role: "SubTitle"
                    text: subTitleText
                }
            }
        }
    }

    ChoiceList {
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
        currentIndex: 2
        model: ["One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"]
    }

    TabBar {
        width: parent.width - parent.spacing
        TabButton { tab: tab1content; text: "1" }
        TabButton { tab: tab2content; text: "2" }
        TabButton { tab: tab3content; text: "3" }
    }

    TabGroup {
        height: 100
        width: parent.width - parent.spacing
        Button { id: tab1content; text: "tab1" }
        Text { id: tab2content; text: "tab2"; horizontalAlignment: "AlignHCenter"; verticalAlignment: "AlignVCenter" }
        Page {
            id: tab3content
            CheckBox { anchors.fill: parent; text: "tab3" }
        }
    }

    ToolButton {
        id: toolButton
        text: "ToolButton"
    }

    ToolButton {
        id: toolButton2
        checkable: true
        iconSource: "image://theme/qtg_graf_radiobutton_normal_selected"
    }

    ToolButton {
        id: toolButton3
        text: "ToolButton"
        iconSource: "image://theme/qtg_graf_radiobutton_normal_selected"
    }
}
