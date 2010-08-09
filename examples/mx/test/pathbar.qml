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
import Qt.labs.Mx 1.0

Item {
    Row {
        id: row
        x: 20; y: 50
        spacing: 4

        Button {
            text: "Add crumb"
            onClicked: { bar.push("Crumb" + (bar.level + 1)); }
        }

        Button {
            text: "Remove crumb"
            onClicked: { bar.pop(); }
        }

        Button {
            text: "Toggle editable"
            onClicked: { bar.editable = !bar.editable; }
        }

        Button {
            text: "Re-label 1st button"
            onClicked: { bar.setLabel(1, bar.entry.text); }
        }
    }

    Rectangle {
        color: "gainsboro"
        anchors.fill: bar
    }

    PathBar {
        id: bar

        anchors.top: row.bottom
        anchors.topMargin: 20
        anchors.left: row.left

        width: parent.width - 40
        clearOnChange: true
    }
}
