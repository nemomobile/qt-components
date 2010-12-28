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
import com.nokia.symbian.themebridge 1.0

ImplicitSizeItem {
    id: button

    // Common Public API
    property bool checked: false
    property bool checkable: false
    property bool pressed: (state == "Pressed" || state == "PressAndHold") && mouseArea.containsMouse
    property alias text: label.text
    property alias iconSource: contentIcon.iconName; // theme id or absolute filename

    signal clicked
    signal released
    signal pressAndHold

    property bool autoRepeat: false
    property int autoRepeatInterval: 50
    property bool longPress: false

    property variant __style: style

    implicitWidth: calculateImplicitWidth()
    implicitHeight: calculateImplicitHeight()

    function calculateImplicitWidth() {
        var prefWidth = 20;

        if (iconSource && text)
            // leftMargin + iconWidth + padding + textWidth + rightMargin
            prefWidth = style.current.get("iconMarginLeft") + style.current.get("iconWidth") + style.current.get("textMarginLeftInner")
                        + style.textWidth(label.text, label.font) + style.current.get("textMarginRight");
        else if (iconSource)
            // leftMargin + iconWidth + rightMargin
            prefWidth = style.current.get("iconMarginLeft") + style.current.get("iconWidth") + style.current.get("iconMarginRight");
        else if (text)
            // leftMargin + textWidth + rightMargin
            prefWidth = style.current.get("textMarginLeft") + style.textWidth(label.text, label.font) + style.current.get("textMarginRight");

        return prefWidth;
    }

    function calculateImplicitHeight() {
        var prefHeight = style.current.get("iconMarginTop") + style.current.get("iconMarginBottom");

        if (iconSource && text)
            prefHeight = prefHeight + Math.max(style.current.get("iconHeight"), style.fontHeight(label.font));
        else if (iconSource)
            prefHeight = prefHeight + style.current.get("iconHeight");
        else if (text)
            prefHeight = prefHeight + style.fontHeight(label.font);

        return prefHeight;
    }

    function toggleChecked() {
        if (checkable)
            checked = !checked;
    }

    function press() {
        // haptics
        if (checkable && checked) {
            // TODO: style.play(ThemeEffect.SensitiveButton);
        } else {
            // TODO: style.play(ThemeEffect.BasicButton);
        }
    }

    function release() {
        if (tapRepeatTimer.running)
            tapRepeatTimer.stop();
        button.released();
    }

    function click() {
        button.toggleChecked();

        // Just to show some effect, TODO: real effect from theme
        clickedEffect.restart();

        // release haptics on succesfull click
        if (!checkable || (checkable && !checked)) {
            // TODO: style.play(ThemeEffect.BasicButton);
        }

        // emit signal
        button.clicked();
    }

    function hold() {
        // If autorepeat is enabled, do not emit long press, but repeat the tap action.
        if (button.autoRepeat)
            tapRepeatTimer.start();

        if (button.longPress) // otherwise emit pressAndHold signal
            button.pressAndHold();
    }

    function repeat() {
        if (!checkable) {
            // TODO: style.play(ThemeEffect.SensitiveButton);
        }
        // emit signal
        button.clicked();
    }

    Style {
        id: style
        styleClass: "Button"
        mode: {
            if (state == "Pressed" || state == "PressAndHold")
                return "pressed"
            else if (focus && checked)
                return "pressed"
            else if (focus)
                return "focused"
            else if (checked)
                return "checked"
            else
                return "default"
        }
    }

    Frame {
        id: background
        frameName: style.current.get("background")
        frameType: style.current.get("frameType")
        anchors.fill: parent
    }

    Icon {
        id: contentIcon
        anchors.leftMargin: style.current.get("iconMarginLeft")
        anchors.rightMargin: style.current.get("iconMarginRight")
        anchors.topMargin: style.current.get("iconMarginTop")
        anchors.bottomMargin: style.current.get("iconMarginBottom")

        anchors.left: parent.left
        anchors.right: text == "" ? parent.right : undefined
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: style.current.get("iconWidth")
        height: style.current.get("iconHeight")
    }

    Text {
        id: label
        elide: Text.ElideRight
        anchors.leftMargin: iconSource != "" ? style.current.get("textMarginLeftInner") : style.current.get("textMarginLeft")
        anchors.rightMargin: style.current.get("textMarginRight")

        anchors.left: iconSource != "" ? contentIcon.right : parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        font: style.current.get("font")
        color: style.current.get("textColor")
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        onPressed: button.state = "Pressed";

        onReleased: button.state = "";

        onCanceled: {
            // Mark as canceled
            button.state = "Canceled";
            // Reset state. Can't expect a release since mouse was ungrabbed
            button.state = "";
        }

        onPressAndHold: {
            if (button.state != "Canceled" && (longPress || autoRepeat))
                button.state = "PressAndHold";
        }

        onExited: button.state = "Canceled"
    }

    Timer {
        id: tapRepeatTimer

        interval: button.autoRepeatInterval; running: false; repeat: true
        onTriggered: { button.repeat(); }
    }

    // TODO: Temporary sequential animation
    SequentialAnimation {
        id: clickedEffect
        PropertyAnimation { target: label; property: "scale"; to: 0.95; easing.type: Easing.Linear; duration: 100 }
        PropertyAnimation { target: label; property: "scale"; to: 1.0; easing.type: Easing.Linear; duration: 100 }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
            button.state = "Pressed";
            event.accepted = true;
        }
    }

    Keys.onReleased: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
            button.state = "";
            event.accepted = true;
        }
    }

    states: [
        State { name: "Pressed"; },
        State { name: "PressAndHold"; },
        State { name: "Canceled"; }
    ]

    transitions: [
        Transition {
            to: "Pressed"
            ScriptAction { script: button.press(); }
        },
        Transition {
            from: "Pressed"
            to: "PressAndHold"
            ScriptAction { script: button.hold(); }
        },
        Transition {
            from: "Pressed"
            to: ""
            ScriptAction { script: button.release(); }
            ScriptAction { script: button.click(); }
        },
        Transition {
            from: "PressAndHold"
            to: ""
            ScriptAction { script: button.release(); }
        },
        Transition {
            from: "Pressed"
            to: "Canceled"
            ScriptAction { script: button.release(); }
        },
        Transition {
            from: "PressAndHold"
            to: "Canceled"
            ScriptAction { script: button.release(); }
        }
    ]
}
