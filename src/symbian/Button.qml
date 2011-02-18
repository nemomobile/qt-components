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
import com.nokia.symbian 1.0
import com.nokia.symbian.themebridge 1.0

ImplicitSizeItem {
    id: button

    // Common Public API
    property bool checked: false
    property bool checkable: false
    property bool pressed: (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold") && mouseArea.containsMouse
    property alias text: label.text
    property alias iconSource: icon.source

    signal clicked

    // Symbian specific signals and properties
    signal released
    signal pressAndHold

    property bool autoRepeat: false
    property bool longPress: false
    property QtObject __style: style

    implicitWidth: {
        var prefWidth = 20;

        if (iconSource != "" && text)
            // leftMargin + iconWidth + padding + textWidth + rightMargin
            prefWidth = icon.anchors.leftMargin + icon.sourceSize.width + label.anchors.leftMargin + style.textWidth(label.text, label.font) + label.anchors.rightMargin
        else if (iconSource != "")
            // leftMargin + iconWidth + rightMargin
            prefWidth = icon.anchors.leftMargin + icon.sourceSize.width + icon.anchors.rightMargin;
        else if (text)
            // leftMargin + textWidth + rightMargin
            prefWidth = icon.anchors.leftMargin + style.textWidth(label.text, label.font) + label.anchors.rightMargin;

        return prefWidth;
    }

    implicitHeight: {
        var prefHeight = icon.anchors.topMargin + icon.anchors.bottomMargin;

        if (iconSource != "" && text)
            prefHeight = prefHeight + Math.max(icon.sourceSize.height, style.fontHeight(label.font));
        else if (iconSource != "")
            prefHeight = prefHeight + icon.sourceSize.height;
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
        if (checkable && checked)
            style.play(Symbian.SensitiveButton);
        else
            style.play(Symbian.BasicButton);
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
        if (!checkable || (checkable && !checked))
            style.play(Symbian.BasicButton);

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
        if (!checkable)
            style.play(Symbian.SensitiveButton);
        // emit signal
        button.clicked();
    }

    QtObject {
        id: internal
        property int autoRepeatInterval: 50
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

    Style {
        id: style
        styleClass: "Button"
        mode: {
            if (stateGroup.state == "Pressed" || stateGroup.state == "PressAndHold")
                return "pressed"
            else if (focus && checked)
                return "pressed"
            else if (checked)
                return "checked"
            else
                return "default"
        }
    }

    BorderImage {
        id: background
        source: style.current.get("background")
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
    }

    Image {
        id: icon
        sourceSize.width : style.current.get("iconWidth")
        sourceSize.height : style.current.get("iconHeight")
        fillMode: Image.PreserveAspectFit
        smooth: true

        anchors.leftMargin : style.current.get("iconMarginLeft")
        anchors.rightMargin : style.current.get("iconMarginRight")
        anchors.topMargin : style.current.get("iconMarginTop")
        anchors.bottomMargin : style.current.get("iconMarginBottom")
        anchors.left: parent.left
        anchors.right: text == "" ? parent.right : undefined
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    Text {
        id: label
        elide: Text.ElideRight
        anchors.leftMargin: iconSource != "" ? style.current.get("textMarginLeftInner") : style.current.get("textMarginLeft")
        anchors.rightMargin: style.current.get("textMarginRight")

        anchors.left: iconSource != "" ? icon.right : parent.left
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

        onPressed: stateGroup.state = "Pressed"

        onReleased: stateGroup.state = ""

        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled";
            // Reset state. Can't expect a release since mouse was ungrabbed
            stateGroup.state = "";
        }

        onPressAndHold: {
            if (stateGroup.state != "Canceled" && (longPress || autoRepeat))
                stateGroup.state = "PressAndHold";
        }

        onExited: stateGroup.state = "Canceled"
    }

    Timer {
        id: tapRepeatTimer

        interval: internal.autoRepeatInterval; running: false; repeat: true
        onTriggered: button.repeat()
    }

    // TODO: Temporary sequential animation
    SequentialAnimation {
        id: clickedEffect
        PropertyAnimation { target: label; property: "scale"; to: 0.95; easing.type: Easing.Linear; duration: 100 }
        PropertyAnimation { target: label; property: "scale"; to: 1.0; easing.type: Easing.Linear; duration: 100 }
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
