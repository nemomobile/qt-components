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
** packaging of this file.  Pleases review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

import Qt 4.7
import "." 1.0
import "themebridge" 1.0


Item {
    id: root

    property variant actions: []
    property string title: ""

    signal triggered(int index)
    signal canceled()

    function open() {
        internalData.objectMenu = component.createObject(root);
        if (internalData.objectMenu != null) {
            internalData.objectMenu.show();
            internalData.objectMenu.state = "Visible";
        }
    }

    function close() {
        if (internalData.objectMenu != null) {
            if (internalData.objectMenu.state != "Hidden") {
                root.canceled();
                internalData.objectMenu.hide();
                internalData.objectMenu.destroy();
            }
        }
    }

    QtObject {
        id: internalData

        property variant objectMenu: null
        property int animationTime: 250
    }

    Component {
        id: component

        Menu {
            id: menu

            function menuY() {
                return style.current.get("appRectHeight") / 2 - menu.height / 2;
            }

            function destroy() {
                menu.state = "Hidden";
                menu.destroy(internalData.animationTime);
            }

            title: root.title
            timeout: 5000
            x: style.current.get("appRectWidth") / 2 - menu.width / 2; y: menu.menuY()
            width: style.current.get("appRectWidth") > style.current.get("appRectHeight")
                   ? style.current.get("appRectHeight") - style.current.get("screenMargin")
                   : style.current.get("appRectWidth") - style.current.get("screenMargin")
            actions: root.actions
            animationDuration: internalData.animationTime
            state: "Hidden"

            onTriggered: {
                root.triggered(index);
                menu.destroy();
            }
            onCanceled: {
                root.canceled();
                menu.destroy();
            }

            Connections {
                target: screen
                onOrientationChanged: connectionTimer.start()
            }

            Timer {
                id: connectionTimer
                interval: 1
                onTriggered: {
                    if (state == "Visible") {
                        y = menuY();
                        width = style.current.get("appRectWidth") > style.current.get("appRectHeight")
                                ? style.current.get("appRectHeight") - style.current.get("screenMargin")
                                : style.current.get("appRectWidth") - style.current.get("screenMargin");
                        x = style.current.get("appRectWidth") / 2 - menu.width / 2;
                    }
                }
            }

            Style {
                id: style
                styleClass: "Menu"
            }

            states: [
                State {
                    name: "Hidden"
                    PropertyChanges { target: menu; opacity: 0 }
                },
                State {
                    name: "Visible"
                    PropertyChanges { target: menu; opacity: 1 }
                }
            ]

            transitions: Transition {
                PropertyAnimation { target: menu; property: "opacity"; duration: internalData.animationTime }
            }
        }
    }
}
