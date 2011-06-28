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
import "../components"

Item {
    id: root
    anchors.fill: parent
    state:  "buttonsInCol"
    property bool platformInverted: false

    ButtonRow {
        id: navButtonRow
        anchors.top: parent.top
        anchors.margins: 20
        width: parent.width
        exclusive: true

        Button {
            id: navButton1
            text: "Buttons"
            opacity: 0.6
            focus: true
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
    }

    Item {
        id: buttonTests
        anchors.top: navButtonRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        opacity: 0

        Column {
            id: col1
            anchors.left: parent.left
            spacing: 10

            Label {
                anchors.left: parent.left
                id: text1
                text: "TextButtons"
            }

            ButtonColumn {
                id: buttonColumn1
                anchors.left: parent.left
                exclusive: true

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

            Label {
                anchors.left: parent.left
                id: text2
                text: "IconButtons"
            }

            ButtonColumn {
                id: buttonColumn2
                anchors.left: parent.left
                exclusive: true

                Button {
                    id: b5
                    objectName: "b5"
                    iconSource: "image://theme/:/list5.svg"
                    onClicked: text2.text = "Clicked " + objectName
                }

                Button {
                    id: b6
                    objectName: "b6"
                    iconSource: "image://theme/:/list6.svg"
                    onClicked: text2.text = "Clicked " + objectName
                }

                Button {
                    id: b7
                    objectName: "b7"
                    iconSource: "image://theme/:/list7.svg"
                    onClicked: text2.text = "Clicked " + objectName
                }

                Button {
                    id: b8
                    objectName: "b8"
                    iconSource: "image://theme/:/list8.svg"
                    onClicked: text2.text = "Clicked " + objectName
                }
            }
        }

        Column {
            id: col3
            anchors.left: col2.right
            spacing: 10

            Label {
                anchors.left: parent.left
                id: text3
                text: "Dyn.Buttons"
            }

            Row {
                Button {
                    id: addButton
                    text: "Add Btn"

                    onClicked: {
                        var invertedString = root.platformInverted ? "; platformInverted: true" : ""
                        var item = Qt.createQmlObject('import QtQuick 1.1; import com.nokia.symbian 1.1;  Button { text: \"Btn'
                                                      + buttonColumn3.children.length + '\"' + invertedString + ' }',
                                                      buttonColumn3, "dynButton");
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
                anchors.left: parent.left
                exclusive: true
                width: parent.width
            }
        }
    }

    Item {
        id: checkBoxTests
        anchors.top: navButtonRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        opacity: 0

        Column {
            anchors.left: parent.left
            spacing: 10

            Label {
                id: text4
                anchors.left: parent.left
                text: "CheckBoxes"
            }

            ButtonColumn {
                id: buttonColumn4
                anchors.left: parent.left
                spacing: platformStyle.paddingMedium
                exclusive: toggleExclusive2.checked

                CheckBox {
                    id: checkBox1
                    anchors.left: parent.left
                    objectName: "checkBox1"
                    text: "checkBox1"
                    onClicked: text4.text = "Clicked " + objectName
                }

                CheckBox {
                    id: checkBox2
                    anchors.left: parent.left
                    objectName: "checkBox2"
                    text: "checkBox2"
                    onClicked: text4.text = "Clicked " + objectName
                }

                CheckBox {
                    id: checkBox3
                    anchors.left: parent.left
                    objectName: "checkBox3"
                    text: "checkBox3"
                    onClicked: text4.text = "Clicked " + objectName
                }
            }

            CheckBox {
                id: toggleExclusive2
                anchors.left: parent.left
                text: "exclusive"
                checked: true
            }
        }
    }

    Item {
        id: radioButtonTests
        anchors.top: navButtonRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        opacity: 0

        Column {
            anchors.left: parent.left
            spacing: 10

            Label {
                id: text5
                anchors.left: parent.left
                text: "RadioButtons"
            }

            ButtonColumn {
                id: buttonColumn5
                anchors.left: parent.left
                spacing: platformStyle.paddingMedium
                width: parent.width
                exclusive: toggleExclusive3.checked

                RadioButton {
                    id: rb1
                    anchors.left: parent.left
                    objectName: "rb1"
                    text: "rb1"
                    onClicked: text5.text = "Clicked " + objectName
                }

                RadioButton {
                    id: rb2
                    anchors.left: parent.left
                    objectName: "rb2"
                    text: "rb2"
                    onClicked: text5.text = "Clicked " + objectName
                }

                RadioButton {
                    id: rb3
                    anchors.left: parent.left
                    objectName: "rb3"
                    text: "rb3"
                    onClicked: text5.text = "Clicked " + objectName
                }
            }

            CheckBox {
                id: toggleExclusive3
                anchors.left: parent.left
                checked: true
                text: "exclusive"
            }
        }
    }

    Item {
        id: toolButtonTests
        anchors.top: navButtonRow.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.right: parent.right
        opacity: 0

        Column {
            id: toolBtnCol1
            spacing: 10
            anchors.left: parent.left

            Label {
                id: text6
                anchors.left: parent.left
                text: "ToolB."
            }

            ButtonColumn {
                id: buttonColumn6
                anchors.left: parent.left
                exclusive: toggleExclusive4.checked

                ToolButton {
                    id: tb1
                    anchors.left: parent.left
                    objectName: "tb1"
                    text: "tb1"
                    onClicked: text6.text = "Clicked " + objectName
                }

                ToolButton {
                    id: tb2
                    anchors.left: parent.left
                    objectName: "tb2"
                    text: "tb2"
                    onClicked: text6.text = "Clicked " + objectName
                }

                ToolButton {
                    id: tb3
                    anchors.left: parent.left
                    objectName: "tb3"
                    text: "tb3"
                    onClicked: text6.text = "Clicked " + objectName
                }
            }

            CheckBox {
                id: toggleExclusive4
                anchors.left: parent.left
                checked: true
                text: "Exclusive"
            }
        }

        Column {
            id: toolBtnCol2
            spacing: 10
            anchors.left: toolBtnCol1.right

            Label {
                id: text7
                anchors.left: parent.left
                text: "ToolB. with icons"
            }

            ButtonColumn {
                id: buttonColumn7
                exclusive: true

                ToolButton {
                    id: tb4
                    anchors.left: parent.left
                    objectName:  "tb4"
                    iconSource: "image://theme/:/list5.svg"
                    onClicked: text7.text = "Clicked " + objectName
                }

                ToolButton {
                    id: tb5
                    anchors.left: parent.left
                    objectName:  "tb5"
                    iconSource: "image://theme/:/list6.svg"
                    onClicked: text7.text = "Clicked " + objectName
                }

                ToolButton {
                    id: tb6
                    anchors.left: parent.left
                    objectName:  "tb6"
                    iconSource: "image://theme/:/list7.svg"
                    onClicked: text7.text = "Clicked " + objectName
                }
            }
        }

        Column {
            id: toolBtnCol3
            anchors.left: toolBtnCol2.right
            spacing: 10

            Label {
                id: text8
                anchors.left: parent.left
                text: "Dyn.TB"
            }

            Row {
                Button {
                    id: addToolButton
                    text: "Add TB"

                    onClicked: {
                        var invertedString = root.platformInverted ? "; platformInverted: true" : ""
                        var item = Qt.createQmlObject('import QtQuick 1.1; import com.nokia.symbian 1.1;  ToolButton { text: \"ToolBtn'
                                                      + buttonColumn8.children.length + '\"' + invertedString + ' }',
                                                      buttonColumn8, "dynButton");
                    }
                }

                Button {
                    id: delToolButton
                    text: "Del TB"

                    onClicked: {
                        for (var i = 0; i < buttonColumn8.children.length; i++) {
                            if (buttonColumn8.children[i].checked)
                                buttonColumn8.children[i].destroy();
                        }
                    }
                }
            }

            ButtonColumn {
                id: buttonColumn8
                anchors.left: parent.left
                width: parent.width
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
