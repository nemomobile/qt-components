/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0
import "UIConstants.js" as UI

// ### Display Entered / Exited! Pause animation when not "on display".
// ### LayoutDirection

ImplicitSizeItem {
    id: root

    property bool running: false
    // Defines the indicator's size, the pixel values are defined in UIConstants.js
    property string size: "medium"

    // Internal properties used for animation timing
    property int __numberOfFrames: 10
    property int __period: 1000

    implicitWidth: UI.SIZE_BUSY_INDICATOR[size]
    implicitHeight: implicitWidth

    Image {
        id: spinner
        property int index: 1
        source: "image://theme/spinner_" + root.implicitWidth + "_" + index
        smooth: true

        NumberAnimation on index {
            from: 1; to: root.__numberOfFrames
            duration: root.__period
            running: root.running && root.visible
            loops: Animation.Infinite
        }
    }
}
