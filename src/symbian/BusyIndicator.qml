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

import Qt 4.7
import "." 1.0

ImplicitSizeItem {
    id: root

    property bool running: false

    implicitWidth: platformStyle.graphicSizeSmall
    implicitHeight: platformStyle.graphicSizeSmall

    Image {
        id: spinner
        property int index: 1

        // cannot use anchors.fill here because the size will be 0 during
        // construction and that gives out nasty debug warnings
        width: parent.width
        height: parent.height
        sourceSize.width: width
        sourceSize.height: height
        source: privateStyle.imagePath("qtg_graf_busyindicator_" + index)
        smooth: true

        NumberAnimation on index {
            from: 1; to: 12
            duration: 1000
            running: root.visible
            paused: !root.running
            loops: Animation.Infinite
        }
    }
}
