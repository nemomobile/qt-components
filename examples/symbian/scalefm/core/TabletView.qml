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
    //![helper]
    Item {
        id: helper
        anchors { fill: parent; margins: appStyle.marginSize }
    }
    //![helper]

    RadioConsole {
        id: infoArea
    }

    FrequencyStrip {
        id: frequencyArea
    }

    RadioControls {
        id: controlArea
        count: 2
    }

    Item {
        id: stationList
    }

    StateGroup {
        states: [
            State {
                when: (true)
                AnchorChanges { target: infoArea; anchors.left: helper.left; anchors.top: helper.top }
                AnchorChanges { target: frequencyArea; anchors.left: helper.left; anchors.right: helper.right;  }
                AnchorChanges { target: controlArea;  anchors.left: helper.left; anchors.right: stationList.left }
                AnchorChanges { target: stationList; anchors.right: helper.right }
            }
        ]
    }

    StateGroup {
        states: [
            State {
                when: (container.width / container.height) > 1.20
                AnchorChanges { target: infoArea; anchors.right: undefined }
                AnchorChanges { target: frequencyArea; anchors.top: undefined; anchors.bottom: helper.bottom }
                AnchorChanges { target: controlArea;  anchors.top: infoArea.bottom; anchors.bottom: frequencyArea.top }

                AnchorChanges { target: stationList; anchors.left: infoArea.right; anchors.top: helper.top; anchors.bottom: frequencyArea.top }

                PropertyChanges { target: infoArea; anchors.rightMargin: appStyle.spacingSize; height: Math.round((helper.height - appStyle.frequencyStripHeight) * appStyle.stationListRatio)  }
                PropertyChanges { target: controlArea; anchors.rightMargin: appStyle.spacingSize; anchors.topMargin: appStyle.spacingSize; anchors.bottomMargin: appStyle.spacingSize }
                PropertyChanges { target: stationList; anchors.bottomMargin: appStyle.spacingSize  }
                PropertyChanges { target: infoArea; width: Math.round(helper.width * appStyle.consoleRatio) }
            },
            //![info_area_a]
            State {
                when: (true)
                //![info_area_a]
                AnchorChanges { target: infoArea; anchors.right: helper.right }
                AnchorChanges { target: frequencyArea; anchors.top: infoArea.bottom; anchors.bottom: undefined }
                AnchorChanges { target: controlArea; anchors.top: frequencyArea.bottom; anchors.bottom: helper.bottom }
                //![info_area_b]
                AnchorChanges { target: stationList; anchors.left: undefined; anchors.top: frequencyArea.bottom; anchors.bottom: helper.bottom }
                //![info_area_b]

                PropertyChanges { target: infoArea; height: Math.round(helper.height * appStyle.consoleRatio) }
                PropertyChanges { target: frequencyArea; anchors.topMargin: appStyle.spacingSize }
                PropertyChanges { target: controlArea; anchors.rightMargin: appStyle.spacingSize; anchors.topMargin: appStyle.spacingSize; anchors.bottomMargin: 0 }
                PropertyChanges { target: stationList; anchors.topMargin: appStyle.spacingSize; anchors.bottomMargin: 0 }
                //![info_area_c]
                PropertyChanges { target: stationList; width: Math.round(helper.width * appStyle.stationListRatio) }
            }
            //![info_area_c]

        ]
    }
    Component.onCompleted: {
        window.stationListContainer = stationList
    }
    Component.onDestruction: {
        window.stationListContainer = null
    }
}
