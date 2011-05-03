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

Item {
    Item {
        id: helper
        anchors { fill: parent; margins: appStyle.marginSize }
    }

    RadioConsole {
        id: infoArea
    }

    FrequencyStrip {
        id: frequencyArea
    }

    RadioControls {
        id: controlArea
    }

    //![stategroup_default_a]
    StateGroup {
        states: [
            State {
                when: (true)
                AnchorChanges { target: infoArea;  anchors.left: helper.left; anchors.top: helper.top }
                AnchorChanges { target: frequencyArea; anchors.left: helper.left; anchors.right: helper.right }
                AnchorChanges { target: controlArea; anchors.right: helper.right }
                //![stategroup_default_a]

                PropertyChanges { target: infoArea;  anchors.bottomMargin: appStyle.spacingSize }
                //![stategroup_default_b]
            }
        ]
    }
    //![stategroup_default_b]
    //![stategroup_a]
    StateGroup {
        states: [
            State {
                when: (container.width / container.height) > 1.20
                AnchorChanges { target: infoArea; anchors.right: undefined; anchors.bottom: frequencyArea.top }
                AnchorChanges { target: frequencyArea; anchors.top: undefined; anchors.bottom: parent.bottom }
                AnchorChanges { target: controlArea; anchors.left: infoArea.right; anchors.top: helper.top; anchors.bottom: frequencyArea.top }
                //![stategroup_a]

                //![stategroup_b]
                PropertyChanges { target: infoArea; width: Math.round(helper.width * appStyle.consoleRatio) }
                PropertyChanges { target: frequencyArea; anchors.bottomMargin: appStyle.marginSize }
                PropertyChanges { target: controlArea; anchors.leftMargin: appStyle.spacingSize; anchors.topMargin: 0; anchors.bottomMargin: appStyle.spacingSize }
                //![stategroup_b]
                //![stategroup_c]
            },
            State {
                when: (true)
                AnchorChanges { target: infoArea;  anchors.right: helper.right; anchors.bottom: undefined }
                AnchorChanges { target: frequencyArea; anchors.top: infoArea.bottom; anchors.bottom: undefined }
                AnchorChanges { target: controlArea; anchors.left: helper.left; anchors.top: frequencyArea.bottom; anchors.bottom: helper.bottom }
                //![stategroup_c]

                PropertyChanges { target: infoArea; height: Math.round(helper.height * appStyle.consoleRatio) }
                PropertyChanges { target: frequencyArea; anchors.topMargin: appStyle.spacingSize; anchors.bottomMargin: 0 }
                PropertyChanges { target: controlArea; anchors.leftMargin: 0; anchors.topMargin: appStyle.spacingSize; anchors.bottomMargin: 0 }
                //![stategroup_d]
            }
        ]
    }
    //![stategroup_d]
}
