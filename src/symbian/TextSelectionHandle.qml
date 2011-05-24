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
import "RectUtils.js" as Utils

Item {
    id: root

    property alias imageSource: image.source
    property int editorPos: 0 //character position of the handle in the editor document
    property Item editor: parent.editor
    // the default size of the touch area should come from some style constant
    property variant touchAreaSize: Qt.size(platformStyle.graphicSizeMedium, platformStyle.graphicSizeMedium)
    property alias showImage: image.visible
    property bool showTouchArea: false // for debugging purposes
    property variant viewPortRect: Qt.rect(0, 0, 0, 0) // item's coordinates
    //parent's coordinate system
    property variant center: Qt.point(root.x + root.width / 2, root.y + root.height / 2)

    // Point is in Item's coordinate system
    function containsPoint(point) {
        var touchAreaRect = Qt.rect(touchArea.x, touchArea.y, touchArea.width, touchArea.height);

        if (!Utils.rectContainsRect(viewPortRect, touchAreaRect)
            && Utils.rectIntersectsRect(viewPortRect, touchAreaRect)) {
                touchAreaRect.x = Math.max(touchAreaRect.x, viewPortRect.x);
                touchAreaRect.x = Math.min(touchAreaRect.x, viewPortRect.x + viewPortRect.width - touchAreaRect.width);
                touchAreaRect.y = Math.max(touchAreaRect.y, viewPortRect.y);
                touchAreaRect.y = Math.min(touchAreaRect.y, viewPortRect.y + viewPortRect.height - touchAreaRect.height);
        }

        effectiveTouchArea.x = touchAreaRect.x;
        effectiveTouchArea.y = touchAreaRect.y;

        if (Utils.rectContainsPoint(touchAreaRect, point.x, point.y))
            return true;

        return false;
    }

    // Point is in parent's coordinate system
    function pointDistanceFromCenter(point) {
        return Utils.manhattan(point, root.center);
    }

    function updateGeometry() {
        var geometry = internal.geometryFromEditorPos(root.editorPos);
        root.x = geometry.x;
        root.y = geometry.y;
        root.width = geometry.width;
        root.height = geometry.height;
    }

    onEditorPosChanged: root.updateGeometry()

    // Private
    QtObject {
        id: internal

        // Returns handle geometry from editor position
        function geometryFromEditorPos(editorPos) {
            var rect = root.editor.positionToRectangle(editorPos);
            var pos = root.editor.mapToItem(root.parent,rect.x,rect.y);
            return Qt.rect(pos.x,pos.y,rect.width,rect.height);
        }
    }

    // This could be an Item, but it’s a Rectangle for debugging purposes.
    Rectangle {
        id: touchArea

        visible: root.showTouchArea
        anchors.centerIn: root
        width: root.touchAreaSize.width
        height: Math.max(root.touchAreaSize.height, root.height)
        border.width: 1
        border.color: "white"
        color: "#00000000"
    }

    // This could be an Item, but it’s a Rectangle for debugging purposes.
    Rectangle {
        id: effectiveTouchArea

        visible: root.showTouchArea
        width: root.touchAreaSize.width
        height: Math.max(root.touchAreaSize.height, root.height)
        border.width: 1
        border.color: "yellow"
        color: "#00000000"
    }

    BorderImage {
        id: image

        property real tiny: Math.round(platformStyle.graphicSizeTiny / 2)

        anchors {
            left: root.objectName == "SelectionEnd" ? root.right : undefined
            right: root.objectName == "SelectionBegin" ? root.left : undefined
            leftMargin: root.objectName == "SelectionEnd" ? -editor.cursorRectangle.width : 0
            top: root.top
            topMargin: -tiny / 2 + 1
            bottom: root.bottom
            bottomMargin: -tiny / 2
        }

        border {
            left: 0
            top: tiny
            right: 0
            bottom: tiny
        }
    }
}
