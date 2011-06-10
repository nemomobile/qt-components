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

import QtQuick 1.0
import com.nokia.symbian 1.1
import Qt.labs.components 1.1

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
        placeholderText: "TextField"
        width: parent.width - parent.spacing
    }

    TextField {
        id: clearable
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: "Clearable TextField"
        text: "Clearable TextField"
        platformRightMargin: clearText.width + platformStyle.paddingMedium * 2
        width: parent.width - parent.spacing

        Image {
            id: clearText
            anchors { top: parent.top; right: parent.right; margins: platformStyle.paddingMedium }
            fillMode: Image.PreserveAspectFit
            smooth: true; visible: parent.text
            source: clear.pressed ? "image://theme/qtg_graf_textfield_clear_pressed" : "image://theme/qtg_graf_textfield_clear_normal"
            height: parent.height - platformStyle.paddingMedium * 2
            width: parent.height - platformStyle.paddingMedium * 2

            MouseArea {
                id: clear
                anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                height: clearable.height; width: clearable.height
                onClicked: clearable.text = ""
            }
        }
    }

    TextField {
        id: customOperation
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: "Custom operation"
        platformRightMargin: addText.width + platformStyle.paddingMedium
        width: parent.width - parent.spacing

        Image {
            id: addText
            anchors { top: parent.top; right: parent.right }
            smooth: true
            fillMode: Image.PreserveAspectFit
            source: add.pressed ? "image://theme/qtg_graf_textfield_add_pressed" : "image://theme/qtg_graf_textfield_add_normal"
            height: parent.height; width: parent.height

            MouseArea {
                id: add
                anchors.fill: parent
                onClicked: textSelection.open()
            }

            SelectionDialog {
                id: textSelection
                titleText: "Preset Texts"
                selectedIndex: -1
                model: ListModel {
                    ListElement { name: "Lorem ipsum." }
                    ListElement { name: "Lorem ipsum dolor sit amet." }
                    ListElement { name: "Lorem ipsum dolor sit amet ipsum." }
                }

                onAccepted: {
                    customOperation.text = textSelection.model.get(textSelection.selectedIndex).name
                    customOperation.forceActiveFocus()
                }

                onRejected: selectedIndex = -1
            }
        }
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
        CommonDialog {
            id: dialog
            titleText: "Dialog"

            buttons: ToolBar {
                id: buttons
                width: parent.width
                height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall

                tools: Row {
                    anchors.centerIn: parent
                    spacing: platformStyle.paddingMedium

                    ToolButton {
                        text: "Ok"
                        width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                        onClicked: dialog.accept()
                    }

                    ToolButton {
                        text: "Cancel"
                        width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                        onClicked: dialog.reject()
                    }
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

            icon: "image://theme/qtg_anim_spinner_large_1"
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
        flat: true
        iconSource: "qrc:ok.svg"
    }

    ToolButton {
        id: toolButton3
        text: "ToolButton"
        iconSource: "qrc:close_stop.svg"
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
            buttons:
                ToolButton {
                    text: "Close"
                    height: platformStyle.graphicSizeMedium
                    onClicked: sectionScroll.close()
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
                        property string section: name[0]

                        width: list.width
                        height: childrenRect.height + (2 * platformStyle.paddingSmall)
                        border { color: "#000"; width: 1 }
                        color: index % 2 == 0 ? "#ffffff" : "#eeeeee"
                        Text {
                            x: platformStyle.paddingSmall
                            y: platformStyle.paddingSmall
                            text: name + " (index " + index + ")"
                        }
                    }
                    model: testModel
                    section.property: "alphabet"
                    section.criteria: ViewSection.FullString
                    section.delegate: Rectangle {
                        width: list.width
                        height: childrenRect.height + (2 * platformStyle.paddingSmall)
                        color: "#888"
                        Text {
                            x: platformStyle.paddingSmall
                            y: platformStyle.paddingSmall
                            text: section
                            font { bold: true; pointSize: platformStyle.fontSizeMedium }
                        }
                    }
                }

                SectionScroller {
                    id: sectionScroller
                    listView: list
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
