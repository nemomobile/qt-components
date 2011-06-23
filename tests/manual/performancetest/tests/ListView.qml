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
import FileAccess 1.0
import "utils"

Item {
    id: root

    anchors.fill: parent
    property int testIndex: 0

    Component {
        id: listViewSetup
        ListViewSetup {
            anchors.fill: parent
            testId: root.testIndex
            onVisibleChanged:
                if (!visible) {
                    symbian.privateClearIconCaches()
                    symbian.privateClearComponentCache()
                    gc()
                }
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        focus: true
        clip: true
        model: model
        delegate: delegate
    }

    ListModel {
        id: model
        ListElement {
            name: "ListView performance test 1"
            description: "ListItems with texts"
        }
        ListElement {
            name: "ListView performance test 2"
            description: "ListItems with image"
        }
        ListElement {
            name: "ListView performance test 3"
            description: "ListItems with image and texts"
        }
        ListElement {
            name: "ListView performance test 4"
            description: "ListView with multiselection"
        }
        ListElement {
            name: "ListView performance test 5"
            description: "ListView with sections"
        }
    }


    Component {
        id: delegate

        ListItem {
            id: listItem
            width: listView.width

            Column {
                anchors.fill: parent.paddingItem

                ListItemText {
                    width: parent.width
                    mode: listItem.mode
                    role: "Title"
                    text: name // Title from model
                }

                ListItemText {
                    width: parent.width
                    mode: listItem.mode
                    role: "SubTitle"
                    text: description // SubTitle from model
                }
            }
            onClicked: {
                    root.testIndex = index
                    mainWindow.pageStack.push(listViewSetup)
            }
        }
    }
}
