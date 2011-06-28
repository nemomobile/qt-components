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
    id: root

    function isPortrait() {
        return screen.height > screen.width
    }
    property variant focusItem: slider1.activeFocus ? slider1
                              : slider2.activeFocus ? slider2
                              : slider3.activeFocus ? slider3
                              : null

    Rectangle {
        border {color: "steelblue"; width: 5}
        color: "#00000000"; radius: 5; opacity: 0.80
        property int margins: focusItem && focusItem.parent ? focusItem.parent.anchors.margins : 0
        x: focusItem ? focusItem.x + margins : 0
        y: focusItem ? focusItem.y + margins : 0
        height: focusItem ? focusItem.height : 0
        width: focusItem ? focusItem.width : 0
        visible: focusItem ? focusItem.activeFocus : false
    }

    Grid {
        id: sliderGrid

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: slider3.top
            margins: 20
        }
        columns: isPortrait() ? 2 : 4;
        spacing: 10

        Label {
            text: "Horizontal slider (" + slider1.value + ")"
            width: 180
        }

        Slider {
            id: slider1
            objectName: "slider1"
            width: parent.width / 3
            height: 50
            focus: true
            maximumValue: 125
            minimumValue: -125
            value: 150
            stepSize: 5
            valueIndicatorVisible: valueIndicatorToggle.checked
            inverted: inversionToggle.checked

            KeyNavigation.up: slider3
            KeyNavigation.down: slider2
            KeyNavigation.left: slider3
            KeyNavigation.right: slider2
        }

        CheckBox {
            id: valueIndicatorToggle
            text: "Value Ind."
            checked: true
        }

        CheckBox {
            id: inversionToggle
            text: "Inverted"
        }

        Label {
            text: "Vertical slider (" + slider2.value + ")"
            width: 180
        }

        Slider {
            id: slider2
            objectName: "slider2"
            width: 50
            height: parent.height / 2
            orientation: Qt.Vertical
            maximumValue: 5
            stepSize: 1
            value: 5
            valueIndicatorVisible: valueIndicatorToggle2.checked
            inverted: inversionToggle2.checked

            KeyNavigation.up: slider1
            KeyNavigation.down: slider3
            KeyNavigation.left: slider1
            KeyNavigation.right: slider3
        }

        CheckBox {
            id: valueIndicatorToggle2
            text: "Value Ind."
            checked: true
        }

        CheckBox {
            id: inversionToggle2
            text: "Inverted"
        }
    }

    Slider {
        id: slider3
        objectName: "slider3"
        valueIndicatorVisible: true
        valueIndicatorText: "Custom: " + (Math.round(value * 1000) / 1000)
        stepSize: 0.0001
        anchors { bottom: parent.bottom; left: parent.left; right: parent.right; margins: 20 }

        KeyNavigation.up: slider2
        KeyNavigation.down: slider1
        KeyNavigation.left: slider2
        KeyNavigation.right: slider1
    }
}
