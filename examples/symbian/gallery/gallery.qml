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
import LayoutDirectionSetter 1.0

Window {
    id: root

    property Menu menu

    // for demonstration and testing purposes each component needs to
    // set its inverted state explicitly
    property bool childrenInverted: false

    LayoutMirroring.enabled: Qt.application.layoutDirection == Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

    LayoutDirectionSetter {
        id: layoutDirectionSetter
    }

    StatusBar {
        id: statusBar
        anchors.top: root.top
        platformInverted: root.childrenInverted
    }

    Flickable {
        id: flickable
        clip: true
        anchors {
            left: root.left
            right: root.right
            top: statusBar.visible ? statusBar.bottom: root.top
            bottom: toolBar.visible ? toolBar.top: root.bottom
        }
        contentHeight: column.height

        SampleColumn {
            id: column
            anchors {
                left: parent.left
                right: parent.right
                margins: column.spacing
            }
            childrenInverted: root.childrenInverted
        }
    }

    ScrollDecorator {
        flickableItem: flickable
        platformInverted: root.childrenInverted
    }

    ToolBar {
        id: toolBar
        anchors.bottom: root.bottom
        platformInverted: root.childrenInverted
        tools: ToolBarLayout {
            id: toolBarlayout
            ToolButton {
                flat: true
                iconSource: "toolbar-back"
                platformInverted: root.childrenInverted
                onClicked: Qt.quit()
            }
            ToolButton {
                iconSource: "toolbar-menu"
                platformInverted: root.childrenInverted
                onClicked: {
                    if (!menu)
                        menu = menuComponent.createObject(root)
                    menu.open()
                }
            }
        }
    }

    Component {
        id: menuComponent

        Menu {
            id: theMenu

            platformInverted: root.childrenInverted

            content: MenuLayout {
                MenuItem {
                    text: column.enabled ? "Disable" : "Enable"
                    platformInverted: root.childrenInverted
                    onClicked: column.enabled = !column.enabled
                }
                MenuItem {
                    text: root.childrenInverted ? "Revert components" : "Invert components"
                    platformInverted: root.childrenInverted
                    onClicked: root.childrenInverted = !root.childrenInverted
                }
                MenuItem {
                    text: "Set layout dir"
                    platformSubItemIndicator: true
                    platformInverted: root.childrenInverted
                    onClicked: layoutDirectionSubMenu.open()
                }
                MenuItem {
                    text: "Quit"
                    platformInverted: root.childrenInverted
                    onClicked: Qt.quit()
                }
            }
        }
    }

    Menu {
        id: layoutDirectionSubMenu

        MenuLayout {
            MenuItem { text: "LeftToRight"; onClicked: layoutDirectionSetter.setLayoutDirection(Qt.LeftToRight) }
            MenuItem { text: "RightToLeft"; onClicked: layoutDirectionSetter.setLayoutDirection(Qt.RightToLeft) }
            MenuItem { text: "Automatic"; onClicked: layoutDirectionSetter.setLayoutDirection(Qt.LayoutDirectionAuto) }
        }
    }
}
