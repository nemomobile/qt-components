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

Item {
    id: root

    property alias content: contentArea.children
    property Popup containingPopup: null
    property bool platformInverted: false

    property int screenHeight: screen.height
    property int screenWidth: screen.width

    signal itemClicked()

    height: flickableArea.height

    QtObject {
        id: internal
        // Add padding to align content area top and bottom with rounded background graphics.
        // Optionsmenu uses mask, thus no padding needed
        property int clipMargin: containingPopup.objectName == "OptionsMenu" ? 0 : platformStyle.paddingSmall
        property int preferredHeight: privateStyle.menuItemHeight * ((screenWidth < screenHeight) ? 5 : 3)
    }

    BorderImage {
        source: containingPopup.objectName == "OptionsMenu"
                ? privateStyle.imagePath("qtg_fr_popup_options", root.platformInverted)
                : privateStyle.imagePath("qtg_fr_popup", root.platformInverted)
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
    }

    Item {
        id: clipItem
        y: internal.clipMargin
        height: flickableArea.height - 2 * internal.clipMargin
        width: root.width
        clip: true
        Flickable {
            id: flickableArea

            property int index: 0
            property bool itemAvailable: (contentArea.children[0] != undefined) && (contentArea.children[0].children[0] != undefined)
            property int itemHeight: itemAvailable ? Math.max(1, contentArea.children[0].children[0].height) : 1
            property int itemsHidden: Math.floor(contentY / itemHeight)
            property int interactionMode: symbian.listInteractionMode

            y: -internal.clipMargin
            height: contentArea.height; width: root.width
            clip: true
            contentHeight: (contentArea.children[0] != undefined) ? contentArea.children[0].height : 0
            contentWidth: width

            Item {
                id: contentArea

                width: flickableArea.width
                height: flickableArea.contentHeight > internal.preferredHeight
                    ? internal.preferredHeight - (internal.preferredHeight % flickableArea.itemHeight)
                    : flickableArea.contentHeight

                function setChildrenWidths() {
                    for (var i = 0; i < children.length; ++i)
                        children[i].width = width
                }

                function connectChildren() {
                    for (var i = 0; i < children.length; ++i) {
                        if (children[i].clicked != undefined)
                            children[i].clicked.connect(root.itemClicked)
                    }
                }

                onWidthChanged: setChildrenWidths()
                onChildrenChanged: {
                    setChildrenWidths()
                    connectChildren()
                }
            }

            onVisibleChanged: {
                enabled = visible
                if (itemAvailable)
                    contentArea.children[0].children[0].focus = visible
                contentY = 0
                index = 0
            }

            onInteractionModeChanged: {
                if (symbian.listInteractionMode == Symbian.KeyNavigation) {
                    contentY = 0
                    scrollBar.flash(Symbian.FadeOut)
                    if (itemAvailable)
                        contentArea.children[0].children[index].focus = true
                } else if (symbian.listInteractionMode == Symbian.TouchInteraction) {
                    index = 0
                }
            }

            Keys.onPressed: {
                if (itemAvailable && (event.key == Qt.Key_Down || event.key == Qt.Key_Up)) {
                    if (event.key == Qt.Key_Down && index < contentArea.children[0].children.length - 1) {
                        index++
                        if (index * itemHeight > contentY + height - itemHeight) {
                            contentY = index * itemHeight - height + itemHeight
                        }
                    } else if (event.key == Qt.Key_Up && index > 0) {
                        index--
                        if (index * itemHeight < contentY) {
                            contentY = index * itemHeight
                        }
                    }
                    scrollBar.flash(Symbian.FadeOut)
                    contentArea.children[0].children[index].focus = true
                    event.accepted = true
                }
            }
        }
    }

    ScrollBar {
        id: scrollBar
        objectName: "scrollBar"
        flickableItem: flickableArea
        interactive: false
        visible: flickableArea.height < flickableArea.contentHeight
        anchors {
            topMargin: platformStyle.paddingMedium
            bottomMargin: platformStyle.paddingMedium
            top: clipItem.top
            bottom: clipItem.bottom
            right: clipItem.right
        }
        platformInverted: root.platformInverted
    }

    Connections {
        target: containingPopup
        onStatusChanged: {
            if (containingPopup.status == DialogStatus.Open)
                scrollBar.flash(Symbian.FadeInFadeOut)
        }
    }
}
