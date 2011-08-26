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
import com.nokia.meego 1.0

PageStackWindow {
    id: rootWindow

    platformStyle: defaultStyle;

    PageStackWindowStyle { id: defaultStyle }
    PageStackWindowStyle { id: customStyle; background: "image://theme/meegotouch-wallpaper-portrait"}

    // ListPage is what we see when the app starts, it links to the component specific pages
    initialPage: ListPage { }

    // These tools are shared by most sub-pages by assigning the id to a page's tools property
    ToolBarLayout {
        id: commonTools
        visible: false
        ToolIcon { iconId: "toolbar-back"; onClicked: { myMenu.close(); pageStack.pop(); } }
        ToolIcon { iconId: "toolbar-view-menu"; onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close() }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {id: b1; text: "Landscape"; onClicked: screen.allowedOrientations = Screen.Landscape}
            MenuItem {id: b2; text: "Portrait"; onClicked: screen.allowedOrientations = Screen.Portrait}
            MenuItem {id: b3; text: "Color theme 1"; onClicked: theme.colorScheme = 1 }
            MenuItem {id: b4; text: "Color theme 2"; onClicked: theme.colorScheme = 2 }
            MenuItem {id: b5; text: "Color theme 3"; onClicked: theme.colorScheme = 3 }
            MenuItem {id: b6; text: "Color theme 4"; onClicked: theme.colorScheme = 4 }
            MenuItem {id: b7; text: "Color theme 5"; onClicked: theme.colorScheme = 5 }
            MenuItem {id: b8; text: "Color theme 6"; onClicked: theme.colorScheme = 6 }
            MenuItem {id: b9; text: "Color theme 7"; onClicked: theme.colorScheme = 7 }
            MenuItem {id: b10; text: "Color theme 8"; onClicked: theme.colorScheme = 8 }
            MenuItem {id: b11; text: "Color theme 9"; onClicked: theme.colorScheme = 9 }
            MenuItem {id: b12; text: "Color theme 10"; onClicked: theme.colorScheme = 10 }
            MenuItem {id: b13; text: "Color theme 11"; onClicked: theme.colorScheme = 11 }
            MenuItem {id: b14; text: "Color theme 12"; onClicked: theme.colorScheme = 12 }
            MenuItem {id: b15; text: "Color theme 13"; onClicked: theme.colorScheme = 13 }
            MenuItem {id: b16; text: "Color theme 14"; onClicked: theme.colorScheme = 14 }
            MenuItem {id: b17; text: "Color theme 15"; onClicked: theme.colorScheme = 15 }
            MenuItem {id: b18; text: "Color theme 16"; onClicked: theme.colorScheme = 16 }
            MenuItem {id: b19; text: "Color theme 17"; onClicked: theme.colorScheme = 17 }
            MenuItem {id: b20; text: "Color theme 18"; onClicked: theme.colorScheme = 18 }
            MenuItem {id: b21; text: "Color theme 19"; onClicked: theme.colorScheme = 19 }
        }
    }
}

