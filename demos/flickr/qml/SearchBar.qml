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
import Qt.labs.components.native 1.0
import "UIConstants.js" as UI

Rectangle {
    id: searchBar

    property string searchTag
    property bool searching: searchBar.visible

    function open() {
        searchBar.focus = true;
        searchBar.visible = true;
    }

    function close() {
        searchBar.focus = false;
        searchBar.visible = false;
        searchBar.state = "";
    }

    function toggle() {
        if (searching)
            close();
        else
            open();
    }

    function accept() {
        searchBar.state = "";
        searchTag = textField.text;
        searchBar.visible = false;
    }

    anchors.top: parent.top
    border { color: UI.SEARCHBAR_BORDER_COLOR; width: UI.SEARCHBAR_BORDER_WIDTH }
    radius: UI.SEARCHBAR_RADIUS
    height: ((tagButton.height > textField.height) ? tagButton.height : textField.height) + UI.SEARCHBAR_MARGIN
    visible:  false
    focus:  false
    Keys.forwardTo: [(returnKey), (textField)]

    Button {
        id: tagButton

        iconSource: "images/tb_search.svg"
        height: textField.height
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
            rightMargin: UI.SEARCHBAR_MARGIN
        }

        onClicked: searchBar.accept();
    }

    TextField {
        id: textField

        focus: searchBar.visible
        font.pixelSize: UI.SEARCHBAR_FONT_PIXELSIZE
        anchors {
            left: parent.left
            right: tagButton.left
            leftMargin: UI.SEARCHBAR_MARGIN
            rightMargin: UI.SEARCHBAR_MARGIN
            verticalCenter: parent.verticalCenter
        }
    }

    Item {
        id: returnKey

        Keys.onReturnPressed: searchBar.accept();
        Keys.onEnterPressed: searchBar.accept();
        Keys.onEscapePressed: searchBar.close();
    }

    transitions: Transition {
        NumberAnimation {
            properties: "opacity"
            easing.type: Easing.InOutQuad
            duration: 500
        }
    }
}
