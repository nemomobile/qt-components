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

Item {
    id: page
    Flickable {
        id: flickableArea
        anchors.fill: parent
        width: 360
        height: 487
        contentWidth: colorfulSquare.width
        contentHeight: colorfulSquare.height
        clip: true

        Rectangle {
            id: colorfulSquare
            width: 1000
            height: 1000
            opacity: 0.5
            gradient: Gradient {
                GradientStop { position: 0.0; color: "blue" }
                GradientStop { position: 1.0; color: "yellow" }
            }
        }

        Rectangle {
            width: colorfulSquare.height
            height: colorfulSquare.width
            opacity: 0.5
            rotation: -90
            gradient: Gradient {
                GradientStop { position: 0.0; color: "red" }
                GradientStop { position: 1.0; color: "green" }
            }
        }

        Text {
            anchors.centerIn: parent
            text: "Flick around!"
            color: "white"
            font.pixelSize: 50
            opacity: 0.85
        }

    }

    ScrollDecorator {
        id: decorator
        flickableItem: flickableArea
    }
}
