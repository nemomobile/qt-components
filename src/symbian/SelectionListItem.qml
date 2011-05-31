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

ListItem {
    id: root
    property string title: ""
    property string subTitle: ""
    implicitHeight: background.height + 2 * platformStyle.paddingLarge

    onModeChanged: {
        if (root.mode == "pressed") {
            pressed.source = privateStyle.imagePath("qtg_fr_choice_list_pressed")
            pressed.opacity = 1
        } else {
            releasedEffect.restart()
        }
    }

    BorderImage {
        id: background
        height: privateStyle.menuItemHeight - platformStyle.paddingSmall // from layout spec.
        anchors {
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: parent.right
            rightMargin: privateStyle.scrollBarThickness
            verticalCenter: parent.verticalCenter
        }
        border {
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
        source: privateStyle.imagePath("qtg_fr_choice_list_") + internal.getBackground()

        BorderImage {
            id: pressed
            border {
                left: platformStyle.borderSizeMedium
                top: platformStyle.borderSizeMedium
                right: platformStyle.borderSizeMedium
                bottom: platformStyle.borderSizeMedium
            }
            opacity: 0
            anchors.fill: parent
        }

        Column {
            anchors {
                verticalCenter: background.verticalCenter
                right: indicator.left
                rightMargin: platformStyle.paddingMedium
                left: background.left
                leftMargin: platformStyle.paddingLarge
            }

            Loader {
                sourceComponent: title != "" ? titleText : undefined
                width: parent.width // elide requires explicit width
            }

            Loader {
                sourceComponent: subTitle != "" ? subTitleText : undefined
                width: parent.width // elide requires explicit width
            }
        }
        Image {
            id: indicator
            source: root.mode == "disabled" ? privateStyle.imagePath("qtg_graf_choice_list_indicator_disabled") :
                                              privateStyle.imagePath("qtg_graf_choice_list_indicator")
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall
            anchors {
                right: background.right
                rightMargin: platformStyle.paddingSmall
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Component {
        id: titleText
        ListItemText {
            mode: root.mode
            role: "SelectionTitle"
            text: root.title
        }
    }
   Component {
        id: subTitleText
        ListItemText {
            mode: root.mode
            role: "SelectionSubTitle"
            text: root.subTitle
        }
    }

    QtObject {
        id: internal
        function getBackground() {
            if (root.mode == "highlighted")
                return "highlighted"
            else if (root.mode == "disabled")
                return "disabled"
            else
                return "normal"
        }
    }

    SequentialAnimation {
        id: releasedEffect
        PropertyAnimation {
            target: pressed
            property: "opacity"
            to: 0
            easing.type: Easing.Linear
            duration: 150
        }
    }
}
