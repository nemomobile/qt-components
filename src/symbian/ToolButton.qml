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
import Qt.labs.components 1.0
import "." 1.0

ImplicitSizeItem {
    id: root

    // Common API
    property alias checkable: checkableItem.enabled
    property alias checked: checkableItem.checked
    property bool enabled: true // overridden from base class
    property alias text: label.text
    property alias iconSource: contentIcon.source
    property bool flat: (!text && iconSource != "" && parent && !internal.isButtonRow(parent))
    property bool pressed: mouseArea.containsMouse && (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold")

    // Platform API
    property alias platformExclusiveGroup: checkableItem.exclusiveGroup

    // Common API
    signal clicked

    // Platform API
    signal platformReleased
    signal platformPressAndHold

    onFlatChanged: {
        background.visible = !flat
    }

    implicitWidth: {
        if (!text)
            return internal.iconButtonWidth()
        else if (iconSource == "")
            return Math.max(internal.iconButtonWidth(), internal.textButtonWidth())
        else
            return internal.iconButtonWidth() + internal.textButtonWidth()
    }
    implicitHeight: {
        if ((iconSource != "") && !text)
            // if there is just an icon, then it's full button height
            return internal.iconButtonWidth()
        else
            // Otherwise frame height is always tool bar's height in landscape, regardless of the current orientation
            return privateStyle.toolBarHeightLandscape
    }

    BorderImage {
        id: background
        source: privateStyle.imagePath(internal.imageName() + internal.modeName())
        border {
            left: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
            top: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
            right: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
            bottom: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
        }
        smooth: true
        anchors.fill: parent
        visible: !flat
        BorderImage {
            id: highlight
            border {
                left: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
                top: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
                right: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
                bottom: internal.isFrameGraphic ? platformStyle.borderSizeMedium : 0;
            }
            smooth: true
            anchors.fill: background
            opacity: 0
        }
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
        smooth: true
    }

    Text {
        id: label
        clip: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
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
        smooth: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: if (root.enabled) stateGroup.state = "Pressed"
        onReleased: if (root.enabled) stateGroup.state = ""
        onCanceled: {
            if (root.enabled) {
                // Mark as canceled
                stateGroup.state = "Canceled"
                // Reset state. Can't expect a release since mouse was ungrabbed
                stateGroup.state = ""
            }
        }
        onPressAndHold: if (stateGroup.state != "Canceled" && root.enabled) stateGroup.state = "PressAndHold"
        onExited: if (root.enabled) stateGroup.state = "Canceled"
    }

    QtObject {
        id: internal

        property bool isFrameGraphic : imageName().search("_fr") > 0

        function belongsToExclusiveGroup() {
            return checkableItem.exclusiveGroup
                   || (root.parent
                   && root.parent.hasOwnProperty("checkedButton")
                   && root.parent.exclusive)
        }

        function modeName() {
            if (isButtonRow(parent))
                return parent.privateModeName(root, 1)
            else if (!enabled)
                return "disabled"
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
            if (!belongsToExclusiveGroup()) {
                if (checkableItem.enabled && checkableItem.checked)
                    privateStyle.play(Symbian.SensitiveButton)
                else
                    privateStyle.play(Symbian.BasicButton)
            } else if (checkableItem.enabled && !checkableItem.checked) {
                privateStyle.play(Symbian.BasicButton)
            }

            if (flat)
                background.visible = true
            highlight.source = privateStyle.imagePath(internal.imageName() + "pressed")
            label.scale = 0.95
            contentIcon.scale = 0.95
            highlight.opacity = 1
        }

        function release() {
            label.scale = 1
            contentIcon.scale = 1
            highlight.opacity = 0
            if (flat)
                visibleEffect.restart()
            root.platformReleased()
        }

        function click() {
            if ((checkableItem.enabled && checkableItem.checked && !belongsToExclusiveGroup()) || !checkableItem.enabled)
                privateStyle.play(Symbian.BasicButton)
            if (flat)
                visibleEffect.restart()
            else
                clickedEffect.restart()
            checkableItem.toggle()
            root.clicked()
        }

        function hold() {
            root.platformPressAndHold()
        }

        function isButtonRow(item) {
            return (item &&
                    item.hasOwnProperty("checkedButton") &&
                    item.hasOwnProperty("privateDirection") &&
                    item.privateDirection == Qt.Horizontal)
        }

        // The function imageName() handles fetching correct graphics for the ToolButton.
        // If the parent of a ToolButton is ButtonRow, segmented-style graphics are used to create a
        // seamless row of buttons. Otherwise normal ToolButton graphics are utilized.
        function imageName() {
            if (isButtonRow(parent))
                return parent.privateGraphicsName(root, 1)
            else
                return (!flat || text || iconSource == "") ? "qtg_fr_toolbutton_" : "qtg_graf_toolbutton_"
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

    ParallelAnimation {
        id: clickedEffect
        PropertyAnimation {
            target: label
            property: "scale"
            from: 0.95
            to: 1.0
            easing.type: Easing.Linear
            duration: 100
        }
        PropertyAnimation {
            target: contentIcon
            property: "scale"
            from: 0.95
            to: 1.0
            easing.type: Easing.Linear
            duration: 100
        }
        PropertyAnimation {
            target: highlight
            property: "opacity"
            from: 1
            to: 0
            easing.type: Easing.Linear
            duration: 150
        }
    }

    SequentialAnimation {
        id: visibleEffect
        ParallelAnimation {
            PropertyAnimation {
                target: label
                property: "scale"
                from: 0.95
                to: 1.0
                easing.type: Easing.Linear
                duration: 100
            }
            PropertyAnimation {
                target: contentIcon
                property: "scale"
                from: 0.95
                to: 1.0
                easing.type: Easing.Linear
                duration: 100
            }
            PropertyAnimation {
                target: highlight
                property: "opacity"
                from: 1
                to: 0
                easing.type: Easing.Linear
                duration: 150
            }
        }
        PropertyAction {
            target: background
            property: "visible"
            value: false
        }
    }

    Checkable {
        id: checkableItem
        value: root.text
    }
}
