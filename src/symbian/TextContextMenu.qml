/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 1.1
import "." 1.1
import "AppManager.js" as AppManager

Item {
    id: root

    property Item editor: null
    property bool copyEnabled: false
    property bool cutEnabled: false
    property bool platformInverted: false

    function show() {
        // Show menu only if some of the options is available
        if(root.copyEnabled || root.cutEnabled || editor.canPaste) {
            parent = AppManager.rootObject();
            root.visible = true;
            calculatePosition();
        }
    }

    function hide() {
        root.visible = false;
    }

    function calculatePosition() {
        if (editor && root.visible) {
            var rectStart = editor.positionToRectangle(editor.selectionStart);
            var rectEnd = editor.positionToRectangle(editor.selectionEnd);

            var posStart = editor.mapToItem(parent, rectStart.x, rectStart.y);
            var posEnd = editor.mapToItem(parent, rectEnd.x, rectEnd.y);

            var selectionCenterX = (posEnd.x + posStart.x) / 2;
            if (posStart.y != posEnd.y)
                // we have multiline selection so center to the screen
                selectionCenterX = parent.width / 2;

            var contextMenuMargin = 10; // the space between the context menu and the line above/below
            var contextMenuAdjustedRowHeight = row.height + contextMenuMargin;
            var tempY = posStart.y - contextMenuAdjustedRowHeight;
            var minVerticalPos = internal.textArea ? internal.textArea.mapToItem(parent, 0, 0).y - row.height : 0
			
            if (tempY < minVerticalPos)
                tempY = minVerticalPos
				
            if (tempY < 0)
                tempY = 0

            root.x = Math.max(0, Math.min(selectionCenterX - row.width / 2, parent.width - row.width));
            root.y = tempY;
        }
    }

    onVisibleChanged: {
        if (visible) {
            internal.editorSceneXChanged.connect(internal.editorMoved)
            internal.editorSceneYChanged.connect(internal.editorMoved)
        } else {
            internal.editorSceneXChanged.disconnect(internal.editorMoved)
            internal.editorSceneYChanged.disconnect(internal.editorMoved)
        }
    }

    x: 0; y: 0
    visible: false

    Binding { target: internal; property: "editorSceneX"; value: AppManager.sceneX(root.editor); when: root.visible && (root.editor != null) }
    Binding { target: internal; property: "editorSceneY"; value: AppManager.sceneY(root.editor); when: root.visible && (root.editor != null) }

    QtObject {
        id: internal

        property real editorSceneX
        property real editorSceneY
        property Item textArea: null

        function editorMoved() {
            root.calculatePosition()
        }
		
        Component.onCompleted: textArea = AppManager.findParent(editor, "errorHighlight")
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

        onWidthChanged: calculatePosition()
        onHeightChanged: calculatePosition()

        Button {
            id: copyButton
            iconSource: privateStyle.imagePath("qtg_toolbar_copy", root.platformInverted)
            visible: root.copyEnabled
            platformInverted: root.platformInverted
            onClicked: editor.copy()
        }
        Button {
            id: cutButton
            iconSource: privateStyle.imagePath("qtg_toolbar_cut", root.platformInverted)
            visible: root.cutEnabled
            platformInverted: root.platformInverted
            onClicked: {
                editor.cut()
                root.visible = false
            }
        }
        Button {
            id: pasteButton
            iconSource: privateStyle.imagePath("qtg_toolbar_paste", root.platformInverted)
            visible: editor.canPaste
            platformInverted: root.platformInverted
            onClicked: {
                editor.paste()
                root.visible = false
            }
        }
    }
}
