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
import "RectUtils.js" as Utils


MouseArea {
    id: root

    property Item editor: parent
    property alias touchTools: touchToolsLoader.item
    property real editorScrolledX: 0
    property real editorScrolledY: 0
    property bool copyEnabled: false
    property bool cutEnabled: false
    property bool selectAllEnabled: false
    property bool selectWordEnabled: false
    property bool platformInverted: false

    enabled: !editor.inputMethodComposing

    LayoutMirroring.enabled: false
    LayoutMirroring.childrenInherit: true

    function updateGeometry() {
        if (touchTools) {
            touchTools.handleBegin.updateGeometry();
            touchTools.handleEnd.updateGeometry();
            touchTools.contextMenu.calculatePosition(); // Update context menu position
        }
    }

    function flickEnded() {
        if (internal.editorHasSelection && internal.selectionVisible())
            touchTools.contextMenu.show();
        else
            touchTools.contextMenu.hide();
    }

    function ensureTouchToolsOnTop() {
        // ensure that the touch tools (text context menu and magnigier glass) are always on top
        var p = parent
        while ( p != null ) {
            if (p.z == touchTools.contextMenu.z) {
                touchToolsLoader.source = ""
                touchToolsLoader.source = "TextTouchTools.qml"
                break
            }
            p = p.parent
        }
    }

    onPressed: {
        if (!touchTools)
            touchToolsLoader.source = "TextTouchTools.qml"

        internal.currentTouchPoint = root.mapToItem(editor, mouse.x, mouse.y);
        internal.initialTouchPoint = internal.currentTouchPoint;
        var hadActiveFocus = editor.activeFocus
        editor.forceActiveFocus();
        touchTools.contextMenu.hide();
        internal.handleMoved = false;
        internal.selectionPainting = false;
        privateStyle.play(Symbian.Editor);

        touchTools.handleBegin.viewPortRect = internal.mapViewPortRectToHandle(touchTools.handleBegin);
        touchTools.handleEnd.viewPortRect = internal.mapViewPortRectToHandle(touchTools.handleEnd);

        if (hadActiveFocus && internal.hasSelection)
            internal.pressedHandle = internal.handleForPoint({x: internal.currentTouchPoint.x, y: internal.currentTouchPoint.y});
        else
            internal.pressedHandle = null

        if (internal.pressedHandle) {
            internal.handleGrabbed();
            var tempStart = editor.selectionStart
            var tempEnd = editor.selectionEnd
            if (internal.pressedHandle == touchTools.handleBegin) {
                editor.cursorPosition = editor.selectionStart
                editor.select(tempEnd, tempStart);
            } else {
                editor.cursorPosition = editor.selectionEnd
                editor.select(tempStart, tempEnd);
            }
        }
    }

    onClicked: {
        if (!internal.handleMoved) {
            editor.deselect();
            editor.cursorPosition = editor.positionAt(internal.initialTouchPoint.x, internal.initialTouchPoint.y);
            touchTools.contextMenu.hide();

            if (!editor.readOnly)
                editor.openSoftwareInputPanel()
        }
    }

    onDoubleClicked: {
        // assume that the 1st click positions the cursor
        editor.selectWord();
        touchTools.contextMenu.show();
    }

    onPressAndHold: {
        if (!internal.handleMoved) {
            internal.longTap = true
            // position the cursor under the long tap and make the cursor handle grabbed
            editor.deselect();
            editor.cursorPosition = editor.positionAt(internal.initialTouchPoint.x,internal.initialTouchPoint.y);
            if (!editor.readOnly) {
                internal.pressedHandle = touchTools.handleEnd;
                internal.handleGrabbed();
                touchTools.contextMenu.hide();
                touchTools.magnifier.show();
            } else {
                privateStyle.play(Symbian.PopUp);
                touchTools.contextMenu.show();
            }
        }
    }

    onReleased: {
        touchTools.magnifier.hide()
        internal.handleGrabEnded()

        if ((internal.longTap && !editor.readOnly) || internal.hasSelection) {
            privateStyle.play(Symbian.PopUp);
            touchTools.contextMenu.show();
        }
        internal.longTap = false;
    }

    onPositionChanged: {

        internal.currentTouchPoint = root.mapToItem(editor, mouse.x, mouse.y);
        var newPosition = editor.positionAt(internal.currentTouchPoint.x, internal.currentTouchPoint.y);

        // Cursor or selection handle is grabbed
        if (internal.pressedHandle) {
            if (newPosition >= 0 && newPosition != editor.cursorPosition) {
                if (internal.hasSelection)
                    editor.moveCursorSelection(newPosition, TextEdit.SelectCharacters)
                else
                    editor.cursorPosition = newPosition;
                touchTools.contextMenu.hide();
                touchTools.magnifier.show();
                internal.handleMoved = true;
                privateStyle.play(Symbian.TextSelection);
            }
        }
        // Update painted selection
        else if (internal.selectionPainting) {
            if (editor.cursorPosition != newPosition) privateStyle.play(Symbian.TextSelection)
            editor.moveCursorSelection(newPosition, TextEdit.SelectWords)
        }
        // Begin selection painting
        else if (Math.abs(internal.currentTouchPoint.x - internal.initialTouchPoint.x) > privateStyle.startDragDistance()) {
            editor.deselect();
            editor.cursorPosition = editor.positionAt(internal.initialTouchPoint.x, internal.initialTouchPoint.y);
            internal.handleGrabbed()
            internal.selectionPainting = true
            internal.handleMoved = true
            editor.moveCursorSelection(newPosition, TextEdit.SelectWords)
            privateStyle.play(Symbian.TextSelection);
            touchTools.contextMenu.hide();
            touchTools.magnifier.show();
        }
    }

    Connections {
        target: editor
        onTextChanged: internal.onEditorTextChanged()
        onCursorPositionChanged: {
            if (touchTools) {
                if (!editor.selectedText)
                    touchTools.contextMenu.hide()
                touchTools.contextMenu.calculatePosition()
            }
        }
        onVisibleChanged: {
            // check that touch tools are on top of the editor
            if (touchTools && editor.visible)
                ensureTouchToolsOnTop()
        }
    }

    // Private
    QtObject {
        id: internal

        property bool forcedSelection: false
        property bool hasSelection: editorHasSelection || forcedSelection
        property bool editorHasSelection: editor.selectionStart != editor.selectionEnd
        property bool handleMoved: false
        property bool longTap: false
        property bool selectionPainting: false
        property variant pressedHandle: null
        property variant initialTouchPoint: Qt.point(0, 0)
        property variant currentTouchPoint: Qt.point(0, 0)

        function onEditorTextChanged() {
            if (touchTools)
                touchTools.contextMenu.hide();
        }

        function handleGrabbed() {
            mouseGrabDisabler.setKeepMouseGrab(root, true);
            internal.forcedSelection = internal.editorHasSelection;
        }

        function handleGrabEnded() {
            mouseGrabDisabler.setKeepMouseGrab(root, false);
            internal.forcedSelection = false;
        }

        function mapViewPortRectToHandle(handle) {
            var position = editor.mapToItem(handle, root.editorScrolledX, root.editorScrolledY);
            var rect = Qt.rect(position.x, position.y, root.width, root.height);
            return rect;
        }

        // point is in Editor's coordinate system
        function handleForPoint(point) {
            var pressed = null;

            if (!editor.readOnly || editorHasSelection) { // to avoid moving the cursor handle in read only editor
                // Find out which handle is being moved
                if (touchTools.handleBegin.visible &&
                    touchTools.handleBegin.containsPoint(editor.mapToItem(touchTools.handleBegin, point.x, point.y))) {
                    pressed = touchTools.handleBegin;
                }
                if (touchTools.handleEnd.containsPoint(editor.mapToItem(touchTools.handleEnd, point.x, point.y))) {
                    var useArea = true;
                    if (pressed) {
                        var distance1 = touchTools.handleBegin.pointDistanceFromCenter(point);
                        var distance2 = touchTools.handleEnd.pointDistanceFromCenter(point);

                        if (distance1 < distance2)
                            useArea = false;
                    }
                    if (useArea)
                        pressed = touchTools.handleEnd;
                }
            }
            return pressed;
        }

        function selectionVisible() {
            var startRect = editor.positionToRectangle(editor.selectionStart);
            var endRect = editor.positionToRectangle(editor.selectionEnd);
            var selectionRect = Qt.rect(startRect.x, startRect.y, endRect.x - startRect.x + endRect.width, endRect.y - startRect.y + endRect.height);
            var viewPortRect = Qt.rect(editorScrolledX, editorScrolledY, editor.width, editor.height);

            return Utils.rectIntersectsRect(selectionRect, viewPortRect) ||
                   Utils.rectContainsRect(viewPortRect, selectionRect) ||
                   Utils.rectContainsRect(selectionRect, viewPortRect);
        }
    }

    Loader {
        id: touchToolsLoader
    }

    MouseGrabDisabler {
        id: mouseGrabDisabler
    }

    Connections {
        target: root.editor
        onActiveFocusChanged: {
            // On focus loss dismiss menu, selection and VKB
            if (!root.editor.activeFocus) {
                if (touchTools)
                    touchTools.contextMenu.hide()
                root.editor.deselect()
                root.editor.closeSoftwareInputPanel()
            }
        }
    }

    Keys.onPressed: {
        if (!touchTools)
            touchToolsLoader.source = "TextTouchTools.qml"

        if (event.key == Qt.Key_Left || event.key == Qt.Key_Right
            || event.key == Qt.Key_Up || event.key == Qt.Key_Down) {
            if (event.modifiers & Qt.ShiftModifier)
                touchTools.contextMenu.show()
            else if (internal.editorHasSelection)
                touchTools.contextMenu.hide()
        } else if (event.key == Qt.Key_Escape) {
            editor.focus = false
        }
    }
}
