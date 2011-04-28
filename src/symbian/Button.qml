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
    id: button

    // Common Public API
    property bool checked: false
    property bool checkable: false
    property bool pressed: (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold") && mouseArea.containsMouse
    property alias text: label.text
    property alias iconSource: icon.source
    property alias font: label.font

    signal clicked

    // Symbian specific signals and properties
    signal platformReleased
    signal platformPressAndHold

    property bool platformAutoRepeat: false
    property bool platformLongPress: false

    implicitWidth: {
        var prefWidth = 20;

        if (iconSource != "" && text)
            prefWidth = icon.sourceSize.width > privateStyle.textWidth(label.text, label.font)
                ? icon.anchors.leftMargin + icon.sourceSize.width + icon.anchors.rightMargin
                : label.anchors.leftMargin + privateStyle.textWidth(label.text, label.font) + label.anchors.rightMargin
        else if (iconSource != "")
            prefWidth = icon.anchors.leftMargin + icon.sourceSize.width + icon.anchors.rightMargin;
        else if (text)
            prefWidth = icon.anchors.leftMargin + privateStyle.textWidth(label.text, label.font) + label.anchors.rightMargin;

        return prefWidth;
    }

    implicitHeight: {
        var prefHeight = icon.anchors.topMargin + icon.anchors.bottomMargin;

        if (iconSource != "" && text)
            prefHeight = prefHeight + icon.sourceSize.height + privateStyle.fontHeight(label.font);
        else if (iconSource != "")
            prefHeight = prefHeight + icon.sourceSize.height;
        else if (text)
            prefHeight = prefHeight + privateStyle.fontHeight(label.font);

        return prefHeight;
    }

    QtObject {
        id: internal
        objectName: "internal"

        property int autoRepeatInterval: 60

        function bg_postfix() {
            if (activeFocus && checked)
                return "pressed"
            else if (checked)
                return "latched"
            else
                return "normal"
        }

        function toggleChecked() {
            if (checkable)
                checked = !checked;
        }

        function press() {
            if (checkable && checked)
                privateStyle.play(Symbian.SensitiveButton);
            else
                privateStyle.play(Symbian.BasicButton);
            highlight.source = privateStyle.imagePath(internal.imageName() + "pressed")
            layout.scale = 0.95;
            highlight.opacity = 1;
        }

        function release() {
            layout.scale = 1;
            highlight.opacity = 0;
            if (tapRepeatTimer.running)
                tapRepeatTimer.stop();
            button.platformReleased();
        }

        function click() {
            if (!checkable || (checkable && !checked))
                privateStyle.play(Symbian.BasicButton);
            internal.toggleChecked();
            clickedEffect.restart();
            button.clicked();
        }

        function hold() {
            // If autorepeat is enabled, do not emit long press, but repeat the tap action.
            if (button.platformAutoRepeat)
                tapRepeatTimer.start();

            if (button.platformLongPress)
                button.platformPressAndHold();
        }

        function repeat() {
            if (!checkable)
                privateStyle.play(Symbian.SensitiveButton);
            button.clicked();
        }

        // The function imageName() handles fetching correct graphics for the Button.
        // If the parent of a Button is ButtonRow, segmented-style graphics are used to create a
        // seamless row of buttons. Otherwise normal Button graphics are utilized.
        function imageName() {
            if (parent && parent.hasOwnProperty("checkedButton") && parent.hasOwnProperty("__direction") && parent.__direction == Qt.Horizontal && parent.children.length > 1)
                return parent.__graphicsName(button, 0)
            return "qtg_fr_pushbutton_"
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed"; },
            State { name: "PressAndHold"; },
            State { name: "Canceled"; }
        ]

        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press(); }
            },
            Transition {
                from: "Pressed"
                to: "PressAndHold"
                ScriptAction { script: internal.hold(); }
            },
            Transition {
                from: "Pressed"
                to: ""
                ScriptAction { script: internal.release(); }
                ScriptAction { script: internal.click(); }
            },
            Transition {
                from: "PressAndHold"
                to: ""
                ScriptAction { script: internal.release(); }
            },
            Transition {
                from: "Pressed"
                to: "Canceled"
                ScriptAction { script: internal.release(); }
            },
            Transition {
                from: "PressAndHold"
                to: "Canceled"
                ScriptAction { script: internal.release(); }
            }
        ]
    }

    BorderImage {
        source: privateStyle.imagePath(internal.imageName() + internal.bg_postfix())
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent

        BorderImage {
            id: highlight
            border { left: 20; top: 20; right: 20; bottom: 20 }
            opacity: 0
            anchors.fill: parent
        }
    }

    Item {
        id: layout
        width: implicitWidth < button.width ? implicitWidth : button.width
        height: implicitHeight < button.height ? implicitHeight : button.height
        anchors.centerIn: parent

        Image {
            id: icon
            sourceSize.width : platformStyle.graphicSizeSmall
            sourceSize.height : platformStyle.graphicSizeSmall
            fillMode: Image.PreserveAspectFit
            smooth: true

            anchors {
                horizontalCenter: layout.horizontalCenter
                verticalCenter: layout.verticalCenter
                verticalCenterOffset: text ? -platformStyle.paddingLarge : 0
                margins: platformStyle.paddingLarge
            }
        }

        Text {
            id: label
            elide: layout.width >= button.width ? Text.ElideRight : Text.ElideNone
            anchors {
                leftMargin: platformStyle.paddingLarge
                rightMargin: platformStyle.paddingLarge
                left: layout.left
                right: layout.right
                top: iconSource != "" ? icon.bottom : layout.top
                bottom: layout.bottom
            }

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
            color: platformStyle.colorNormalLight
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        onPressed: stateGroup.state = "Pressed"

        onReleased: stateGroup.state = ""

        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled";
            // Reset state. Can't expect a release since mouse was ungrabbed
            stateGroup.state = "";
        }

        onPressAndHold: {
            if (stateGroup.state != "Canceled" && (platformLongPress || platformAutoRepeat))
                stateGroup.state = "PressAndHold";
        }

        onExited: stateGroup.state = "Canceled"
    }

    Timer {
        id: tapRepeatTimer

        interval: internal.autoRepeatInterval; running: false; repeat: true
        onTriggered: internal.repeat()
    }

    ParallelAnimation {
        id: clickedEffect
        PropertyAnimation {
            target: layout
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

    Keys.onPressed: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
            stateGroup.state = "Pressed";
            event.accepted = true;
        }
    }

    Keys.onReleased: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
            stateGroup.state = "";
            event.accepted = true;
        }
    }
}
