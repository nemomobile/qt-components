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

import QtQuick 1.0
import com.nokia.symbian 1.0
import Qt.labs.components 1.0

Item {
    id: toolButtonTest

    CheckableGroup { id: checkGroup }

    Grid {
        id: buttonGrid
        anchors.fill: parent
        columns: 2

        ToolButton {
            id: toolButton1
            text: enabled ? "Clicked " + clicks : "Disabled"
            enabled: toolButton2.checked
            property int clicks: 0

            onClicked: ++clicks
        }

        ToolButton {
            id: toolButton2
            text: "Toggle"
            checkable: true
            checked: true
        }

        ToolButton {
            id: toolButton3
            iconSource: "image://theme/:/list1.png"
        }

        ToolButton {
            id: toolButton4
            text: "Group"
            platformExclusiveGroup: checkGroup
        }

        ToolButton {
            id: toolButton5
            iconSource: "image://theme/:/list1.png"
            flat: true
        }

        ToolButton {
            id: toolButton6
            text: "Group"
            platformExclusiveGroup: checkGroup
        }

        ToolButton {
            id: toolButton7
            iconSource: "image://theme/:/list1.png"
            text: "Icon&text"
        }

        ToolButton {
            id: toolButton8
            text: "Group"
            platformExclusiveGroup: checkGroup
        }

        ChoiceList {
            Component.onCompleted: {console.log("ChoiceList::onCompleted: " + currentValue + " " + currentIndex)}

            id: iconChoicelist
            width: toolButtonTest.width / 2
            onCurrentValueChanged: {
                currentValue != "<none>" ? toolButton3.iconSource = "image://theme/:/" + currentValue : toolButton3.iconSource = ""
                currentValue != "<none>" ? toolButton5.iconSource = "image://theme/:/" + currentValue : toolButton5.iconSource = ""
                currentValue != "<none>" ? toolButton7.iconSource = "image://theme/:/" + currentValue : toolButton7.iconSource = ""
            }
            model: ["list1.png", "list2.png", "list3.png", "<none>"]
        }

        ToolButton {
            id: toolButton9
            text: "Disabled"
            enabled: false
        }
    }
}
