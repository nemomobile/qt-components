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
    Button {
        x: 50
        y: 100
        text: "Hide Me"
        tooltipText: "Disappear"

        onClicked: { visible = false; }
    }

    // ### was a MxFrame...
    Row {
        x: 50
        y: 200
        width: child.width
        height: child.height

        Button {
            id: child
            text: "Testing 123"
            tooltipText: "Testing tooltips in a container..."
        }
    }

    // ### Tooltip should be inside the app screen... (small offset here is needed)
    Button {
        x: 50
        y: 300
        text: "Testing Long Text"
        tooltipText: "Here is some really long text to test the handling in MxTooltip"
    }

    Button {
        x: 360
        y: 300
        text: "Testing Long Text"
        tooltipText: "Here is some really long text to test the handling in MxTooltip"
    }

    // ### Eliding support (and knowing the screen size)
    Button {
        x: 250
        y: 5
        text: "Crazy"
        tooltipText: ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" +
                      " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" +
                      " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" +
                      " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
    }

    Button {
        x: 250
        y: 430
        text: "Bottom"
        tooltipText: "Hello Hello Hello"
    }
}
