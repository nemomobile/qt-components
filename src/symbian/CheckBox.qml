/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

import Qt 4.7
import com.nokia.symbian.themebridge 1.0

ImplicitSizeItem {
    id: checkbox

    // Common Public API
    property bool checked: false
    property bool pressed: mouseArea.containsMouse && mouseArea.pressed && checkable

    signal clicked

    property bool checkable: true
    property alias text: label.text

    Style {
        id: style
        styleClass: "CheckBox"
        mode: {
            if (pressed)
                return "pressed"
            else if (checkbox.checked)
                return "selected"
            else
                return "default"
        }
    }

    implicitWidth: style.current.get("iconWidth") + style.current.get("padding") + style.textWidth(label.text, label.font)
    implicitHeight: style.current.get("topMargin") + Math.max( style.current.get("iconHeight"), style.fontHeight(label.font) ) + style.current.get("bottomMargin")

    Icon {
        id: contentIcon
        iconName: style.current.get("iconName")
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: style.current.get("iconWidth")
        height: style.current.get("iconHeight")
    }

    Text {
        id: label
        anchors.left: contentIcon.right
        anchors.leftMargin: style.current.get("padding")
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        elide: Text.ElideRight
        font: style.current.get("font")
        color: style.current.get("textColor")
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        SequentialAnimation {
            id: clickedEffect
            PropertyAnimation { target: contentIcon; property: "scale"; from: 0.8; to: 1.0; easing.type: "OutQuad"; duration: 300 }
        }

        onClicked: {
            if (checkable) {
                clickedEffect.restart();
                checkbox.checked = !checkbox.checked;
            }
            checkbox.clicked();
        }
    }
}
