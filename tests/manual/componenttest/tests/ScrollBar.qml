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
        anchors { bottom: page.bottom; top: flickableAreaRectangle.bottom }
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
            width: 150
            height: 50
            text: "Flickable"
            checked: false
            onClicked: flickableArea.interactive = checked
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
