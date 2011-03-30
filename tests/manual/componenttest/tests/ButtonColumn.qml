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

Item {
    id: root
    anchors.fill: parent
    state:  "buttonsInCol"

    ButtonRow {
        id: navButtonRow
        anchors.top: parent.top
        anchors.margins: 20
        width: parent.width
        exclusive: true
        checkedButton: navButton1

        Button {
            id: navButton1
            text: "Buttons"
            opacity: 0.6
            onClicked: root.state = "buttonsInCol"
            KeyNavigation.left: navButton4
            KeyNavigation.right: navButton2
        }

        Button {
            id: navButton2
            text: "CheckBoxes"
            opacity: 0.6
            onClicked: root.state = "checkBoxesInCol"
            KeyNavigation.left: navButton1
            KeyNavigation.right: navButton3
        }

        Button {
            id: navButton3
            text: "RadioButtons"
            opacity: 0.6
            onClicked: root.state = "radioButtonsInCol"
            KeyNavigation.left: navButton2
            KeyNavigation.right: navButton4
        }

        Button {
            id: navButton4
            text: "ToolButtons"
            opacity: 0.6
            onClicked: root.state = "toolButtonsInCol"
            KeyNavigation.left: navButton3
            KeyNavigation.right: navButton1
        }

        Component.onCompleted: navButton1.forceActiveFocus()
    }

    Item {
        id: buttonTests
        anchors.top: navButtonRow.bottom
        anchors.left: parent.left
        anchors.margins: 10
        width: parent.width
        opacity: 0

        Column {
            id: col1
            spacing: 10

            Text {
                id: text1
                text: "TextButtons"
                color: "white"
            }

            ButtonColumn {
                id: buttonColumn1
                exclusive: true
                checkedButton: b1

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
                text: "exclusive"
                checked: true

                onCheckedChanged: buttonColumn1.exclusive = toggleExclusive.checked
            }
        }

        Column {
            id: col2
            anchors.left: col1.right
            spacing: 10

            Text {
                id: text2
                text: "IconButtons"
                color: "white"
            }

            ButtonColumn {
                id: buttonColumn2
                exclusive: true
                checkedButton: b5

                Button {
                    id: b5
                    objectName: "b5"
                    iconSource: "image://theme/:/list5.png"
                    onClicked: text2.text = "Clicked " + objectName
                }

                Button {
                    id: b6
                    objectName: "b6"
                    iconSource: "image://theme/:/list6.png"
                    onClicked: text2.text = "Clicked " + objectName
                }

                Button {
                    id: b7
                    objectName: "b7"
                    iconSource: "image://theme/:/list7.png"
                    onClicked: text2.text = "Clicked " + objectName
                }

                Button {
                    id: b8
                    objectName: "b8"
                    iconSource: "image://theme/:/list8.png"
                    onClicked: text2.text = "Clicked " + objectName
                }
            }
        }

        Column {
            id: col3
            anchors.left: col2.right
            spacing: 10

            Text {
                id: text3
                text: "Dyn.Buttons"
                color: "white"
            }

            Row {
                Button {
                    id: addButton
                    text: "Add Btn"

                    onClicked: {
                        var item = Qt.createQmlObject('import Qt 4.7; import com.nokia.symbian 1.0;  Button { text: \"Btn' + buttonColumn3.children.length +'\" }', buttonColumn3, "dynButton");
                    }
                }

                Button {
                    id: delButton
                    text: "Del Btn"

                    onClicked: {
                        for (var i = 0; i < buttonColumn3.children.length; i++) {
                            if (buttonColumn3.children[i].checked)
                                buttonColumn3.children[i].destroy();
                        }
                    }
                }
            }

            ButtonColumn {
                id: buttonColumn3
                exclusive: true
                width: parent.width / 2
            }
        }
    }

    // Example use of CheckBoxes in a ButtonColumn
    Item {
        id: checkBoxTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 20
        width: parent.width
        opacity: 0

        Column {
            spacing: 10

            Text {
                id: text4
                text: "CheckBoxes"
                color: "white"
            }

            ButtonColumn {
                id: buttonColumn4
                checkedButton: checkBox3
                exclusive: toggleExclusive2.checked

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
                id: toggleExclusive2
                text: "exclusive"
                checked: true
            }
        }
    }

    // Example use of RadioButtons in a ButtonColumn
    Item {
        id: radioButtonTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 20
        width: parent.width
        opacity: 0

        Column {
            spacing: 10

            Text {
                id: text5
                text: "RadioButtons"
                color: "white"
            }

            ButtonColumn {
                id: buttonColumn5
                width: parent.width
                exclusive: toggleExclusive3.checked

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
                id: toggleExclusive3
                checked: true
                text: "exclusive"
            }
        }
    }

    // Example use of ToolButtons in a ButtonColumn
    Item {
        id: toolButtonTests
        anchors.top: navButtonRow.bottom
        anchors.margins: 20
        width: parent.width
        opacity: 0

        Column {
            id: toolBtnCol1
            spacing: 10
            anchors.left: parent.left

            Text {
                id: text6
                color:  "white"
                text: "ToolB."
            }

            ButtonColumn {
                id: buttonColumn6
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
        }

        Column {
            id: toolBtnCol2
            spacing: 10
            anchors.left: toolBtnCol1.right

            Text {
                id: text7
                color:  "white"
                text: "ToolB. with icons"
            }

            ButtonColumn {
                id: buttonColumn7
                exclusive: true
                checkedButton: tb4

                ToolButton {
                    id: tb4
                    objectName:  "tb4"
                    iconSource: "image://theme/:/list5.png"
                    onClicked: text7.text = "Clicked " + objectName
                }

                ToolButton {
                    id: tb5
                    objectName:  "tb5"
                    iconSource: "image://theme/:/list6.png"
                    onClicked: text7.text = "Clicked " + objectName
                }

                ToolButton {
                    id: tb6
                    objectName:  "tb6"
                    iconSource: "image://theme/:/list7.png"
                    onClicked: text7.text = "Clicked " + objectName
                }
            }
        }
    }

    states: [
        State {
            name: "buttonsInCol"
            PropertyChanges { target: buttonTests; opacity: 1 }
            PropertyChanges { target: navButton1; opacity: 1 }
        },
        State {
            name: "checkBoxesInCol"
            PropertyChanges { target: checkBoxTests; opacity: 1 }
            PropertyChanges { target: navButton2; opacity: 1 }
        },
        State {
            name:  "radioButtonsInCol"
            PropertyChanges { target:  radioButtonTests; opacity: 1 }
            PropertyChanges { target: navButton3; opacity: 1 }
        },
        State {
            name: "toolButtonsInCol"
            PropertyChanges { target:  toolButtonTests; opacity: 1 }
            PropertyChanges { target: navButton4; opacity: 1 }
        }
    ]

    transitions: Transition {
        NumberAnimation { property: "opacity"; duration: 500 }
    }
}
