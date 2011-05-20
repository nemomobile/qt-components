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
        errorHighlight: errorHlgt.checked
        placeholderText: "Enter text here"
        inputMethodHints: selectInputMethodHints.inputMethodHintsFlag
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

    Grid {
        id: buttons
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors {
            left: privy.portrait ? parent.left : copyPasteCut.right
            right: parent.right
            top: privy.portrait ? copyPasteCut.bottom : textField.bottom
            bottom: parent.bottom
            margins: privy.margin
        }
        columns: privy.portrait ? 1 : 2
        rows: privy.portrait ? 6 : 3
        spacing: privy.margin

        Button {
            id: echoModeButton
            height: parent.h; width: parent.w
            text: "Echo mode: " + (echoModeDialog.selectedIndex >= 0
                                   ? echoModeDialog.model[echoModeDialog.selectedIndex] : "default")
            onClicked: echoModeDialog.open()

            SelectionDialog {
                id: echoModeDialog
                titleText: "Select echo mode"
                selectedIndex: -1

                onAccepted: {
                    if (selectedIndex == 0)
                        textField.echoMode = TextInput.Normal
                    else if (selectedIndex == 1)
                        textField.echoMode = TextInput.Password
                    else if (selectedIndex == 2)
                        textField.echoMode = TextInput.NoEcho
                    else if (selectedIndex == 3)
                        textField.echoMode = TextInput.PasswordEchoOnEdit
                }

                model: ["Normal", "Password", "NoEcho", "PwEchoOnEdit"]
            }
        }

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
            id: errorHlgt; objectName: "errorHlgt"
            height: parent.h; width: parent.w
            checkable: true
            text: checked ? "ErrorHighlight" : "No ErrorHighlight"
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

    InputMethodHintsDialog {
        id: selectInputMethodHints
    }
}
