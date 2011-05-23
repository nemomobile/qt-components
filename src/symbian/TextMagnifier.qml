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
import "AppManager.js" as AppManager

Magnifier {
    id: root

    property Item editor: parent.editor
    // Defines the center of the content to be magnified in X axis
    // Note: the center of the content to be magnified in Y axis is determined by the cursor position.
    property variant contentCenter: Qt.point(0, 0) //editor's coordinate system

    function show() {
        parent = AppManager.rootObject();
        sourceRect = internal.calculateSourceGeometry();
        internal.calculatePosition();
        root.visible = true;
    }

    function hide() {
        root.visible = false;
    }

    sourceRect: Qt.rect(0, 0, 0, 0)
    visible: false
    scaleFactor: 1.5
    maskFileName: ":/graphics/qtg_graf_magnifier_mask.svg"
    overlayFileName: ":/graphics/qtg_graf_magnifier.svg"

    onContentCenterChanged: internal.updateSourceRect()

    onSourceRectChanged: if (visible) internal.calculatePosition()

    Connections {
        target: editor
        onCursorPositionChanged: internal.updateSourceRect
    }

    // Private
    QtObject {
        id: internal

        function updateSourceRect () {
            if (visible)
                sourceRect = internal.calculateSourceGeometry();
        }

        function calculatePosition() {
            width = sourceRect.width * scaleFactor;
            height = sourceRect.height * scaleFactor;

            var magnifierMargin = -platformStyle.paddingLarge; // the offset between the magnifier and top of the line
            var pos = parent.mapFromItem(editor,
                                         contentCenter.x - width/2,
                                         contentCenter.y - sourceRect.height/2 - height - magnifierMargin);

            root.x = pos.x;
            root.y = pos.y;
        }

        // Calculates and returns the source geometry of the content to be magnified in scene coordinates
        function calculateSourceGeometry() {
            var magniferSize = Qt.size(platformStyle.graphicSizeMedium * 2, platformStyle.graphicSizeMedium * 2);
            var sourceSize = Qt.size(magniferSize.width / scaleFactor, magniferSize.height / scaleFactor);
            var contentCenterScene = editor.mapToItem(null, contentCenter.x, contentCenter.y);

            var rect = Qt.rect(contentCenterScene.x - sourceSize.width / 2,
                               contentCenterScene.y - sourceSize.height / 2,
                               sourceSize.width, sourceSize.height);
            return rect;
        }
    }
}
