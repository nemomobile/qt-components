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

Dialog {
    id: root

    property alias titleText: titleTextArea.text
    property bool privateCloseIcon: false

    title: Item {
        anchors.fill: parent

        LayoutMirroring.enabled: privateCloseIcon ? false : undefined
        LayoutMirroring.childrenInherit: true

        Text {
            id: titleTextArea

            anchors {
                left: parent.left
                leftMargin: platformStyle.paddingLarge
                right: iconMouseArea.left
                top: parent.top
                bottom: parent.bottom
            }

            font {
                family: platformStyle.fontFamilyRegular
                pixelSize: platformStyle.fontSizeLarge
            }
            color: root.platformInverted ? platformStyle.colorNormalLinkInverted
                                         : platformStyle.colorNormalLink
            clip: true
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            id: iconMouseArea

            property bool pressCancelled

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: privateCloseIcon ? 2 * platformStyle.paddingLarge + platformStyle.graphicSizeSmall : 0
            enabled: root.status == DialogStatus.Open

            onPressed: {
                pressCancelled = false
                privateStyle.play(Symbian.BasicButton)
            }
            onClicked: {
                if (!pressCancelled)
                    root.reject()
            }
            onReleased: {
                if (!pressCancelled)
                    privateStyle.play(Symbian.PopupClose)
            }
            onExited: pressCancelled = true

            Loader {
                id: iconLoader
                anchors.centerIn: parent
                sourceComponent: privateCloseIcon ? closeIconComponent : undefined
            }

            Component {
                id: closeIconComponent

                Image {
                    sourceSize.height: platformStyle.graphicSizeSmall
                    sourceSize.width: platformStyle.graphicSizeSmall
                    smooth: true
                    source: privateStyle.imagePath((iconMouseArea.pressed && !iconMouseArea.pressCancelled
                                                    ? "qtg_graf_popup_close_pressed"
                                                    : "qtg_graf_popup_close_normal"),
                                                   root.platformInverted)
                }
            }
        }
    }
}
