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

    // Common Public API
    property bool checked: false
    signal clicked
    // Symbian specific
    property bool pressed: stateGroup.state == "Pressed" || stateGroup.state == "Dragging"
    property alias text: label.text

    QtObject {
        id: internal
        objectName: "internal"

        property bool canceled

        function press() {
            internal.canceled = false;
            privateStyle.play(Symbian.BasicItem);
        }

        function toggle() {
            root.checked = !root.checked;
            root.clicked();
            privateStyle.play(Symbian.CheckBox);
        }

        function cancel() {
            internal.canceled = true;
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed" },
            State { name: "Dragging" },
            State { name: "Canceled" }
        ]

        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press(); }
            },
            Transition {
                from: "Pressed"
                to: ""
                ScriptAction { script: internal.toggle(); }
            },
            Transition {
                to: "Canceled"
                ScriptAction { script: internal.cancel(); }
            }
        ]
    }

    implicitWidth: label.text ? 2 * privateStyle.switchButtonHeight + platformStyle.paddingMedium + privateStyle.textWidth(label.text, label.font)
                              : 2 * privateStyle.switchButtonHeight
    implicitHeight: privateStyle.switchButtonHeight

    Image {
        id: track

        function trackPostfix() {
            if (!root.enabled && root.checked)
                return "disabled_on"
            else if (!root.enabled && !root.checked)
                return "disabled_off"
            else
                return "track"
        }

        source: privateStyle.imagePath("qtg_graf_switchbutton_" + track.trackPostfix());
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        sourceSize.width: 2 * privateStyle.switchButtonHeight
        sourceSize.height: privateStyle.switchButtonHeight
    }

    Item {
        id: fill
        clip: true
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: privateStyle.switchButtonHeight
        width: handle.x + handle.width / 2 - mouseArea.drag.minimumX
        visible: root.enabled

        Image {
            source: privateStyle.imagePath("qtg_graf_switchbutton_fill");
            anchors.left: parent.left
            anchors.top: parent.top
            height: parent.height
        }
    }

    Image {
        id: handle
        source: privateStyle.imagePath("qtg_graf_switchbutton_" + (root.pressed ? "handle_pressed" : "handle_normal"));
        anchors.verticalCenter: root.verticalCenter
        sourceSize.width: privateStyle.switchButtonHeight
        sourceSize.height: privateStyle.switchButtonHeight
        visible: root.enabled

        states: [
            State {
                name: "Off"
                when: !mouseArea.drag.active && !checked
                PropertyChanges {
                    target: handle
                    restoreEntryValues: false
                    x: mouseArea.drag.minimumX
                }
            },
            State {
                name: "On"
                when: !mouseArea.drag.active && checked
                PropertyChanges {
                    target: handle
                    restoreEntryValues: false
                    x: mouseArea.drag.maximumX
                }
            }
        ]

        transitions: [
            Transition {
                to: "Off"
                SmoothedAnimation {properties: "x"; easing.type: Easing.InOutQuad; duration: 200 }
            },
            Transition {
                to: "On"
                SmoothedAnimation {properties: "x"; easing.type: Easing.InOutQuad; duration: 200 }
            }
        ]
    }

    Text {
        id: label
        anchors {
            left: track.right
            leftMargin: text ? platformStyle.paddingMedium : 0
            rightMargin: text ? platformStyle.paddingLarge : 0
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        elide: Text.ElideRight
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
        color: platformStyle.colorNormalLight
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        property bool overHandle: mouseX >= handle.x && mouseX <= handle.x + handle.width && mouseY >= handle.y && mouseY <= handle.y + handle.height

        onPressed: stateGroup.state = "Pressed"
        onReleased: stateGroup.state = ""
        onClicked: stateGroup.state = ""
        onExited: stateGroup.state = "Canceled"
        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled"
            // Reset state. Can't expect a release since mouse was ungrabbed
            stateGroup.state = ""
        }

        drag {
            axis: Drag.XAxis
            target: mouseArea.pressed && overHandle ? handle : null
            minimumX: track.x
            maximumX: mouseArea.drag.minimumX + track.width - handle.width

            onActiveChanged: {
                if (mouseArea.drag.active)
                    stateGroup.state = "Dragging";
                else if (!mouseArea.drag.active && !internal.canceled &&
                         root.checked != (handle.x > mouseArea.drag.minimumX + (mouseArea.drag.maximumX -  mouseArea.drag.minimumX) / 2))
                    internal.toggle()
            }
        }
    }
}
