/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

import QtQuick 1.0
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
