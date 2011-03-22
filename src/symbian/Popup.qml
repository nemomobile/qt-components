
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

Item {
    id: root

    property alias visualParent: fader.visualParent
    property int status: DialogStatus.Closed
    property alias animationDuration: fader.animationDuration

    signal faderClicked

    function open() {
        status = DialogStatus.Opening
        fader.state = "Visible"
    }

    function close() {
        status = DialogStatus.Closing
        fader.state = "Hidden"
    }

    Component.onCompleted: {
        parentCache.oldParent = parent
        fader.parent = parent
        parent = fader
    }

    //if this is not given, application may crash in some cases
    Component.onDestruction: {
        if (parentCache.oldParent != null) {
            parent = parentCache.oldParent
        }
    }

    QtObject {
        id: parentCache
        property QtObject oldParent: null
    }

    //This eats mouse events when popup area is clicked
    MouseArea {
        anchors.fill: parent
    }

    Fader {
        id: fader
        onClicked: root.faderClicked()
    }
}
