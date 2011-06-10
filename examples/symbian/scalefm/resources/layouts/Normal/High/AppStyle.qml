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

Item {

    //![conversion]
    function mm(number) {
        return Math.round(number * screen.dpi / 25.4)
    }

    function dp(number) {
        return Math.round(number * screen.dpi / 160);
    }
    //![conversion]

    property alias hugeFont: textForHugeFont.font
    property alias largeFont: textForLargeFont.font
    property alias mediumFont: textForMediumFont.font

    property int marginSize: dp(8)
    property int spacingSize: dp(5)

    property int starIconSize: dp(25)
    property int toneIconSize: dp(25)
    property int starTouchSize: mm(6)
    property int frequencyStripHeight: dp(45)
    property int frequencyButtonSize: frequencyStripHeight - 2

    property real consoleRatio: 0.6
    property real stationListRatio: 0.8

    Text {
        id: textForHugeFont
        font.family: "Nokia Sans"
        font.weight: Font.DemiBold
        font.pixelSize: dp((screen.currentOrientation == Screen.Landscape) ? 60 : 50)
    }
    Text {
        id: textForLargeFont
        font.family: "Nokia Sans"
        font.weight: Font.Normal
        font.pixelSize: dp(18)
    }
    Text {
        id: textForMediumFont
        font.family: "Nokia Sans"
        font.weight: Font.Normal
        font.pixelSize: dp((screen.currentOrientation == Screen.Landscape) ? 12 : 14)
    }
}
