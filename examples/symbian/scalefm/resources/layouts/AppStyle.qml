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

    property real consoleRatio: 0.6
    property real stationListRatio: 0.8

    Text {
        id: textForHugeFont
        font.family: platformStyle.fontFamilyRegular
        font.pixelSize: {
            if((screen.currentOrientation == Screen.Landscape))
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
