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
import "../components"

Item {
    Menu {
        id: menu

        content: MenuLayout {
            MenuItem {text: "Zero"; onClicked: signalLabel.text = "itemClicked(0)"}
            MenuItem {text: "One"; onClicked: signalLabel.text = "itemClicked(1)"}
            MenuItem {text: "Two"; platformSubItemIndicator: true; onClicked: subMenu.open()}
            MenuItem {text: "Three - This is a long text which does not fit to the screen"; onClicked: signalLabel.text = "itemClicked(3)"}
            MenuItem {text: "Four - This is a long text which does not fit to the screen"; platformSubItemIndicator: true; onClicked: subMenu.open()}
            MenuItem {text: "Five"; onClicked: signalLabel.text = "itemClicked(5)"}
        }
    }

    Button {
        id: menuButton
        x: 10
        y: 50
        width: 160
        height: 50
        text: "Show menu"

        onClicked: {
            signalLabel.text = "No signal"
            menu.open()
        }
    }

    Button {
        id: menuButton2
        x: 180
        y: 50
        width: 170
        height: 50
        text: "Menu, ext dismiss"

        onClicked: {
            signalLabel.text = "No signal"
            menu.open()
            menuDismissTimer.start()
        }
    }

    Timer {
        id: menuDismissTimer
        interval: 3000
        onTriggered: menu.close()
    }

    Row {
        anchors { top: menuButton.bottom; topMargin: 20 }
        x: 50

        Label {
            text: "Signal: "
        }
        Label {
            id: signalLabel
        }
    }

    ContextMenu {
        id: subMenu

        content: MenuLayout {
            MenuItem {text: "Zero"; onClicked: signalLabel.text = "subItemClicked(0)"}
            MenuItem {text: "One"; onClicked: signalLabel.text = "subItemClicked(1)"}
        }
    }
}
