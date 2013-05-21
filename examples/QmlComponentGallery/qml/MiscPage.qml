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
    id: miscPage
    tools: commonTools
    anchors.margins: UiConstants.DefaultMargin

    Flickable {
        id: topFlick
        anchors {fill: parent}
        contentWidth: parent.width
        contentHeight: row.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id:row
            spacing:10
            width: 400

            Label {
                id: label1
                text: "Busy Indicator"
            }
            Row {
                spacing: 10
                anchors.horizontalCenter:parent.horizontalCenter

                BusyIndicator {
                    id: indicator1
                    platformStyle: BusyIndicatorStyle { size: "small" }
                }
                BusyIndicator {
                    id: indicator2
                    running: indicator1.running
                }
                BusyIndicator {
                    id: indicator3
                    platformStyle: BusyIndicatorStyle { size: "large" }
                    running:  indicator1.running
                }
            }
            Button {
                id: button1
                text: "Toggle running"
                onClicked: indicator1.running = !indicator2.running
                anchors.horizontalCenter:parent.horizontalCenter
            }
            Item {width:16; height:16}
            Label {
                id: label2
                text: "Progress Bar"
            }
            ProgressBar {
                id: progressBar
                width: parent.width
                value: slider1.value
            }
            Button {
                id: button2
                text: "Toggle running"
                onClicked: {
                    progressBar.indeterminate = !progressBar.indeterminate;
                    slider1.enabled = !slider1.enabled;
                }
                anchors.horizontalCenter:parent.horizontalCenter
            }
            Slider {
                id: slider1
                width: parent.width
            }

            Item {width:16; height:16}
            Label {
                id: label3
                text: "Scroll Decorator"
            }
            Item {
                width: 250
                height: 150
                Flickable {
                    id: flick
                    contentWidth: 400
                    contentHeight: 400
                    width: parent.width
                    height: parent.height
                    contentX: 70
                    contentY: 120
                    clip: true

                    Rectangle {
                        anchors.fill: parent
                        color: "lightgrey"
                        Text {
                            text: "Flick me!"
                            anchors.centerIn: parent
                        }
                    }
                }
                ScrollDecorator {
                    id: scrollDecorator
                    flickableItem: flick
                }
                Rectangle {
                    anchors.fill: flick
                    border.color: "#333"
                    color: "transparent"
                }
                anchors.horizontalCenter:parent.horizontalCenter
            }
            Item {width:16; height:16}
            Label {
                id: label4
                text: "Switch"
            }
            Grid {
                id: grid1
                columns: 2
                spacing: 20

                Switch {
                    id: switch2
                    checked: true
                }
                Switch {
                    id: switch1
                    checked: false
                    platformStyle: SwitchStyle {
                        inverted: true
                    }
                }
                Text {
                    text: "default: enabled"
                    font.pixelSize: 15
                }
                Text {
                    text: "default: disabled"
                    font.pixelSize: 15
                }
                anchors.horizontalCenter:parent.horizontalCenter
            }
            anchors.horizontalCenter:parent.horizontalCenter
        }
    }

    ScrollDecorator {
        flickableItem: topFlick
    }
}
