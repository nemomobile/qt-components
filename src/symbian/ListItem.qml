/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
    id: listItem
    property variant style: itemStyle
    property alias padding: paddingRectangle
    property real verticalSpacing: itemStyle.current.get("verticalSpacing")
    property real horizontalSpacing: itemStyle.current.get("horizontalSpacing")
    property bool enabled: true

    signal clicked
    signal pressAndHold

    function preferredImageHeight(ListImageSize) {
        imageStyle.imageSize = ListImageSize
        return imageStyle.current.preferredHeight
    }

    function preferredImageWidth(ListImageSize) {
        imageStyle.imageSize = ListImageSize
        return imageStyle.current.preferredWidth
    }

    clip: true
    implicitWidth: itemStyle.current.preferredWidth
    implicitHeight: itemStyle.current.preferredHeight

    Style {
        id: itemStyle
        styleClass: "ListItem"
        styleObjectName: "ListItem"
        mode: {
            if (internal.state == "Pressed" || internal.state == "PressAndHold")
                return "pressed"
            else if (internal.state == "Focused")
                return "focused"
            else if (internal.state == "Disabled")
                return "disabled"
            else
                return "default"
        }
    }

    Style {
        id: imageStyle
        styleClass: "ListItemImage"
        property int imageSize: Symbian.Undefined
    }

    BorderImage {
        id: frame
        source: itemStyle.current.get("background")
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent

        BorderImage {
            id: frameFader
            opacity: 0
            anchors.fill: frame
            source: itemStyle.current.get("faderBackground")
            border { left: 20; top: 20; right: 20; bottom: 20 }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: listItem.enabled
        onPressed: {
            symbian.listInteractionMode = Symbian.TouchInteraction
            internal.state = "Pressed"
        }
        onClicked: {
            internal.state = ""
            listItem.clicked()
        }
        onCanceled: {
            internal.state = ""
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

    Keys.onReleased: {
        if (listItem.enabled) {
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
                    if (listItem.enabled)
                        listItem.clicked()
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
        PropertyAction { target: listItem; property: "ListView.delayRemove"; value: true }
        NumberAnimation { target: listItem; property: "height"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
        PropertyAction { target: listItem; property: "ListView.delayRemove"; value: false }
    }

    ListView.onAdd: SequentialAnimation {
         PropertyAction { target: listItem; property: "height"; value: 0 }
         NumberAnimation { target: listItem; property: "height"; to: listItem.height; duration: 500; easing.type: Easing.InOutQuad }
    }

    SequentialAnimation {
        id: pressedEffect
        PropertyAnimation { target: frameFader; property: "opacity"; to: 1; easing.type: Easing.Linear; duration: 100 }
    }

    SequentialAnimation {
        id: releasedEffect
        PropertyAnimation { target: frameFader; property: "opacity"; to: 0; easing.type: Easing.Linear; duration: 100 }
    }

    Item {
        // non-visible item to create a padding boundary that content items can bind to
        id: paddingRectangle
        anchors {
            fill: parent
            leftMargin: itemStyle.current.get("marginLeft")
            rightMargin: itemStyle.current.get("marginRight")
            topMargin: itemStyle.current.get("marginTop")
            bottomMargin: itemStyle.current.get("marginBottom")
        }
        width: itemStyle.current.get("marginLeft") + itemStyle.current.get("marginRight")
        height: itemStyle.current.get("marginTop") + itemStyle.current.get("marginBottom")
    }

    Item {
        id: internal

        function press() {
            pressedEffect.restart()
            itemStyle.play(Symbian.BasicItem)
            if (ListView.view)
                ListView.view.currentIndex = index
        }

        function release() {
            releasedEffect.restart()
            itemStyle.play(Symbian.BasicItem)
        }

        function hold() {
            listItem.pressAndHold()
        }

        function disable() {
            frameFader.opacity = 1
        }

        function focus() {
            frameFader.opacity = 1
        }

        states: [
            State { name: "Pressed" },
            State { name: "PressAndHold" },
            State { name: "Disabled"; when: !listItem.enabled },
            State { name: "" },
            State { name: "Focused"; when: (listItem.ListView.isCurrentItem &&
                symbian.listInteractionMode == Symbian.KeyNavigation) }
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
            }
        ]
    }
}
