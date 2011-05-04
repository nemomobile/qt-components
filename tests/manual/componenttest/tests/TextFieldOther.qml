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
import "content"

Item {
    id: root
    anchors.fill: parent
    anchors.margins: privy.margin

    QtObject {
        id: privy
        property bool portrait:    screen.currentOrientation == Screen.Portrait
                                || screen.currentOrientation == Screen.PortraitInverted
        property int margin: 5
    }

    TextField {
        id: textField; objectName: "textField"
        anchors {
            left: privy.portrait ? parent.left : copyPasteCut.right
            right: parent.right
            margins: privy.margin
        }
        readOnly: readOnly.checked
        enabled: !enabled.checked
        placeholderText: "Enter text here"
    }

    Grid {
        id: copyPasteCut
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors {
            left: parent.left
            top: privy.portrait ? textField.bottom : parent.top
            margins: privy.margin
        }
        columns: privy.portrait ? 3 : 1
        rows: privy.portrait ? 1 : 3
        height: parent.height * (privy.portrait ? 1/6 : 1)
        width: parent.width * (privy.portrait ? 1 : 1/6)
        spacing: privy.margin

        Button {
            id: copy; objectName: "copy"
            height: parent.h; width: parent.w
            text: "Copy"; onClicked: textField.copy()
        }

        Button {
            id: cut; objectName: "cut"
            height: parent.h; width: parent.w
            text: "Cut"; onClicked: textField.cut()
        }

        Button {
            id: paste; objectName: "paste"
            height: parent.h; width: parent.w
            text: "Paste"; onClicked: textField.paste()
        }
    }

    ChoiceList {
        id: echoMode; objectName: "echoMode"
        anchors {
            left: privy.portrait ? parent.left : copyPasteCut.right
            right: parent.right
            top: privy.portrait ? copyPasteCut.bottom : textField.bottom
            margins: privy.margin
        }
        height: parent.height * 1/10
        currentIndex: 0
        model: ["Normal", "Password", "NoEcho", "PwEchoOnEdit"]
        onCurrentIndexChanged: {
            if (currentIndex == 0)
                textField.echoMode = TextInput.Normal
            else if (currentIndex == 1)
                textField.echoMode = TextInput.Password
            else if (currentIndex == 2)
                textField.echoMode = TextInput.NoEcho
            else if (currentIndex == 3)
                textField.echoMode = TextInput.PasswordEchoOnEdit
        }
    }

    Grid {
        id: buttons
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors {
            left: privy.portrait ? parent.left : copyPasteCut.right
            right: parent.right
            top: echoMode.bottom
            bottom: parent.bottom
            margins: privy.margin
        }
        columns: privy.portrait ? 1 : 2
        rows: privy.portrait ? 4 : 2
        spacing: privy.margin

        Button {
            id: readOnly; objectName: "readOnly"
            height: parent.h; width: parent.w
            checkable: true
            text: checked ? "ReadOnly" : "Editable"
        }

        Button {
            id: enabled; objectName: "enabled"
            height: parent.h; width: parent.w
            checkable: true
            text: checked ? "Disabled" : "Enabled"
        }

        Button {
            id: inputMethodHints; objectName: "inputMethodHints"
            height: parent.h; width: parent.w
            text: "InputMethodHints"
            onClicked: selectInputMethodHints.open()
        }

        Button {
            id: defocus; objectName: "defocus"
            height: parent.h; width: parent.w
            text: "Defocus"
            onClicked: forceActiveFocus()
        }
    }

    Dialog {
        id: selectInputMethodHints
        height: parent.height; width: parent.width

        title: Text {
            text: "InputMethodHints"
            anchors.fill: parent
            color: "white"
            font { pointSize: 16; bold: true }
            horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
        }

        buttons: Button {
                    text: "Close"
                    width: parent.width
                    onClicked: selectInputMethodHints.accept()
                }

        content: ListView {
                    id: listView
                    height: parent.height; width: parent.width
                    clip: true
                    model: InputMethodHints { }
                    delegate: textFieldDelegate
                    section.property: "category"
                    section.criteria: ViewSection.FullString
                    section.delegate: sectionDelegate
                    ScrollBar {
                        flickableItem: listView
                        anchors { top: listView.top; right: listView.right }
                    }
                }
    }

    Component {
        id:textFieldDelegate

        ListItem {
            id: listItem
            height: listView.height / 7; width: listView.width

            ListItemText {
                id: listText
                role: "SubTitle"; text: title
                anchors {
                    left: listItem.paddingItem.left; right:checkBox.left
                    verticalCenter: parent.verticalCenter
                }
            }

            CheckBox {
                id: checkBox
                checked: selected
                anchors { right:listItem.paddingItem.right; verticalCenter: parent.verticalCenter }
                onClicked: {
                    textField.inputMethodHints ^= flag
                    listView.model.set(index, { "selected": checkBox.checked })
                }

            }

            onClicked: {
                textField.inputMethodHints ^= flag
                checkBox.checked = !checkBox.checked
                listView.model.set(index, { "selected": checkBox.checked })
            }
        }
    }

    Component {
        id: sectionDelegate

        ListHeading {
            width: listView.width
            id: sectionHeader

            ListItemText {
                anchors.fill: sectionHeader.paddingItem
                role: "Heading"; text: section
            }
        }
    }
}
