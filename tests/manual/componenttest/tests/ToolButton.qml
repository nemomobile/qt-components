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
import com.nokia.symbian 1.1
import Qt.labs.components 1.1

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
            Text {
                id: selectImageLabel
                anchors.fill: parent
                anchors.margins: parent.labelPadding
                text: "Select image:"
                color: platformStyle.colorNormalLight
                font.family: platformStyle.fontFamilyRegular
                font.pixelSize: platformStyle.fontSizeMedium
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
