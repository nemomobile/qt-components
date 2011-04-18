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
import "UIConstants.js" as UI

Item {
    id: wrapper

    signal photoClicked(string url, int photoWidth, int photoHeight,
                        string author, string date, string description,
                        string tags, string title)

    width: wrapper.ListView.view.width
    height: UI.THUMBNAIL_WRAPPER_SIDE + UI.THUMBNAIL_SPACING

    Thumbnail {
        id: thumb

        source: photoThumbnailUrl
        anchors.left: wrapper.left
    }

    Column {
        id: textColumn

        anchors.left: thumb.right
        anchors.leftMargin: UI.LISTDELEGATE_MARGIN
        anchors.rightMargin: UI.LISTDELEGATE_MARGIN
        anchors.verticalCenter: thumb.verticalCenter
        anchors.right: parent.right
        opacity: thumb.scale
        spacing: UI.LISTDELEGATE_SPACING

        Text {
            text: photoTitle
            color: UI.LISTDELEGATE_TITLE_COLOR
            width: parent.width
            font.bold: true
            elide: Text.ElideRight
            style: Text.Raised
            styleColor: UI.LISTDELEGATE_STYLE_COLOR
        }

        Text {
            text: photoAuthor
            width: parent.width
            elide: Text.ElideRight
            color: UI.LISTDELEGATE_TEXT_COLOR
            style: Text.Raised
            styleColor: UI.LISTDELEGATE_STYLE_COLOR
        }

        Text {
            text: photoDate
            width: parent.width
            elide: Text.ElideRight
            color: UI.LISTDELEGATE_TEXT_COLOR
            style: Text.Raised
            styleColor: UI.LISTDELEGATE_STYLE_COLOR
        }

        Text {
            text: photoWidth + "x" + photoHeight;
            width: parent.width
            elide: Text.ElideRight
            color: UI.LISTDELEGATE_TEXT_COLOR
            style: Text.Raised
            styleColor: UI.LISTDELEGATE_STYLE_COLOR
        }
    }

    MouseArea {
        anchors.fill: wrapper
        onClicked: {
            wrapper.photoClicked(photoUrl, photoWidth, photoHeight,
                                 photoAuthor, photoDate, photoDescription,
                                 photoTags, photoTitle);
        }
    }
}
