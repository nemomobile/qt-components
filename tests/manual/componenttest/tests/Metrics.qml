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

import QtQuick 1.0
import com.nokia.symbian 1.0

Item {

    function diagonal() {
        return Math.sqrt(screen.physicalSize.width * screen.physicalSize.width
               + screen.physicalSize.height * screen.physicalSize.height) / 25.4
    }

    function round(number) {
        return Math.round(number * 100) / 100
    }

    Text {
        color: "white"
        font.pixelSize: 20
        text: "Pixel width:\n"
              + "Pixel height:\n"
              + "Physical width (mm):\n"
              + "Physical height (mm):\n"
              + "Diagonal (inch):\n"
              + "PPI:"
        anchors { fill: parent; margins: 20 }
        horizontalAlignment: Text.AlignLeft
    }
    Text {
        color: "white"
        font.pixelSize: 20
        text: screen.width + '\n'
              + screen.height + '\n'
              + round(screen.physicalSize.width) + '\n'
              + round(screen.physicalSize.height) + '\n'
              + round(diagonal()) + '\n'
              + round(screen.ppi)
        anchors { fill: parent; margins: 20 }
        horizontalAlignment: Text.AlignRight
    }
}
