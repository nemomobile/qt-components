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
    anchors.fill: parent
    state:  "buttonsInRow"
    property bool platformInverted: false
    property color labelColor: platformInverted ? platformStyle.colorNormalLightInverted
                                                : platformStyle.colorNormalLight

    ButtonRow {
        id: navButtonRow
        objectName: "navButtonRow"
        anchors.top:  parent.top
        anchors.margins: 20
        width: parent.width

        Button {
            id: navButton1
            objectName: "navButton1"
            text: "Buttons"
            opacity: 0.6
            focus: true
            KeyNavigation.left: navButton4
            KeyNavigation.right: navButton2
            onClicked: root.state = "buttonsInRow"
        }

        Button {
            id: navButton2
            objectName: "navButton2"
            text: "CheckBoxes"
            opacity: 0.6
            KeyNavigation.left: navButton1
            KeyNavigation.right: navButton3
            onClicked: root.state = "checkBoxesInRow"
        }

        Button {
            id: navButton3
            objectName: "navButton3"
            text: "RadioButtons"
            opacity: 0.6
            KeyNavigation.left: navButton2
            KeyNavigation.right: navButton4
            onClicked: root.state = "radioButtonsInRow"
        }

        Button {
            id: navButton4
            objectName: "navButton4"
            text: "ToolButtons"
            opacity: 0.6
            KeyNavigation.left: navButton3
            KeyNavigation.right: navButton1
            onClicked: root.state = "toolButtonsInRow"
        }
    }

    Column {
        id: buttonTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 10
        width: parent.width
        spacing: 10
        visible: false

        Text {
            id: text1
            anchors.left: parent.left
            text: "Buttons with text"
            color: root.labelColor
        }

        ButtonRow {
            id: buttonRow1
            objectName: "buttonRow1"
            width: parent.width
            exclusive: toggleExclusive.checked
            visible: toggleVisible0.checked

            Button {
                id: b1
                objectName: "b1"
                text: "b1"
                onClicked: text1.text = "Clicked " + objectName
            }

            Button {
                id: b2
                objectName: "b2"
                text: "b2"
                onClicked: text1.text = "Clicked " + objectName
            }

            Button {
                id: b3
                objectName: "b3"
                text: "b3"
                onClicked: text1.text = "Clicked " + objectName
            }

            Button {
                id: b4
                objectName: "b4"
                text: "b4"
                onClicked: text1.text = "Clicked " + objectName
            }
        }

        Row {
            anchors.left: parent.left
            spacing: 10
            CheckBox {
                id: toggleExclusive
                objectName: "toggleExclusive"
                text: "exclusive"
                checked: true
            }

            Text {
                text: "CheckedBtn text:"
                anchors.verticalCenter: parent.verticalCenter
                color: root.labelColor
            }

            TextField {
                id: buttonTextField
                objectName: "buttonTextField"
                text: buttonRow1.checkedButton !== null ? buttonRow1.checkedButton.text : " "
                anchors.verticalCenter: parent.verticalCenter
                onTextChanged: {
                    if (buttonRow1.checkedButton !== null)
                        buttonRow1.checkedButton.text = buttonTextField.text
                }
            }
        }

        CheckBox {
            id: toggleVisible0
            anchors.left: parent.left
            text: "visible"
            checked: true
        }

        Text {
            id: text2
            anchors.left: parent.left
            text: "Buttons with icons"
            color: root.labelColor
        }

        ButtonRow {
            id: buttonRow2
            objectName: "buttonRow2"
            width: parent.width

            Button {
                id: b5
                objectName: "b5"
                iconSource: "image://theme/:/list1.svg"
                onClicked: text2.text = "Clicked " + objectName
            }

            Button {
                id: b6
                objectName: "b6"
                iconSource: "image://theme/:/list2.svg"
                onClicked: text2.text = "Clicked " + objectName
            }

            Button {
                id: b7
                objectName: "b7"
                iconSource: "image://theme/:/list3.svg"
                onClicked: text2.text = "Clicked " + objectName
            }

            Button {
                id: b8
                objectName: "b8"
                iconSource: "image://theme/:/list4.svg"
                onClicked: text2.text = "Clicked " + objectName
            }
        }

        Text {
            id: text3
            anchors.left: parent.left
            text: "Dynamic buttons"
            color: root.labelColor
        }

        Row {
            width: parent.width

            ButtonRow {
                id: toolRow
                exclusive: false
                width: parent.width * 3/4

                Button {
                    id: addButton
                    objectName: "addButton"
                    text: "Add"

                    onClicked: {
                        var invertedString = root.platformInverted ? "; platformInverted: true" : ""
                        var item = Qt.createQmlObject('import QtQuick 1.1; import com.nokia.symbian 1.1;  Button { text: \"Btn'
                                                      + buttonRow3.children.length + '\"; objectName: \"' + "Btn" + buttonRow3.children.length
                                                      + '\"' + invertedString + ' }', buttonRow3, "dynButton");
                    }
                }

                Button {
                    id: delButton
                    objectName: "delButton"
                    text: "Del"

                    onClicked: {
                        for (var i = 0; i < buttonRow3.children.length; i++) {
                            if (buttonRow3.children[i].checked)
                                buttonRow3.children[i].destroy()
                        }
                    }
                }

                Button {
                    id: variousButton
                    objectName: "variousButton"
                    width: parent.width * 1/4
                    text: "Various"
                    onClicked: btnDialog.open()
                }
            }

            ContextMenu {
                id: btnContextMenu
                objectName: "btnContextMenu"
                height: parent.height
                width: parent.width

                content: MenuLayout {
                    MenuItem {
                        text: "Delete button"
                        onClicked: {
                            if (dynBtnListView.currentIndex >= 0) {
                                buttonRow3.children[dynBtnListView.currentIndex].destroy()
                                dynBtnListView.model.remove(dynBtnListView.currentIndex)
                            }
                        }
                    }
                    MenuItem {
                        text: "Set as checkedButton"
                        onClicked: buttonRow3.checkedButton = buttonRow3.children[dynBtnListView.currentIndex];
                    }
                }
            }

            Component {
                id: dynBtnDelegate

                ListItem {
                    id: listItem
                    width: dynBtnListView.width
                    platformInverted: root.platformInverted

                    ListItemText {
                        id: itemText
                        mode: listItem.mode
                        role: toggleVisible.checked ? "Title" : "SubTitle"
                        text: model.title
                        platformInverted: root.platformInverted
                        anchors {
                            left: listItem.paddingItem.left
                            right: toggleVisible.left
                            verticalCenter: parent.verticalCenter
                        }
                    }

                    CheckBox {
                        id: toggleVisible
                        anchors {
                            right: listItem.paddingItem.right
                            verticalCenter: parent.verticalCenter
                        }
                        checked: model.visible
                        platformInverted: root.platformInverted

                        onCheckedChanged: {
                            dynBtnListView.model.set(index, { "visible": toggleVisible.checked })
                            buttonRow3.children[index].visible = toggleVisible.checked
                        }
                    }

                    onClicked: toggleVisible.checked = !toggleVisible.checked

                    onPressAndHold: btnContextMenu.open()
                }
            }

            Dialog {
                id: btnDialog
                objectName: "btnDialog"
                height: root.height
                width: root.width

                title: ListItemText {
                    text: "Check visible. Long tap for ctx menu"
                }

                buttons: Button {
                    text: "close"
                    width: parent.width
                    onClicked: btnDialog.accept()
                }

                content: ListView {
                    id: dynBtnListView
                    objectName: "dynBtnListView"
                    anchors.fill:  parent
                    clip: true
                    model: ListModel { id: model }
                    delegate: dynBtnDelegate
                }
            }

            CheckBox {
                id: toggleExclusive2
                objectName: "toggleExclusive2"
                width: parent.width * 1/4
                checked: true
                text: "Excl."
            }
        }

        ButtonRow {
            id: buttonRow3
            objectName: "buttonRow3"
            width: parent.width
            exclusive: toggleExclusive2.checked

            onChildrenChanged: {
                model.clear()
                for (var i = 0; i < buttonRow3.children.length; i++)
                    model.append( { "title": buttonRow3.children[i].objectName, "visible": buttonRow3.children[i].visible } )
                dynBtnListView.positionViewAtIndex(0, ListView.Center)
            }
        }
    }

    Column {
        id: checkBoxTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 10
        width: parent.width
        spacing: 10
        visible: false

        Text {
            id: text4
            text: "Checkable CheckBoxes in a ButtonRow"
            color: root.labelColor
        }

        ButtonRow {
            id: buttonRow4
            objectName: "buttonRow4"
            width: parent.width
            exclusive: toggleExclusive3.checked
            visible: toggleVisible3.checked

            CheckBox {
                id: checkBox1
                objectName: "checkBox1"
                text: "checkBox1"
                onClicked: text4.text = "Clicked " + objectName
            }

            CheckBox {
                id: checkBox2
                objectName: "checkBox2"
                text: "checkBox2"
                onClicked: text4.text = "Clicked " + objectName
            }

            CheckBox {
                id: checkBox3
                objectName: "checkBox3"
                text: "checkBox3"
                onClicked: text4.text = "Clicked " + objectName
            }
        }

        CheckBox {
            id: toggleExclusive3
            objectName: "toggleExclusive3"
            checked: true
            text: "Exclusive"
        }

        CheckBox {
            id: toggleVisible3
            checked: true
            text: "Visible"
        }
    }

    Column {
        id: radioButtonTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 10
        width: parent.width
        spacing: 10
        visible: false

        Text {
            id: text5
            text: "RadioButtons"
            color: root.labelColor
        }

        ButtonRow {
            id: buttonRow5
            objectName: "buttonRow5"
            width: parent.width
            exclusive: toggleExclusive4.checked
            visible: toggleVisible4.checked

            RadioButton {
                id: rb1
                objectName: "rb1"
                text: "rb1"
                onClicked: text5.text = "Clicked " + objectName
            }

            RadioButton {
                id: rb2
                objectName: "rb2"
                text: "rb2"
                onClicked: text5.text = "Clicked " + objectName
            }

            RadioButton {
                id: rb3
                objectName: "rb3"
                text: "rb3"
                onClicked: text5.text = "Clicked " + objectName
            }
        }

        CheckBox {
            id: toggleExclusive4
            objectName: "toggleExclusive4"
            checked: true
            text: "exclusive"
        }

        CheckBox {
            id: toggleVisible4
            checked: true
            text: "Visible"
        }
    }

    Column {
        id: toolButtonTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 10
        width: parent.width
        spacing: 10
        visible: false

        Text {
            id: text6
            color:  root.labelColor
            text: "Toolbuttons with text"
        }

        ButtonRow {
            id: buttonRow6
            objectName: "buttonRow6"
            width: parent.width
            exclusive: true

            ToolButton {
                id: tb1
                objectName: "tb1"
                text: "tb1"
                onClicked: text6.text = "Clicked " + objectName
            }

            ToolButton {
                id: tb2
                objectName: "tb2"
                text: "tb2"
                onClicked: text6.text = "Clicked " + objectName
            }

            ToolButton {
                id: tb3
                objectName: "tb3"
                text: "tb3"
                onClicked: text6.text = "Clicked " + objectName
            }
        }

        Text {
            id: text7
            color:  root.labelColor
            text: "Toolbuttons with icons"
        }

        ButtonRow {
            id: buttonRow7
            objectName: "buttonRow7"
            width: parent.width
            exclusive: toggleExclusive5.checked
            visible: toggleVisible5.checked

            ToolButton {
                id: tb4
                objectName: "tb4"
                iconSource: "image://theme/:/list1.svg"
                onClicked: text7.text = "Clicked " + objectName
            }

            ToolButton {
                id: tb5
                objectName: "tb5"
                iconSource: "image://theme/:/list2.svg"
                onClicked: text7.text = "Clicked " + objectName
            }

            ToolButton {
                id: tb6
                objectName: "tb6"
                iconSource: "image://theme/:/list3.svg"
                onClicked: text7.text = "Clicked " + objectName
            }
        }

        CheckBox {
            id: toggleExclusive5
            objectName: "toggleExclusive5"
            checked: true
            text: "Exclusive"
        }

        CheckBox {
            id: toggleVisible5
            checked: true
            text: "Visible"
        }

        Text {
            id: text8
            text: "Dynamic ToolButtons"
            color: root.labelColor
        }

        Row {
            width: parent.width

            ButtonRow {
                id: toolRow2
                width: parent.width * 3/4
                exclusive: false

                Button {
                    id: addToolButton
                    objectName: "addToolButton"
                    text: "Add ToolBtn"

                    onClicked: {
                        var invertedString = root.platformInverted ? "; platformInverted: true" : ""
                        var item = Qt.createQmlObject('import QtQuick 1.1; import com.nokia.symbian 1.1;  ToolButton { text: \"ToolBtn'
                                                      + buttonRow8.children.length + '\"; objectName: \"' + "ToolBtn"
                                                      + buttonRow8.children.length +'\"'+ invertedString + ' }', buttonRow8, "dynButton");
                    }
                }

                Button {
                    id: delToolButton
                    objectName: "delToolButton"
                    text: "Delete"

                    onClicked: {
                        for (var i = 0; i < buttonRow8.children.length; i++) {
                            if (buttonRow8.children[i].checked)
                                buttonRow8.children[i].destroy()
                        }
                    }
                }

                Button {
                    id: toggleShowToolButton
                    objectName: "toggleShowToolButton"
                    text: "Show/Hide"

                    onClicked: toolBtnDialog.open()
                }
            }

            ContextMenu {
                id: toolBtnContextMenu
                objectName: "toolBtnContextMenu"
                height: parent.height
                width: parent.width

                content: MenuLayout {
                    MenuItem {
                        text: "Delete ToolButton"
                        onClicked: {
                            if (toolButtonsListView.currentIndex >= 0) {
                                buttonRow8.children[toolButtonsListView.currentIndex].destroy()
                                toolButtonsListView.model.remove(toolButtonsListView.currentIndex)
                            }
                        }
                    }
                    MenuItem {
                        text: "Set as checkedButton"
                        onClicked: buttonRow8.checkedButton = buttonRow8.children[toolButtonsListView.currentIndex];
                    }
                }
            }

            Component {
                id: toolBtnListDelegate

                ListItem {
                    id: listItem2
                    width: toolButtonsListView.width
                    platformInverted: root.platformInverted

                    ListItemText {
                        id: itemText2
                        mode: listItem2.mode
                        role: toggleVisible2.checked ? "Title" : "SubTitle"
                        text: model.title
                        platformInverted: root.platformInverted
                        anchors {
                            left: listItem2.paddingItem.left
                            right: toggleVisible2.left
                            verticalCenter: parent.verticalCenter
                        }
                    }

                    CheckBox {
                        id: toggleVisible2
                        anchors {
                            right: listItem2.paddingItem.right
                            verticalCenter: parent.verticalCenter
                        }
                        checked: model.visible
                        platformInverted: root.platformInverted

                        onCheckedChanged: {
                            toolButtonsListView.model.set(index, {"visible" : toggleVisible2.checked})
                            buttonRow8.children[index].visible = toggleVisible2.checked
                        }
                    }

                    onClicked: toggleVisible2.checked = !toggleVisible2.checked

                    onPressAndHold: toolBtnContextMenu.open()
                }
            }

            Dialog {
                id: toolBtnDialog
                objectName: "toolBtnDialog"
                height: root.height
                width: root.width

                title: ListItemText {
                    text: "Check visible buttons"
                }

                buttons: Button {
                    text: "close"
                    width: parent.width
                    onClicked: toolBtnDialog.accept()
                }

                content: ListView {
                    id: toolButtonsListView
                    height: parent.height
                    width: parent.width
                    clip: true
                    model: ListModel { id: model2 }
                    delegate: toolBtnListDelegate
                }
            }

            CheckBox {
                id: toggleExclusive6
                objectName: "toggleExclusive6"
                width: parent.width * 1/4
                checked: true
                text: "Excl."
            }
        }

        ButtonRow {
            id: buttonRow8
            objectName: "buttonRow8"
            width: parent.width
            exclusive: toggleExclusive6.checked

            onChildrenChanged: {
                model2.clear()
                for (var i = 0; i < buttonRow8.children.length; i++)
                    model2.append( { "title": buttonRow8.children[i].objectName, "visible": buttonRow8.children[i].visible } )
                toolButtonsListView.positionViewAtIndex(0, ListView.Center)
            }
        }
    }

    states: [
        State {
            name: "buttonsInRow"
            PropertyChanges { target: buttonTests; visible: true }
            PropertyChanges { target: navButton1; opacity: 1 }
        },
        State {
            name: "checkBoxesInRow"
            PropertyChanges { target: checkBoxTests; visible: true }
            PropertyChanges { target: navButton2; opacity: 1 }
        },
        State {
            name:  "radioButtonsInRow"
            PropertyChanges { target:  radioButtonTests; visible: true }
            PropertyChanges { target: navButton3; opacity: 1 }
        },
        State {
            name: "toolButtonsInRow"
            PropertyChanges { target:  toolButtonTests; visible: true }
            PropertyChanges { target: navButton4; opacity: 1 }
        }
    ]
}
