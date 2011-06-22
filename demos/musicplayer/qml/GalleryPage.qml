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
import QtMobility.gallery 1.1

Page {
    property int numOfSelectedItems: 0
    property alias backgroundImage:  background.source

    function clearSelection(add) {
        for (var i = 0; i < selection.count; i++) {
            if (selection.get(i).selected) {
                if (add) {
                    var song = galleryModel.get(i)
                    var file = "file:///" + song.filePath
                    player.addSong(file, song.title, song.artist, song.duration * 1000)
                }
                selection.setProperty(i, "selected", false)
            }
        }
        numOfSelectedItems = 0
    }

    tools: galleryTools

    Image {
        id: background

        anchors.fill: parent
    }

    Component {
        id: heading

        PageHeading {
            width: parent.width
            text: qsTr("Music gallery")
        }
    }

    Component {
        id: galleryDelegate

        Item {
            height: 75
            width: galleryView.width

            Rectangle {
                width: 4
                height: parent.height
                color: "#2d2875"
                visible: index % 2
            }

            Column {
                anchors { left: parent.left; leftMargin: 10; verticalCenter: parent.verticalCenter }
                width: parent.width - selectIndicator.width
                spacing: 3

                Text {
                    id: titleText

                    width: parent.width
                    elide: Text.ElideRight
                    font.pixelSize: 22
                    font.letterSpacing: -1
                    color: "white"
                    text: title
                }

                Text {
                    id: artistText

                    width: parent.width
                    elide: Text.ElideRight
                    text: artist == "" ? qsTr("<unknown artist>") : artist
                    font.letterSpacing: -1
                    font.pixelSize: 18
                    color: artist == "" ? "grey" : "white"
                }
            }

            Item {
                id: selectIndicator

                anchors.right: parent.right
                height: parent.height
                width: 70

                CheckBox {
                    anchors.centerIn: parent
                    checked: selection.count > 0 ? selection.get(index).selected : false
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    var selected = selection.get(index).selected
                    selection.setProperty(index, "selected", !selected)
                    if (selected)
                        numOfSelectedItems--
                    else
                        numOfSelectedItems++
                }
            }
        }
    }

    //![0]
    DocumentGalleryModel {
        id: galleryModel

        rootType: DocumentGallery.Audio
        properties: ["artist", "title", "filePath", "duration"]
        sortProperties: ["artist", "title"]

        onCountChanged: {
            selection.clear()
            for (var i = 0; i < count; i++)
                selection.append({"selected": false})
        }
    }
    //![0]

    //![1]
    ListModel {
        id: selection
    }
    //![1]

    ListView {
        id: galleryView

        anchors.fill: parent
        header: heading
        model: galleryModel
        delegate: galleryDelegate
        cacheBuffer: height
        clip: true
        section.property: "artist"
        section.criteria: ViewSection.FullString

        ScrollDecorator {
            flickableItem: parent
        }
    }

    ToolBarLayout {
        id: galleryTools

        ToolButton {
            text: qsTr("ADD")
            enabled: numOfSelectedItems > 0

            onClicked: {
                clearSelection(true)
                pageStack.pop()
            }
        }

        ToolButton {
            text: qsTr("CANCEL")

            onClicked: {
                clearSelection(false)
                pageStack.pop()
            }
        }

        ToolButton {

            flat: true
            iconSource:"qrc:///qml/images/tb_menu.svg"
            enabled: selection.count

            onClicked: viewMenu.open()
        }
    }

    Menu {
        id: viewMenu

        content:
            Column {
            width: viewMenu.width

            MenuItem {
                text: qsTr("Select All")

                onClicked: {
                    viewMenu.close()
                    for (var i = 0; i < selection.count; i++)
                        selection.setProperty(i, "selected", true)

                    numOfSelectedItems = selection.count
                }
            }

            MenuItem {
                text: qsTr("Unselect All")

                onClicked: {
                    viewMenu.close()
                    for (var i = 0; i < selection.count; i++)
                        selection.setProperty(i, "selected", false)

                    numOfSelectedItems = 0
                }
            }
        }
    }
}
