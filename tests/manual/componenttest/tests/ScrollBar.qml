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

Item {
    id: page
    property int policySetting: Symbian.ScrollBarWhenNeeded

    function isLandscape() {
        return screen.width > screen.height
    }

    onHeightChanged: {
        flickableAreaRectangle.height = page.height / 2
    }

    onWidthChanged: {
        flickableAreaRectangle.width = page.width
    }

    Rectangle {
        id: flickableAreaRectangle
        anchors.top: page.top
        width: 360
        height: 250
        color: "black"

        FocusScope {
            id: scrollBarFocusScope
            width: flickableAreaRectangle.width
            height: flickableAreaRectangle.height

            Flickable {
                id: flickableArea
                anchors.fill: scrollBarFocusScope
                contentWidth: colorfulSquare.width
                contentHeight: colorfulSquare.height
                clip: true
                interactive: flickableSettingCheckBox.checked
                visible: true

                Rectangle {
                    id: colorfulSquare
                    width: 1000
                    height: 1000
                    opacity: 0.5
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "blue" }
                        GradientStop { position: 1.0; color: "yellow" }
                    }
                }

                Rectangle {
                    height: colorfulSquare.height
                    width: colorfulSquare.width
                    opacity: 0.5
                    rotation: -90
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "red" }
                        GradientStop { position: 1.0; color: "green" }
                    }
                }

                Text {
                    anchors.centerIn: colorfulSquare
                    text: "Flick around!"
                    color: "white"
                    font.pixelSize: 50
                    opacity: 0.85
                }
            }

            ScrollBar {
                id: vertical
                anchors { top: flickableArea.top; right: flickableArea.right }
                orientation: Qt.Vertical
                interactive: interactiveSettingCheckBox.checked
                visible: verticalScrollBarSettingCheckBox.checked
                policy: policySetting
                flickableItem: flickableArea
            }

            ScrollBar {
                id: horizontal
                anchors { left: flickableArea.left; bottom: flickableArea.bottom; rightMargin: verticalScrollBarSettingCheckBox.checked ? vertical.width : 0 }
                orientation: Qt.Horizontal
                interactive: interactiveSettingCheckBox.checked
                visible: horizontalScrollBarSettingCheckBox.checked
                policy: policySetting
                flickableItem: flickableArea
            }
        }
    }

    Grid {
        anchors { bottom: page.bottom; top: flickableAreaRectangle.bottom; left: page.left }
        columns: isLandscape() ? 4 : 2
        spacing: 1

        CheckBox {
            id: verticalScrollBarSettingCheckBox
            width: 150
            height: 50
            text: "Vertical"
            checked: false
        }

        CheckBox {
            id: horizontalScrollBarSettingCheckBox
            width: 150
            height: 50
            text: "Horizontal"
            checked: false
        }

        CheckBox {
            id: flickableSettingCheckBox
            width: 150
            height: 50
            text: "Flickable"
            checked: true
        }

        CheckBox {
            id: interactiveSettingCheckBox
            width: 150
            height: 50
            text: "Interactive"
            checked: true
        }

        CheckBox {
            id: policyWhenNeededCheckBox
            width: 150
            height: 50
            text: "When needed"
            checked: true

            onClicked: {
                policyWhenScrollingCheckbox.checked = !checked
                policySetting = checked ? Symbian.ScrollBarWhenNeeded : Symbian.ScrollBarWhenScrolling
            }
        }

        CheckBox {
            id: policyWhenScrollingCheckbox
            width: 150
            height: 50
            text: "When scrolling"
            checked: false

            onClicked: {
                policyWhenNeededCheckBox.checked = !checked
                policySetting = checked ? Symbian.ScrollBarWhenScrolling : Symbian.ScrollBarWhenNeeded
            }
        }

        CheckBox {
            width: 150
            height: 50
            text: "Small content"
            checked: false

            onClicked: {
                if (checked) {
                    colorfulSquare.width = 120
                    colorfulSquare.height = 120
                } else {
                    colorfulSquare.width = 1000
                    colorfulSquare.height = 1000
                }
            }
        }

        Button {
            id: flash; objectName: "flash"
            height: 50; width: 150
            text: "flash"
            onClicked: {
                vertical.flash()
                horizontal.flash()
            }
        }
    }
}
