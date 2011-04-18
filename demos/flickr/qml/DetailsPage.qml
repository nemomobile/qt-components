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
import "UIConstants.js" as UI

FlickrPage {
    id: details

    property string photoAuthor
    property string photoDate
    property string photoDescription
    property string photoTags
    property string photoTitle
    property int photoHeight
    property int photoWidth

    signal backClicked
    signal infoClicked

    function setPhotoData(author, date, description, tags, title, photoWidth, photoHeight) {
        details.photoAuthor = author;
        details.photoDate = date;
        details.photoDescription = description;
        details.photoTags = tags;
        details.photoTitle = title;
        details.photoHeight = photoHeight;
        details.photoWidth = photoWidth;
    }

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "images/tb_back.svg"
            onClicked: details.backClicked();
        }
        ToolButton {
            iconSource: "images/tb_info.svg"
            checked: true
            onClicked: details.infoClicked();
        }
    }

    Flickable {
        id: flickable

        width: parent.width; height: parent.height
        contentWidth: width
        contentHeight: content.childrenRect.height + UI.MARGIN_XLARGE * 2
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: content

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: UI.MARGIN_XLARGE
            spacing: UI.DETAILS_SPACING

            DetailText {
                text: "<big><b>" + details.photoTitle + "</b></big>"
            }

            DetailText {
                text: "<big><b>" + qsTr("Description") + "&nbsp;</b></big>" + details.photoDescription
            }

            DetailText {
                text: "<big><b>" + qsTr("Author") + "&nbsp;</b></big>" + details.photoAuthor
            }

            DetailText {
                text: "<big><b>" + qsTr("Published") + "&nbsp;</b></big>" + details.photoDate
            }

            DetailText {
                text: "<big><b>" + qsTr("Size") + "&nbsp;</b></big>" + details.photoWidth + qsTr("x")
                      + details.photoHeight
            }

            DetailText {
                text: "<big><b>" + qsTr("Tags") + "&nbsp;</b></big>" + details.photoTags
            }
        }
    }
}
