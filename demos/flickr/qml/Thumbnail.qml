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
import "UIConstants.js" as UI

Item {
    id: wrapper

    property alias source: thumb.source

    width: UI.THUMBNAIL_WRAPPER_SIDE; height: UI.THUMBNAIL_WRAPPER_SIDE
    scale: scaleMe.scale

    Item {
        id: scaleMe

        anchors.centerIn: parent
        scale: 0.0

        Behavior on scale {
            NumberAnimation {
                easing.type: Easing.InOutQuad
                duration: 500
            }
        }

        Rectangle {
            id: blackRect

            width: UI.THUMBNAIL_WRAPPER_SIDE; height: UI.THUMBNAIL_WRAPPER_SIDE
            anchors.centerIn: parent
            color: UI.THUMBNAIL_WRAPPER_COLOR
            smooth: true

            Image {
                id: thumb

                anchors.centerIn: parent
                smooth: true
                width: UI.THUMBNAIL_SIDE; height: UI.THUMBNAIL_SIDE

                onStatusChanged: {
                    if (thumb.status == Image.Ready)
                        scaleMe.scale = 1;
                }
            }
        }
    }
}
