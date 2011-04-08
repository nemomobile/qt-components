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

Item {

    function physicalWidth() {
        return (screen.width / screen.dpi) * 25.4
    }

    function physicalHeight() {
        return (screen.height / screen.dpi) * 25.4
    }

    function diagonal() {
        return Math.sqrt(screen.width * screen.width + screen.height * screen.height) / screen.dpi
    }

    function round(number) {
        return Math.round(number * 100) / 100
    }

    function category() {
        if (screen.displayCategory == Screen.Small)
            return "Small"
        else if (screen.displayCategory == Screen.Normal)
            return "Normal"
        else if (screen.displayCategory == Screen.Large)
            return "Large"
        else
            return "ExtraLarge"
    }

    function density() {
        if (screen.density == Screen.Low)
            return "Low"
        else if (screen.density == Screen.Medium)
            return "Medium"
        else if (screen.density == Screen.High)
            return "High"
        else
            return "ExtraHigh"
    }

    function orientation() {
        if (screen.orientation == Screen.Portrait)
            return "Portrait"
        else if (screen.orientation == Screen.Landscape)
            return "Landscape"
        else if (screen.orientation == Screen.PortraitInverted)
            return "PortraitInverted"
        else
            return "LandscapeInverted"
    }

    Text {
        color: "white"
        font.pixelSize: 20
        text: "Pixel width:\n"
              + "Pixel height:\n"
              + "Display width:\n"
              + "Display height:\n"
              + "Physical width (mm):\n"
              + "Physical height (mm):\n"
              + "Diagonal (inch):\n"
              + "DPI:\n"
              + "Rotation:\n"
              + "Orientation:\n"
              + "Category:\n"
              + "Density:"
        anchors { fill: parent; margins: 20 }
        horizontalAlignment: Text.AlignLeft
    }
    Text {
        color: "white"
        font.pixelSize: 20
        text: screen.width + '\n'
              + screen.height + '\n'
              + screen.displayWidth + '\n'
              + screen.displayHeight + '\n'
              + round(physicalWidth()) + '\n'
              + round(physicalHeight()) + '\n'
              + round(diagonal()) + '\n'
              + round(screen.dpi) + '\n'
              + screen.rotation + '\n'
              + orientation() + '\n'
              + category() + '\n'
              + density()
        anchors { fill: parent; margins: 20 }
        horizontalAlignment: Text.AlignRight
    }
}
