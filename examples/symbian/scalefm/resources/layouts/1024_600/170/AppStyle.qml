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

// 7.0" WSVGA
//![measurements_a]
Item {
//![measurements_a]
    property alias hugeFont: textForHugeFont.font
    property alias largeFont: textForLargeFont.font
    property alias mediumFont: textForMediumFont.font

    property int marginSize: 12
    property int spacingSize: 8

    //![measurements_b]
    property int starIconSize: 45
    property int toneIconSize: 45
    property int starTouchSize: 54
    property int frequencyStripHeight: 78
    property int frequencyButtonSize: 82
    //![measurements_b]

    property real consoleRatio: 0.40
    property real stationListRatio: 0.8

    Text {
        id: textForHugeFont
        font.family: "Nokia Sans"
        font.weight: Font.DemiBold
        font.pixelSize: (screen.currentOrientation == Screen.Landscape) ? 62 : 75
    }
    Text {
        id: textForLargeFont
        font.family: "Nokia Sans"
        font.weight: Font.Normal
        font.pixelSize: 26
    }
    Text {
        id: textForMediumFont
        font.family: "Nokia Sans"
        font.weight: Font.Normal
        font.pixelSize: 19
    }
//![measurements_c]
}
//![measurements_c]
