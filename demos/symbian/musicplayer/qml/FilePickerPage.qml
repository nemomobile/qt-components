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
import Qt.labs.folderlistmodel 1.0

Page {
    tools: filePickerTools
    property alias backgroundImage:  background.source
    Image {
        id: background

        anchors.fill: parent
    }

    Component {
        id: heading

        PageHeading {
            width: parent.width
            text: folderListModel.folder
        }
    }

    Component {
        id: filePickerDelegate

        Item {
            height: 75
            width: folderListView.width

            Rectangle {
                width: 4
                height: parent.height
                color: "#2d2875"
                visible: index % 2
            }

            Image {
                id: folderIcon

                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                source: "qrc:///qml/images/folder.svg"
                visible: folderListModel.isFolder(index)
            }

            Text {
                anchors {
                    left: folderIcon.right
                    right: parent.right
                    leftMargin: 5
                    verticalCenter: parent.verticalCenter
                }
                elide: Text.ElideRight
                font.pixelSize: 22
                font.letterSpacing: -1
                color: "white"
                text: fileName
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (folderListModel.isFolder(index)) {
                        if (fileName == "..")
                            folderListModel.folder = folderListModel.parentFolder
                        else
                            folderListModel.folder += "/" + fileName
                    } else {
                        var file = folderListModel.folder + "/" + fileName
                        player.addSong(file, fileName, "", 0)
                        pageStack.pop()
                    }
                }
            }
        }
    }

    //![0]
    FolderListModel {
        id: folderListModel

        nameFilters: ["*.mp3"]
        showDotAndDotDot: true
        showOnlyReadable: true
        sortField: FolderListModel.Type
        folder: "file://"
    }
    //![0]

    ListView {
        id: folderListView

        anchors.fill: parent
        header: heading
        model: folderListModel
        delegate: filePickerDelegate
        cacheBuffer: height
        clip: true
        section.property: "artist"
        section.criteria: ViewSection.FullString

        ScrollDecorator {
            flickableItem: parent
        }
    }

    ToolBarLayout {
        id: filePickerTools

        ToolButton {
            flat: true
            iconSource: "qrc:///qml/images/tb_back.svg"
            onClicked: pageStack.pop()
        }
    }
}
