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
import com.meego 1.0

Item {
    width: 800
    height: 600

    Row {
        x: 0
        y: parent.headerBottom
        width: parent.width
        height: parent.height - y

        ButtonColumn {
            id: group
            width: 205
            height: parent.height

            Repeater {
                model: ["Button", "Button Groups"]
                Button {
                    text: modelData
                    width: 200
                    height: 50
                }
            }

            onCheckedButtonChanged: {
                loader.source = group.checkedButton.text.toLowerCase().replace(" ", "") + ".qml";
            }
        }

        Loader {
            id: loader;
            anchors.left: group.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            opacity: 1.0

            MouseArea {
                anchors.fill: parent;
                z: 2;
                onPressed: {
                    loader.focus = true;
                    mouse.accepted = false;
                }
            }
        }

        Item {
            id: notImplemented
            anchors.fill: loader
            opacity: 0.0

            Text {
                x: 40
                y: 40
                text: "Not implemented yet."
                font.pixelSize: 20
            }
        }
    }

    states: [
        State {
            name: "not"
            when: { loader.status === Loader.Error; }
            PropertyChanges {
                target: notImplemented
                opacity: 1.0
            }
            PropertyChanges {
                target: loader
                opacity: 0.0
            }
        }
    ]
}
