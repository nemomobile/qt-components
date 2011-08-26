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
import com.nokia.extras 1.1
import LayoutDirectionSetter 1.0

PageStackWindow {
    id: mainWindow

    property Menu menu
    initialPage: pageComponent

    // for demonstration and testing purposes each component needs to
    // set its inverted state explicitly
    property bool childrenInverted: false

    LayoutMirroring.enabled: Qt.application.layoutDirection == Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

    LayoutDirectionSetter {
        id: layoutDirectionSetter
    }

    ToolBarLayout {
        id: commonTools
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            platformInverted: mainWindow.childrenInverted
            onClicked: mainWindow.pageStack.depth <= 1 ? Qt.quit() : mainWindow.pageStack.pop()
        }
        ToolButton {
            flat: true
            iconSource: "toolbar-menu"
            platformInverted: mainWindow.childrenInverted
            onClicked: {
                if (!menu)
                    menu = menuComponent.createObject(mainWindow)
                menu.open()
            }
        }
    }

    Component.onCompleted: {
        // clear the toolBar pointer, prevents subpages from
        // accidentally removing common application tools
        mainWindow.pageStack.toolBar = null
    }

    Component {
        id: pageComponent

        Page {
            id: testPage
            tools: commonTools

            Item {
                id: testList
                anchors.fill: parent

                function openFile(file) {
                    var component = Qt.createComponent(file)

                    if (component.status == Component.Ready) {
                        mainWindow.pageStack.push(component);
                        console.log("Loading component okay");
                    }
                    else {
                        console.log("Error loading component:", component.errorString());
                    }
                }

                ListModel {
                    id: pagesModel

                    ListElement {
                        page: "InfoBannerPage.qml"
                        title: "InfoBanner"
                        subtitle: "Info Banner"
                    }

                    ListElement {
                        page: "RatingIndicator.qml"
                        title: "RatingIndicator"
                        subtitle: "Indicates ratings"
                    }
                    ListElement {
                        page: "Tumbler.qml"
                        title: "Tumbler"
                        subtitle: "Show tumbler"
                    }
                    ListElement {
                        page: "TumblerDialog.qml"
                        title: "TumblerDialog"
                        subtitle: "Show tumbler dialog"
                    }
                    ListElement {
                        page: "DatePickerDialog.qml"
                        title: "DatePickerDialog"
                        subtitle: "Show date picker dialog"
                    }
                    ListElement {
                        page: "TimePickerDialog.qml"
                        title: "TimePickerDialog"
                        subtitle: "Show time picker dialog"
                    }
                }

                ListView {
                    id: list
                    model: pagesModel
                    anchors.fill: parent

                    delegate: ListItem {
                        id: listItem
                        height: 68
                        platformInverted: mainWindow.childrenInverted

                        Row {
                            id: listItemRow
                            anchors.fill: parent
                            anchors.leftMargin: 18

                            Column {
                                anchors.verticalCenter: parent.verticalCenter

                                ListItemText {
                                    id: mainText
                                    width: listItemRow.width
                                    role: "Title"
                                    text: title
                                    platformInverted: mainWindow.childrenInverted
                                }

                                ListItemText {
                                    id: subText
                                    width: listItemRow.width
                                    role: "SubTitle"
                                    text: subtitle
                                    visible: text != ""
                                    platformInverted: mainWindow.childrenInverted
                                }
                            }
                        }

                        onClicked: { testList.openFile(page); }
                    } // listItem
                } // listView

                ScrollDecorator {
                    flickableItem: list
                    platformInverted: mainWindow.childrenInverted
                }
            } // item
        } // page
    } // component

    Component {
        id: menuComponent

        Menu {
            id: theMenu
            platformInverted: mainWindow.childrenInverted
            content: MenuLayout {
                MenuItem {
                    text: mainWindow.childrenInverted ? "Revert components" : "Invert components"
                    platformInverted: mainWindow.childrenInverted
                    onClicked: mainWindow.childrenInverted = !mainWindow.childrenInverted
                }
                MenuItem {
                    text: mainWindow.platformInverted ? "Revert background" : "Invert background"
                    platformInverted: mainWindow.childrenInverted
                    onClicked: mainWindow.platformInverted = !mainWindow.platformInverted
                }
                MenuItem {
                    text: "Set layout dir"
                    platformSubItemIndicator: true
                    platformInverted: mainWindow.childrenInverted
                    onClicked: layoutDirectionSubMenu.open()
                }
                MenuItem {
                    text: "Quit"
                    platformInverted: mainWindow.childrenInverted
                    onClicked: Qt.quit()
                }
            }
        }
    }

    Menu {
        id: layoutDirectionSubMenu
        platformInverted: mainWindow.childrenInverted
        MenuLayout {
            MenuItem {
                text: "LeftToRight"
                platformInverted: mainWindow.childrenInverted
                onClicked: layoutDirectionSetter.setLayoutDirection(Qt.LeftToRight)
            }
            MenuItem {
                text: "RightToLeft"
                platformInverted: mainWindow.childrenInverted
                onClicked: layoutDirectionSetter.setLayoutDirection(Qt.RightToLeft)
            }
            MenuItem {
                text: "Automatic"
                platformInverted: mainWindow.childrenInverted
                onClicked: layoutDirectionSetter.setLayoutDirection(Qt.LayoutDirectionAuto)
            }
        }
    }
}
