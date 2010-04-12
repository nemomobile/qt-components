/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
import Qt 4.7
import Components 1.0

Item {
    id: basicProgressBar;
    property alias progress: model.value
    property alias inverted: model.inverted
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue

    width: 160
    height: 22

    BorderImage {
        id: background;
        source: "images/progress-bar-background.png";
        anchors.fill: parent;
        border.left:5;
        border.right:5;
    }

    Rectangle {
        id: progressbar;
        x: 3;

        width: model.position
        color: '#84dbf5';

        anchors.top: background.top;
        anchors.topMargin: 3;
        anchors.bottom: background.bottom;
        anchors.bottomMargin: 3;
    }

    Rectangle {
        id: infinitebar;
        visible: false;
        width: parent.width * 0.2;
        color: '#84dbf5';

        anchors.top: background.top;
        anchors.topMargin: 3;
        anchors.bottom: background.bottom;
        anchors.bottomMargin: 3;

        SequentialAnimation on x {
            loops: Animation.Infinite;
            NumberAnimation {
                to: 279 - infinitebar.width;
                duration: 2000;
            }
            NumberAnimation {
                to: 0;
                duration: 2000;
            }
        }
    }

    RangeModel {
        id: model
        minimumValue: 0
        maximumValue: 100
        positionAtMinimum: 0
        positionAtMaximum: background.width - 6
    }

    states: [
        State {
            name: "still";
            PropertyChanges {
                target: progressbar;
                visible: true;
            }
            PropertyChanges {
                target: infinitebar;
                visible: false;
            }
        },
        State {
            name: "infinite";
            PropertyChanges {
                target: progressbar;
                visible: false;
            }
            PropertyChanges {
                target: infinitebar;
                visible: true;
            }
        }
    ]
}
