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
import com.nokia.extras 1.1

Page {
    id: container
    anchors.margins: UiConstants.DefaultMargin

    tools: ToolBarLayout {

        ToolIcon {
            iconId: "toolbar-back"
            onClicked: tabGroup.currentTab.depth > 1 ? tabGroup.currentTab.pop() : pageStack.pop()
        }
        ButtonRow{
            TabButton {
                text: "Customize"
                tab: tab1
            }
            TabButton {
                text: "Examples"
                tab: tab2
            }
        }
    }

    TabGroup {
        id: tabGroup

        currentTab: tab1

        Page {
            id: tab1

            Label {
                id: label1
                text: "Create Custom InfoBanner"
                font.bold: true
            }

            Column {
                id: col1
                spacing: 0
                anchors { top: label1.bottom; topMargin: 15 }

                TextField {
                    id: textField
                    width: container.width-x-20
                    text: "Information to be shown to user"
                }

                Row {
                    spacing: 20
                    CheckBox {
                        id: checkBox1
                        checked: true
                        text: "Timer"
                    }

                    CheckBox {
                        id: checkBox2
                        checked: true
                        text: "Icon"
                    }
                }

                Row {
                    Label { text: "Time to dismiss: " }
                    Label { text: sl1.value; color: "green" }
                    Label { text: " sec" ; color: "green" }
                    Slider { id: sl1; maximumValue: 9; minimumValue: 1; stepSize:1; value: 3 }
                }

                Row {
                    Label { text: "Top margin: " }
                    Label { text: sl2.value; color: "green" }
                    Label { text: " pixel" ; color: "green" }
                    Slider { id: sl2; maximumValue: 50; minimumValue: 0; stepSize:1; value: 0 }
                }

                Row {
                    Label { text: "Left margin: " }
                    Label { text: sl3.value; color: "green" }
                    Label { text: " pixel" ; color: "green" }
                    Slider { id: sl3; maximumValue: 50; minimumValue: 0; stepSize:1; value: 0 }
                }

                Row {
                    spacing: 10

                    Button {
                        text: "Show"
                        onClicked: {
                            banner0.show();
                        }
                    }

                    Button {
                        text: "Hide"
                        onClicked: {
                            banner0.hide();
                        }
                    }
                }
            }

            InfoBanner{
                id: banner0
                text: textField.text
                timerEnabled: checkBox1.checked
                iconSource: checkBox2.checked ? "assets/system_banner_thumbnail.png" : ""
                timerShowTime: sl1.value*1000
                topMargin: sl2.value
                leftMargin: sl3.value
            }

        }
        Page {
            id: tab2

            Flickable {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                width:  parent.width
                height: parent.height
                contentHeight: 600

                Label {
                    id: label
                    text: "Click on SHOW button to display example InfoBanners"
                    font.bold: true
                }

                Button {
                    id: button
                    text: "Show"
                    anchors.top: label.bottom
                    anchors.topMargin: 10
                    onClicked: {
                        banner1.show();
                        banner2.show();
                        banner3.show();
                        banner4.show();
                        banner5.show();
                        banner6.show();
                    }
                }

                InfoBanner{
                    id: banner1
                    objectName: "infoBanner1Object"
                    text: "List title #3 lorem ipum dolor."
                    anchors.top: button.bottom
                    anchors.topMargin: 10
                }

                InfoBanner{
                    id:banner2
                    text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in."
                    anchors.top: banner1.bottom
                    anchors.topMargin: 10
                }

                InfoBanner{
                    id:banner3
                    text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in at metus erat, a sodales ipsum dolor sit."
                    anchors.top: banner2.bottom
                    anchors.topMargin: 10
                }

                InfoBanner{
                    id:banner4
                    objectName: "infoBanner2Object"
                    text: "List title #3 lorem ipum dolor."
                    iconSource: "assets/system_banner_thumbnail.png"
                    anchors.top: banner3.bottom
                    anchors.topMargin: 10
                }

                InfoBanner{
                    id:banner5
                    text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in."
                    iconSource: "assets/system_banner_thumbnail.png"
                    anchors.top: banner4.bottom
                    anchors.topMargin: 10
                }

                InfoBanner{
                    id:banner6
                    text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in at metus erat, a sodales."
                    iconSource: "assets/system_banner_thumbnail.png"
                    anchors.top: banner5.bottom
                    anchors.topMargin: 10
                }
            }
        }
    }
}
