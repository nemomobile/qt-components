/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

import QtQuick 1.0
import "." 1.0

Item {
    id: root

    property alias content: contentArea.children
    property Popup containingPopup: null

    signal itemClicked()

    height: flickableArea.height
    clip: true

    QtObject {
        id: internal
        property int preferredHeight: Math.floor(
            screen.height * ((screen.width < screen.height) ? 0.45 : 0.6) / privateStyle.menuItemHeight)
            * privateStyle.menuItemHeight
    }

    BorderImage {
        source: containingPopup.objectName == "OptionsMenu"
            ? privateStyle.imagePath("qtg_fr_popup_options") : privateStyle.imagePath("qtg_fr_popup") 
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
    }

    Flickable {
        id: flickableArea

        property int index: 0
        property bool itemAvailable: (contentArea.children[0] != undefined) && (contentArea.children[0].children[0] != undefined)
        property int itemHeight: itemAvailable ? Math.max(1, contentArea.children[0].children[0].height) : 1
        property int interactionMode: symbian.listInteractionMode

        height: contentArea.height; width: root.width
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        contentHeight: contentArea.childrenRect.height
        contentWidth: width

        Item {
            id: contentArea

            property int itemsHidden: Math.floor(flickableArea.contentY / flickableArea.itemHeight)

            width: flickableArea.width
            height: childrenRect.height > internal.preferredHeight
                ? internal.preferredHeight - (internal.preferredHeight % flickableArea.itemHeight)
                : childrenRect.height

            onWidthChanged: {
                for (var i = 0; i < children.length; ++i)
                    children[i].width = width
            }

            onItemsHiddenChanged: {
                // Check that popup is really open in order to prevent unnecessary feedback
                if (containingPopup.status == DialogStatus.Open
                    && symbian.listInteractionMode == Symbian.TouchInteraction)
                    privateStyle.play(Symbian.ItemScroll)
            }

            Component.onCompleted: {
                for (var i = 0; i < children.length; ++i) {
                    if (children[i].clicked != undefined)
                        children[i].clicked.connect(root.itemClicked)
                }
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
                        scrollBar.flash(Symbian.FadeOut)
                    }
                } else if (event.key == Qt.Key_Up && index > 0) {
                    index--
                    if (index * itemHeight < contentY) {
                        contentY = index * itemHeight
                        scrollBar.flash(Symbian.FadeOut)
                    }
                }
                contentArea.children[0].children[index].focus = true
                event.accepted = true
            }
        }
    }

    ScrollBar {
        id: scrollBar
        flickableItem: flickableArea
        interactive: false
        visible: flickableArea.height < flickableArea.contentHeight
        anchors {
            top: flickableArea.top
            right: flickableArea.right
        }
    }

    Connections {
        target: containingPopup
        onStatusChanged: {
            if (containingPopup.status == DialogStatus.Open)
                scrollBar.flash(Symbian.FadeInFadeOut)
        }
    }
}
