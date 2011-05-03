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
import "StyleLoader.js" as StyleLoader

Item {
    Rectangle {
        color: "#9CC9E7"
        z: -2
        anchors.fill: parent
        Image {
            source: "qrc:/graphics/qtg_fr_lcd.svg"
            sourceSize.width: width; sourceSize.height: height
            anchors.fill: parent
            anchors.margins: 2
        }
    }
    Image {
        source: "qrc:/graphics/qtg_fr_lcd_overlay.svg"
        sourceSize.width: width; sourceSize.height: height
        anchors.fill: parent
        z: -1
    }

    Image {
        id: starIcon
        property bool selected: window.forceFallbackStyle
        source: selected ? "qrc:/graphics/qtg_small_favorite.svg" : "qrc:/graphics/qtg_small_star_non_favourited.svg"
        sourceSize.width: appStyle.starIconSize; sourceSize.height: appStyle.starIconSize
        anchors {
            left: parent.left; top: parent.top
            leftMargin: appStyle.marginSize; topMargin: appStyle.marginSize
        }

        MouseArea {
            id: starTouchArea
            anchors.centerIn: parent
            width: appStyle.starTouchSize; height: appStyle.starTouchSize
            onClicked: window.forceFallbackStyle = !window.forceFallbackStyle
        }

        rotation: starIcon.selected ? -360 : 360
        Behavior on rotation {
            RotationAnimation { easing.type: "InBounce"; duration: 100 }
        }
    }

    Item {
        id: starIconDummy
        height: appStyle.starIconSize; width: appStyle.starIconSize
        anchors {
            right: parent.right; top: parent.top
            rightMargin: appStyle.marginSize; topMargin: appStyle.marginSize
        }
    }

    Text {
        id: genreLabel
        font: appStyle.mediumFont
        color: "lightgray"
        text: "You're listening to radio ScaleFM!"
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        anchors {
            left: starIcon.right; right: starIconDummy.left; top: parent.top
            leftMargin: appStyle.marginSize; rightMargin: appStyle.marginSize; topMargin: appStyle.marginSize
        }
    }
    Item {
        id: nameLabelLayoutHelper
        anchors { top: genreLabel.bottom; bottom: categoryLabel.top }
    }

    Text {
        id: nameLabel
        font: appStyle.hugeFont
        color: "white"
        text: {
            var temp = Math.round(10*screen.dpi) / 10
            return (temp == Math.round(screen.dpi)) ? temp + '.0' : temp
        }
        anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: nameLabelLayoutHelper.verticalCenter }
    }

    Text {
        id: mhzLabel
        font: appStyle.largeFont
        color: "white"
        text: "dpi"
        anchors { left: nameLabel.right; leftMargin: appStyle.spacingSize; baseline: nameLabel.baseline }
    }

    Image {
        id: toneIcon
        source: "qrc:/graphics/qtg_large_tone.svg"
        sourceSize.width: appStyle.toneIconSize; sourceSize.height: appStyle.toneIconSize
        anchors { left: parent.left; leftMargin: appStyle.marginSize; verticalCenter: nameLabel.verticalCenter }
    }

    Text {
        id: categoryLabel
        font: appStyle.mediumFont
        color: "lightgray"
        text: "Category:\n" + StyleLoader.displayCategory() + "/" + StyleLoader.displayDensity()
        horizontalAlignment: Text.AlignHCenter
        anchors {
            left: parent.left; right: parent.right; bottom: resolutionLabel.top
            leftMargin: appStyle.marginSize; rightMargin: appStyle.marginSize; bottomMargin: appStyle.marginSize
        }
    }
    Text {
        id: resolutionLabel
        font: appStyle.mediumFont
        color: "lightgray"
        text: screen.width + "x" + screen.height
        horizontalAlignment: Text.AlignHCenter
        anchors {
            left: parent.left; right: parent.right; bottom: parent.bottom
            leftMargin: appStyle.marginSize; rightMargin: appStyle.marginSize; bottomMargin: appStyle.marginSize
        }
    }
}
