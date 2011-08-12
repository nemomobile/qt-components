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

Window {
    id: root

    Rectangle {
        id: rect
        anchors { left: parent.left; right:parent.right; top: parent.top; bottom: toolbar.top }
        Text {
            anchors.centerIn: parent
            text: "Choose a colour from the menu"
        }
    }

    //! [0]
    ToolBar {
        id: toolbar
        anchors.bottom: parent.bottom
        tools: ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }
            // add the standard menu button to the toolbar
            ToolButton {
                iconSource: "toolbar-menu"
                onClicked: mainMenu.open()
            }
        }
    }
    //! [0]

    //! [1a]
    // define the menu
    Menu {
        id: mainMenu
        // define the items in the menu and corresponding actions
        content: MenuLayout {
            MenuItem {
                text: "Red"
                onClicked: rect.color = "Red"
            }
    //! [1a]
            MenuItem {
                text: "Green"
                onClicked: rect.color = "Green"
            }
            MenuItem {
                text: "Blue"
                onClicked: rect.color = "Blue"
            }
    // Exceptional indentation used to make the snippet look better in the documentation
    //! [2]
    MenuItem {
        text: "CMYK menu"
        platformSubItemIndicator: true
        onClicked: cmykMenu.open()
    }
    //! [2]
    //! [1b]
        }
    }
    //! [1b]


    //! [3a]
    ContextMenu {
        id: cmykMenu
        MenuLayout {
            MenuItem {
                text: "Cyan"
                onClicked: rect.color = "Cyan"
            }
    //! [3a]
            MenuItem {
                text: "Magenta"
                onClicked: rect.color = "Magenta"
            }
            MenuItem {
                text: "Yellow"
                onClicked: rect.color = "Yellow"
            }
            MenuItem {
                text: "Black"
                onClicked: rect.color = "Black"
            }
    //! [3b]
        }
    }
    //! [3b]
}
