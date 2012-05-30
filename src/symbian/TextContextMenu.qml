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
    property bool selectAllEnabled: false
    property bool selectWordEnabled: false
    property bool platformInverted: false

    x: 0
    y: 0
    z: Infinity
    visible: false

    function show() {
        if (row.enabledButtonCount() > 0) {
            parent = AppManager.visualRoot();
            internal.show = true;
            calculatePosition();
        }
    }

    function hide() {
        internal.show = false;
    }

    function calculatePosition() {
        if (editor && internal.show) {
            var rectStart = editor.positionToRectangle(editor.selectionStart);
            var rectEnd = editor.positionToRectangle(editor.selectionEnd);

            var posStart = editor.mapToItem(parent, rectStart.x, rectStart.y);
            var posEnd = editor.mapToItem(parent, rectEnd.x, rectEnd.y);

            var selectionCenterX = (posEnd.x + posStart.x) / 2;
            if (posStart.y != posEnd.y)
                // we have multiline selection so center to the screen
                selectionCenterX = parent.width / 2;

            var minVerticalPos = internal.textArea ? internal.textArea.mapToItem(parent, 0, 0).y + platformStyle.paddingLarge : 0

            root.x = Math.max(0, Math.min(selectionCenterX - row.width / 2, parent.width - row.width));
            root.y = Math.max(0, Math.max(minVerticalPos, posStart.y) - row.height - platformStyle.paddingLarge * 2);
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

    Binding { target: internal; property: "editorSceneX"; value: AppManager.sceneX(root.editor); when: root.visible && (root.editor != null) }
    Binding { target: internal; property: "editorSceneY"; value: AppManager.sceneY(root.editor); when: root.visible && (root.editor != null) }

    QtObject {
        id: internal

        property real editorSceneX
        property real editorSceneY
        property Item textArea: null
        property bool show: false
        property int animationDuration: 250
        function editorMoved() {
            root.calculatePosition()
        }
		
        Component.onCompleted: textArea = AppManager.findParent(editor, "errorHighlight")
    }

    BorderImage {
        id: dropShadow

        property int borderSize: Math.round(sourceSize.width / 2 - 1)
        property int paddingSize: sourceSize.width - privateStyle.buttonSize

        anchors { top: row.top; horizontalCenter: row.horizontalCenter }
        width: row.width + paddingSize
        height: row.height + paddingSize
        border { left: borderSize; top: borderSize; right: borderSize; bottom: borderSize }
        source: privateStyle.imagePath("qtg_fr_pushbutton_segmented_shadow", false)
        scale: row.scale
    }

    ButtonRow {
        id: row

        function visibleButtonCount() {
            var count = 0
            if (selectButton.visible) ++count
            if (selectAllButton.visible) ++count
            if (copyButton.visible) ++count
            if (cutButton.visible) ++count
            if (pasteButton.visible) ++count
            return count
        }

        function enabledButtonCount() {
            var count = 0
            if (root.selectWordEnabled) ++count
            if (root.selectAllEnabled) ++count
            if (root.copyEnabled) ++count
            if (root.cutEnabled) ++count
            if (editor.canPaste) ++count
            return count
        }

        exclusive: false
        width: Math.round(privateStyle.buttonSize * 2) * visibleButtonCount()

        onWidthChanged: calculatePosition()
        onHeightChanged: calculatePosition()

        Button {
            id: selectButton
            iconSource: privateStyle.imagePath("qtg_toolbar_select_word", root.platformInverted)
            visible: root.selectWordEnabled
            platformInverted: root.platformInverted
            onClicked: {
                editor.selectWord()
                root.show()
            }
        }
        Button {
            id: selectAllButton
            iconSource: privateStyle.imagePath("qtg_toolbar_select_all_text", root.platformInverted)
            visible: root.selectAllEnabled
            platformInverted: root.platformInverted
            onClicked: {
                editor.selectAll()
                root.show()
            }
        }
        Button {
            id: copyButton
            iconSource: privateStyle.imagePath("qtg_toolbar_copy", root.platformInverted)
            visible: root.copyEnabled
            platformInverted: root.platformInverted
            onClicked: {
                editor.copy()
                root.hide()
            }
        }
        Button {
            id: cutButton
            iconSource: privateStyle.imagePath("qtg_toolbar_cut", root.platformInverted)
            visible: root.cutEnabled
            platformInverted: root.platformInverted
            onClicked: {
                editor.cut()
                root.hide()
            }
        }
        Button {
            id: pasteButton
            iconSource: privateStyle.imagePath("qtg_toolbar_paste", root.platformInverted)
            visible: editor.canPaste
            platformInverted: root.platformInverted
            onClicked: {
                editor.paste()
                root.hide()
            }
        }
    }

    transformOrigin: Item.Center

    states: [
        State {
            name: "hidden"
            when: !internal.show
            PropertyChanges { target: root; visible: false; opacity: 0.0 }
            PropertyChanges { target: row; scale: 0.6 }
        },
        State {
            name: "visible"
            when: internal.show
            PropertyChanges { target: root; visible: true; opacity: 1.0 }
            PropertyChanges { target: row; scale: 1.0 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"; to: "visible"
            SequentialAnimation {
                PropertyAction { target: root; property: "visible"; value: "true" }
                ParallelAnimation {
                    PropertyAnimation { target: root; properties: "opacity"; duration: internal.animationDuration }
                    PropertyAnimation { target: row; properties: "scale"; duration: internal.animationDuration; easing.type: Easing.OutQuad }
                }
            }
        },
        Transition {
            from: "visible"; to: "hidden"
            SequentialAnimation {
                PropertyAction { target: root; property: "visible"; value: "true" }
                ParallelAnimation {
                    PropertyAnimation { target: root; properties: "opacity"; duration: internal.animationDuration }
                    PropertyAnimation { target: row; properties: "scale"; duration: internal.animationDuration; easing.type: Easing.InQuad }
                }
            }
        }
    ]
}
