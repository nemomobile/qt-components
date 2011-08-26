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

    onPressed: {
        if (!touchTools)
            touchToolsLoader.sourceComponent = touchToolsComponent

        internal.currentTouchPoint = root.mapToItem(editor, mouse.x, mouse.y);

        if (internal.currentTouchPoint.x < 0)
            internal.currentTouchPoint.x = 0

        if (internal.currentTouchPoint.y < 0)
            internal.currentTouchPoint.y = 0

        if (internal.tapCounter == 0)
            internal.touchPoint = internal.currentTouchPoint;

        editor.forceActiveFocus();
        touchTools.contextMenu.hide();
        internal.handleMoved = false;

        touchTools.handleBegin.viewPortRect = internal.mapViewPortRectToHandle(touchTools.handleBegin);
        touchTools.handleEnd.viewPortRect = internal.mapViewPortRectToHandle(touchTools.handleEnd);

        internal.pressedHandle = internal.handleForPoint({x: internal.currentTouchPoint.x, y: internal.currentTouchPoint.y});

        if (internal.pressedHandle != null) {
            internal.handleGrabbed();
            // Position cursor at the pressed selection handle
            // TODO: Add bug ID!!
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
        ++internal.tapCounter;
        if (internal.tapCounter == 1) {
            internal.onSingleTap();
            clickTimer.start();
        } else if (internal.tapCounter == 2 && clickTimer.running) {
            internal.onDoubleTap();
            clickTimer.restart();
        } else if (internal.tapCounter == 3 && clickTimer.running)
            internal.onTripleTap();
    }

    onPressAndHold: {
        clickTimer.stop();
        internal.tapCounter = 0;
        internal.longTap = true
        if (!internal.handleMoved) {
            if (internal.pressedHandle == null) {
                // position the cursor under the long tap and make the cursor handle grabbed
                editor.select(editor.cursorPosition, editor.cursorPosition);
                editor.cursorPosition = editor.positionAt(internal.touchPoint.x,internal.touchPoint.y);
                internal.pressedHandle = touchTools.handleEnd;
                if (editor.readOnly)
                    touchTools.magnifier.hide();
                internal.handleGrabbed();
            }
            touchTools.contextMenu.hide();
        }

        if (!editor.readOnly || internal.editorHasSelection)
            touchTools.magnifier.show();
    }

    onReleased: {
        touchTools.magnifier.hide();

        mouseGrabDisabler.setKeepMouseGrab(root, false);
        internal.forcedSelection = false;

        if ((internal.pressedHandle != null && internal.handleMoved) ||
           (internal.longTap && !editor.readOnly) ||
           (internal.pressedHandle != null && internal.longTap))
            touchTools.contextMenu.show();
        internal.longTap = false;
    }

    onPositionChanged: {

        internal.currentTouchPoint = root.mapToItem(editor, mouse.x, mouse.y);

        if (internal.pressedHandle != null) {
            internal.hitTestPoint = {x:internal.currentTouchPoint.x, y:internal.currentTouchPoint.y};

            var newPosition = editor.positionAt(internal.hitTestPoint.x, internal.hitTestPoint.y);
            if (newPosition >= 0 && newPosition != editor.cursorPosition) {
                if (internal.hasSelection) {
                    var anchorPos = internal.pressedHandle == touchTools.handleBegin ? editor.selectionEnd
                                                                                     : editor.selectionStart
                    if (editor.selectionStart == editor.cursorPosition)
                        anchorPos = editor.selectionEnd;
                    else if (editor.selectionEnd == editor.cursorPosition)
                        anchorPos = editor.selectionStart;
                    editor.select(anchorPos, newPosition);
                } else {
                    editor.cursorPosition = newPosition;
                }
                if (!editor.readOnly || internal.editorHasSelection)
                    touchTools.magnifier.show();
                internal.handleMoved = true;
            }
        }
    }

    Connections {
        target: editor
        onTextChanged: internal.onEditorTextChanged
    }

    // Private
    QtObject {
        id: internal

        property bool forcedSelection: false
        property bool hasSelection: editorHasSelection || forcedSelection
        property bool editorHasSelection: editor.selectionStart != editor.selectionEnd
        property bool handleMoved: false
        property bool longTap: false
        property int tapCounter: 0
        property variant pressedHandle: null
        property variant hitTestPoint: Qt.point(0, 0)
        property variant touchPoint: Qt.point(0, 0)
        property variant currentTouchPoint: Qt.point(0, 0)

        function onSingleTap() {
            if (!internal.handleMoved) {
                // need to deselect, because if the cursor position doesn't change the selection remains
                // even after setting to cursorPosition
                editor.deselect();
                editor.cursorPosition = editor.positionAt(internal.touchPoint.x, internal.touchPoint.y);
                touchTools.contextMenu.hide();
                if (!editor.readOnly)
                    editor.openSoftwareInputPanel()
            }
        }

        function onDoubleTap() {
            // assume that the 1st click positions the cursor
            editor.selectWord();
            touchTools.contextMenu.show();
        }

        function onTripleTap() {
            editor.selectAll();
            touchTools.contextMenu.show();
        }

        function onEditorTextChanged() {
            if (touchTools && !internal.editorHasSelection)
                touchTools.contextMenu.hide();
        }

        function handleGrabbed() {
            mouseGrabDisabler.setKeepMouseGrab(root, true);
            internal.hitTestPoint = {x:internal.currentTouchPoint.x, y:internal.currentTouchPoint.y};

            internal.forcedSelection = internal.editorHasSelection;
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
                    if (pressed != null) {
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

    Component {
        id: touchToolsComponent

        Item {
            id: touchTools
            property alias handleBegin: selBegin
            property alias handleEnd: selEnd
            property alias contextMenu: cxtMenu
            property alias magnifier: magnif

            TextSelectionHandle {
                id: selBegin

                objectName: "SelectionBegin"
                editor: root.editor
                imageSource: privateStyle.imagePath("qtg_fr_textfield_handle_start", root.platformInverted)
                editorPos: editor.selectionStart
                visible: editor.selectionStart != editor.selectionEnd
            }

            TextSelectionHandle { // also acts as the cursor handle when no selection
                id: selEnd

                objectName: "SelectionEnd"
                editor: root.editor
                imageSource: privateStyle.imagePath("qtg_fr_textfield_handle_end", root.platformInverted)
                editorPos: editor.selectionEnd
                visible: true
                showImage: internal.hasSelection //show image only in selection mode
            }

            TextContextMenu {
                id: cxtMenu

                editor: root.editor
                platformInverted: root.platformInverted
                copyEnabled: root.copyEnabled
                cutEnabled: root.cutEnabled
            }

            TextMagnifier {
                id: magnif

                editor: root.editor
                contentCenter: internal.hitTestPoint
                platformInverted: root.platformInverted

            }
        }

    }

    MouseGrabDisabler {
        id: mouseGrabDisabler
    }

    Timer {
        id: clickTimer

        interval: 400; repeat: false
        onTriggered: {
            running = false;
            internal.tapCounter = 0;
        }
    }

    Connections {
        target: root.editor
        onActiveFocusChanged: {
            // On focus loss dismiss menu, selection and VKB
            if (!root.editor.activeFocus) {
                if (touchTools)
                    touchTools.contextMenu.hide()
                root.editor.select(root.editor.cursorPosition, root.editor.cursorPosition)
                root.editor.closeSoftwareInputPanel()
            }
        }
    }

    Keys.onPressed: {
        if (!touchTools)
            touchToolsLoader.sourceComponent = touchToolsComponent

        if (internal.editorHasSelection && event.modifiers & Qt.ShiftModifier
            && (event.key == Qt.Key_Left || event.key == Qt.Key_Right
            || event.key == Qt.Key_Up || event.key == Qt.Key_Down))
            touchTools.contextMenu.show()
    }
}
