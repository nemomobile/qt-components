/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
import com.meego 1.0

Window {
    id: window

    property bool appActionEnabled

    actions: [
        Action {
            iconId: "icon-m-toolbar-addressbook"
            onTriggered: { console.log("App Action"); }
            interactive: appActionEnabled
        }
    ]

    property Component firstPage: Page {
	title: "First Page"

        actions: [
            Action {
                iconId: "icon-m-toolbar-list"
                onTriggered: { console.log("List Action!"); }
                interactive: listButton.checked
            },
            Action {
                iconId: "icon-m-toolbar-grid"
                onTriggered: { console.log("Grid Action!"); }
                interactive: gridButton.checked
            }
        ]

        ButtonRow {
            id: firstRow
            groupType: "None"

            Button {
                text: "Next page"
                onClicked: { window.nextPage(secondPage); }
            }

            Button {
                text: "Fullscreen"
                onClicked: { window.fullscreen = !window.fullscreen; }
            }

            Button {
                text: "Rotate"
                onClicked: {
                    screen.orientation = screen.orientation == Screen.Portrait ? Screen.Landscape : Screen.Portrait;
                }
            }
        }

        ButtonRow {
            groupType: "Many"
            anchors.top: firstRow.bottom
            anchors.topMargin: 10


            Button {
                id: button
                text: "Enable app action"
            }

            Button {
                id: listButton
                text: "List"
            }

            Button {
                id: gridButton
                text: "Grid"
            }
        }

        Binding {
            target: window
            property: "appActionEnabled"
            value: button.checked
        }
    }

    property Component secondPage: Page {
        actions: [
            Action {
                item: Item {
                    height: 60 // ###
                    width: lineedit.width
                    LineEdit {
                        id: lineedit
                        styleType: "toolbar"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            },
            Action {
                iconId: "icon-m-toolbar-camera"
                text: "Camera"
            },
            Action {
                iconId: "icon-m-toolbar-gallery"
                text: "Gallery"
            }
        ]

        ButtonRow {
            id: firstRow
            groupType: "None"

            Button {
                text: "Fullscreen"
                onClicked: { window.fullscreen = !window.fullscreen; }
            }

            Button {
                text: "Rotate"
                onClicked: {
                    screen.orientation = screen.orientation == Screen.Portrait ? Screen.Landscape : Screen.Portrait;
                }
            }
        }
    }

    Component.onCompleted: {
        window.nextPage(firstPage);
    }
}
