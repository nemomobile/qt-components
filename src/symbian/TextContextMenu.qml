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
import "AppManager.js" as AppManager

Item {
    id: root

    property Item editor: null
    property real editorScrolledY: 0
    property bool copyEnabled: false
    property bool pasteEnabled: false
    property bool cutEnabled: false

    function show() {
        parent = AppManager.rootObject();
        internal.calculatePosition();
        root.visible = true;
    }

    function hide() {
        root.visible = false;
    }

    x: 0; y: 0
    visible: false

    // Private
    QtObject {
        id: internal
        // Reposition the context menu so that it centers on top of the selection
        function calculatePosition() {
            if (editor) {
                var rectStart = editor.positionToRectangle(editor.selectionStart);
                var rectEnd = editor.positionToRectangle(editor.selectionEnd);

                var posStart = editor.mapToItem(parent, rectStart.x, rectStart.y);
                var posEnd = editor.mapToItem(parent, rectEnd.x, rectEnd.y);

                var selectionCenterX = (posEnd.x + posStart.x) / 2;
                if (posStart.y != posEnd.y)
                    // we have multiline selection so center to the screen
                    selectionCenterX = parent.width / 2;

                var editorScrolledParent = editor.mapToItem(parent, 0, editorScrolledY);
                var contextMenuMargin = 10; // the space between the context menu and the line above/below
                var contextMenuAdjustedRowHeight = row.height + contextMenuMargin;

                var tempY = Math.max(editorScrolledParent.y - contextMenuAdjustedRowHeight,
                                     posStart.y - contextMenuAdjustedRowHeight);
                if (tempY < 0)
                    // it doesn't fit to the top -> try bottom
                    tempY = Math.min(editorScrolledParent.y + editor.height + contextMenuMargin,
                                     posEnd.y + rectEnd.height + contextMenuMargin);

                if (tempY + contextMenuAdjustedRowHeight > parent.height)
                    //it doesn't fit to the bottom -> center
                    tempY= (editorScrolledParent.y + editor.height) / 2 - row.height / 2;

                root.x = Math.max(0, Math.min(selectionCenterX - row.width / 2, parent.width - row.width));
                root.y = tempY;
            }
        }
    }

    ButtonRow {
        id: row

        function visibleButtonCount() {
            var count = 0
            if (copyButton.visible) ++count
            if (cutButton.visible) ++count
            if (pasteButton.visible) ++count
            return count
        }

        exclusive: false
        width: Math.round(privateStyle.buttonSize * 1.5) * visibleButtonCount()

        onWidthChanged: internal.calculatePosition()
        onHeightChanged: internal.calculatePosition()

        Button {
            id: copyButton
            iconSource: privateStyle.imagePath("qtg_toolbar_copy")
            visible: root.copyEnabled
            onClicked: editor.copy()
        }
        Button {
            id: cutButton
            iconSource: privateStyle.imagePath("qtg_toolbar_cut")
            visible: root.cutEnabled
            onClicked: {
                editor.cut()
                root.visible = false
            }
        }
        Button {
            id: pasteButton
            iconSource: privateStyle.imagePath("qtg_toolbar_paste")
            visible: root.pasteEnabled
            onClicked: {
                editor.paste()
                root.visible = false
            }
        }
    }
}
