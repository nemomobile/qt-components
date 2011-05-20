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

Item {
    property bool inPortrait: screen.width < screen.height

    property alias largeFont: largeText.font
    property alias smallFont: smallText.font

    property int columns: inPortrait ? 2 : 4
    property int rows: inPortrait ? 2 : 1
    property int spacing: inPortrait ? 5 : 10

    property int interval: 1000
    property int intervalDecrease: 10

    property int radius: 30

    Text {
        id: largeText

        font.family: "Nokia Sans"
        font.weight: Font.DemiBold
        font.pixelSize: 130
    }

    Text {
        id: smallText

        font.family: "Nokia Sans"
        font.weight: Font.Bold
        font.pixelSize: inPortrait ? 30 : 50
    }
}
