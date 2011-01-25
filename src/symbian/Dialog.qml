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
import "AppManager.js" as AppManager

ImplicitSizeItem {
    id: root

    property alias title: titleLabel.text
    property alias buttonsText: buttons.model
    property int preferredWidth: 0
    property int preferredHeight: 0
    property alias timeout: timeoutTimer.interval
    property bool modal: true
    property Component contentComponent

    signal triggered(int index)

    function open() {
        root.anchors.horizontalCenter = internal.appRoot.horizontalCenter;
        popup.visible = true;

        if (root.modal)
            fader.activate();

        if (timeoutTimer.interval > 0)
            timeoutTimer.start();
    }

    function close() {
        popup.visible = false;

        if (fader.isActive())
            fader.deactivate();

        root.destroy(internal.animationDuration);
    }

    opacity: 0
    implicitWidth: internal.getWidth()
    implicitHeight: internal.getHeight()
    y: internal.getTopPosition()

    QtObject {
        id: internal

        property int animationDuration: 250
        property Item appRoot: AppManager.getRoot()

        function getTopPosition() {
            return style.current.get("appRectHeight") / 2 - internal.getHeight() / 2;
        }

        function getWidth() {
            var size;
            var defaultWidth = style.current.get("appRectHeight") - 2 * style.current.get("screenMargin");
            if (style.current.get("appRectWidth") < style.current.get("appRectHeight"))
                defaultWidth = style.current.get("appRectWidth") - 2 * style.current.get("screenMargin");

            var maxWidth = style.current.get("appRectWidth") - 2 * style.current.get("screenMargin");
            if (root.preferredWidth > 0 && root.preferredWidth < maxWidth) {
               size = root.preferredWidth;
            } else {
                if (root.preferredWidth == 0)
                    size = defaultWidth;
                else
                    size = maxWidth;
            }

            return size;
        }

        function getHeight() {
            var size;
            var defaultHeight = style.current.get("appRectHeight") - 2 * style.current.get("screenMargin");
            if (style.current.get("appRectWidth") < style.current.get("appRectHeight"))
                defaultHeight = style.current.get("appRectWidth") - 2 * style.current.get("screenMargin");

            var maxHeight = style.current.get("appRectHeight") - 2 * style.current.get("screenMargin");
            if (root.preferredHeight > 0 && root.preferredHeight < maxHeight) {
               size = root.preferredHeight;
            } else {
                if (root.preferredHeight == 0)
                    size = defaultHeight;
                else
                    size = maxHeight;
            }
            return size;
        }
    }

    Timer {
        id: timeoutTimer
        interval: 0
        onTriggered: root.close();
    }

    Timer {
        id: connectionTimer
        interval: 1
        onTriggered: {
            if (popup.visible) {
                root.implicitWidth = internal.getWidth();
                root.implicitHeight = internal.getHeight();
                root.y = internal.getTopPosition();
            }
        }
    }

    Connections {
        target: screen
        onOrientationChanged: connectionTimer.start()
    }

    Fader {
        id: fader
        animationDuration: internal.animationDuration
    }

    Item {
        id: popup

        parent: internal.appRoot
        z: fader.z + 1
        anchors.fill: root
        visible: false

        Frame {
            frameName: style.current.get("popupFrameName")
            frameType: style.current.get("popupFrameType")
            anchors.fill: parent
        }

        Style {
            id: style
            styleClass: "Dialog"
        }

        Item {
            id: titleBar

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: style.current.get("titleTextHeight") + 2 * style.current.get("titleMargin")

            Frame {
                frameName: style.current.get("titleFrameName")
                frameType: style.current.get("titleFrameType")
                anchors.fill: parent
            }

            Text {
                id: titleLabel

                anchors {
                    fill: parent
                    margins: style.current.get("titleMargin")
                }
                color: style.current.get("titleTextColor")
                font: style.current.get("font")
            }
        }

        Loader {
            anchors {
                top: titleBar.bottom
                left: parent.left
                right: parent.right
                bottom: buttonRow.top
                margins: style.current.get("contentMargin")
            }
            sourceComponent: contentComponent
        }

        Row {
            id: buttonRow
            anchors {
                topMargin: style.current.get("buttonMargin")
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            height: style.current.get("buttonHeight")

            Repeater {
                id: buttons
                Button {
                    objectName: "DialogButton" + index
                    text: modelData
                    width: root.width / buttons.model.length
                    height: buttonRow.height
                    onClicked: {
                        if (popup.visible) {
                            root.close();
                            root.triggered(index);
                        }
                    }
                }
            }
        }

        states: [
            State {
                name: "Hidden"
                when: !popup.visible
                PropertyChanges { target: root; opacity: 0 }
            },
            State {
                name: "Visible"
                when: popup.visible
                PropertyChanges { target: root; opacity: 1 }
            }
        ]

        transitions: Transition {
            PropertyAnimation { duration: internal.animationDuration; properties: "opacity"; easing.type: Easing.InOutCubic }
        }
    }
}
