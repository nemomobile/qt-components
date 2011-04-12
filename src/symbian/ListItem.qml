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

ImplicitSizeItem {
    id: root
    property alias style: itemStyle // deprecated
    onStyleChanged: { console.log("warning: ListItem.style is deprecated.") }
    property string mode: internal.getMode() // Read-only
    property alias padding: paddingItem // Read-only
    onPaddingChanged: { console.log("warning: ListItem.padding is deprecated. Use ListItem.paddigItem instead") }
    property alias paddingItem: paddingItem // Read-only

    property real verticalSpacing: 0 // deprecated
    property real horizontalSpacing: platformStyle.paddingMedium // deprecated
    property bool enabled: true
    property bool subItemIndicator: false
    property bool drillDownIndicator: false // deprecated
    onDrillDownIndicatorChanged: { // TODO: Remove after grace period
        console.log("ListItem.drillDownIndicator deprecated, use ListItem.subItemIndicator instead!");
        subItemIndicator = drillDownIndicator
    }

    signal clicked
    signal pressAndHold

    function preferredImageHeight(ListImageSize) { // deprecated
        console.log("warning: ListItem.preferredImageHeight is deprecated. Use platformStyle context properties instead")
        switch (ListImageSize) {
            case Symbian.Small: return platformStyle.graphicSizeTiny; break
            case Symbian.Medium: return platformStyle.graphicSizeSmall; break
            case Symbian.Large: return platformStyle.graphicSizeMedium; break
            case Symbian.ImagePortrait: return platformStyle.graphicSizeLarge; break
            default: return 0
        }
    }

    function preferredImageWidth(ListImageSize) { // deprecated
        console.log("warning: ListItem.preferredImageWidth is deprecated. Use platformStyle context properties instead")
        switch (ListImageSize) {
            case Symbian.Small: return platformStyle.graphicSizeTiny; break
            case Symbian.Medium: return platformStyle.graphicSizeSmall; break
            case Symbian.Large: return platformStyle.graphicSizeMedium; break
            case Symbian.ImagePortrait: return platformStyle.graphicSizeLarge; break
            default: return 0
        }
    }

    implicitWidth: ListView.view ? ListView.view.width : screen.width
    implicitHeight: platformStyle.graphicSizeLarge

    Style {
        id: itemStyle
        styleClass: "ListItem"
        styleObjectName: "ListItem"
        mode: internal.getMode()
    }

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
            sourceComponent: root.mode != "normal" ? fader : undefined
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
                    if (root.enabled)
                        root.clicked()
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
        PropertyAction { target: root; property: "clip"; value: true }
        PropertyAction { target: root; property: "ListView.delayRemove"; value: true }
        NumberAnimation { target: root; property: "height"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
        PropertyAction { target: root; property: "ListView.delayRemove"; value: false }
    }

    ListView.onAdd: SequentialAnimation {
         PropertyAction { target: root; property: "height"; value: 0 }
         NumberAnimation { target: root; property: "height"; to: root.height; duration: 500; easing.type: Easing.InOutQuad }
    }

    SequentialAnimation {
        id: pressedEffect
        PropertyAnimation { target: faderLoader; property: "opacity"; to: 1; easing.type: Easing.Linear; duration: 100 }
    }

    SequentialAnimation {
        id: releasedEffect
        PropertyAnimation { target: faderLoader; property: "opacity"; to: 0; easing.type: Easing.Linear; duration: 100 }
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
                return "highlight"
            else if (internal.state == "Disabled")
                return "disabled"
            else
                return "normal"
        }

        function press() {
            pressedEffect.restart()
            privateStyle.play(Symbian.BasicItem)
            if (ListView.view)
                ListView.view.currentIndex = index
        }

        function release() {
            releasedEffect.restart()
            privateStyle.play(Symbian.BasicItem)
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
