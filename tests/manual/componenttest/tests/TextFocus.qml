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

FocusScope {
    id: root
    anchors.fill: parent
    property variant focusItem: textFields.activeFocus ? textFields : textAreas.activeFocus
                                                       ? textAreas  : null
    property bool platformInverted: false
    focus: true

    Component.onCompleted: mainWindow.softwareInputPanelEnabled = true
    Component.onDestruction:  mainWindow.softwareInputPanelEnabled = false

    Component {
        id: highlight
        Rectangle {
            border { color: "steelblue"; width: 4 }
            color: "#00000000"; radius: 5
            height: focusItem ? focusItem.currentItem.height : 0
            width: focusItem ? focusItem.currentItem.width : 0
            y: focusItem ? focusItem.currentItem.y : 0; z: 5
            Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
            visible: ListView.view.activeFocus
        }
    }

    Rectangle {
        border { color: "hotpink"; width: 4 }
        color: "#00000000"; radius: 5; opacity: 0.80
        x: focusItem ? focusItem.x - focusItem.anchors.margins / 2 : 0
        y: focusItem ? focusItem.y - focusItem.anchors.margins / 2: 0
        height: focusItem ? focusItem.height + focusItem.anchors.margins : 0
        width: focusItem ? focusItem.width + focusItem.anchors.margins : 0
        visible: focusItem ? focusItem.activeFocus : false
        Behavior on y { SpringAnimation { spring: 1; damping: 0.1 } }
    }

    ListView {
        id: textFields

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: inputContext.visible ? parent.bottom : undefined
            margins: platformStyle.paddingLarge
        }

        delegate: TextField {
            text: name
            placeholderText: "Enter Name"
            width: textFields.width
            platformInverted: root.platformInverted
            onActiveFocusChanged: if (activeFocus) textFields.currentIndex = index
        }

        model: ListModel {
            ListElement {name: "Bob"}
            ListElement {name: "John"}
            ListElement {name: "Michael"}
            ListElement {name: ""}
            ListElement {name: "Greg"}
            ListElement {name: "Eric"}
            ListElement {name: "Oliver"}
            ListElement {name: "Jack"}
        }

        clip: true; spacing: platformStyle.paddingMedium
        height: visible ? parent.height * 1/2 - platformStyle.paddingMedium * 2 : 0
        highlight: highlight
        highlightFollowsCurrentItem: false
        focus: true
        visible: activeFocus || !inputContext.visible
        KeyNavigation.up: textAreas; KeyNavigation.down: textAreas
    }

    ListView {
        id: textAreas

        anchors {
            top: activeFocus && inputContext.visible ? parent.top : textFields.bottom
            left: parent.left
            right: parent.right
            bottom: inputContext.visible ? parent.bottom : undefined
            margins: platformStyle.paddingLarge
        }

        delegate: TextArea {
            text: name
            placeholderText: "Enter Text"
            width: textAreas.width
            platformInverted: root.platformInverted
            onActiveFocusChanged: if (activeFocus) textAreas.currentIndex = index
        }

        model: ListModel {
            ListElement {name: "This is a really long piece of text."}
            ListElement {name: "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. "}
            ListElement {name: "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet."}
            ListElement {name: ""}
            ListElement {name: "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Nam tristique augue ac mauris cursus vel porta tortor aliquet."}
        }

        clip: true; spacing: platformStyle.paddingMedium
        height: visible ? parent.height * 1/2 - platformStyle.paddingMedium * 2 : 0
        highlight: highlight
        highlightFollowsCurrentItem: false
        visible: activeFocus || !inputContext.visible
        KeyNavigation.up: textFields; KeyNavigation.down: textFields
    }
}
