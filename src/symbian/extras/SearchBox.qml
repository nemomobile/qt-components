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

Item {
    id: root

    property bool backButton: false
    property alias placeHolderText: textPanel.placeholderText
    property alias searchText: textPanel.text
    property bool platformInverted: false
    property alias maximumLength: textPanel.maximumLength

    signal clearClicked()
    signal backClicked()

    implicitWidth: Math.max(80, screen.width)
    implicitHeight: privateStyle.tabBarHeightPortrait

    LayoutMirroring.enabled: false
    LayoutMirroring.childrenInherit: true

    QtObject {
        id: internal
        property int animationtime: 250
    }

    BorderImage {
        anchors.fill: parent
        source: privateStyle.imagePath("qtg_fr_tab_bar", root.platformInverted)
        border {
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
    }

    ToolButton {
        id: backToolButton; objectName: "backToolButton"
        flat: true
        platformInverted: root.platformInverted
        iconSource: privateStyle.imagePath("toolbar-back", root.platformInverted)
        height: privateStyle.tabBarHeightPortrait
        width: privateStyle.tabBarHeightPortrait
        anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
        visible: backButton
        onClicked: root.backClicked()
    }

    TextField {
        id: textPanel
        platformLeftMargin: searchIndicator.width + platformStyle.paddingMedium
        platformRightMargin: (clearButton.opacity > 0 ? clearButton.width : 0) + platformStyle.paddingMedium
        anchors {
            left: backButton ? backToolButton.right : parent.left
            leftMargin: backButton ? 0 : platformStyle.paddingLarge
            right: parent.right
            rightMargin: platformStyle.paddingLarge
            verticalCenter: parent.verticalCenter
        }
        width: parent.width - parent.spacing
        platformInverted: root.platformInverted
        inputMethodHints: Qt.ImhNoPredictiveText

        Image {
            id: searchIndicator
            width: platformStyle.graphicSizeSmall
            height: platformStyle.graphicSizeSmall
            anchors {
                left: textPanel.left
                leftMargin: platformStyle.paddingLarge / 2
                verticalCenter: textPanel.verticalCenter
            }
            source: privateStyle.imagePath("qtg_graf_search_indicator", root.platformInverted)
        }

        Image {
            id: clearButton; objectName: "clearButton"
            height: platformStyle.graphicSizeSmall
            width: platformStyle.graphicSizeSmall
            anchors { right: parent.right; margins: platformStyle.paddingMedium; verticalCenter: parent.verticalCenter }
            source: privateStyle.imagePath(
                        clearMouseArea.pressed ? "qtg_graf_textfield_clear_pressed"
                                               : "qtg_graf_textfield_clear_normal", root.platformInverted)

            MouseArea {
                id: clearMouseArea
                anchors.fill: parent
                onPressed: privateStyle.play(Symbian.BasicButton)
                onClicked: {
                    clearMouseArea.forceActiveFocus()
                    textPanel.closeSoftwareInputPanel()
                    textPanel.text = ""
                    root.clearClicked()
                    privateStyle.play(Symbian.BasicButton)
                }
            }

            states: [
                State { name: "Visible"; when: textPanel.text
                    PropertyChanges {target: clearButton; opacity: 1}
                },
                State { name: "Hidden"; when: !textPanel.text
                    PropertyChanges {target: clearButton; opacity: 0}
                }
            ]
            transitions: [
                Transition { from: "Visible"; to: "Hidden"; reversible: true
                    NumberAnimation { properties: "opacity"; duration: internal.animationtime }
                }
            ]
        }
    }
}
