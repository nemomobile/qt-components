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
