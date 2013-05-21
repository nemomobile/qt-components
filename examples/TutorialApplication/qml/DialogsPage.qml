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
    id: root
    tools: tabTools
    anchors.margins: UiConstants.DefaultMargin

    QueryDialog {
        id: query

        icon: "image://theme/icon-l-contacts"
        titleText: "Query Dialog Example"
        message: "Press accept or reject button"
        acceptButtonText: "Accept"
        rejectButtonText: "Reject"
        onAccepted: labelQueryResult.text = "Result: Accepted";
        onRejected: labelQueryResult.text = "Result: Rejected";
    }

    SelectionDialog {
        id: singleSelectionDialog

        titleText: "Single Selection Dialog Header"
        selectedIndex: 1

        model: ListModel {
            ListElement { name: "ListElement #1" }
            ListElement { name: "ListElement #2" }
            ListElement { name: "ListElement #3" }
            ListElement { name: "ListElement #4" }
            ListElement { name: "ListElement #5" }
            ListElement { name: "ListElement #6" }
            ListElement { name: "ListElement #7" }
            ListElement { name: "ListElement #8" }
            ListElement { name: "ListElement #9" }
            ListElement { name: "ListElement #10" }
            ListElement { name: "ListElement #11" }
            ListElement { name: "ListElement #12" }
            ListElement { name: "ListElement #14" }
            ListElement { name: "ListElement #15" }
            ListElement { name: "ListElement #16" }
            ListElement { name: "ListElement #17" }
            ListElement { name: "ListElement #18" }
            ListElement { name: "ListElement #19" }
            ListElement { name: "ListElement #20" }
        }
    }

    // Create page and buttons
    ScrollDecorator {
        flickableItem: container
    }

    Flickable {
        id: container

        x: 0 // we need to set the width and height
        y: 0
        width: root.width
        height: root.height
        contentWidth: dialogs.width
        contentHeight: dialogs.height

        flickableDirection: Flickable.VerticalFlick
        pressDelay: 100

        Column {
            id: dialogs
            spacing: 24

            Row {
                spacing: 32

                Button {
                    text: "Query"
                    width: 200
                    onClicked: {
                        query.open();
                    }
                }

                Label {
                    id: labelQueryResult
                    text: "Result: N/A"
                }
            }

            Row {
                spacing: 32

                Button {
                    text: "SingleSelection"
                    width: 200
                    onClicked: {
                        singleSelectionDialog.open();
                    }
                }


                Grid {
                    rows: screen.orientation == Screen.Landscape || screen.orientation == Screen.LandscapeInverted ? 1 : 2

                    Rectangle {
                        width: 200
                        height: 30
                        color: "white"

                        Text {
                            y: 10
                            anchors.centerIn: parent
                            text: "Selected:"
                            font.pixelSize: 15
                            font.bold: true
                        }
                    }

                    Rectangle {
                        width: 200
                        height: 30
                        color: "lightgray"

                        Text {
                            anchors.centerIn: parent
                            text: singleSelectionDialog.model.get(singleSelectionDialog.selectedIndex).name
                            font.pixelSize: 15
                            font.bold: true
                        }
                    }
                }
            }

            Row {
                spacing: 32

                Button {
                    text: "Color menu"
                    width: 200
                    onClicked: {
                        colorMenu.open();
                    }
                }

                Rectangle {
                    id : colorRect
                    width: 50; height: 50;
                    color : "black"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: { colorMenu.open(); }
                    }
                }
            }
        }
    }

    ToolBarLayout {
        id: tabTools

        ToolIcon { iconId: "toolbar-back"; onClicked: { colorMenu.close(); pageStack.pop(); }  }
        ToolIcon { iconId: "toolbar-view-menu" ; onClicked: colorMenu.open(); }
    }

    Menu {
        id: colorMenu
        visualParent: pageStack

        MenuLayout {
            MenuItem {text: "Red"; onClicked: { colorRect.color = "darkred" } }
            MenuItem {text: "Green"; onClicked: { colorRect.color = "darkgreen" }}
            MenuItem {text: "Blue"; onClicked: { colorRect.color = "darkblue" }}
            MenuItem {text: "Yellow"; onClicked: { colorRect.color = "yellow" }}
        }
    }
}
