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
    property alias placeHolderText: placeHolderText.text
    property alias searchText: searchTextInput.text
    property bool platformInverted: false
    property alias maximumLength: searchTextInput.maximumLength

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
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: backButton
        onClicked: root.backClicked()
    }

    FocusScope {
        id: textPanel
        anchors.left: backButton ? backToolButton.right : parent.left
        anchors.leftMargin: backButton ? 0 : platformStyle.paddingLarge
        anchors.right: parent.right
        anchors.rightMargin: platformStyle.paddingLarge
        anchors.verticalCenter: parent.verticalCenter
        height: privateStyle.textFieldHeight

        BorderImage {
            id: frame
            anchors.fill: parent
            source: privateStyle.imagePath(
                        searchTextInput.activeFocus ? "qtg_fr_textfield_highlighted"
                                                    : "qtg_fr_textfield_editable", root.platformInverted)
            border {
                left: platformStyle.borderSizeMedium
                top: platformStyle.borderSizeMedium
                right: platformStyle.borderSizeMedium
                bottom: platformStyle.borderSizeMedium
            }
            smooth: true
        }

        Image {
            id: searchIndicator
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall
            fillMode: Image.PreserveAspectFit
            smooth: true
            anchors.left: textPanel.left
            anchors.leftMargin: platformStyle.paddingSmall
            anchors.verticalCenter: textPanel.verticalCenter
            source: privateStyle.imagePath("qtg_graf_search_indicator", root.platformInverted)
        }

        TextInput {
            id: searchTextInput; objectName: "searchTextInput"
            anchors.left: searchIndicator.right
            anchors.leftMargin: platformStyle.paddingSmall
            anchors.right: clearButton.left
            anchors.rightMargin: platformStyle.paddingSmall
            anchors.verticalCenter: textPanel.verticalCenter
            clip: true
            color: root.platformInverted ? platformStyle.colorNormalLightInverted
                                         : platformStyle.colorNormalDark
            selectByMouse: true
            selectedTextColor: platformStyle.colorNormalLight
            selectionColor: platformStyle.colorTextSelection
            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
            activeFocusOnPress: false
            inputMethodHints: Qt.ImhNoPredictiveText
            onTextChanged: {
                if (text) {
                    clearButton.state = "ClearVisible"
                } else {
                    clearButton.state = "ClearHidden"
                }
            }
            onActiveFocusChanged: {
                if (!searchTextInput.activeFocus) {
                    searchTextInput.closeSoftwareInputPanel()
                }
            }
        }
        MouseArea {
            id: searchMouseArea
            anchors {
                left: textPanel.left;
                right: clearButton.state=="ClearHidden" ? textPanel.right : clearButton.left
                verticalCenter : textPanel.verticalCenter
            }
            height: textPanel.height
            onPressed: {
                if (!searchTextInput.activeFocus) {
                    searchTextInput.forceActiveFocus()
                }
            }
            onClicked: {
                searchTextInput.openSoftwareInputPanel()
                privateStyle.play(Symbian.PopUp)
            }
        }

        Text {
            id: placeHolderText; objectName: "placeHolderText"
            anchors.left: searchIndicator.right
            anchors.leftMargin: platformStyle.paddingMedium
            anchors.right: clearButton.left
            anchors.rightMargin: platformStyle.paddingMedium
            anchors.verticalCenter: textPanel.verticalCenter
            color: root.platformInverted ? platformStyle.colorNormalMidInverted
                                         : platformStyle.colorNormalMid
            font: searchTextInput.font
            visible: (!searchTextInput.activeFocus) && (!searchTextInput.text) && text
        }

        Image {
            id: clearButton; objectName: "clearButton"
            height: platformStyle.graphicSizeSmall
            width: platformStyle.graphicSizeSmall
            anchors.right: textPanel.right
            anchors.rightMargin: platformStyle.paddingSmall
            anchors.verticalCenter: textPanel.verticalCenter
            state: "ClearHidden"
            source: privateStyle.imagePath(
                        clearMouseArea.pressed ? "qtg_graf_textfield_clear_pressed"
                                               : "qtg_graf_textfield_clear_normal", root.platformInverted)

            MouseArea {
                id: clearMouseArea
                anchors.fill: parent
                onClicked: {
                    searchTextInput.focus = false
                    searchTextInput.cursorVisible = false
                    searchTextInput.closeSoftwareInputPanel()
                    searchTextInput.text = ""
                    root.clearClicked()
                    clearButton.state = "ClearHidden"
                }
            }

            states: [
                State {
                    name: "ClearVisible"
                    PropertyChanges {target: clearButton; opacity: 1}
                },
                State {
                    name: "ClearHidden"
                    PropertyChanges {target: clearButton; opacity: 0}
                }
            ]

            transitions: [
                Transition {
                    from: "ClearVisible"; to: "ClearHidden"
                    NumberAnimation { properties: "opacity"; duration: internal.animationtime; easing.type: Easing.Linear }
                },
                Transition {
                    from: "ClearHidden"; to: "ClearVisible"
                    NumberAnimation { properties: "opacity"; duration: internal.animationtime; easing.type: Easing.Linear }
                }
            ]

        }
    }
}
