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
import com.nokia.symbian 1.0
import Qt.labs.components 1.0 as QtComponents

ImplicitSizeItem {
    id: root

    property alias maximumValue: model.maximumValue
    property alias ratingValue: model.value
    property int count: -1
    property bool inverted: false

    implicitHeight: Math.max(background.height, text.paintedHeight);
    implicitWidth: background.width + (count >= 0 ? internal.textSpacing + text.paintedWidth : 0);

    QtComponents.RangeModel {
        id: model
        value: 0.0
        minimumValue: 0.0
        maximumValue: 0.0
    }

    QtObject {
        id: internal
        property int indicatorSpacing: 5  // spacing between images
        property int textSpacing: 8  // spacing between image and text
        property string backgroundImageSource: inverted ?
                                                   "qtg_graf_rating_indicator_background_inverted" :
                                                   "qtg_graf_rating_indicator_background_normal"
        property string indicatorImageSource: inverted ?
                                                  "qtg_graf_rating_indicator_inverted" :
                                                  "qtg_graf_rating_indicator_normal"

        // TODO: internal for now until UI spec is complete & colors are available in style
        property string textColor: inverted ? "#fafafa" : "#505050"
    }

    Image {
        id: background
        width: privateStyle.ratingIndicatorImageWidth * maximumValue + Math.max(Math.ceil(maximumValue - 1), 0) * internal.indicatorSpacing
        height: privateStyle.ratingIndicatorImageHeight
        anchors.verticalCenter: height < text.paintedHeight ? text.verticalCenter : undefined
        fillMode: Image.Tile
        source: privateStyle.imagePath(internal.backgroundImageSource)

        Image {
            id: indicator
            width: privateStyle.ratingIndicatorImageWidth * ratingValue + Math.max((Math.ceil(ratingValue) - 1), 0) * internal.indicatorSpacing
            height: privateStyle.ratingIndicatorImageHeight
            fillMode: Image.Tile
            source: privateStyle.imagePath(internal.indicatorImageSource)
        }
    }

    Text {
        id: text
        visible: count >= 0
        text: "(" + count + ")"
        color: internal.textColor
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall }
        anchors.left: background.right
        anchors.leftMargin: internal.textSpacing
    }
}
