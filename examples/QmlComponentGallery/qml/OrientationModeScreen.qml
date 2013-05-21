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
    id: orientationModeScreen
    anchors.margins: UiConstants.DefaultMargin

    Column {
        anchors.centerIn: parent
        spacing: 20
        Row {
            Label {
                text: "Orientation Locking"
                font.pixelSize: 30
            }
        }
        Row {
            Column {
                CheckBox {
                    id: cbLandscape
                    text: "Landscape"
                    checked: screen.allowedOrientations & Screen.Landscape
                    onClicked:
                        if(cbLandscape.checked)
                            screen.allowedOrientations = screen.allowedOrientations | Screen.Landscape;
                        else
                            screen.allowedOrientations = screen.allowedOrientations & ~Screen.Landscape;
                }
                CheckBox {
                    id: cbPortrait
                    text: "Portrait"
                    checked: screen.allowedOrientations & Screen.Portrait
                    onClicked:
                        if(cbPortrait.checked)
                            screen.allowedOrientations = screen.allowedOrientations | Screen.Portrait;
                        else
                            screen.allowedOrientations = screen.allowedOrientations & ~Screen.Portrait;
                }
                CheckBox {
                    id: cbLandscapeInverted
                    text: "LandscapeInverted"
                    checked: screen.allowedOrientations & Screen.LandscapeInverted
                    onClicked:
                        if(cbLandscapeInverted.checked)
                            screen.allowedOrientations = screen.allowedOrientations | Screen.LandscapeInverted;
                        else
                            screen.allowedOrientations = screen.allowedOrientations & ~Screen.LandscapeInverted;
                }
                CheckBox {
                    id: cbPortraitInverted
                    text: "PortraitInverted"
                    checked: screen.allowedOrientations & Screen.PortraitInverted
                    onClicked:
                        if(cbPortraitInverted.checked)
                            screen.allowedOrientations = screen.allowedOrientations | Screen.PortraitInverted;
                        else
                            screen.allowedOrientations = screen.allowedOrientations & ~Screen.PortraitInverted;
                }
                Label {
                    text: "Text field for VKB orientation lock testing:"
                }
                TextField {
                    anchors {left: parent.left; right: parent.right;}
                    placeholderText: "Default text"
                    maximumLength: 80

                    Keys.onReturnPressed: {
                        text = "Return key pressed";
                        parent.focus = true;
                    }
                }
                Label {
                    text: "Custom VKB:"
                }

                TextField {
                    id: aTextField
                    anchors {left: parent.left; right: parent.right;}
                    platformCustomSoftwareInputPanel: redVkb
                    property string backspaceTitle: "Delete"

                    Connections {
                        target: inputContext
                        onSoftwareInputPanelEventChanged: {
                            if(aTextField.activeFocus) {
                                if(inputContext.softwareInputPanelEvent == "Backspace") {
                                    if(aTextField.text.length > 0)
                                        aTextField.text = aTextField.text.substring(0, aTextField.text.length - 1)
                                } else {
                                    aTextField.text = aTextField.text + inputContext.softwareInputPanelEvent
                                }
                            }
                        }
                    }
                }

            }
        }
        Row {
            Button {
                text: "Default"
                onClicked: {
                    screen.allowedOrientations = Screen.Portrait | Screen.Landscape
                    cbLandscape.checked = true;
                    cbPortrait.checked = true;
                    cbLandscapeInverted.checked = false;
                    cbPortraitInverted.checked = false;
                }
            }
        }
        Row {
            Label {
                text: "If hardware keyboard is open, \norientation is always locked to landscape."
                wrapMode: Text.Wrap
            }
        }
    }

    Component {
        id: redVkb

        Rectangle {
            id: rec
            color: "red"
            height: 100

            Item {
                anchors.centerIn: parent
                Button {
                    id: addA
                    width: 150
                    anchors.right: removeA.left
                    anchors.rightMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Add 'A'"
                    onClicked: inputContext.softwareInputPanelEvent = "A"
                }

                Button {
                    id: removeA
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    text: inputContext.targetInputFor(redVkb).backspaceTitle
                    onClicked: inputContext.softwareInputPanelEvent = "Backspace"
                }
            }
        }
    }


    tools: ToolBarLayout {

        ToolIcon {
            iconId: "toolbar-back"; onClicked: pageStack.pop();
        }

        ToolIcon {
            iconId: "toolbar-view-menu"
        }
    }
}
