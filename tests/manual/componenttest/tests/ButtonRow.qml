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
    anchors.fill: parent
    state:  "buttonsInRow"

    ButtonRow {
        id: navButtonRow
        objectName: "navButtonRow"
        anchors.top:  parent.top
        anchors.margins: 20
        width: parent.width
        exclusive: true
        checkedButton: navButton1

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
        opacity: 0

        Text {
            id: text1
            text: "Buttons with text"
            color: "white"
        }

        ButtonRow {
            id: buttonRow1
            objectName: "buttonRow1"
            width: parent.width
            exclusive: toggleExclusive.checked
            checkedButton:  b2

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

        CheckBox {
            id: toggleExclusive
            objectName: "toggleExclusive"
            text: "exclusive"
            checked: true
        }

        Row {
            visible: toggleExclusive.checked && buttonRow1.checkedButton !== null

            Text {
                text: "Text of checkedButton:"
                color: "white"
            }

            TextField {
                id: buttonTextField
                text: buttonRow1.checkedButton !== null ? buttonRow1.checkedButton.text : " "
                onTextChanged: {
                    if (buttonRow1.checkedButton !== null)
                        buttonRow1.checkedButton.text = buttonTextField.text
                }
            }
        }

        Text {
            id: text2
            text: "Buttons with icons"
            color: "white"
        }

        ButtonRow {
            id: buttonRow2
            objectName: "buttonRow2"
            width: parent.width

            Button {
                id: b5
                objectName: "b5"
                iconSource: "image://theme/:/list1.png"
                onClicked: text2.text = "Clicked " + objectName
            }

            Button {
                id: b6
                objectName: "b6"
                iconSource: "image://theme/:/list2.png"
                onClicked: text2.text = "Clicked " + objectName
            }

            Button {
                id: b7
                objectName: "b7"
                iconSource: "image://theme/:/list3.png"
                onClicked: text2.text = "Clicked " + objectName
            }

            Button {
                id: b8
                objectName: "b8"
                iconSource: "image://theme/:/list4.png"
                onClicked: text2.text = "Clicked " + objectName
            }
        }

        Text {
            id: text3
            text: "Dynamic buttons"
            color: "white"
        }

        ButtonRow {
            exclusive: false
            width: parent.width

            Button {
                id: addButton
                objectName: "addButton"
                text: "Add"

                onClicked: {
                    var item = Qt.createQmlObject('import QtQuick 1.0; import com.nokia.symbian 1.0;  Button { text: \"Btn' +buttonRow3.children.length +'\" }', buttonRow3, "dynButton");
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
        }

        Row {
            width: parent.width

            TextField {
                id: indexTextField
                objectName: "indexTextField"
                placeholderText: "index: "
            }

            Button {
                id: toggleShowButton
                objectName: "toggleShowButton"
                text: "Hide/show"
                onClicked: {
                    if (buttonRow3.children[indexTextField.text])
                        buttonRow3.children[indexTextField.text].visible = !buttonRow3.children[indexTextField.text].visible
                }
            }

            CheckBox {
                id: toggleExclusive2
                objectName: "toggleExclusive2"
                checked: true
                text: "Exclusive"
            }
        }

        ButtonRow {
            id: buttonRow3
            objectName: "buttonRow3"
            width: parent.width
            exclusive: toggleExclusive2.checked
        }
    }

    Column {
        id: checkBoxTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 10
        width: parent.width
        spacing: 10
        opacity: 0

        Text {
            id: text4
            text: "Checkable CheckBoxes in a ButtonRow"
            color: "white"
        }

        ButtonRow {
            id: buttonRow4
            objectName: "buttonRow4"
            width: parent.width
            checkedButton: checkBox3
            exclusive: toggleExclusive3.checked

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
    }

    // Example use of RadioButtons in a ButtonRow
    Column {
        id: radioButtonTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 10
        width: parent.width
        spacing: 10
        opacity: 0

        Text {
            id: text5
            text: "RadioButtons"
            color: "white"
        }

        ButtonRow {
            id: buttonRow5
            objectName: "buttonRow5"
            width: parent.width
            exclusive: toggleExclusive4.checked

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
    }

    // Example use of ToolButtons in a ButtonRow
    Column {
        id: toolButtonTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 10
        width: parent.width
        spacing: 10
        opacity: 0

        Text {
            id: text6
            color:  "white"
            text: "Toolbuttons with text"
        }

        ButtonRow {
            id: buttonRow6
            objectName: "buttonRow6"
            width: parent.width
            exclusive: true
            checkedButton: tb1

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
            color:  "white"
            text: "Toolbuttons with icons"
        }

        ButtonRow {
            id: buttonRow7
            objectName: "buttonRow7"
            width: parent.width
            exclusive: toggleExclusive5.checked
            checkedButton: tb4

            ToolButton {
                id: tb4
                objectName: "tb4"
                iconSource: "image://theme/:/list1.png"
                onClicked: text7.text = "Clicked " + objectName
            }

            ToolButton {
                id: tb5
                objectName: "tb5"
                iconSource: "image://theme/:/list2.png"
                onClicked: text7.text = "Clicked " + objectName
            }

            ToolButton {
                id: tb6
                objectName: "tb6"
                iconSource: "image://theme/:/list3.png"
                onClicked: text7.text = "Clicked " + objectName
            }
        }

        CheckBox {
            id: toggleExclusive5
            objectName: "toggleExclusive5"
            checked: true
            text: "Exclusive"
        }

        Text {
            id: text8
            text: "Dynamic ToolButtons"
            color: "white"
        }

        ButtonRow {
            width: parent.width
            exclusive: false

            Button {
                id: addToolButton
                objectName: "addToolButton"
                text: "Add ToolBtn"

                onClicked: {
                    var item = Qt.createQmlObject('import QtQuick 1.0; import com.nokia.symbian 1.0;  ToolButton { text: \"Btn' +buttonRow8.children.length +'\" }', buttonRow8, "dynButton");
                }
            }

            Button {
                id: delToolButton
                objectName: "delToolButton"
                text: "Del checked"

                onClicked: {
                    if (buttonRow8.checkedButton !== null)
                        buttonRow8.checkedButton.destroy()
                }
            }
        }

        Row {
            width: parent.width

            TextField {
                id: indexTextField2
                placeholderText: "index: "
            }

            Button {
                id: toggleShowButton2
                objectName: "toggleShowButton2"
                text: "Hide/show"
                onClicked: {
                    if (buttonRow8.children[indexTextField2.text])
                        buttonRow8.children[indexTextField2.text].visible = !buttonRow8.children[indexTextField2.text].visible
                }
            }

            CheckBox {
                id: toggleExclusive6
                objectName: "toggleExclusive6"
                checked: true
                text: "Exclusive"
            }
        }

        ButtonRow {
            id: buttonRow8
            objectName: "buttonRow8"
            width: parent.width
            exclusive: toggleExclusive6.checked
        }
    }

    states: [
        State {
            name: "buttonsInRow"
            PropertyChanges { target: buttonTests; opacity: 1 }
            PropertyChanges { target: navButton1; opacity: 1 }
        },
        State {
            name: "checkBoxesInRow"
            PropertyChanges { target: checkBoxTests; opacity: 1 }
            PropertyChanges { target: navButton2; opacity: 1 }
        },
        State {
            name:  "radioButtonsInRow"
            PropertyChanges { target:  radioButtonTests; opacity: 1 }
            PropertyChanges { target: navButton3; opacity: 1 }
        },
        State {
            name: "toolButtonsInRow"
            PropertyChanges { target:  toolButtonTests; opacity: 1 }
            PropertyChanges { target: navButton4; opacity: 1 }
        }
    ]

    transitions: Transition {
        NumberAnimation { property: "opacity"; duration: 500 }
    }
}
