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
import com.meego.themebridge 1.0

Item {
    id: container
    property alias __flickable: container.parent
    anchors.fill: __flickable

    Style {
        id: meegostyle
        styleClass: "MPositionIndicatorStyle"
    }

    Timer {
        // Hack to have the indicators flash when the view is shown the first time.
        // Ideally we would wait until the Flickable is complete, but it doesn't look
        // possible given the current limitations of QML.
        interval: 100
        running: true
        repeat: false
        onTriggered: {
            if (verticalIndicator.shouldShow) {
                verticalIndicator.state = "visible";
                verticalIndicator.state = "";
            }
            if (horizontalIndicator.shouldShow) {
                horizontalIndicator.state = "visible";
                horizontalIndicator.state = "";
            }
        }
    }

    Item {
        id: verticalIndicator
        property bool shouldShow: __flickable.height > 0 && __flickable.contentHeight > __flickable.height
        opacity: 0
        height: parent.height
        anchors.right: parent.right

        ScalableImage {
            style: meegostyle
            imageProperty: "backgroundImage"
            height: parent.height
            anchors.right: parent.right
        }
        ScalableImage {
            style: meegostyle
            imageProperty: "indicatorImage"
            y: __flickable.visibleArea.yPosition * container.height
            height: Math.max(meegostyle.current.get("minIndicatorSize"), __flickable.visibleArea.heightRatio * container.height)
            anchors.right: parent.right
        }

        states: State {
            name: "visible"
            when: verticalIndicator.shouldShow && __flickable.moving
            PropertyChanges {
                target: verticalIndicator
                opacity: 1
            }
        }

        transitions: Transition {
            from: "visible"; to: ""
            NumberAnimation {
                properties: "opacity"
                duration: meegostyle.current.get("hideTimeout")
            }
        }
    }

    Item {
        id: horizontalIndicator
        property bool shouldShow: __flickable.width > 0 && __flickable.contentWidth > __flickable.width
        opacity: 0
        width: parent.width
        anchors.bottom: parent.bottom

        // TODO With some themes, these should rotate the scalable images.
        ScalableImage {
            style: meegostyle
            imageProperty: "backgroundImage"
            width: parent.width
            anchors.bottom: parent.bottom
        }
        ScalableImage {
            style: meegostyle
            imageProperty: "indicatorImage"
            x: __flickable.visibleArea.xPosition * container.width
            width: Math.max(meegostyle.current.get("minIndicatorSize"), __flickable.visibleArea.widthRatio * container.width)
            anchors.bottom: parent.bottom
        }

        states: State {
            name: "visible"
            when: horizontalIndicator.shouldShow && __flickable.moving
            PropertyChanges {
                target: horizontalIndicator
                opacity: 1
            }
        }

        transitions: Transition {
            from: "visible"; to: ""
            NumberAnimation {
                properties: "opacity"
                duration: meegostyle.current.get("hideTimeout")
            }
        }
    }
}
