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
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Item {
    id: page;
    clip: true

    property bool __animationEnabled: false
    property alias __pageNavigationState: pageNavigationInternal.state

    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    Style {
        id: meegostyle
        styleClass: "MApplicationPageStyle"
    }
    Background {
        anchors.fill: parent
        style: meegostyle
    }

    Item {
        id: pageNavigationInternal
        states: [
            State {
                name: "left"
                PropertyChanges {
                    target: page
                    x: -page.width
                }
            },
            State {
                name: "right"
                PropertyChanges {
                    target: page
                    x: page.width
                }
            }
        ]
    }

    Behavior on x {
        id: slideAnimation
        enabled: __animationEnabled
        SequentialAnimation {
            ScriptAction {
                script:
                    if (pageNavigationInternal.state == "")
                        page.visible = true
            }
            NumberAnimation {}
            ScriptAction {
                script:
                    if (pageNavigationInternal.state == "right") {
                        page.destroy(100)
                    } else if (pageNavigationInternal.state == "left") {
                        page.visible = false
                    }
            }
        }
    }
}
