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
import com.nokia.symbian 1.1
import "AppManager.js" as AppManager

Magnifier {
    id: root

    property Item editor: parent.editor
    // Defines the center of the content to be magnified in X axis
    // Note: the center of the content to be magnified in Y axis is determined by the cursor position.
    property variant contentCenter: Qt.point(0, 0) //editor's coordinate system
    property bool platformInverted: false

    function show() {
        parent = AppManager.visualRoot();
        internal.show = true;
        internal.updateGeometry();
    }

    function hide() {
        internal.show = false;
    }

    sourceRect: Qt.rect(0, 0, 0, 0)
    visible: false
    scaleFactor: 1.2
    maskFileName: ":/graphics_1_1_3/qtg_graf_magnifier_mask.svg"
    overlayFileName: ":/graphics_1_1_3/qtg_graf_magnifier.svg"
    width: internal.magnifierSize
    height: internal.magnifierSize
    z: Infinity

    onContentCenterChanged: internal.updateGeometry()

    Connections {
        target: screen
        onCurrentOrientationChanged: internal.orientation()
    }

    Component.onCompleted: internal.orientation()

    // Private
    QtObject {
        id: internal

        function orientation() {
            if (!screen.privateSensorOrientationMethod())
                return;

            root.rotation = screen.rotation
        }

        property int magnifierSize: platformStyle.graphicSizeMedium * 2
        property int sourceSize: magnifierSize / scaleFactor
        property bool show: false
        property int animationDuration: 250

        function updateGeometry() {
            if (internal.show) {
                var pos = parent.mapFromItem(editor, contentCenter.x, contentCenter.y);
                var sourcePos = editor.mapToItem(null, contentCenter.x, contentCenter.y);
                root.x = pos.x - magnifierSize/2;
                root.y = Math.max(-magnifierSize*0.4, pos.y - magnifierSize - platformStyle.paddingLarge*2);
                root.sourceRect = Qt.rect(sourcePos.x - sourceSize / 2,
                                          sourcePos.y - sourceSize / 2 + Math.min(0, root.y*0.35),
                                          sourceSize, sourceSize);
            }
        }
    }

    transformOrigin: Item.Center

    states: [
        State {
            name: "hidden"
            when: !internal.show
            PropertyChanges { target: root; visible: false; opacity: 0.0 }
            PropertyChanges { target: magnifier; scale: 0.6 }
        },
        State {
            name: "visible"
            when: internal.show
            PropertyChanges { target: root; visible: true; opacity: 1.0 }
            PropertyChanges { target: magnifier; scale: 1.0 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"; to: "visible"
            SequentialAnimation {
                PropertyAction { target: root; property: "visible"; value: "true" }
                ParallelAnimation {
                    PropertyAnimation { target: root; properties: "opacity"; duration: internal.animationDuration }
                    PropertyAnimation { target: magnifier; properties: "scale"; duration: internal.animationDuration; easing.type: Easing.OutQuad }
                }
            }
        },
        Transition {
            from: "visible"; to: "hidden"
            SequentialAnimation {
                PropertyAction { target: root; property: "visible"; value: "true" }
                ParallelAnimation {
                    PropertyAnimation { target: root; properties: "opacity"; duration: internal.animationDuration }
                    PropertyAnimation { target: magnifier; properties: "scale"; duration: internal.animationDuration; easing.type: Easing.InQuad }
                }
            }
        }
    ]
}
