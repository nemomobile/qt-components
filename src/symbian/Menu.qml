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

Item {
    id: root

    default property alias content: menu.content
    property alias visualParent: popup.visualParent
    property alias status: popup.status

    function open() {
        popup.open()
    }

    function close() {
        popup.close()
    }

    visible: false

    Popup {
        id: popup

        y: screen.height - popup.height - toolBarHeight()

        function toolBarHeight() {
            return (screen.width < screen.height)
                ? privateStyle.toolBarHeightPortrait
                : privateStyle.toolBarHeightLandscape
        }

        animationDuration: 600
        state: "Hidden"
        visible: status != DialogStatus.Closed
        enabled: status == DialogStatus.Open
        width: screen.width
        height: menu.height

        onFaderClicked: {
            privateStyle.play(Symbian.PopUpClose)
            close()
        }

        Flickable {
            id: animationArea

            anchors.fill: parent
            contentWidth: width
            contentHeight: childrenRect.height
            contentY: -height
            interactive: false; clip: true

            MenuContent {
                id: menu
                width: parent.width
                onItemClicked: popup.close()
            }
        }

        states: [
            State {
                name: "Hidden"
                when: status == DialogStatus.Closing || status == DialogStatus.Closed
                PropertyChanges { target: animationArea; contentY: -animationArea.height - 5 }
            },
            State {
                name: "Visible"
                when: status == DialogStatus.Opening || status == DialogStatus.Open
                PropertyChanges { target: animationArea; contentY: 0 }
            }
        ]

        transitions: [
            Transition {
                from: "Visible"; to: "Hidden"
                SequentialAnimation {
                    NumberAnimation { property: "contentY"; duration: popup.animationDuration }
                    PropertyAction { target: animationArea; property: "opacity"; value: 0}
                    PropertyAction { target: popup; property: "status"; value: DialogStatus.Closed }
                }
            },
            Transition {
                from: "Hidden"; to: "Visible"
                SequentialAnimation {
                    PropertyAction { target: animationArea; property: "opacity"; value: 1}
                    NumberAnimation { property: "contentY"; duration: popup.animationDuration }
                    PropertyAction { target: popup; property: "status"; value: DialogStatus.Open }
                }
            }
        ]
    }
}
