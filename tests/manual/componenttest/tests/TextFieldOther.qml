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
