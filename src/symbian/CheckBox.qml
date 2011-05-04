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
    property bool pressed: stateGroup.state == "Pressed"

    signal clicked

    property alias text: label.text

    QtObject {
        id: internal
        objectName: "internal"

        function bg_postfix() {
            if (!root.enabled) {
                if (root.checked)
                    return "disabled_selected"
                else
                    return "disabled_unselected"
            } else {
                if (root.pressed)
                    return "pressed"
                else if (root.checked)
                    return "normal_selected"
                else
                    return "normal_unselected"
            }
        }

        function press() {
            privateStyle.play(Symbian.BasicItem)
        }

        function toggle() {
            privateStyle.play(Symbian.CheckBox)
            clickedEffect.restart()
            root.checked = !root.checked
            root.clicked()
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed" },
            State { name: "Canceled" }
        ]

        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press() }
            },
            Transition {
                from: "Pressed"
                to: ""
                ScriptAction { script: internal.toggle() }
            }
        ]
    }

    implicitWidth: label.text ? privateStyle.buttonSize + platformStyle.paddingMedium + privateStyle.textWidth(label.text, label.font)
                              : privateStyle.buttonSize
    implicitHeight: privateStyle.menuItemHeight

    Image {
        id: contentIcon
        source: privateStyle.imagePath("qtg_graf_checkbox_" + internal.bg_postfix());
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        sourceSize.width: privateStyle.buttonSize
        sourceSize.height: privateStyle.buttonSize
        smooth: true
    }

    Text {
        id: label
        anchors.left: contentIcon.right
        anchors.leftMargin: text ? platformStyle.paddingMedium : 0
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        elide: Text.ElideRight
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
        color: platformStyle.colorNormalLight
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        SequentialAnimation {
            id: clickedEffect
            PropertyAnimation {
                target: contentIcon
                property: "scale"
                from: 1.0
                to: 0.8
                easing.type: Easing.Linear
                duration: 50
            }
            PropertyAnimation {
                target: contentIcon
                property: "scale"
                from: 0.8
                to: 1.0
                easing.type: Easing.OutQuad
                duration: 170
            }
        }

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
    }
}
