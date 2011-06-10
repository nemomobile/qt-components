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
