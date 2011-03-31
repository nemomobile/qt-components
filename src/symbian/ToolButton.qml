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
import Qt.labs.components 1.0
import "." 1.0

ImplicitSizeItem {
    id: root

    // Common API
    property alias checkable: checkable.enabled
    property alias checked: checkable.checked
    property bool enabled: true // overridden from base class
    property alias text: label.text
    property alias iconSource: contentIcon.source
    property bool flat: false
    property bool pressed: mouseArea.containsMouse && (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold")

    // Platform API
    property alias platformExclusiveGroup: checkable.exclusiveGroup

    // Common API
    signal clicked

    // Platform API
    signal platformReleased
    signal platformPressAndHold

    // Deprecated on w13 ->
    property QtObject exclusiveGroup
    onExclusiveGroupChanged: {
        console.log("ToolButton.exclusiveGroup is deprecated. Use platformExclusiveGroup instead.")
        platformExclusiveGroup = exclusiveGroup
    }
    property url pressedIconSource
    onPressedIconSourceChanged: {
        console.log("ToolButton.pressedIconSource is deprecated. Use onPressedChanged to control the iconSource.")
    }

    property url checkedIconSource
    onCheckedIconSourceChanged: {
        console.log("ToolButton.checkedIconSource is deprecated. Use onCheckedChanged to control the iconSource.")
    }

    signal released
    signal pressAndHold
    // <- Deprecated on w13

    implicitWidth: {
        if (!text)
            return internal.iconButtonWidth()
        else if (iconSource == "")
            return Math.max(internal.iconButtonWidth(), internal.textButtonWidth())
        else
            return internal.iconButtonWidth() + internal.textButtonWidth()
    }
    implicitHeight: {
        if (!text)
            return internal.iconButtonWidth()
        else
            //Text button's frame height is always tool bar's height in landscape, regardless of the current orientation
            return privateStyle.toolBarHeightLandscape
    }

    BorderImage {
        id: background
        source: internal.imageSource()
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
        visible: internal.mode() != "normal" || !flat
    }

    Image {
        id: contentIcon
        visible: iconSource != ""
        sourceSize.width: platformStyle.graphicSizeSmall
        sourceSize.height: platformStyle.graphicSizeSmall
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
            left: iconSource != "" ? contentIcon.right : parent.left; right: parent.right
            leftMargin: iconSource != "" ? platformStyle.paddingSmall : platformStyle.paddingMedium
            rightMargin: platformStyle.paddingMedium
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: if (root.enabled) stateGroup.state = "Pressed"
        onReleased: if (root.enabled) stateGroup.state = ""
        onCanceled: {
            if (root.enabled) {
                stateGroup.state = ""
                internal.release()
            }
        }
        onPressAndHold: if (stateGroup.state != "Canceled" && root.enabled) stateGroup.state = "PressAndHold"
        onExited: if (root.enabled) stateGroup.state = "Canceled"
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
            root.platformReleased()
            root.released()
        }

        function click() {
            checkable.toggle()
            root.clicked()

            if (!checkable.enabled || !checkable.checked)
                privateStyle.play(Symbian.BasicButton)
        }

        function hold() {
            if (root.longPress) {
                root.platformPressAndHold()
                root.pressAndHold()
            }
        }

        // The function imageSource() handles fetching correct graphics for the ToolButton.
        // If the parent of a ToolButton is ButtonRow, segmented-style graphics are used to create a
        // seamless row of buttons. Otherwise normal ToolButton graphics are utilized.
        function imageSource() {
            if (parent && parent.hasOwnProperty("checkedButton") && parent.hasOwnProperty("__direction") && parent.__direction == Qt.Horizontal && parent.children.length > 1) {
                var imageName = "qtg_fr_toolbutton_segmented"
                if (root === parent.children[0])
                    imageName += "_l_"
                else if(root === parent.children[parent.children.length - 1])
                    imageName += "_r_"
                else
                    imageName += "_c_"

                return privateStyle.imagePath(imageName + internal.mode())
            }

            return privateStyle.imagePath("qtg_fr_toolbutton_" + internal.mode())
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
        value: root.text
    }
}
