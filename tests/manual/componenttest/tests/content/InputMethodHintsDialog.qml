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
import Qt.labs.components 1.1
import "../../components"

Dialog {
    id: selectInputMethodHints
    height: parent.height; width: parent.width
    property variant inputMethodHintsFlag : 0
    property bool platformInverted: false

    title: Label {
        text: "InputMethodHints"
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
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

    Component {
        id:textFieldDelegate

        ListItem {
            id: listItem
            height: listView.height / 7; width: listView.width
            platformInverted: selectInputMethodHints.platformInverted

            ListItemText {
                id: listText
                role: "SubTitle"; text: title
                platformInverted: selectInputMethodHints.platformInverted
                anchors {
                    left: listItem.paddingItem.left; right:checkBox.left
                    verticalCenter: parent.verticalCenter
                }
            }

            CheckBox {
                id: checkBox
                checked: selected
                anchors { right:listItem.paddingItem.right; verticalCenter: parent.verticalCenter }
                platformInverted: selectInputMethodHints.platformInverted
                onClicked: {
                    inputMethodHintsFlag ^= flag
                    listView.model.set(index, { "selected": checkBox.checked })
                }
            }

            onClicked: {
                inputMethodHintsFlag ^= flag
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
            platformInverted: selectInputMethodHints.platformInverted

            ListItemText {
                anchors.fill: sectionHeader.padding
                role: "Heading"; text: section
                platformInverted: selectInputMethodHints.platformInverted
            }
        }
    }
}
