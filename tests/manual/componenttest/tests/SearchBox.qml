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

Item {
    id: root
    objectName: "searchPage"

    Component.onCompleted: mainWindow.softwareInputPanelEnabled = true
    Component.onDestruction:  mainWindow.softwareInputPanelEnabled = false

    property string searchString

    function getDisplayString(displayString, searchString) {
        var startIndex = displayString.match(new RegExp(searchString, "i"))
        var returnValue = displayString
        if (startIndex) {
            returnValue = displayString.replace(new RegExp( searchString, "gi" ), "<FONT style=\"BACKGROUND-COLOR: blue\">"+startIndex+"</FONT>")
        }
        return returnValue;
    }

    // model
    ListModel {
        id: listModel
        ListElement { name: "Lester Cain" }
        ListElement { name: "Nasim Gutierrez" }
        ListElement { name: "Vincent Randolph" }
        ListElement { name: "Mannix Pena" }
        ListElement { name: "Roberts" }
        ListElement { name: "David Gaines" }
        ListElement { name: "Thomas Gillespie" }
        ListElement { name: "David-Gaines" }
        ListElement { name: "Anddy-23@xyz.com" }
        ListElement { name: "Text#Test!&" }
    }

    Component {
        id: contactDelegate
        ListItem {
            id: listItem
            anchors.fill: listItem.padding
            ListItemText {
                anchors.fill: parent.padding
                role: "Title"
                text: getDisplayString(name, searchString)
            }
        }
    }

    QueryDialog {
        id: dialog; objectName: "dialog"
        width: 300
        height: 200
        titleText: "Searchbox"
        message: "Searchbox Backbutton is clicked"
        acceptButtonText: "Ok"
    }

    SearchBox {
        id: searchbox
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        objectName: "SearchBox"
        placeHolderText: "Enter Text"
        onSearchTextChanged: {
            searchString = searchbox.searchText
        }
        onBackClicked: {
            dialog.open()
        }
    }

    Item {
        id: searchlist
        anchors.left: parent.left
        anchors.top: searchbox.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Rectangle {
            anchors.fill: parent
            color: "black"
            border.color: "black"
            border.width: 1

            ListView {
                id: list
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                clip: true
                model: listModel
                delegate: contactDelegate
            }
        }
    }

    Menu {
        id: menu

        content: MenuLayout {
            MenuItem {
                id: placeHolderTextMenu; objectName: "placeHolderTextMenu"
                text: "Change PlaceHolderText"
                onClicked: (searchbox.placeHolderText == "Enter Text") ? searchbox.placeHolderText="Search"
                                                                       : searchbox.placeHolderText="Enter Text"
            }
            MenuItem {
                id: toggleSearchBackMenu; objectName: "toggleSearchBackMenu"
                text: "ToggleBackButton"
                onClicked: searchbox.backButton = !searchbox.backButton
            }
        }
    }

    Connections {
        target: optionsButton
        onClicked: menu.open()
    }
}

