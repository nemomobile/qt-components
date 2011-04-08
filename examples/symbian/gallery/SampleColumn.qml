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

import Qt 4.7
import com.nokia.symbian 1.0
import Qt.labs.components 1.0

Column {
    id: column
    spacing: 14

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Qt Components " + (enabled ? "(enabled)" : "(disabled)")
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
        color: platformStyle.colorNormalLight
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
            platformExclusiveGroup: group
        }

        RadioButton {
            platformExclusiveGroup: group
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

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: parent.spacing

        Switch {
        }

        Switch {
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

    Component {
        id: dialogComponent
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
                ToolButton {
                    text: "Ok"
                    width: parent.width / 2
                    height: parent.height
                    onClicked: dialog.accept()
                }

                ToolButton {
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
    }

    Button {
        property Dialog dialog
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
        text: "Dialog"
        onClicked: {
            if (!dialog)
                dialog = dialogComponent.createObject(column)
            dialog.open()
        }
    }

    Component {
        id: singleSelectionDialogComponent
        SelectionDialog {
            titleText: "Select background color"
            selectedIndex: 1

            model: ListModel {
                id: colorModel

                ListElement { name: "Red" }
                ListElement { name: "Blue" }
                ListElement { name: "Green" }
                ListElement { name: "Yellow" }
                ListElement { name: "Black" }
                ListElement { name: "White" }
                ListElement { name: "Grey" }
                ListElement { name: "Orange" }
                ListElement { name: "Pink" }
            }

            onAccepted: { selectionDialogButton.parent.color = colorModel.get(selectedIndex).name }
        }
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        height: selectionDialogButton.height
        width: parent.width - parent.spacing
        radius: 10
        color: "black"
        Button {
            id: selectionDialogButton
            property SelectionDialog singleSelectionDialog
            anchors.centerIn: parent
            text: "Selection Dialog"

            onClicked: {
                if (!singleSelectionDialog)
                    singleSelectionDialog = singleSelectionDialogComponent.createObject(column)
                singleSelectionDialog.open()
            }
        }
    }

    Button {
        property QueryDialog queryDialog
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
        text: "QueryDialog"
        onClicked: {
            if (!queryDialog)
                queryDialog = queryDialogComponent.createObject(column)
            queryDialog.open()
        }
    }

    Component {
        id: queryDialogComponent
        QueryDialog {
            titleText: "Query Dialog"
            message: "Lorem ipsum dolor sit amet, consectetur adipisici elit,"
                     + "sed eiusmod tempor incidunt ut labore et dolore magna aliqua."
                     + "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris"
                     + "nisi ut aliquid ex ea commodi consequat. Quis aute iure reprehenderit"
                     + "in voluptate velit esse cillum dolore eu fugiat nulla pariatur."
                     + "Excepteur sint obcaecat cupiditat non proident, sunt in culpa qui"
                     + "officia deserunt mollit anim id est laborum."

            acceptButtonText: "Ok"
            rejectButtonText: "Cancel"

            icon: "image://theme/qtg_graf_busyindicator_1"
        }
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        height: contentMenuButton.height
        width: parent.width - parent.spacing
        radius: 10
        color: "black"
        Button {
            id: contentMenuButton
            property ContextMenu contextMenu
            anchors.horizontalCenter: parent.horizontalCenter
            text: "ContextMenu"
            onClicked: {
                if (!contextMenu)
                    contextMenu = contextMenuComponent.createObject(column)
                contextMenu.open()
            }
        }
    }

    Component {
        id: contextMenuComponent
        ContextMenu {
            content: MenuLayout {
                MenuItem { text: "White"; onClicked: { contentMenuButton.parent.color = "White" } }
                MenuItem { text: "Red"; onClicked: { contentMenuButton.parent.color = "Red" } }
                MenuItem { text: "LightBlue"; onClicked: { contentMenuButton.parent.color = "LightBlue" } }
                MenuItem { text: "LightGreen"; onClicked: { contentMenuButton.parent.color = "LightGreen" } }
            }
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
                anchors.fill: parent.paddingItem
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
                anchors.fill: listItem.paddingItem
                ListItemText {
                    mode: listItem.mode
                    role: "Title"
                    text: titleText
                }
                ListItemText {
                    mode: listItem.mode
                    role: "SubTitle"
                    text: subTitleText
                }
            }
        }
    }

    SelectionListItem {
        property SelectionDialog selectionDialog
        title: {
            if (selectionDialog) {
                if (selectionDialog.selectedIndex >= 0)
                    return selectionDialog.model.get(selectionDialog.selectedIndex).name
            }
            return "Three"
        }
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing

        onClicked: {
            if (!selectionDialog)
                selectionDialog = selectionDialogComponent.createObject(column)
            selectionDialog.open()
        }

        Component {
            id: selectionDialogComponent
            SelectionDialog {
                titleText: "Select"
                selectedIndex: 2
                model: ListModel {
                    ListElement { name: "One" }
                    ListElement { name: "Two" }
                    ListElement { name: "Three" }
                    ListElement { name: "Four" }
                    ListElement { name: "Five" }
                    ListElement { name: "Six" }
                    ListElement { name: "Seven" }
                    ListElement { name: "Eight" }
                    ListElement { name: "Nine" }
                }
            }
        }
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
        Text {
            id: tab2content
            text: "tab2"
            horizontalAlignment: "AlignHCenter"
            verticalAlignment: "AlignVCenter"
            color: platformStyle.colorNormalLight
        }
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

    Row {
        spacing: 5

        BusyIndicator {
            id: busyInd1
            width: 20
            height: 20
            running: true
        }

        BusyIndicator {
            // default width/height is 40
            id: busyInd2
            running: true
        }

        BusyIndicator {
            id: busyInd3
            width: 60
            height: 60
            running: true
        }

        Button {
            text: "Toggle"
            onClicked: {
                busyInd1.running = !busyInd1.running
                busyInd2.running = !busyInd2.running
                busyInd3.running = !busyInd3.running
            }
        }
    }

    Button {
        property Dialog sectionScroll
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - parent.spacing
        text: "SectionScroller"
        onClicked: {
            if (!sectionScroll)
                sectionScroll = sectionScrollComponent.createObject(column)
            sectionScroll.open()
        }
    }

    Component {
        id: sectionScrollComponent
        Dialog {
            id: sectionScroll
            title: Text {
                text: "Section Scroller"
                font { bold: true; pixelSize: 16 }
                color: "white"
                anchors.fill: parent
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            buttons: Row {
                ToolButton {
                    text: "Close"
                    height: parent.height
                    onClicked: sectionScroll.close()
                }
                CheckBox {
                    id: singleRow
                    text: "Single Row"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            content:
                Rectangle {
                width: parent.width
                height: Math.round(screen.height / 3)

                ListModel {
                    id: testModel
                    ListElement { name: "A Cat 1"; alphabet: "A" }
                    ListElement { name: "A Cat 2"; alphabet: "A" }
                    ListElement { name: "Boo 1"; alphabet: "B" }
                    ListElement { name: "Boo 2"; alphabet: "B" }
                    ListElement { name: "Cat 1"; alphabet: "C" }
                    ListElement { name: "Cat 2"; alphabet: "C" }
                    ListElement { name: "Dog 1"; alphabet: "D" }
                    ListElement { name: "Dog 2"; alphabet: "D" }
                    ListElement { name: "Elephant 1"; alphabet: "E" }
                    ListElement { name: "Elephant 2"; alphabet: "E" }
                    ListElement { name: "FElephant 1"; alphabet: "F" }
                    ListElement { name: "FElephant 2"; alphabet: "F" }
                    ListElement { name: "Guinea pig"; alphabet: "G" }
                    ListElement { name: "Goose"; alphabet: "G" }
                    ListElement { name: "Horse"; alphabet: "H" }
                    ListElement { name: "Horse"; alphabet: "H" }
                    ListElement { name: "Parrot"; alphabet: "P" }
                    ListElement { name: "Parrot"; alphabet: "P" }
                }

                ListView {
                    id: list
                    anchors.fill: parent
                    clip: true
                    delegate:  Rectangle {
                        width: list.width
                        height: 20
                        border.color: "#000"
                        border.width: 1
                        color: index % 2 == 0 ? "#ffffff" : "#eeeeee"
                        property string section: name[0]
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            x: 20
                            text: name + " (index " + index + ")"
                        }
                    }

                    model: testModel
                    section.property: "alphabet"
                    section.criteria: ViewSection.FullString
                    section.delegate: Rectangle {
                        width: list.width
                        height: 30
                        color: "#888"
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            x: 5
                            text: section
                            font.bold: true
                            font.pointSize: 16
                        }
                    }
                }
                ScrollDecorator {
                    flickableItem: list
                }

                SectionScroller {
                    id: sectionScroller
                    listView: list
                    platformSingleRow: singleRow.checked
                }
            }
        }
    }

    ButtonRow {
       id: buttonRow1
       width: parent.width - parent.spacing
       exclusive: true
       checkedButton: b2

       Button { text: "b1" }
       Button { text: "b2" }
       Button { text: "b3" }
   }

   ButtonRow {
       id: buttonRow2
       width: parent.width - parent.spacing
       exclusive: true

       ToolButton { text: "tb1" }
       ToolButton { text: "tb2" }
   }

   ButtonColumn {
       id: buttonColumn
       width: parent.width - parent.spacing
       exclusive: true

       Button { text: "b4" }
       Button { text: "b5" }
       Button { text: "b6" }
       Button { text: "b7" }
   }
}
