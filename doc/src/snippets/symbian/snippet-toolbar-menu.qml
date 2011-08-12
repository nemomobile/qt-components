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

//! [0]
import com.nokia.symbian 1.1

Window {
    id: root
    property Menu menu

    function launchMenu() {
        if (!menu)
            menu = menuComponent.createObject(root)
        menu.open()
    }

    Text {
        anchors {
            fill: parent
            margins: platformStyle.paddingLarge
        }
        text: "This is the basic structure for a normal ToolBar that has a back ToolButton, and a ToolButton that launches a Menu. Note that if the last child of a ToolBarLayout is an icon ToolButton then it will be displayed at the right hand side."
        wrapMode: Text.WordWrap;
        color: platformStyle.colorNormalLight
        font.family: platformStyle.fontFamilyRegular
        font.pixelSize: platformStyle.fontSizeMedium
    }

    ToolBar {
        anchors.bottom: parent.bottom
        tools: toolBarLayout1
    }

    //! [toolbarlayout1]
    ToolBarLayout {
        id: toolBarLayout1

        ToolButton {
            iconSource: "toolbar-back"
            onClicked: Qt.quit()
        }
        ToolButton {
            iconSource: "toolbar-menu"
            onClicked: launchMenu()
        }
    }
    //! [toolbarlayout1]

    Component {
        id: menuComponent
        Menu {
            content: MenuLayout {
                MenuItem { text: "Quit"; onClicked: Qt.quit() }
            }
        }
    }
}
//! [0]
