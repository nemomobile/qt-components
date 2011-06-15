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

import QtQuick 1.1
import com.nokia.symbian 1.1

//![measurements_a]
Item {
//![measurements_a]
    property alias hugeFont: textForHugeFont.font
    property alias largeFont: textForLargeFont.font
    property alias mediumFont: textForMediumFont.font

    //![measurements_b]
    property int marginSize: platformStyle.paddingMedium
    property int spacingSize: platformStyle.paddingSmall
    //![measurements_b]

    property int starIconSize: platformStyle.graphicSizeSmall
    property int toneIconSize: platformStyle.graphicSizeSmall
    property int starTouchSize: platformStyle.graphicSizeMedium
    property int frequencyStripHeight: platformStyle.graphicSizeLarge
    property int frequencyButtonSize: frequencyStripHeight - 2

    property real consoleRatio: (screen.currentOrientation == Screen.Landscape) ? 0.55 : 0.50
    property real stationListRatio: 0.8

    Text {
        id: textForHugeFont
        font.family: platformStyle.fontFamilyRegular
        font.pixelSize: {
            if (screen.currentOrientation == Screen.Landscape)
                return 2.5 * platformStyle.fontSizeLarge
            else
                return 2 * platformStyle.fontSizeLarge
        }
    }
    Text {
        id: textForLargeFont
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
    }
    Text {
        id: textForMediumFont
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
    }
//![measurements_c]
}
//![measurements_c]
