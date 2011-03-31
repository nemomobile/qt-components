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

import Qt 4.7
import "." 1.0

Dialog {
    id: root
    objectName: "root"

    property string titleText
    property string message
    property alias acceptButtonText: acceptButton.text
    property alias rejectButtonText: rejectButton.text
    property alias icon: icon.source

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
            color: platformStyle.colorNormalLight
            text: root.titleText
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
        height: Math.min(label.height, preferredHeight)
        width: parent.width

        property real nonContentHeight: title.height + buttons.height + 2 * platformStyle.paddingLarge
        property real minContentHeight: privateStyle.dialogMinSize - nonContentHeight

        property real visualContentHeight: Math.max(visualParent.height - nonContentHeight, minContentHeight)
        property real parentContentHeight: Math.max(parent.parent.height - nonContentHeight, minContentHeight)

        property real preferredHeight: visualParent ? visualContentHeight : parent.parent ? parentContentHeight : 0

        Flickable {
            id: flickable
            anchors.fill: parent
            contentHeight: label.height
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: label.height > content.preferredHeight

            Text {
                id: label
                width: flickable.width
                horizontalAlignment: Text.AlignHCenter
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
                color: platformStyle.colorNormalLight
                wrapMode: Text.WordWrap
                text: root.message
            }
        }

        ScrollDecorator {
            id: scrollDecorator
            flickableItem: flickable
        }
    }

    buttons: ToolBar {
        id: buttons
        width: parent.width

        Row {
            id: buttonRow
            anchors.centerIn: parent

            ToolButton {
                id: acceptButton
                width: buttons.width / 2
                onClicked: accept()
                visible: text != ""
            }
            ToolButton {
                id: rejectButton
                width: buttons.width / 2
                onClicked: reject()
                visible: text != ""
            }
        }
    }
}
