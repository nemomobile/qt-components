/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
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

import QtQuick 1.0
import com.nokia.symbian 1.0

Window {
    id: mainWindow

    Rectangle {
        id: rect
        anchors {left:parent.left;right:parent.right;top:parent.top;bottom:toolbar.top}
        Text {
            anchors.centerIn: parent
            text: "Choose a colour from the menu"
        }
    }
    state: "rgb" // default to red-green-blue menu state

    //! [0]
    ToolBar {
        id: toolbar
        anchors.bottom: parent.bottom
        tools: ToolBarLayout {
            id: tools
            visible: true
            // add the standard 'back' button to the toolbar
            backButton: true
            ToolButton {
                flat: true
                iconSource: "image://theme/qtg_toolbar_back"
                onClicked: Qt.quit()
            }
    //! [0]
    //! [5]
            // a button to allow you to swap between different menus
            ToolButton {
                id: swapMenuButton
                // button text bound to current state
                text: mainWindow.state == "rgb" ? "B&W menu" : "RGB menu"
                onClicked: {
                    if (mainWindow.state == "rgb") {
                        // set the new menu and change to the new state
                        mainMenu.content = bwMenuContent
                        mainWindow.state = "bw"
                    }
                    else {
                        // set the new menu and change to the new state
                        mainMenu.content = rgbMenuContent
                        mainWindow.state = "rgb"
                    }
                }
            }
    //! [5]
    //! [4]
            // add the standard menu button to the toolbar
            ToolButton {
                flat: true
                iconSource: "image://theme/qtg_toolbar_options"
                onClicked: mainMenu.open()
            }
        }
    }
    //! [4]

    //! [1]
    // define the menu
    Menu {
        id: mainMenu
        content: rgbMenuContent
    }
    //! [1]

    //! [2]
    // define the items in the menu and corresponding actions
    MenuLayout {
        id: rgbMenuContent
        objectName: "rgbMenu"
        MenuItem {
            text: "Red"
            onClicked: rect.color = "Red"
        }
        //! [2]
        MenuItem {
            text: "Green"
            onClicked: rect.color = "Green"
        }
        MenuItem {
            text: "Blue"
            onClicked: rect.color = "Blue"
        }
        //! [3]
        MenuItem {
            text: "CMYK menu"
            platformSubItemIndicator: true
            onClicked: {
                cmykMenu.open()
            }
        }
        //! [3]
    }

    //! [6]
    ContextMenu {
        id: cmykMenu
        objectName: "cmykMenu"
        MenuLayout {
            MenuItem {
                text: "Cyan"
                onClicked: rect.color = "Cyan"
            }
    //! [6]
            MenuItem {
                text: "Magenta"
                onClicked: rect.color = "Magenta"
            }
            MenuItem {
                text: "Yellow"
                onClicked: rect.color = "Yellow"
            }
            MenuItem {
                id: blackItem
                text: "Black"
                onClicked: rect.color = "Black"
            }
        }
    }

    MenuLayout {
        id: bwMenuContent
        objectName: "bwMenu"
        MenuItem {
            text: "Black"
            onClicked: rect.color = "Black"
        }
        MenuItem {
            text: "White"
            onClicked: rect.color = "White"
        }
    }

}
