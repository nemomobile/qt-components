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
    id: sheetPage
    anchors.margins: UiConstants.DefaultMargin

    tools: ToolBarLayout {
        ToolIcon { iconId: "toolbar-back"; onClicked: pageStack.pop(); }
        ToolIcon { iconId: "toolbar-view-menu";}
    }

    Flickable {
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: col
            spacing: 30

            Button {
                text: "Launch Sheet"
                onClicked: sheet.open()
            }

            Button {
                text: "Launch empty Sheet"
                onClicked: emptySheet.open()
            }

            Label {
                id: label
            }
        }
    }

    Sheet {
        id: sheet

        acceptButtonText: "Save"
        rejectButtonText: "Cancel"
        rejectButton.enabled: !disableCancelButton.checked

        title: BusyIndicator {
            anchors.centerIn: parent; running: sheet.status == DialogStatus.Open;
        }

        content: Flickable {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.topMargin: 10
            contentWidth: col2.width
            contentHeight: col2.height
            flickableDirection: Flickable.VerticalFlick

            Column {
                id: col2
                anchors.top: parent.top
                spacing: 10
                Button {
                    id: disableCancelButton
                    text: "Disable cancel button"
                    checkable: true
                }
                Button {
                    text: "Three"
                }
                Button {
                    text: "Four"
                }
                Button {
                    text: "Five"
                }
                TextField {
                    text: "Six"
                }
                Button {
                    text: "Seven"
                }
                Button {
                    text: "Eight"
                }
                Button {
                    text: "Nine"
                }
                TextField {
                    text: "Ten"
                }

            }
        }
        onAccepted: label.text = "Accepted!"
        onRejected: label.text = "Rejected!"
    }

    Sheet {
        id: emptySheet

        acceptButtonText: "Accept"
        rejectButtonText: "Cancel"

        title: BusyIndicator {
            anchors.centerIn: parent; running: emptySheet.status == DialogStatus.Open;
        }

        content: Rectangle {
                anchors.fill: parent
                Label {
                    id: labelEmpty
                    text: "Empty sheet"
                }
        }

        onAccepted: label.text = "Accepted!"
        onRejected: label.text = "Rejected!"
    }
}
