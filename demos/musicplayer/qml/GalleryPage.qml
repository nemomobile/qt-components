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
