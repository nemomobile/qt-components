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

Dialog {
    id: root
    objectName: "root"

    property string titleText
    property string message
    property alias acceptButtonText: acceptButton.text
    property alias rejectButtonText: rejectButton.text
    property alias icon: icon.source

    onStatusChanged: if (status == DialogStatus.Open) vertical.flash()

    title: Item {
        id: title
        height: platformStyle.graphicSizeSmall + 2 * platformStyle.paddingLarge
        width: parent.width

        Image {
            id: icon
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall

            anchors {
                right: parent.right; rightMargin: icon.source != "" ? platformStyle.paddingLarge : 0
                verticalCenter: parent.verticalCenter
            }
        }

        Text {
            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
            color: platformStyle.colorNormalLink
            text: root.titleText
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight

            anchors {
                left: title.left; leftMargin: platformStyle.paddingLarge
                top: title.top; bottom: title.bottom
                right: icon.left; rightMargin: platformStyle.paddingLarge
            }
        }
    }

    content: Item {
        id: content
        height: internal.getContentAreaHeight()
        width: parent.width

        Item {
            Flickable {
                id: flickable
                width: parent.width
                height: parent.height
                anchors { left: parent.left; top: parent.top }
                contentHeight: label.height
                flickableDirection: Flickable.VerticalFlick
                clip: true
                interactive: label.height > flickable.height

                Text {
                    id: label
                    width: flickable.width - privateStyle.scrollBarThickness
                    font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
                    color: platformStyle.colorNormalLight
                    wrapMode: Text.WordWrap
                    text: root.message
                    anchors { right: parent.right; rightMargin: privateStyle.scrollBarThickness }
                }
            }

            ScrollBar {
                id: vertical
                height: parent.height
                anchors { top: flickable.top; right: flickable.right }
                flickableItem: flickable
                interactive: false
                orientation: Qt.Vertical
            }

            anchors {
                top: parent.top; topMargin: platformStyle.paddingLarge
                bottom: parent.bottom; bottomMargin: platformStyle.paddingLarge
                left: parent.left; leftMargin: platformStyle.paddingLarge
                right: parent.right
            }
        }
    }

    buttons: ToolBar {
        id: buttons
        width: parent.width
        height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall
        tools: Row {
            id: buttonRow
            anchors.centerIn: parent
            spacing: platformStyle.paddingMedium

            ToolButton {
                id: acceptButton
                // Different widths for 1 and 2 button cases
                width: rejectButton.text == ""
                    ? Math.round((privateStyle.dialogMaxSize - 3 * platformStyle.paddingMedium) / 2)
                    : (buttons.width - 3 * platformStyle.paddingMedium) / 2
                onClicked: accept()
                visible: text != ""
            }
            ToolButton {
                id: rejectButton
                width: acceptButton.text == ""
                    ? Math.round((privateStyle.dialogMaxSize - 3 * platformStyle.paddingMedium) / 2)
                    : (buttons.width - 3 * platformStyle.paddingMedium) / 2
                onClicked: reject()
                visible: text != ""
            }
        }
    }

    QtObject {
        id: internal

        property int defaultContentHeight: root.height ? root.height - title.height - buttons.height
            : label.height + 2 * platformStyle.paddingLarge

        function getContentAreaHeight() {
            // Constrain the default height within the bounds of the min and max heights
            return Math.max(Math.min(defaultContentHeight, platformContentMaximumHeight),
                privateStyle.dialogMinSize - title.height - buttons.height)
        }
    }
}
