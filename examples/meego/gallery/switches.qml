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
import com.meego 1.0

Item {
    width: 500
    height: 400

    Grid {
        columns: 2
        spacing: 20
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        Switch {
            id: switch1
        }

        Label {
            text: "First switch is " + (switch1.checked ? "ENABLED" : "Disabled")
        }

        Switch {
            id: switch2
        }

        Label {
            text: "Second switch is " + (switch2.checked ? "ENABLED" : "Disabled")
        }

        Switch {
            id: bindSwitch

            Binding {
                target: switch1
                property: "checked"
                value: switch2.checked
                when: bindSwitch.checked
            }

            Binding {
                target: switch2
                property: "checked"
                value: switch1.checked
                when: bindSwitch.checked
            }
        }

        Label {
            text: "Bind the values of the two switches above"
        }
    }
}
