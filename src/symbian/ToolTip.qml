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
import "AppManager.js" as AppView

Item {
    id: root

    // Public API
    property alias font: label.font
    property alias text: label.text
    property variant target: null
    property bool platformInverted: false

    implicitWidth: Math.min(privy.maxWidth, (privateStyle.textWidth(text, font) + privy.hMargin * 2))
    implicitHeight: privateStyle.fontHeight(font) + privy.vMargin * 2
    opacity: 0

    function show() {
        opacity = 1
        visible = true
    }

    function hide() {
        opacity = 0
    }

    Behavior on opacity {
        PropertyAnimation { duration: 100 }
    }

    Component.onCompleted: {
        if (visible)
            show()
    }

    onOpacityChanged: {
        if (opacity == 0)
            visible = false
        else
            visible = true
    }

    onVisibleChanged: {
        if (visible) {
            root.parent = AppView.rootObject()
            privy.calculatePosition()
            opacity = 1
            privy.targetSceneXChanged.connect(privy.targetMoved)
            privy.targetSceneYChanged.connect(privy.targetMoved)
        } else {
            privy.targetSceneXChanged.disconnect(privy.targetMoved)
            privy.targetSceneYChanged.disconnect(privy.targetMoved)
        }
    }

    Binding { target: privy; property: "targetSceneX"; value: AppView.sceneX(root.target); when: root.visible && (root.target != null) }
    Binding { target: privy; property: "targetSceneY"; value: AppView.sceneY(root.target); when: root.visible && (root.target != null) }

    QtObject {
        id: privy

        property real hMargin: platformStyle.paddingMedium * 2
        property real vMargin: platformStyle.paddingMedium
        property real spacing: platformStyle.paddingLarge
        property real maxWidth: screen.width - spacing * 2
        property real targetSceneX
        property real targetSceneY

        function targetMoved() {
            if (root.opacity == 1)
                hide()
        }

        function calculatePosition() {
            if (!target)
                return

            // Determine and set the main position for the ToolTip, order: top, right, left, bottom
            var targetPos = root.parent.mapFromItem(target, 0, 0)

            // Top
            if (targetPos.y >= (root.height + privy.vMargin + privy.spacing)) {
                root.x = targetPos.x + (target.width / 2) - (root.width / 2)
                root.y = targetPos.y - root.height - privy.vMargin

            // Right
            } else if (targetPos.x <= (screen.width - target.width - privy.hMargin - root.width - privy.spacing)) {
                root.x = targetPos.x + target.width + privy.hMargin;
                root.y = targetPos.y + (target.height / 2) - (root.height / 2)

            // Left
            } else if (targetPos.x >= (root.width + privy.hMargin + privy.spacing)) {
                root.x = targetPos.x - root.width - privy.hMargin
                root.y = targetPos.y + (target.height / 2) - (root.height / 2)

            // Bottom
            } else {
                root.x = targetPos.x + (target.width / 2) - (root.width / 2)
                root.y = targetPos.y + target.height + privy.vMargin
            }

            // Fine-tune the ToolTip position based on the screen borders
            if (root.x > (screen.width - privy.spacing - root.width))
                root.x = screen.width - root.width - privy.spacing

            if (root.x < privy.spacing)
                root.x = privy.spacing;

            if (root.y > (screen.height - root.height - privy.spacing))
                root.y = screen.height - root.height - privy.spacing

            if (root.y < privy.spacing)
                root.y = privy.spacing
        }
    }

    BorderImage {
        id: frame
        anchors.fill: parent
        source: privateStyle.imagePath("qtg_fr_tooltip", root.platformInverted)
        border { left: 20; top: 20; right: 20; bottom: 20 }
    }

    Text {
       id: label
       clip: true
       color: root.platformInverted ? platformStyle.colorNormalLightInverted
                                    : platformStyle.colorNormalLight
       elide: Text.ElideRight
       font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
       verticalAlignment: Text.AlignVCenter
       horizontalAlignment: Text.AlignHCenter
       anchors {
           fill: parent
           leftMargin: privy.hMargin
           rightMargin: privy.hMargin
           topMargin: privy.vMargin
           bottomMargin: privy.vMargin
       }
    }
}
