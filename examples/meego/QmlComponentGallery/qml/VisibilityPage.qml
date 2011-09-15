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

Page {
    id: visibilityPage
    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    function updateViewMode() {
        if (platformWindow.viewMode == WindowState.Fullsize) {
            l1.item.color = "green";
        } else {
            l1.item.color = "red";
        }

        l1.item.text = platformWindow.viewModeString;
    }

    function updateVisible() {
        if (platformWindow.visible) {
            l2.item.color = "green";
            l2.item.text = "visible";
        } else {
            l2.item.color = "red";
            l2.item.text = "invisible";
        }
    }

    function updateActive() {
        if (platformWindow.active) {
            l3.item.color = "green";
            l3.item.text = "active";
        } else {
            l3.item.color = "red";
            l3.item.text = "inactive";
        }
    }

    Connections {
        target: platformWindow

        onViewModeChanged: updateViewMode()
        onVisibleChanged: updateVisible()
        onActiveChanged: updateActive()
    }

    Component {
        id: textBox

        Rectangle {
            property alias text: textItem.text

            width: 200; height: 150
            color: "yellow"
            border.color: "black"
            border.width: 5
            radius: 10

            Text {
                id: textItem
                anchors.centerIn: parent
                font.pointSize: 32
                color: "black"
            }
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: col
            spacing: 10
            width: flickable.width

            Loader {
                id: l1
                sourceComponent: textBox
            }

            Loader {
                id: l2
                sourceComponent: textBox
            }

            Loader {
                id: l3
                sourceComponent: textBox
            }

            Component.onCompleted: {
                updateViewMode();
                updateVisible();
                updateActive();

                var count = children.length;
                for (var i = 0; i < count; i++) {
                    var item = children[i];
                    item.anchors.horizontalCenter = item.parent.horizontalCenter;
                }
            }
        }
    }
}
