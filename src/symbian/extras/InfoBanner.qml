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
* you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/
import QtQuick 1.0
import com.nokia.symbian 1.0
import "Constants.js" as CONSTANTS

ImplicitSizeItem {
    id: root

    property alias iconSource: image.source
    property alias text: text.text
    property alias timeout : timer.interval
    property bool interactive : false

    signal clicked

    function open() {
        root.parent = internal.rootObject();
        internal.setZOrder();
        background.source = root.interactive ? privateStyle.imagePath("qtg_fr_popup_infobanner_normal")
                                             : privateStyle.imagePath("qtg_fr_popup_infobanner");
        stateGroup.state = "Visible";
        if (timer.interval)
            timer.restart();
    }
    function close() {
        stateGroup.state = "Hidden"
    }

    x: 0
    implicitHeight: internal.bannerHeight()
    implicitWidth: screen.width

    BorderImage {
        id: background

        anchors.fill: parent
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        border { left: CONSTANTS.INFOBANNER_BORDER_MARGIN; top: CONSTANTS.INFOBANNER_BORDER_MARGIN;
                 right: CONSTANTS.INFOBANNER_BORDER_MARGIN; bottom: CONSTANTS.INFOBANNER_BORDER_MARGIN }
        opacity: CONSTANTS.INFOBANNER_OPACITY

        Image {
            id: image

            anchors { top: parent.top; topMargin: platformStyle.paddingLarge;
                      left: parent.left; leftMargin:  platformStyle.paddingLarge; }
            sourceSize { width: platformStyle.graphicSizeSmall; height: platformStyle.graphicSizeSmall }
            visible: source != ""
        }

        Text {
            id: text

            anchors { top: parent.top; topMargin: internal.textTopMargin(); left: image.visible ? image.right : parent.left;
                      leftMargin: image.visible ? platformStyle.paddingMedium : platformStyle.paddingLarge }
            width: image.visible ? (root.width - image.width - platformStyle.paddingLarge * 2 - platformStyle.paddingMedium)
                                 : (root.width - platformStyle.paddingLarge * 2);
            verticalAlignment: Text.AlignVCenter
            color: mouseArea.pressed ?  platformStyle.colorPressed : platformStyle.colorNormalLight
            wrapMode: Text.Wrap
            font {
                pixelSize: platformStyle.fontSizeMedium
                family: platformStyle.fontFamilyRegular
            }
        }
    }

    Timer {
        id: timer
        interval: CONSTANTS.INFOBANNER_DISMISS_TIMER
        onTriggered: close()
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onPressed: if (root.interactive) stateGroup.state = "Pressed"
        onReleased: if (root.interactive) {
                        if (stateGroup.state != "Cancelled" && stateGroup.state != "Hidden")
                            stateGroup.state = "Released";
                    } else {
                        stateGroup.state = "Hidden";
                    }
        onPressAndHold: if (root.interactive) {
                            if (timer.running) timer.stop();
                            stateGroup.state = "PressAndHold";
                        }
        onExited: if (root.interactive) {
                      if (stateGroup.state == "Pressed") {
                          stateGroup.state = "Cancelled";
                      } else if (stateGroup.state == "PressAndHold") {
                          stateGroup.state = "Cancelled";
                          if (timer.interval) timer.restart();
                      } else stateGroup.state = "Hidden"
                  }
    }

    QtObject {
        id: internal

        function rootObject() {
            var next = root.parent
            while (next && next.parent)
                next = next.parent
            return next
        }

        function setZOrder() {
            if (root.parent) {
                var maxZ = 0;
                var siblings = root.parent.children;
                for (var i = 0; i < siblings.length; ++i)
                    maxZ = Math.max(maxZ, siblings[i].z);
                root.z = maxZ + 1;
            }
        }

        function bannerHeight() {
            if (text.paintedHeight > privateStyle.fontHeight(text.font))
                return textTopMargin() + text.paintedHeight + platformStyle.paddingLarge;
            return Math.max(image.height, text.paintedHeight) + platformStyle.paddingLarge * 2;
        }

        function textTopMargin() {
            if (image.visible)
                return platformStyle.paddingLarge + image.height / 2 - privateStyle.fontHeight(text.font) / 2;
            return platformStyle.paddingLarge;
        }

        function press() {
            if (root.interactive) {
                privateStyle.play(Symbian.BasicButton);
                background.source = privateStyle.imagePath("qtg_fr_popup_infobanner_pressed");
            }
        }

        function release() {
            if (root.interactive)
                background.source = privateStyle.imagePath("qtg_fr_popup_infobanner_normal");
        }

        function click() {
            if (root.interactive) {
                privateStyle.play(Symbian.BasicButton);
                root.clicked();
            }
        }
    }

    StateGroup {
        id: stateGroup
        state: "Hidden"

        states: [
            State {
                name: "Visible"
                PropertyChanges { target: root; y: 0 }
            },
            State {
                name: "Hidden"
                PropertyChanges { target: root; y: -height }
            },
            State { name: "Pressed" },
            State { name: "PressAndHold" },
            State { name: "Released" },
            State { name: "Cancelled" }
        ]
        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press(); }
            },
            Transition {
                from: "Pressed"; to: "Released"
                ScriptAction { script: internal.release(); }
                ScriptAction { script: internal.click(); }
            },
            Transition {
                from: "PressAndHold"; to: "Released"
                ScriptAction { script: internal.release(); }
                ScriptAction { script: internal.click(); }
            },
            Transition {
                to: "Cancelled"
                ScriptAction { script: internal.release(); }
            },
            Transition {
                from: "Hidden"; to: "Visible"
                SequentialAnimation {
                    NumberAnimation { target: root; properties: "y"; duration: CONSTANTS.INFOBANNER_ANIMATION_DURATION; easing.type: Easing.OutQuad }
                    ScriptAction { script: privateStyle.play(Symbian.PopUp) }
                }
            },
            Transition {
                to: "Hidden"
                NumberAnimation { target: root; properties: "y"; duration: CONSTANTS.INFOBANNER_ANIMATION_DURATION; easing.type: Easing.OutQuad }
            }
        ]
    }
}
