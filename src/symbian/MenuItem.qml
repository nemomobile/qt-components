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
import "." 1.0

Item {
    id: root

    property alias text: textArea.text
    property bool platformSubItemIndicator: false

    signal clicked

    width: parent.width; height: privateStyle.menuItemHeight

    QtObject {
        id: internal

        function bg_postfix() {
            if (activeFocus && symbian.listInteractionMode == Symbian.KeyNavigation)
                return "highlighted"
            else if (mouseArea.pressed && mouseArea.containsMouse)
                return "pressed"
            else
                return "popup_normal"
        }

        function textColor() {
            if (activeFocus && symbian.listInteractionMode == Symbian.KeyNavigation)
                return platformStyle.colorHighlighted
            else if (mouseArea.pressed && mouseArea.containsMouse)
                return platformStyle.colorPressed
            else
                return platformStyle.colorNormalLight
        }
    }

    BorderImage {
        source: privateStyle.imagePath("qtg_fr_list_" + internal.bg_postfix())
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
    }

    Text {
        id: textArea
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: iconLoader.status == Loader.Ready ? iconLoader.left : parent.right
            rightMargin: iconLoader.status == Loader.Ready ? platformStyle.paddingMedium : privateStyle.scrollBarThickness
        }
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
        color: internal.textColor()
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
    }

    Loader {
        id: iconLoader
        sourceComponent: root.platformSubItemIndicator ? subItemIcon : undefined
        anchors {
            right: parent.right
            rightMargin: privateStyle.scrollBarThickness
            verticalCenter: parent.verticalCenter
        }
    }

    Component {
        id: subItemIcon

        Image {
            source: privateStyle.imagePath("qtg_graf_drill_down_indicator.svg")
            sourceSize.width: platformStyle.graphicSizeSmall
            sourceSize.height: platformStyle.graphicSizeSmall
        }
    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: {
            symbian.listInteractionMode = Symbian.TouchInteraction
            privateStyle.play(Symbian.BasicItem)
        }
        onClicked: root.clicked()
        onReleased: privateStyle.play(Symbian.PopupClose)
    }

    Keys.onPressed: {
        event.accepted = true
        switch (event.key) {
            case Qt.Key_Select:
            case Qt.Key_Enter:
            case Qt.Key_Return: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation)
                    symbian.listInteractionMode = Symbian.KeyNavigation
                else
                    root.clicked()
                break
            }

            case Qt.Key_Up: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation) {
                    symbian.listInteractionMode = Symbian.KeyNavigation
                    if (ListView.view != null)
                        ListView.view.positionViewAtIndex(index, ListView.Beginning)
                } else {
                    if (ListView.view != null)
                        ListView.view.decrementCurrentIndex()
                    else
                        event.accepted = false
                }
                break
            }

            case Qt.Key_Down: {
                if (symbian.listInteractionMode != Symbian.KeyNavigation) {
                    symbian.listInteractionMode = Symbian.KeyNavigation
                    if (ListView.view != null)
                        ListView.view.positionViewAtIndex(index, ListView.Beginning)
                } else {
                    if (ListView.view != null)
                        ListView.view.incrementCurrentIndex()
                    else
                        event.accepted = false
                }
                break
            }
            default: {
                event.accepted = false
                break
            }
        }
    }
}
