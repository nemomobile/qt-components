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
import Qt.labs.components 1.0
import "." 1.0

ImplicitSizeItem {
    id: toolButton

    property alias checkable: checkable.enabled
    property alias checked: checkable.checked
    property bool enabled: true

    property alias exclusiveGroup: checkable.exclusiveGroup
    property alias text: label.text

    property string iconSource
    property string pressedIconSource
    property string checkedIconSource

    property bool flat: false

    property bool pressed: mouseArea.containsMouse && (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold")
    signal clicked
    signal released
    signal pressAndHold

    implicitWidth: {
        if (!text)
            return internal.iconButtonWidth()
        else if (!iconSource)
            return Math.max(internal.iconButtonWidth(), internal.textButtonWidth())
        else
            return internal.iconButtonWidth() + internal.textButtonWidth()
    }
    implicitHeight: internal.iconButtonWidth()

    BorderImage {
        id: background
        source: privateStyle.imagePath("qtg_fr_toolbutton_" + internal.mode())
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
        visible: internal.mode() != "normal" || !flat
    }

    Image {
        id: contentIcon
        visible: iconSource
        sourceSize.width: platformStyle.graphicSizeSmall
        sourceSize.height: platformStyle.graphicSizeSmall
        source: {
            if (internal.mode() == "latched" && checkedIconSource)
                return checkedIconSource
            else if (internal.mode() == "pressed" && pressedIconSource)
                return pressedIconSource
            else
                return iconSource
        }
        anchors {
            centerIn: !text ? parent : undefined
            verticalCenter: !text ? undefined : parent.verticalCenter
            left: !text ? undefined : parent.left
            leftMargin: !text ? 0 : 2 * platformStyle.paddingMedium
        }
    }

    Text {
        id: label
        clip: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
        color: {
            if (!enabled)
                platformStyle.colorDisabledLight
            else if (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold")
                platformStyle.colorPressed
            else
                platformStyle.colorNormalLight
        }
        visible: text
        anchors {
            top: parent.top; bottom: parent.bottom
            left: iconSource ? contentIcon.right : parent.left; right: parent.right
            leftMargin: iconSource ? platformStyle.paddingSmall : platformStyle.paddingMedium
            rightMargin: platformStyle.paddingMedium
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: if (toolButton.enabled) stateGroup.state = "Pressed"
        onReleased: if (toolButton.enabled) stateGroup.state = ""
        onCanceled: {
            if (toolButton.enabled) {
                stateGroup.state = ""
                internal.release()
            }
        }
        onPressAndHold: if (stateGroup.state != "Canceled" && toolButton.enabled) stateGroup.state = "PressAndHold"
        onExited: if (toolButton.enabled) stateGroup.state = "Canceled"
    }

    QtObject {
        id: internal

        function mode() {
            if (!enabled)
                return "disabled"
            else if (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold")
                return "pressed"
            else if (checkable.checked)
                return "latched"
            else
                return "normal"
        }

        function iconButtonWidth() {
            return (screen.width < screen.height) ? privateStyle.toolBarHeightPortrait : privateStyle.toolBarHeightLandscape
        }

        function textButtonWidth() {
            return platformStyle.paddingMedium * ((screen.width < screen.height) ? 15 : 25)
        }

        function press() {
            if (checkable.enabled && checkable.checked)
                privateStyle.play(Symbian.SensitiveButton)
            else
                privateStyle.play(Symbian.BasicButton)
        }

        function release() {
            toolButton.released()
        }

        function click() {
            checkable.toggle()
            toolButton.clicked()

            if (!checkable.enabled || !checkable.checked)
                privateStyle.play(Symbian.BasicButton)
        }

        function hold() {
            if (toolButton.longPress)
                toolButton.pressAndHold()
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed" },
            State { name: "PressAndHold" },
            State { name: "Canceled" }
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
                from: "Pressed"
                to: ""
                ScriptAction { script: internal.release() }
                ScriptAction { script: internal.click() }
            },
            Transition {
                from: "PressAndHold"
                to: ""
                ScriptAction { script: internal.release() }
                ScriptAction { script: internal.click() }
            },
            Transition {
                from: "Pressed"
                to: "Canceled"
                ScriptAction { script: internal.release() }
            },
            Transition {
                from: "PressAndHold"
                to: "Canceled"
                ScriptAction { script: internal.release() }
            }
        ]
    }

    Checkable {
        id: checkable
        value: toolButton.text
    }
}
