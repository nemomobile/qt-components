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
    height: appStyle.frequencyStripHeight

    Item {
        id: frequencyStrip
        anchors.fill: parent

        Image {
            source: "qrc:/graphics/qtg_fr_lcd.svg"
            sourceSize.width: width; sourceSize.height: height
            anchors.fill: parent
        }

        Image {
            source: "qrc:/graphics/qtg_fr_lcd_overlay.svg"
            sourceSize.width: width; sourceSize.height: height
            anchors.fill: parent
        }

        Rectangle {
            id: frequencyStripFade
            anchors.fill:  parent
            border.width: 0
            gradient: Gradient {
                GradientStop { position: 0.00; color: "black" }
                GradientStop { position: 0.10; color: "transparent" }
                GradientStop { position: 0.90; color: "transparent" }
                GradientStop { position: 1.00; color: "black" }
            }
        }

        Button {
            id: frequencyLeftArrow
            iconSource: "qrc:/graphics/qtg_mono_previous.svg"
            width: appStyle.frequencyButtonSize
            height: appStyle.frequencyButtonSize
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            onClicked: {
                var newDpi = StyleLoader.roundNearestFive(screen.dpi) - 5
                screen.privateSetDisplay(screen.width, screen.height, newDpi)
            }
        }
        Button {
            id: frequencyRightArrow
            iconSource: "qrc:/graphics/qtg_mono_next.svg"
            width: appStyle.frequencyButtonSize
            height: appStyle.frequencyButtonSize
            anchors { right: parent.right; verticalCenter: parent.verticalCenter }
            onClicked: {
                var newDpi = StyleLoader.roundNearestFive(screen.dpi) + 5
                screen.privateSetDisplay(screen.width, screen.height, newDpi)
            }
        }

        Text {
            id: paramLabel
            font: appStyle.mediumFont
            color: "lightgray"
            text: {
                var tempTxt = window.appStyleSrc.substring(12)
                return tempTxt.substring(0, tempTxt.lastIndexOf('/') + 1)
            }
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
        }
    }
}
