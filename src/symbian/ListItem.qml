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

ImplicitSizeItem {
    id: root
    property string mode: "normal" // Read-only
    property alias paddingItem: paddingItem // Read-only

    property bool enabled: true
    property bool subItemIndicator: false

    signal clicked
    signal pressAndHold

    implicitWidth: ListView.view ? ListView.view.width : screen.width
    implicitHeight: platformStyle.graphicSizeLarge

    Item {
        id: background
        anchors.fill: parent

        Rectangle {
            height: 1
            color: privateStyle.listItemSeparatorColor
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }
        Loader {
            id: faderLoader
            opacity: 0
            anchors.fill: background
            sourceComponent: root.mode != "normal" && root.mode != "pressed" ? fader : undefined
        }

        BorderImage {
            id: highlight
            border { left: 20; top: 20; right: 20; bottom: 20 }
            opacity: 0
            anchors.fill: background
        }
    }

    Component {
        id: fader

        BorderImage {
            source: privateStyle.imagePath("qtg_fr_list_" + mode)
            border { left: 20; top: 20; right: 20; bottom: 20 }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: root.enabled
        onPressed: {
            symbian.listInteractionMode = Symbian.TouchInteraction
            internal.state = "Pressed"
        }
        onClicked: {
            internal.state = ""
            root.clicked()
        }
        onCanceled: {
            internal.state = "Canceled"
        }
        onPressAndHold: {
            internal.state = "PressAndHold"
        }
        onReleased: {
            internal.state = ""
        }
        onExited: {
            internal.state = ""
        }
    }

    Loader {
        id: iconLoader
        sourceComponent: root.subItemIndicator ? subItemIcon : undefined
        anchors {
            right: parent.right
            rightMargin: privateStyle.scrollBarThickness
            verticalCenter: parent.verticalCenter
        }
    }

    Component {
        id: subItemIcon

        Image {
            source: privateStyle.imagePath("qtg_graf_drill_down_indicator.svg")
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall
        }
    }

    Keys.onReleased: {
        if (root.enabled) {
            if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
                event.accepted = true
                internal.state = "Focused"
            }
        }
    }

    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Select:
            case Qt.Key_Return: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation)
                    symbian.listInteractionMode = Symbian.KeyNavigation
                else
                    if (root.enabled) {
                        highlight.source = privateStyle.imagePath("qtg_fr_list_pressed")
                        highlight.opacity = 1
                        releasedEffect.restart()
                        root.clicked()
                    }
                event.accepted = true
                break
            }

            case Qt.Key_Up: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation) {
                    symbian.listInteractionMode = Symbian.KeyNavigation
                    internal.state = "Focused"
                    ListView.view.positionViewAtIndex(index, ListView.Beginning)
                } else
                    ListView.view.decrementCurrentIndex()
                event.accepted = true
                break
            }

            case Qt.Key_Down: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation) {
                    symbian.listInteractionMode = Symbian.KeyNavigation
                    ListView.view.positionViewAtIndex(index, ListView.Beginning)
                    internal.state = "Focused"
                } else
                    ListView.view.incrementCurrentIndex()
                event.accepted = true
                break
            }
            default: {
                event.accepted = false
                break
            }
        }
    }

    ListView.onRemove: SequentialAnimation {
        PropertyAction { target: root; property: "ListView.delayRemove"; value: true }
        ParallelAnimation {
            SequentialAnimation {
                PauseAnimation { duration: 50 }
                NumberAnimation {
                    target: root
                    property: "height"
                    to: 0
                    duration: 200
                    easing.type: Easing.OutQuad
                }
            }
            NumberAnimation {
                target: root
                property: "opacity"
                from: 1
                to: 0
                duration: 200
                easing.type: Easing.Linear
            }
        }
        PropertyAction { target: root; property: "ListView.delayRemove"; value: false }
    }

    ListView.onAdd: SequentialAnimation {
        PropertyAction { target: root; property: "height"; value: 0 }
        ParallelAnimation {
            NumberAnimation {
                target: root
                property: "height"
                to: root.height
                duration: 250
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                target: root
                property: "opacity"
                from: 0
                to: 1
                duration: 250
                easing.type: Easing.Linear
            }
        }
    }

    SequentialAnimation {
        id: releasedEffect
        PropertyAnimation {
            target: highlight
            property: "opacity"
            to: 0
            easing.type: Easing.Linear
            duration: 150
        }
    }

    Item {
        // non-visible item to create a padding boundary that content items can bind to
        id: paddingItem
        anchors {
            fill: parent
            leftMargin: platformStyle.paddingLarge
            rightMargin: iconLoader.status == Loader.Ready ?
                    privateStyle.scrollBarThickness + iconLoader.width + platformStyle.paddingMedium :
                    privateStyle.scrollBarThickness
            topMargin: platformStyle.paddingLarge
            bottomMargin: platformStyle.paddingLarge
        }
    }

    StateGroup {
        id: internal

        function getMode() {
            if (internal.state == "Pressed" || internal.state == "PressAndHold")
                return "pressed"
            else if (internal.state == "Focused")
                return "highlighted"
            else if (internal.state == "Disabled")
                return "disabled"
            else
                return "normal"
        }

        // Performance optimization:
        // Use value assignment when property changes instead of binding to js function
        onStateChanged: { root.mode = internal.getMode() }

        function press() {
            privateStyle.play(Symbian.BasicItem)
            highlight.source = privateStyle.imagePath("qtg_fr_list_pressed")
            highlight.opacity = 1
            if (ListView.view)
                ListView.view.currentIndex = index
        }

        function release() {
            if (symbian.listInteractionMode != Symbian.KeyNavigation)
                privateStyle.play(Symbian.BasicItem)
            releasedEffect.restart()
        }

        function hold() {
            root.pressAndHold()
        }

        function disable() {
            faderLoader.opacity = 1
        }

        function focus() {
            faderLoader.opacity = 1
        }

        function canceled() {
            releasedEffect.restart()
        }

        states: [
            State { name: "Pressed" },
            State { name: "PressAndHold" },
            State { name: "Disabled"; when: !root.enabled },
            State { name: "Focused"; when: (root.ListView.isCurrentItem &&
                symbian.listInteractionMode == Symbian.KeyNavigation) },
            State { name: "Canceled" },
            State { name: "" }
        ]

        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press() }
            },
            Transition {
                from: "Pressed"
                to: "PressAndHold"
                ScriptAction { script: internal.hold() }
            },
            Transition {
                to: ""
                ScriptAction { script: internal.release() }
            },
            Transition {
                to: "Disabled"
                ScriptAction { script: internal.disable() }
            },
            Transition {
                to: "Focused"
                ScriptAction { script: internal.focus() }
            },
            Transition {
                to: "Canceled"
                ScriptAction { script: internal.canceled() }
            }
        ]
    }
}
