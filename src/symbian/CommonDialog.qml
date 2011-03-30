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

    property alias titleText: titleTextArea.text
    property bool privateCloseIcon: false

    title: Row {
        id: titleArea

        anchors {
            fill: parent
            leftMargin: platformStyle.paddingLarge
            rightMargin: platformStyle.paddingLarge
        }

        Text {
            id: titleTextArea

            font {
                family: platformStyle.fontFamilyRegular
                pixelSize: platformStyle.fontSizeLarge
            }
            color: platformStyle.colorNormalLight
            clip: true
            elide: Text.ElideRight
            width: parent.width - iconLoader.width
            anchors.verticalCenter: parent.verticalCenter
        }

        Loader {
            id: iconLoader

            sourceComponent: privateCloseIcon ? closeIconComponent : undefined
            anchors.verticalCenter: parent.verticalCenter
        }

        Component {
            id: closeIconComponent

            Item {
                id: closeIconArea

                height: platformStyle.graphicSizeSmall
                width: platformStyle.graphicSizeSmall + platformStyle.paddingLarge

                // Extra Item is needed here to get ToolButton positioned correctly
                Item {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    width: platformStyle.graphicSizeSmall

                    ToolButton {
                        iconSource: privateStyle.imagePath("qtg_graf_dialog_close_icon.svg")
                        anchors.centerIn: parent
                        flat: true
                        // ToolButton size is increased by 15 to get larger mousearea
                        width: platformStyle.graphicSizeSmall + 15
                        height: width
                        onClicked: root.reject()
                    }
                }
            }
        }
    }
}
