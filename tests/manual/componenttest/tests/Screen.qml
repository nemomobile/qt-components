/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import com.nokia.symbian 1.1
import "../components"

Item {

    anchors.fill: parent

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
        if (screen.currentOrientation == Screen.Portrait)
            return "Portrait"
        else if (screen.currentOrientation == Screen.Landscape)
            return "Landscape"
        else if (screen.currentOrientation == Screen.PortraitInverted)
            return "PortraitInverted"
        else
            return "LandscapeInverted"
    }

    Label {
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
        anchors { left: parent.left; leftMargin: 20; top: parent.top; topMargin: 20}
    }
    Label {
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
        anchors { right: parent.right; rightMargin: 20; top: parent.top; topMargin: 20}
        horizontalAlignment: Text.AlignRight
    }
}
