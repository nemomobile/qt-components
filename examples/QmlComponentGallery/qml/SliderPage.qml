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
import com.nokia.meego 1.1

Page {
    id: root
    tools: commonTools
    anchors.margins: UiConstants.DefaultMargin

    property int textColumnWidth: 350
    property int sliderWidth: 384

    Flickable {
        id: flickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick

        contentHeight: col.height
        contentWidth:flickable.width
        Column {
            id: col
            spacing: 30
            Row {
                Label { text: sl1.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Default Slider" }
                Slider { id: sl1 ; width:sliderWidth}
            }
            Row {
                Label { text: sl2.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Slider [0-100], indicator visible" }
                Slider { id:sl2; stepSize:1 ; valueIndicatorVisible: true; minimumValue:0 ; maximumValue:100 ; width:sliderWidth}
            }

            Row {
                Label { text: sl3.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Slider from -100 to 100, indicator visible" }
                Slider { id: sl3; minimumValue: -100; maximumValue: 100; valueIndicatorVisible: true ; width:sliderWidth}
            }
            Row {
                Label { text: sl4.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Slider from -100 to 100, inverted, indicator visible" }
                Slider {
                    id: sl4;
                    minimumValue: -100; maximumValue: 100;
                    valueIndicatorVisible: true
                    inverted: true
                    width: sliderWidth
                }
            }
            Row {
                Label { text: sl5.value ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Slider from -100 to 100, step size 10, indicator visible" }
                Slider {
                    id: sl5;
                    minimumValue: -100; maximumValue: 100; stepSize: 30
                    valueIndicatorVisible: true
                    width: sliderWidth
                }
            }
            Row {
                Label { text: sl6.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Slider from -100 to 100, indicator visible, value not updated while dragging" }
                Slider {
                    id: sl6;
                    minimumValue: -100; maximumValue: 100
                    valueIndicatorVisible: true
                    valueIndicatorText: sl6.value.toFixed(0)
                    width: sliderWidth
                }
            }
            Row {
                Label { text: sl7.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Vertical Slider from -100 to 100, indicator visible" }
                Slider {
                    id: sl7;
                    orientation: Qt.Vertical
                    minimumValue: -100; maximumValue: 100
                    valueIndicatorVisible: true
                }
            }
            Row {
                Label { text: progressBar.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Progressbar 0 to 100" }
                ProgressBar {
                    id: progressBar
                    width: 300
                    minimumValue: 0
                    maximumValue: 100
                    value: 50
                }
            }
            Row {
                Label { text: progressBar2.value.toFixed(2) ; color: "green"; width: 80 }
                Label { visible: screen.currentOrientation == Screen.Landscape; width: root.textColumnWidth; wrapMode: Text.Wrap; text: "Progressbar, indeterminate" }
                ProgressBar {
                    id: progressBar2
                    width: 300
                    indeterminate: true
                    value: 50
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }
}
