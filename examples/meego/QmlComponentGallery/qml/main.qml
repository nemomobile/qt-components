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

PageStackWindow {
    id: rootWindow

    platformStyle: defaultStyle;

    property bool enableSwipe: true

    PageStackWindowStyle { id: defaultStyle }
    PageStackWindowStyle {
        id: customStyle;
        background: "image://theme/meegotouch-wallpaper-portrait";
        backgroundFillMode: Image.PreserveAspectCrop
    }

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
//        visualParent: pageStack
        onStatusChanged: {
            if (status === DialogStatus.Closing) {
                screen.allowSwipe = enableSwipe;
            }
        }
        MenuLayout {
            MenuItem { text: "Theme color default"; onClicked: theme.colorScheme = 1 }
            MenuItem { text: "Theme color lightGreen"; onClicked: theme.colorScheme = 2 }
            MenuItem { text: "Theme color green"; onClicked: theme.colorScheme = 3 }
            MenuItem { text: "Theme color darkGreen"; onClicked: theme.colorScheme = 4 }
            MenuItem { text: "Theme color darkestGreen"; onClicked: theme.colorScheme = 5 }
            MenuItem { text: "Theme color lightBlue"; onClicked: theme.colorScheme = 6 }
            MenuItem { text: "Theme color blue"; onClicked: theme.colorScheme = 7 }
            MenuItem { text: "Theme color darkBlue"; onClicked: theme.colorScheme = 8 }
            MenuItem { text: "Theme color darkestBlue"; onClicked: theme.colorScheme = 9 }
            MenuItem { text: "Theme color darkPurple"; onClicked: theme.colorScheme = 10 }
            MenuItem { text: "Theme color purple"; onClicked: theme.colorScheme = 11 }
            MenuItem { text: "Theme color pink"; onClicked: theme.colorScheme = 12 }
            MenuItem { text: "Theme color lightPink"; onClicked: theme.colorScheme = 13 }
            MenuItem { text: "Theme color lightOrange"; onClicked: theme.colorScheme = 14 }
            MenuItem { text: "Theme color orange"; onClicked: theme.colorScheme = 15 }
            MenuItem { text: "Theme color darkOrange"; onClicked: theme.colorScheme = 16 }
            MenuItem { text: "Theme color darkYellow"; onClicked: theme.colorScheme = 17 }
            MenuItem { text: "Theme color yellow"; onClicked: theme.colorScheme = 18 }
            MenuItem { text: "Theme color lightYellow"; onClicked: theme.colorScheme = 19 }
            MenuItem { text: "Very long and extremely verbose ListTitle #20" }
        }
    }
}

