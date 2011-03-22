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
    id: scrollBar

    property Flickable flickableItem: null
    property int orientation: Qt.Vertical
    property bool interactive: true
    property int policy: Symbian.ScrollBarWhenScrolling
    property real position: internal.getPosition()
    property real pageSize: internal.getPageSize()

    //implicit values for qml designer
    implicitHeight: orientation == Qt.Vertical ? 3 * style.current.get("minHandleSize") : style.current.get("thickness")
    implicitWidth: orientation == Qt.Horizontal ? 3 * style.current.get("minHandleSize") : style.current.get("thickness")
    height: internal.getHeight()
    width: internal.getWidth()

    //For showing explicitly a ScrollBar if policy is Symbian.ScrollBarWhenScrolling
    function flash() {
        if (!flashTimer.running)
            flashTimer.running = true
    }

    Style { id: style; styleClass: "ScrollBar"; mode: interactive ? "interactive" : "indicative" }
    Style { id: handleStyle; styleClass: "ScrollBar"; mode: "default" }

    Timer {
        id: flashTimer
        interval: 100
        running: true
        onTriggered: {
            if (policy == Symbian.ScrollBarWhenScrolling) {
                scrollBar.state = "OnlyWhenScrolling"
                scrollBar.state = "Hidden"
            }
        }
    }

    QtObject {
        id: internal
        //Trigger for move the scrollBar handles when using keys
        property bool keyNavigationTrigger: false
        //internal styling values
        property real handleOpacity: 0.7
        //A light, semi-transparent background
        property real trackOpacity: 0
        property real handleTouchAreaOpacity: 0

        function getHeight() {
            if (flickableItem == null)
                return undefined
            if (orientation == Qt.Vertical) {
                if (anchors.topMargin > 0 || anchors.bottomMargin > 0)
                    return flickableItem.height - style.current.get("thickness")
                else
                    return flickableItem.height
            } else if (orientation == Qt.Horizontal) {
                return style.current.get("thickness")
            }
        }

        function getWidth() {
            if (flickableItem == null)
                return undefined
            if (orientation == Qt.Vertical) {
                return style.current.get("thickness")
            } else if (orientation == Qt.Horizontal) {
                if (anchors.rightMargin > 0 || anchors.leftMargin > 0)
                    return flickableItem.width - style.current.get("thickness")
                else
                    return flickableItem.width
            }
        }

        function getPageSize() {
            if (flickableItem == null)
                return 0//cannot return undefined as a double in qml
            if (orientation == Qt.Vertical)
                return flickableItem.visibleArea.heightRatio
            else
                return flickableItem.visibleArea.widthRatio
        }

        function getPosition() {
            if (flickableItem == null)
                return 0//cannot return undefined as a double in qml
            if (orientation == Qt.Vertical)
                return flickableItem.visibleArea.yPosition
            else
                return flickableItem.visibleArea.xPosition
        }
    }

    onPositionChanged: {
        if (policy == Symbian.ScrollBarWhenScrolling) {
            // Trigger for OnlyWhenScrolling state
            internal.keyNavigationTrigger =! internal.keyNavigationTrigger
            // Trigger back to original value
            internal.keyNavigationTrigger =! internal.keyNavigationTrigger
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Up) {
            if (flickableItem.contentY >= style.current.get("keyEventSize"))
                flickableItem.contentY -= style.current.get("keyEventSize")
            else
                flickableItem.contentY = 0
            event.accepted = false
        }
        else if (event.key === Qt.Key_Down) {
            if (flickableItem.contentY <= flickableItem.contentHeight - flickableItem.parent.height - style.current.get("keyEventSize"))
                flickableItem.contentY += style.current.get("keyEventSize")
            else
                flickableItem.contentY = flickableItem.contentHeight - flickableItem.parent.height
            event.accepted = false
        }
        else if (event.key === Qt.Key_Left) {
            if (flickableItem.contentX >= style.current.get("keyEventSize"))
                flickableItem.contentX -= style.current.get("keyEventSize")
            else
                flickableItem.contentX = 0
            event.accepted = false
        }
        else if (event.key === Qt.Key_Right) {
            if (flickableItem.contentX <= flickableItem.contentWidth - flickableItem.parent.width - style.current.get("keyEventSize"))
                flickableItem.contentX += style.current.get("keyEventSize")
            else
                flickableItem.contentX = flickableItem.contentWidth - flickableItem.parent.width
            event.accepted = false
        }
        else {
            event.accepted = false
        }
    }

    Rectangle {
        id: track
        //to support component identification in tdriver tests
        objectName: "track"
        anchors.fill: flickableItem != null ? scrollBar : undefined
        radius: orientation == Qt.Vertical ? width / 2 - 2 : height / 2 - 2
        color: style.current.get("trackColor")
        opacity: internal.trackOpacity
    }

    Rectangle {
        id: handle
        //to support component identification in tdriver tests
        objectName: "handle"
        x: orientation == Qt.Vertical ? 1 : Math.min(scrollBar.width - handle.width, Math.max(0, scrollBar.position * scrollBar.width))
        y: orientation == Qt.Vertical ? Math.min(scrollBar.height - handle.height, Math.max(0, scrollBar.position * scrollBar.height)) : 1
        width: {
            if (orientation == Qt.Vertical)
                return scrollBar.width - 2
            else
                return Math.max(style.current.get("minHandleSize"), scrollBar.pageSize * scrollBar.width)
        }
        height: {
            if (orientation == Qt.Horizontal)
                return scrollBar.height - 2
            else
                return Math.max(style.current.get("minHandleSize"), scrollBar.pageSize * scrollBar.height)
        }
        radius: orientation == Qt.Vertical ? width / 2 - 2 : height / 2 - 2
        color: handleStyle.current.get("handleColor")
        opacity: internal.handleOpacity
    }

    Rectangle {
        id: handleTouchArea
        //to support component identification in tdriver tests
        objectName: "handleTouchArea"
        x: handle.x
        y: handle.y
        anchors {
            bottom: flickableItem != null && orientation == Qt.Horizontal ? handle.bottom : undefined
            right: flickableItem != null && orientation == Qt.Vertical ? handle.right : undefined
        }
        width: orientation == Qt.Vertical ? style.current.get("touchAreaThickness") - scrollBar.width : handle.width
        height: orientation == Qt.Horizontal ? style.current.get("touchAreaThickness") - scrollBar.height : handle.height
        opacity: internal.handleTouchAreaOpacity
    }

    // MouseArea for the move content "page by page" by tapping
    MouseArea {
        id: trackMouseArea
        //to support component identification in tdriver tests
        objectName: "trackMouseArea"
        enabled: interactive
        anchors.fill: flickableItem != null ? track : undefined

        onPressed: {
            if (orientation == Qt.Vertical) {
                if (mouseY > handle.y) {
                    flickableItem.contentY += flickableItem.visibleArea.heightRatio * flickableItem.contentHeight
                    flickableItem.contentY = Math.min(flickableItem.contentHeight - flickableItem.height, flickableItem.contentY)
                }
                else if (mouseY < handle.y) {
                    flickableItem.contentY -= flickableItem.visibleArea.heightRatio * flickableItem.contentHeight
                    flickableItem.contentY = Math.max(0, flickableItem.contentY)
                }
            }
            else if (orientation == Qt.Horizontal) {
                if (mouseX > handle.x) {
                    flickableItem.contentX += flickableItem.visibleArea.widthRatio * flickableItem.contentWidth
                    flickableItem.contentX = Math.min(flickableItem.contentWidth - flickableItem.width, flickableItem.contentX)
                }
                else if (mouseX < handle.x) {
                    flickableItem.contentX -= flickableItem.visibleArea.widthRatio * flickableItem.contentWidth
                    flickableItem.contentX = Math.max(0, flickableItem.contentX)
                }
            }
        }
    }

    MouseArea {
        id: handleMouseArea
        //to support component identification in tdriver tests
        objectName: "handleMouseArea"
        enabled: interactive

        anchors {
            fill: flickableItem != null ? handleTouchArea : undefined
            verticalCenter: flickableItem != null ? handle.verticalCenter : undefined
            horizontalCenter: flickableItem != null ? handle.horizontalCenter : undefined
        }

        drag {
            target: handle
            axis: orientation == Qt.Vertical ? Drag.YAxis : Drag.XAxis
            minimumY: 0
            maximumY: scrollBar.height - handle.height
            minimumX: 0
            maximumX: scrollBar.width - handle.width
        }

        onPressed: {
            handleStyle.mode = "pressed"
        }
        onReleased: {
            handleStyle.mode = "default"
        }
        onPositionChanged: {
            if (orientation == Qt.Vertical)
                flickableItem.contentY = handle.y / scrollBar.height * flickableItem.contentHeight
            else if (orientation == Qt.Horizontal)
                flickableItem.contentX = handle.x / scrollBar.width * flickableItem.contentWidth
        }
        Component.onCompleted: {
            if (flickableItem != null && orientation == Qt.Vertical)
                flickableItem.contentY = Math.max(0, handle.y / scrollBar.height * flickableItem.contentHeight)
            else if (flickableItem != null && orientation == Qt.Horizontal)
                flickableItem.contentX = Math.max(0, handle.x / scrollBar.width * flickableItem.contentWidth)
        }
    }

    states: [
        State {
            name: "OnlyWhenScrolling"
            when: (flickableItem.moving || internal.keyNavigationTrigger) && policy == Symbian.ScrollBarWhenScrolling && ((orientation == Qt.Vertical && flickableItem.contentHeight > scrollBar.height) ||
                   (orientation == Qt.Horizontal && flickableItem.contentWidth > scrollBar.width))
            PropertyChanges { target: scrollBar; opacity: 1 }
        },
        State {
            name: "Hidden"
            when: policy == Symbian.ScrollBarWhenScrolling || policy == Symbian.ScrollBarWhenNeeded && ((orientation == Qt.Vertical && flickableItem.contentHeight < scrollBar.height) ||
                    (orientation == Qt.Horizontal && flickableItem.contentWidth < scrollBar.width))
            PropertyChanges { target: scrollBar; opacity: 0 }
        },
        State {
            name: "OnlyWhenNeeded"
            when: policy == Symbian.ScrollBarWhenNeeded && (orientation == Qt.Vertical && flickableItem.contentHeight > scrollBar.height) ||
                    (orientation == Qt.Horizontal && flickableItem.contentWidth > scrollBar.width)
            PropertyChanges { target: scrollBar; opacity: 1 }
        }
    ]
    transitions: [
        Transition {
            from: "OnlyWhenNeeded"
            to: "OnlyWhenScrolling"
            NumberAnimation {
                properties: "opacity"
                duration: style.current.get("hideTimeout")
            }
        },
        Transition {
            from: "OnlyWhenScrolling"
            to: "Hidden"
            NumberAnimation {
                properties: "opacity"
                duration: style.current.get("hideTimeout")
            }
        }
    ]
}
