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

Item {
    id: scrollview;

    width: 400;
    height: 400;

    property bool vertical: true;
    property int maxstride: 3;
    property int cellWidth: 90;
    property int cellHeight: 35;
    property int count: 200;

    function ensureVisible(index) {
        grid.positionViewAtIndex(index, GridView.Contain);
    }

    GridView {
        id: grid;
        width: rightBar.x;
        height: bottomBar.y;
        cellWidth: parent.cellWidth;
        cellHeight: parent.cellHeight;
        boundsBehavior: "StopAtBounds";

        clip: true;
        contentX: bottomBar.value
        contentY: rightBar.value

        delegate: Button {
            width: scrollview.cellWidth;
            height: scrollview.cellHeight;
            text: "Button " + (index + 1);
            tooltipText: "test";
            onClicked: { scrollview.vertical = !scrollview.vertical }
        }

        model: scrollview.count;
    }

    ScrollBar {
        id: rightBar;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.right;

        minimum: 0
        maximum: Math.max(0, (grid.contentHeight - grid.height))
        vertical: true;
        visible: false;
    }

    ScrollBar {
        id: bottomBar;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.bottom;

        minimum: 0
        maximum: Math.max(0, (grid.contentWidth - grid.width))
        visible: false;
    }

    states: [
        State {
            name: "horizontal";
            when: !scrollview.vertical;
            PropertyChanges {
                target: grid;
                height: Math.min(bottomBar.y, scrollview.maxstride * scrollview.cellHeight);
                flow: GridView.TopToBottom;
            }
            PropertyChanges {
                target: bottomBar;
                anchors.topMargin: -height;
                visible: true;
            }
        },
        State {
            name: "vertical";
            when: scrollview.vertical;
            PropertyChanges {
                target: grid;
                width: Math.min(rightBar.x, scrollview.maxstride * scrollview.cellWidth);
                flow: GridView.LeftToRight;
            }
            PropertyChanges {
                target: rightBar;
                anchors.leftMargin: -width;
                visible: true;
            }
        }
    ]
}
