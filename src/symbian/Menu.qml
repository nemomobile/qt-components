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

    property alias actions: actionsRepeater.model
    property int animationDuration: 500
    property int maxVisibleItems: 5
    property int timeout: 0
    property alias title: titleText.text

    signal triggered(int index)
    signal canceled()

    function show() {
        fader.activate();
        content.enabled = true;
        content.visible = true;
        if (root.timeout > 0)
            timeoutTimer.start();
    }

    function hide() {
        if (fader.isActive()) {
            content.enabled = false;
            fader.deactivate();
            timeoutTimer.stop();
        }
    }

    implicitHeight: internalData.getHeight()
    implicitWidth: style.current.get("appRectWidth")

    QtObject {
        id: internalData

        property Item appRoot: AppManager.rootObject()

        function getHeight() {
            var itemsShown = Math.min(actionsRepeater.model.length, root.maxVisibleItems);
            itemsShown = Math.min(root.maxVisibleItems, itemsShown);
            itemsShown = Math.max(itemsShown, 1);
            var menuHeight = itemsShown * style.current.get("itemHeight");

            if (menuTitle.height)
                menuHeight += style.current.get("listMargin") + menuTitle.height;
            else
                menuHeight += 2 * style.current.get("listMargin");

            while (menuHeight > style.current.get("appRectHeight") - style.current.get("screenMargin"))
                menuHeight -= style.current.get("itemHeight");

            return menuHeight;
        }
    }

    Connections {
        target: screen
        onOrientationChanged: connectionTimer.start()
    }

    Fader {
        id: fader
        animationDuration: root.animationDuration
        onClicked: {
            if (fader.isActive()) {
                root.canceled();
                root.hide();
            }
        }
    }

    Timer {
        id: connectionTimer
        interval: 1
        onTriggered: parent.implicitHeight = internalData.getHeight()
    }

    Timer {
        id: timeoutTimer
        repeat: true
        interval: root.timeout
        onTriggered: {
            if (fader.isActive()) {
                root.canceled();
                root.hide();
            }
        }
    }

    Item {
        id: content

        parent: internalData.appRoot
        opacity: root.opacity
        width: root.width; height: root.height; x: root.x; y: root.y; z: fader.z + 1
        visible: false

        Style {
            id: style
            styleClass: "Menu"
        }

        Frame {
            frameName: style.current.get("frameName")
            frameType: style.current.get("frameType")
            anchors.fill: parent
        }

        Item {
            id: menuTitle
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: titleText.text != "" ? style.current.get("titleTextHeight") + 2 * style.current.get("margin") : 0

            Frame {
                frameName: style.current.get("titleFrameName")
                frameType: style.current.get("titleFrameType")
                anchors.fill: parent
            }

            Text {
                id: titleText
                anchors {
                    fill: parent
                    margins: style.current.get("margin")
                }
                color: style.current.get("titleTextColor")
                font: style.current.get("font")
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Repeater {
            id: actionsRepeater

            Item {
                Component.onCompleted: actionModel.append({"itemText": modelData})
            }
        }

        ListModel {
            id: actionModel
        }

        // Popup list
        ListView {
            id: listView

            onMovementStarted: timeoutTimer.stop();
            onMovementEnded: timeoutTimer.start();
            onFlickStarted: timeoutTimer.stop();
            onFlickEnded: timeoutTimer.start();

            anchors {
                top: menuTitle.bottom
                left: parent.left
                right: parent.right
                leftMargin: style.current.get("listMargin")
                rightMargin: style.current.get("listMargin")
                topMargin: menuTitle.height ? 0 : style.current.get("listMargin")
            }
            height: menuTitle.height ? parent.height - style.current.get("listMargin") - menuTitle.height :
                    parent.height - style.current.get("listMargin") * 2
            delegate: listDelegate
            model: actionModel
            clip: true
            boundsBehavior: Flickable.StopAtBounds
        }

        Component {
            id: listDelegate

            Item {
                id: activeItem

                width: listView.width; height: style.current.get("itemHeight")

                Style {
                    id: itemStyle
                    styleClass: "Menu"
                    mode: "default"
                }

                Frame {
                    frameType: itemStyle.current.get("itemFrameType")
                    frameName: itemStyle.current.get("itemFrameName")
                    anchors.fill: parent
                }

                Text {
                    anchors {
                        fill: parent
                        topMargin: itemStyle.current.get("itemMarginTop")
                        bottomMargin: itemStyle.current.get("itemMarginBottom")
                        leftMargin: itemStyle.current.get("itemMarginLeft")
                        rightMargin: itemStyle.current.get("itemMarginRight")
                    }
                    font: itemStyle.current.get("font")
                    color: itemStyle.current.get("color")
                    text: itemText
                }

                MouseArea {
                    anchors.fill: parent

                    onPressed: {
                        itemStyle.mode = "pressed";
                        timeoutTimer.stop();
                    }
                    onReleased: {
                        if (containsMouse) {
                            itemStyle.mode = "default";
                            timeoutTimer.start();
                            root.triggered(index);
                            root.hide();
                        }
                    }
                    onCanceled: {
                        itemStyle.mode = "default";
                        timeoutTimer.start();
                    }
                    onExited: {
                        itemStyle.mode = "default";
                        timeoutTimer.start();
                    }
                }
            }
        }
    }
}
