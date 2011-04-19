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

    property alias title: titleItem.children
    property alias content: contentItem.children
    property alias buttons: buttonItem.children
    property alias visualParent: dialog.visualParent
    property alias status: dialog.status

    // read-only
    property int platformContentMaximumWidth: dialog.maxWidth()
    // read-only
    property int platformContentMaximumHeight:
        dialog.maxHeight() - titleBar.height - buttonItem.height

    signal accepted
    signal rejected

    function open() {
        dialog.open()
        dialog.focus = true
    }

    function accept() {
        dialog.close()
        accepted()
    }

    function reject() {
        dialog.close()
        rejected()
    }

    function close() {
        dialog.close()
    }

    visible: false

    Popup {
        id: dialog

        function defaultWidth() {
            if (root.width > 0)
                return root.width
            else
                return Math.max(titleBar.childrenRect.width,
                    Math.max(contentItem.childrenRect.width, buttonItem.childrenRect.width))
        }

        function defaultHeight() {
            if (root.height > 0)
                return root.height
            else
                return titleBar.height + contentItem.childrenRect.height + buttonItem.height
        }

        function maxWidth() {
            if (screen.width < screen.height)
                return screen.width - 2 * platformStyle.paddingMedium
            else
                return privateStyle.dialogMaxSize
        }

        function maxHeight() {
            if (screen.width < screen.height)
                return privateStyle.dialogMaxSize
            else
                return screen.height - 2 * platformStyle.paddingMedium
        }

        function minWidth() {
            return Math.min(screen.displayWidth, screen.displayHeight) - 2 * platformStyle.paddingMedium
        }

        width: Math.max(Math.min(defaultWidth(), maxWidth()), minWidth())
        height: Math.max(Math.min(defaultHeight(), maxHeight()), privateStyle.dialogMinSize)

        state: "Hidden"
        visible: true
        anchors.centerIn: parent
        animationDuration: 250

        BorderImage {
            source: privateStyle.imagePath("qtg_fr_popup")
            border { left: 20; top: 20; right: 20; bottom: 20 }
            anchors.fill: parent
        }

        Item {
            id: titleBar

            height: platformStyle.graphicSizeSmall + 2 * platformStyle.paddingLarge
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            BorderImage {
                source: privateStyle.imagePath("qtg_fr_popup_heading")
                border { left: 40; top: 0; right: 40; bottom: 0 }
                anchors.fill: parent
            }

            Item {
                id: titleItem

                clip: true
                anchors {
                    fill: parent
                }
            }
        }

        Item {
            id: contentItem

            clip: true
            anchors {
                top: titleBar.bottom
                left: parent.left
                right: parent.right
                bottom: buttonItem.top
            }
        }

        Item {
            id: buttonItem

            height: childrenRect.height
            clip: true
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }

        states: [
            State {
                name: "Visible"
                when: status == DialogStatus.Opening || status == DialogStatus.Open
                PropertyChanges { target: dialog; opacity: 1.0; scale: 1; }
            },
            State {
                name: "Hidden"
                when: status == DialogStatus.Closing || status == DialogStatus.Closed
                PropertyChanges { target: dialog; opacity: 0.0; scale: 0.9; }
            }
        ]

        transitions: [
            Transition {
                from: "Visible"; to: "Hidden"
                SequentialAnimation {
                    ScriptAction {script: status = DialogStatus.Closing }
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "opacity"
                            duration: dialog.animationDuration
                            easing.type: Easing.Linear
                        }
                        NumberAnimation {
                            property: "scale"
                            duration: dialog.animationDuration
                            easing.type: Easing.InQuad
                        }
                    }
                    ScriptAction {script: status = DialogStatus.Closed }
                }
            },
            Transition {
                from: "Hidden"; to: "Visible"
                SequentialAnimation {
                    ScriptAction { script: status = DialogStatus.Opening }
                    PropertyAction { target: dialog; property: "visible"; value: true}
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "opacity"
                            duration: dialog.animationDuration
                            easing.type: Easing.Linear
                        }
                        NumberAnimation {
                            property: "scale"
                            duration: dialog.animationDuration
                            easing.type: Easing.OutQuad
                        }
                    }
                    ScriptAction { script: status = DialogStatus.Open }
                }
            }
        ]
    }
}
