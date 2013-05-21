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
    id: root
    property int level: 0
    property string pageText: "PAGE"
    anchors.margins: UiConstants.DefaultMargin

    ToolBarLayout {
        id: navigationToolbar1
        visible: false
        ToolIcon { iconId: "toolbar-back"; onClicked: pageStack.pop(); }
        ToolIcon { iconId: ["toolbar-send-email",
                            "toolbar-new-chat",
                            "toolbar-headphones",
                            "toolbar-clock",
                            "toolbar-settings",
                            "toolbar-tag"
                            ][level % 6]}
        ToolIcon { iconId: "toolbar-view-menu";}
    }

    ToolBarLayout {
        id: navigationToolbar2
        visible: false
        ToolIcon { iconId: "toolbar-back"; onClicked: pageStack.pop(); }
        ToolIcon { iconId: ["toolbar-send-email",
                            "toolbar-headphones",
                            "toolbar-settings",
                            ][level % 3]}
        ToolIcon { iconId: ["toolbar-new-chat",
                            "toolbar-clock",
                            "toolbar-tag"
                            ][level % 3]}
        ToolIcon { iconId: "toolbar-view-menu";}
    }

    tools: (level % 3 == 0) ? navigationToolbar1 : navigationToolbar2

    Column {
        spacing: 30
        Text {
            text: "This is a " + pageText
            font.pixelSize: 30
            font.bold: true
        }
        Button {
            text: "Go to a sub page";
            onClicked: {
                pageStack.push(Qt.createComponent("DynamicNavigationPage.qml"), { pageText: "sub-" + root.pageText, level: root.level + 1 });
            }
        }
        Column {
            Text { text: "Go directly to n'th subpage:"; font.pixelSize: 30 }

            Row {
                TextField { id: tf; width: 100; text: "1" }
                Button {
                    text: "Go!"
                    onClicked: {
                        var n = parseInt(tf.text)
                        var prefix = "sub-";
                        var level = root.level + 1;
                        var pages = [];
                        for (var i = 0; i < n; i++) {
                            pages.push({ page: Qt.resolvedUrl("DynamicNavigationPage.qml"), properties: { pageText: prefix + root.pageText, level: level } });
                            prefix += "sub-";
                            level += 1;
                        }
                        pageStack.push(pages);
                    }
                }
            }
        }
    }
}
