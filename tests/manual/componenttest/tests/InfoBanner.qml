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
import "../components"

Item {
    id: container

    property bool platformInverted: false
    Label {
        id: title
        anchors { top: parent.top; topMargin: platformStyle.paddingLarge }
        text: "Create Custom InfoBanner"
        width: parent.width
    }

    Column {
        anchors { top: title.bottom; left: parent.left; right: parent.right; margins: platformStyle.paddingLarge }
        spacing: platformStyle.paddingMedium

        TextArea {
            id: textArea
            width: parent.width
            text: "Information to be shown to user"
        }

        Row {
            spacing: platformStyle.paddingMedium
            CheckBox {
                id: iconCheckBox
                checked: true
                text: "Enable icon"
            }
            CheckBox {
                id: interactive
                checked: false
                text: "Interactive"
            }
            Label {
                id: clicked
                color: "green"
                horizontalAlignment: Text.AlignLeft
            }
        }

        Row {
            LayoutMirroring.enabled: false
            spacing: platformStyle.paddingSmall
            Label { text: "Time to dismiss:" }
            Label { text: durationSlider.value + " sec"; color: "green" }
            Slider { id: durationSlider; maximumValue: 9; minimumValue: 0; stepSize:1; value: 3 }
        }

        Row {
            spacing: platformStyle.paddingMedium

            Button {
                text: "Show"
                onClicked: { clicked.text = ""; banner.open() }

            }

            Button {
                text: "Hide"
                onClicked: banner.close();
            }

            Button {
                text: bgButton.visible ? "Hide background" : "Show background"
                onClicked: bgButton.visible = !bgButton.visible;
            }
        }
    }

    InfoBanner {
        id: banner

        text: textArea.text
        iconSource: iconCheckBox.checked ? "qrc:info_banner_thumbnail.png" : ""
        timeout: durationSlider.value*1000
        interactive: interactive.checked
        onClicked: clicked.text = "clicked"
    }

    Button {
        id: bgButton
        parent: internal.visualRoot()
        visible: false
        z: 500
        platformInverted: container.platformInverted
        text: "Background button"
    }

    QtObject {
        id: internal

        function rootObject() {
            var next = container.parent
            while (next && next.parent)
                next = next.parent
            return next
        }

        function visualRoot() {
            var root = rootObject()
            if (root.hasOwnProperty("privateWindow"))
                return root.privateWindow;
            return root;
        }
    }
}
