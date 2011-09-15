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
import com.nokia.meego 1.1


Page {
    id: buttonsPage
    tools: buttonTools
    anchors.margins: UiConstants.DefaultMargin

    ToolBarLayout {
        id: buttonTools
        ToolIcon { iconId: "toolbar-back"; onClicked: { myMenu.close(); pageStack.pop(); }  }
        ToolButtonRow {
            ToolButton { text: "ToolButton 1" }
            ToolButton { text: "ToolButton 2" }
        }
        ToolIcon { iconId: "toolbar-view-menu" ; onClicked: myMenu.open(); }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: col
            spacing: 30
            width:  flickable.width

            Component.onCompleted: {
                var count = children.length;
                for (var i = 0; i < count; i++) {
                    var item = children[i];
                    item.anchors.horizontalCenter = item.parent.horizontalCenter;
                }
            }

            Button { text: "Set theme.inverted to " + !theme.inverted; onClicked: { theme.inverted = !theme.inverted; console.log("theme.inverted: " + theme.inverted) } }
            Button { text: "Generic"; }
            Button { text: "Generic forced to inverted style"; platformStyle: ButtonStyle{ inverted: true } }

            Button { id: disabledButton; text: "Disabled"; enabled: false }

            Button {
                text: "Toggle Disabled";
                onClicked: {
                    if (disabledButton.enabled) {
                        disabledButton.enabled = false;
                    } else {
                        disabledButton.enabled = true;
                    }
                }
            }

            Button { text: "Checkable"; checkable: true }
            Button { text: "Checkable, initially checked"; checkable: true; checked: true}
            Button { text: "Checked & disabled"; checkable: true; checked: true; enabled: false }

            Button { iconSource: "image://theme/icon-s-telephony-second-call"; text: "Icon with label" }
            Button { iconSource: "image://theme/icon-s-telephony-end-call" }
            CheckBox {
                text: "CheckBox, initially unchecked "
            }
            CheckBox {
                checked: true
                text: "CheckBox, initially checked"
            }
            CheckBox {
                width: 400
                checked: true
                platformStyle: CheckBoxStyle { elideMode: Text.ElideRight }
                text: "CheckBox, with long long long long long text"
            }

            ButtonRow {
                Button { text: "Two" }
                Button { text: "Buttons" }
            }
            ButtonRow {
                width: 200
                exclusive: false
                Button { iconSource: "image://theme/icon-s-telephony-second-call" }
                Button { iconSource: "image://theme/icon-s-telephony-hold" }
                Button { iconSource: "image://theme/icon-s-telephony-end-call" }
            }
            ButtonRow {
                platformStyle: ButtonStyle { inverted: true }
                Button { text: "Three" }
                Button { text: "Inverted" }
                Button { text: "Buttons" }
            }
            ButtonColumn {
                Button { text: "Column" }
                Button { text: "of" }
                Button { text: "Buttons" }
            }
            ButtonColumn {
                RadioButton { text: "AM" }
                RadioButton { text: "FM" }
                RadioButton { text: "DAB" }
                spacing: 10
            }
        }

    }
    ScrollDecorator {
        flickableItem: flickable
    }
}
