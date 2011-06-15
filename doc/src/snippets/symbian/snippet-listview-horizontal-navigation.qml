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
import Qt 4.7
import com.nokia.symbian 1.1

Window {
    id: root

    ListView {
        id: listView
        anchors.fill: parent
        focus: true
        clip: true
        model: listModel
        delegate: listDelegate
        Component.onCompleted: {
            listView.forceActiveFocus()
        }
    }
//! [0]
    Component {
        id: listDelegate

        ListItem {
            id: listItem

            Row {
                width: parent.width
                anchors.fill: parent.paddingItem

                ListItemText {
                    mode: listItem.mode
                    role: "Title"
                    text: name // Title text is from the 'name' property in the model item (ListElement)
                    width: parent.width - (button1.width + button2.width)
                }
                Button {
                    id : button1
                    text: "Reply"
                    checked: activeFocus
                }
                Button {
                    id : button2
                    text: "Delete"
                    checked: activeFocus
                    onClicked: listView.model.remove(listView.currentIndex)
                }
            }
        KeyNavigation.right: button2
        KeyNavigation.left: button1
        }
    }
//! [0]
    ListModel {
        id: listModel
        ListElement {
            name: "Message 1"
        }
        ListElement {
            name: "Message 2"
        }
        ListElement {
            name: "Message 3"
        }
        ListElement {
            name: "Message 4"
        }
        ListElement {
            name: "Message 5"
        }
    }
}

