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
//![0]
import QtQuick 1.0
import Qt.labs.components.native 1.0
import "UIConstants.js" as UI

FlickrPage {
    id: thumbnailPage

    property alias tags: photoFeedModel.tags
    property bool inPortrait

    signal backClicked
    signal reloadClicked
    signal searchClicked
    signal photoClicked(string url, int photoWidth, int photoHeight,
                        string author, string date, string description,
                        string tags, string title)

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "images/tb_back.svg"
            onClicked: thumbnailPage.backClicked();
        }

        ToolButton {
            iconSource: "images/tb_reload.svg"
            onClicked: {
                photoFeedModel.reload();
                thumbnailPage.reloadClicked();
            }
        }

        ToolButton {
            iconSource: "images/tb_search.svg"
            onClicked: thumbnailPage.searchClicked();
        }
    }

    PhotoFeedModel { id: photoFeedModel }

    GridView {
        property int thumbnailsInRow: 4

        function cellWidth() {
            return Math.floor(width / thumbnailsInRow);
        }

        anchors { fill: parent; margins: UI.GRIDVIEW_MARGIN }
        cacheBuffer: 2 * height
        cellHeight: cellWidth
        cellWidth: cellWidth()
        delegate: GridDelegate {
            onPhotoClicked: {
                thumbnailPage.photoClicked(url, photoWidth, photoHeight, author,
                                           date, description, tags, title);
            }
        }
        model: photoFeedModel
        visible: !thumbnailPage.inPortrait

        onWidthChanged: {
            thumbnailsInRow = width / (UI.THUMBNAIL_WRAPPER_SIDE + UI.THUMBNAIL_SPACING);
        }
    }

    ListView {
        anchors { fill: parent; margins: UI.LISTVIEW_MARGIN }
        cacheBuffer: 2 * height
        delegate: ListDelegate {
            onPhotoClicked: {
                thumbnailPage.photoClicked(url, photoWidth, photoHeight, author,
                                           date, description, tags, title);
            }
        }
        model: photoFeedModel
        visible: thumbnailPage.inPortrait
    }
}
//![0]
