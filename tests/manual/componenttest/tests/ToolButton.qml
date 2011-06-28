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
import Qt.labs.components 1.1
import "../components"

Item {
    id: toolButtonTest

    CheckableGroup { id: checkGroup }

    Grid {
        id: buttonGrid
        anchors.fill: parent
        columns: 2

        ToolButton {
            id: toolButton1
            text: enabled ? "Clicked " + clicks : "Disabled"
            enabled: toolButton2.checked
            property int clicks: 0

            onClicked: ++clicks
        }

        ToolButton {
            id: toolButton2
            text: "Toggle"
            checkable: true
            checked: true
        }

        ToolButton {
            id: toolButton3
            iconSource: imageDialog.currentImage
            flat: false
        }

        ToolButton {
            id: toolButton4
            text: "Group"
            platformExclusiveGroup: checkGroup
        }

        Row {
            ToolButton {
                id: toolButton5
                iconSource: imageDialog.currentImage
            }
            ToolButton {
                id: toolButtonX
                iconSource: imageDialog.currentImage
                checkable: true
                checked: true
            }
        }

        ToolButton {
            id: toolButton6
            text: "Group"
            platformExclusiveGroup: checkGroup
        }

        ToolButton {
            id: toolButton7
            iconSource: imageDialog.currentImage
            text: "Icon&text"
        }

        ToolButton {
            id: toolButton8
            text: "Group"
            platformExclusiveGroup: checkGroup
        }

        ToolButton {
            id: toolButton9
            text: "Disabled"
            enabled: false
        }

        Item {
            // empty grid item
            width: 1
            height: 1
        }

        Item {
            property int labelPadding: 10
            height: privateStyle.fontHeight(selectImageLabel.font) + 2 * labelPadding
            width: privateStyle.textWidth(selectImageLabel.text, selectImageLabel.font) + 2 * labelPadding
            Label {
                id: selectImageLabel
                anchors.fill: parent
                anchors.margins: parent.labelPadding
                text: "Select image:"
            }
        }

        ToolButton {
            id: imageButton
            text: imageDialog.model[imageDialog.selectedIndex]
            onClicked: imageDialog.open()

            SelectionDialog {
                id: imageDialog
                titleText: "Select image"
                selectedIndex: 1
                property string currentImage
                currentImage: {
                    if (selectedIndex <= 0)
                        return ""
                    return model[selectedIndex]
                }

                model: ["<none>", "toolbar-back", "toolbar-home", "toolbar-next", "toolbar-previous",
                        "toolbar-menu", "toolbar-settings", "toolbar-search", "toolbar-refresh",
                        "toolbar-add", "toolbar-delete", "toolbar-list", "toolbar-share",
                        "toolbar-dialer", "toolbar-mediacontrol-play", "toolbar-mediacontrol-stop",
                        "toolbar-mediacontrol-pause", "toolbar-mediacontrol-forward",
                        "toolbar-mediacontrol-backwards", "toolbar-invalid-id", "qrc:/list1.svg",
                        "qrc:/list2.svg", "qrc:/list3.svg", "qrc:/list4.svg", "qrc:/list5.svg",
                        "qrc:/list6.svg", "qrc:/list7.svg", "qrc:/list8.svg", "qrc:/list9.svg",
                        "qrc:/list10.svg", "qrc:/list11.svg", "qrc:/list12.svg", "qrc:/list13.svg",
                        "qrc:/list14.svg", "qrc:/list15.svg"
                     ]
            }
        }
    }
}
