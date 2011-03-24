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

Item {
    id: root

    property variant actions: []

    signal triggered(int index)
    signal canceled()

    function open() {
        console.log("warning: use of deprecated component, use Menu instead")
        if (internalData.menuObject == null) {
            internalData.menuObject = component.createObject(root);
            if (internalData.menuObject != null) {
                internalData.menuObject.state = "Visible";
                internalData.menuObject.show();
                return internalData.menuObject;
            }
        }
        return null;
    }

    function close() {
        console.log("warning: use of deprecated component, use Menu instead")
        if (internalData.menuObject != null) {
            if (internalData.menuObject.state != "Hidden") {
                internalData.menuObject.state = "Hidden";
                root.canceled();
                internalData.menuObject.hide();
                internalData.menuObject.destroy(internalData.animationDuration);
            }
        }
    }

    Style {
        id: style
        styleClass: "MenuContent"
    }

    QtObject {
        id: internalData

        property variant menuObject: null
        property int animationDuration: 250;
        property int timeout: 5000
    }

    Component {
        id: component

        MenuBaseOld {
            id: menu

            function menuY() {
                return style.current.get("appRectHeight") - menu.height;
            }

            x: 0; width: style.current.get("appRectWidth")
            state: "Hidden"
            actions: root.actions
            animationDuration: internalData.animationDuration
            timeout: internalData.timeout

            onTriggered: {
                root.triggered(index);
                menu.state = "Hidden";
                menu.destroy(menu.animationDuration);
            }
            onCanceled: {
                root.canceled();
                menu.state = "Hidden";
                menu.destroy(menu.animationDuration);
            }

            Connections {
                target: screen
                onCurrentOrientationChanged: connectionTimer.start()
            }

            Timer {
                id: connectionTimer
                interval: 1
                onTriggered: {
                    if (menu.state == "Visible") {
                        menu.y = menu.menuY();
                        menu.width = style.current.get("appRectWidth");
                    }
                }
            }

            states: [
                State {
                    name: "Hidden"
                    PropertyChanges { target: menu; y: style.current.get("appRectHeight") + 1 }
                },
                State {
                    name: "Visible"
                    PropertyChanges { target: menu; y: menu.menuY() }
                }
            ]

            transitions: Transition {
                NumberAnimation { target: menu; property: "y"; duration: menu.animationDuration }
            }
        }
    }
    Component.onCompleted: console.log("warning: use of deprecated component, use Menu instead")
}
