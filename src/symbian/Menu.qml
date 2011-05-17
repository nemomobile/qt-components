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
        objectName: "OptionsMenu"

        y: screen.height - popup.height - toolBarHeight()

        function toolBarHeight() {
            return (screen.width < screen.height)
                ? privateStyle.toolBarHeightPortrait
                : privateStyle.toolBarHeightLandscape
        }

        animationDuration: 200
        state: "Hidden"
        visible: status != DialogStatus.Closed
        enabled: status == DialogStatus.Open
        width: screen.width
        height: menu.height
        clip: true

        onFaderClicked: {
            privateStyle.play(Symbian.PopupClose)
            close()
        }

        MenuContent {
            id: menu
            containingPopup: popup
            width: parent.width
            onItemClicked: popup.close()
        }

        states: [
            State {
                name: "Hidden"
                when: status == DialogStatus.Closing || status == DialogStatus.Closed
                PropertyChanges { target: menu; y: menu.height + 5; opacity: 0 }
            },
            State {
                name: "Visible"
                when: status == DialogStatus.Opening || status == DialogStatus.Open
                PropertyChanges { target: menu; y: 0; opacity: 1 }
            }
        ]

        transitions: [
            Transition {
                from: "Visible"; to: "Hidden"
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation {
                            target: menu
                            property: "y"
                            duration: popup.animationDuration
                            easing.type: Easing.Linear
                        }
                        NumberAnimation {
                            target: menu
                            property: "opacity"
                            duration: popup.animationDuration
                            easing.type: Easing.Linear
                        }
                    }
                    PropertyAction { target: popup; property: "status"; value: DialogStatus.Closed }
                }
            },
            Transition {
                from: "Hidden"; to: "Visible"
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation {
                            target: menu
                            property: "y"
                            duration: popup.animationDuration
                            easing.type: Easing.OutQuad
                        }
                        NumberAnimation {
                            target: menu
                            property: "opacity"
                            duration: popup.animationDuration
                            easing.type: Easing.Linear
                        }
                    }
                    PropertyAction { target: popup; property: "status"; value: DialogStatus.Open }
                }
            }
        ]
    }
}
