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
import com.nokia.symbian 1.0
import Qt.labs.components 1.0

Item {
    id: root

    CheckableGroup { id: group1 }
    CheckableGroup { id: group2 }
    CheckableGroup { id: group3 }

    Column {
        Row {
            RadioButton {
                id: button1
                text: "1"
                platformExclusiveGroup: group1
                focus: true
            }
            RadioButton {
                id: button2
                text: "2"
                platformExclusiveGroup: group1
                checked: true
            }
            RadioButton {
                id: button3
                text: "3"
                platformExclusiveGroup: group1
            }
        }

        Row {
            RadioButton {
                id: button4
                text: "4"
                platformExclusiveGroup: group2
            }
            RadioButton {
                id: button5
                text: "5"
                platformExclusiveGroup: group2
                checked: true
            }
            RadioButton {
                id: button6
                text: "6"
                platformExclusiveGroup: group2
            }
        }

        Grid {
            columns: 3

            Text {
                color: "red"
                text: "Disabled"
            }
            Text {
                color: "red"
                text: "No group set"
            }
            Text {
                color: "red"
                text: "Single group member"
            }

            // Disabled
            RadioButton {
                id: button7
                text: "7"
                enabled: false
            }
            // No group set
            RadioButton {
                id: button8
                text: "8"
            }
            // Single group member
            RadioButton {
                id: button9
                text: "9"
                platformExclusiveGroup: group3
            }
        }
    }
}
