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
