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

    // Common Public API
    property alias checked: checkable.checked
    property bool pressed: stateGroup.state == "Pressed"
    signal clicked
    property alias text: label.text

    // Symbian specific API
    property alias platformExclusiveGroup: checkable.exclusiveGroup

    QtObject {
        id: internal
        objectName: "internal"

        function press() {
            privateStyle.play(Symbian.BasicItem)
        }

        function toggle() {
            privateStyle.play(Symbian.CheckBox)
            clickedEffect.restart()
            checkable.toggle()
            root.clicked()
        }

        function icon_postfix() {
            if (pressed)
                return "pressed"
            else if (root.checked) {
                if (!root.enabled)
                    return "disabled_selected"
                else
                    return "normal_selected"
            } else {
                if (!root.enabled)
                    return "disabled_unselected"
                else
                    return "normal_unselected"
            }
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

    implicitWidth: privateStyle.textWidth(label.text, label.font) + platformStyle.paddingMedium + privateStyle.buttonSize
    implicitHeight: privateStyle.buttonSize

    Image {
        id: image
        source: privateStyle.imagePath("qtg_graf_radiobutton_" + internal.icon_postfix())
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        sourceSize.width: privateStyle.buttonSize
        sourceSize.height: privateStyle.buttonSize
    }
    Text {
        id: label
        elide: Text.ElideRight
        anchors.left: image.right
        anchors.leftMargin: platformStyle.paddingMedium
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
        color: {
            if (!root.enabled)
                platformStyle.colorDisabledLight
            else if (pressed)
                platformStyle.colorPressed
            else
                platformStyle.colorNormalLight
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

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

    ParallelAnimation {
        id: clickedEffect
        SequentialAnimation {
            PropertyAnimation {
                target: image
                property: "scale"
                from: 1.0
                to: 0.8
                easing.type: Easing.Linear
                duration: 50
            }
            PropertyAnimation {
                target: image
                property: "scale"
                from: 0.8
                to: 1.0
                easing.type: Easing.OutQuad
                duration: 170
            }
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
            checkable.toggle()
            clickedEffect.restart()
            root.clicked()
            event.accepted = true
        }
    }

    Checkable {
        id: checkable
        value: root.text
        enabled: true
    }
}
