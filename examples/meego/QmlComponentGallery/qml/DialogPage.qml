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

    TabGroup {
        id: tabGroup
        anchors.fill: parent
        currentTab: dialogTab

        // DIALOGS TAB
        Item {
            id: dialogTab

            property QtObject dialogStyle: DialogStyle {}

            property ListModel displayModel: ListModel {}

            // Dialog instances

            Dialog {
                id: myDialog
                title: Item {
                    id: titleField
                    height: myDialog.platformStyle.titleBarHeight
                    width: parent.width
                    Image {
                        id: supplement
                        source: "image://theme/icon-l-contacts"
                        height: parent.height - 10
                        width: 75
                        fillMode: Image.PreserveAspectFit
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                    }

                    Label {
                        id: titleLabel
                        anchors.left: supplement.right
                        anchors.verticalCenter: titleField.verticalCenter
                        font.capitalization: Font.MixedCase
                        color: "white"
                        text: "Generic Dialog"
                    }

                    Image {
                        id: closeButton
                        anchors.verticalCenter: titleField.verticalCenter
                        anchors.right: titleField.right

                        source: "image://theme/icon-m-common-dialog-close"

                        MouseArea {
                            id: closeButtonArea
                            anchors.fill: parent
                            onClicked:  { myDialog.reject(); }
                        }
                    }
                }

                content:Item {
                    id: name
                    height: childrenRect.height
                    Text {
                        id: text
                        font.pixelSize: 22
                        color: "white"
                        text: "Dialog example\nWould you like to see more?"
                    }
                }

                buttons: ButtonRow {
                    platformStyle: ButtonStyle { }
                    anchors.horizontalCenter: parent.horizontalCenter
                    Button {id: b1; text: "OK"; onClicked: myDialog.accept()}
                    Button {id: b2; text: "Cancel"; onClicked: myDialog.reject()}
                }
            }


            QueryDialog {
                id: query

                icon: "image://theme/icon-l-contacts"
                titleText: "This is a Query Dialog. Enjoy."
                message: "Lorem ipsum dolor sit amet, consectetur adipisici elit,"
                + "sed eiusmod tempor incidunt ut labore et dolore magna aliqua."
                + "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris"
                + "nisi ut aliquid ex ea commodi consequat. Quis aute iure reprehenderit"
                + "in voluptate velit esse cillum dolore eu fugiat nulla pariatur."
                + "Excepteur sint obcaecat cupiditat non proident, sunt in culpa qui"
                + "officia deserunt mollit anim id est laborum."
                acceptButtonText: "All right"
                rejectButtonText: "Reject"
            }



            SelectionDialog {
                id: singleSelectionDialog
                titleText: "Dialog Header #1"
//                selectedIndex: 1

                model: ListModel {
                    ListElement { name: "ListTitle #1" }
                    ListElement { name: "ListTitle #2" }
                    ListElement { name: "ListTitle #3" }
                    ListElement { name: "ListTitle #4" }
                    ListElement { name: "ListTitle #5" }
                    ListElement { name: "ListTitle #6" }
                    ListElement { name: "ListTitle #7" }
                    ListElement { name: "ListTitle #8" }
                    ListElement { name: "ListTitle #9" }
                    ListElement { name: "ListTitle #10" }
                    ListElement { name: "ListTitle #11" }
                    ListElement { name: "ListTitle #12" }
                    ListElement { name: "ListTitle #14" }
                    ListElement { name: "ListTitle #15" }
                    ListElement { name: "ListTitle #16" }
                    ListElement { name: "ListTitle #17" }
                    ListElement { name: "ListTitle #18" }
                    ListElement { name: "ListTitle #19" }
                    ListElement { name: "Very long and extremely verbose ListTitle #20" }
                }
            }


            MultiSelectionDialog {
                id: multiSelectionDialog
                titleText: "Dialog Header #1"
                selectedIndexes: [1, 3, 5, 7]
                model: ListModel {
                    ListElement { name: "ListTitle #1" }
                    ListElement { name: "ListTitle #2" }
                    ListElement { name: "ListTitle #3" }
                    ListElement { name: "ListTitle #4" }
                    ListElement { name: "ListTitle #5" }
                    ListElement { name: "ListTitle #6" }
                    ListElement { name: "ListTitle #7" }
                    ListElement { name: "ListTitle #8" }
                    ListElement { name: "ListTitle #9" }
                    ListElement { name: "ListTitle #10" }
                    ListElement { name: "ListTitle #11" }
                    ListElement { name: "ListTitle #12" }
                    ListElement { name: "ListTitle #14" }
                    ListElement { name: "ListTitle #15" }
                    ListElement { name: "ListTitle #16" }
                    ListElement { name: "ListTitle #17" }
                    ListElement { name: "ListTitle #18" }
                    ListElement { name: "ListTitle #19" }
                    ListElement { name: "Very long and extremely verbose ListTitle #20" }
                }
                acceptButtonText: "OK"
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
                        spacing: 24

                        Button {
                            text: "Normal"
                            width: 190
                            onClicked: {
                                myDialog.open();
                            }
                        }

                        Button {
                            text: "Query"
                            width: 190
                            onClicked: {
                                query.open();
                            }
                        }
                    }



                    Button {
                        text: "SingleSelection"
                        onClicked: {
                            singleSelectionDialog.open();
                        }
                    }


                    Grid {
                        rows: screen.orientation == Screen.Landscape || screen.orientation == Screen.LandscapeInverted ? 1 : 2
                        Rectangle {
                            width: 100
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
                            width: 100
                            height: 30
                            color: "lightgray"
                            Text {
                                anchors.centerIn: parent
                                text: singleSelectionDialog.selectedIndex >= 0 ? singleSelectionDialog.model.get(singleSelectionDialog.selectedIndex).name : "None"
                                font.pixelSize: 15
                                font.bold: true
                            }
                        }
                    }


                    Button {
                        text: "MultiSelection"
                        onClicked: {
                            multiSelectionDialog.open();
                        }
                    }

                    Grid {
                        columns: screen.orientation == Screen.Landscape || screen.orientation == Screen.LandscapeInverted ? 8 : 1
                        Component {
                            id: name_delegate
                            Rectangle {
                                width: 100
                                height: 30
                                color: "lightgray"
                                Text {
                                    anchors.centerIn: parent
                                    text: multiSelectionDialog.model.get(multiSelectionDialog.selectedIndexes[index]).name
                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }
                        }

                        Rectangle {
                            width: 100
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

                        Repeater {
                            model: multiSelectionDialog.selectedIndexes
                            delegate: name_delegate
                        }
                    }
                }

            }

        } // dialogTab page

        // MENU TAB
        Item {
            id: menuTab
            anchors.fill: parent

            // Create menu instances

            Menu {
                id: colorMenu
//                visualParent: root.pageStack
                MenuLayout {
                    MenuItem {text: "Red"; onClicked: { colorRect.color = "darkred" } }
                    MenuItem {text: "Green"; onClicked: { colorRect.color = "darkgreen" }}
                    MenuItem {text: "Blue"; onClicked: { colorRect.color = "darkblue" }}
                    MenuItem {text: "Black is an extremely beautiful and stylish color"; onClicked: { theme.inverted = !theme.inverted; colorRect.color = "black" }}
                }
            }

            ContextMenu {
                id: contextMenu
//                visualParent: root.pageStack
                MenuLayout {
                  id: menuLayout
                  MenuItem {id: itemRed; enabled: false; text: "Red";
                            onClicked: { contextColorRect.color = "darkred"; menuLayout.enableItems(); itemRed.enabled = false } }
                  MenuItem {id: itemGreen; text: "Green";
                            onClicked: { contextColorRect.color = "darkgreen"; menuLayout.enableItems(); itemGreen.enabled = false } }
                  MenuItem {id: itemBlue; text: "Blue";
                            onClicked: { contextColorRect.color = "darkblue"; menuLayout.enableItems(); itemBlue.enabled = false } }
                  MenuItem {id: itemBlack;text: "Black is an extremely beautiful and stylish color";
                            onClicked: { contextColorRect.color = "black"; menuLayout.enableItems(); itemBlack.enabled = false } }

                  function enableItems() { itemRed.enabled = true; itemGreen.enabled = true;
                                           itemBlue.enabled = true; itemBlack.enabled = true }
                }
            }


            // Create page and buttons

            Column {
                spacing: 24

                Row {
                    spacing: 24

                    Rectangle {
                        id : colorRect
                        width: 50; height: 50;
                        color : "black"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: { colorMenu.open(); }
                        }
                    }

                    Button {
                        text: "Menu"
                        onClicked: {
                            colorMenu.open();
                        }
                    }
                }

                Row {
                    spacing: 24

                    Rectangle {
                        id : contextColorRect
                        width: 50; height: 50;
                        color : "black"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: { contextMenu.open(); }
                        }
                    }

                    Button {
                        text: "Context Menu"
                        onClicked: {
                            contextMenu.open();
                        }
                    }
                }
            }

        }

        ToolBarLayout {
            id: tabTools
            ToolIcon {
                id: toolIcon 
                iconId: "toolbar-back"
                onClicked: tabGroup.currentTab.depth > 1 ? tabGroup.currentTab.pop() : pageStack.pop()
            }
            ButtonRow {
                anchors {left: toolIcon.right; right: parent.right}
                TabButton {
                    text: "Dialogs"
                    tab: dialogTab
                }
                TabButton {
                    text: "Menus"
                    tab: menuTab
                }
            }
        }
    } //TabGroup
}
