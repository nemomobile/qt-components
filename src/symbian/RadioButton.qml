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
import Qt.labs.components 1.0
import "." 1.0
import "themebridge" 1.0

ImplicitSizeItem {
    id: root

    // Common Public API
    property alias checked: checkable.checked
    property bool pressed: mouseArea.containsMouse && mouseArea.pressed
    signal clicked

    // Symbian specific API
    property alias exclusiveGroup: checkable.exclusiveGroup
    property alias text: label.text

    Style {
        id: style
        styleClass: "RadioButton"
        mode: {
            if (pressed)
                return "pressed"
            else if (root.checked) {
                if (!root.enabled)
                    return "checkedAndDisabled"
                else if (root.focus)
                    return "checkedAndFocused"
                else
                    return "checked"
            } else {
                if (!root.enabled)
                    return "disabled"
                else if (root.focus)
                    return "focused"
                else
                    return "normal"
            }
        }
    }

    implicitWidth: style.current.get("marginLeft") + style.current.get("iconWidth") + style.current.get("spacing") + style.textWidth(label.text, label.font) + style.current.get("marginRight")
    implicitHeight: style.current.get("marginTop") + Math.max(style.current.get("iconHeight"), style.fontHeight(label.font)) + style.current.get("marginBottom")

    Frame {
        frameName: style.current.get("frameName")
        frameType: style.current.get("frameType")
        anchors.fill: parent

        Image {
            id: image
            source: style.current.get("iconName")
            anchors.left: parent.left
            anchors.leftMargin: style.current.get("marginLeft")
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.width: style.current.get("iconWidth")
            sourceSize.height: style.current.get("iconHeight")
        }
        Text {
            id: label
            elide: Text.ElideRight
            anchors.left: image.right
            anchors.leftMargin: style.current.get("spacing")
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: style.current.get("marginRight")
            anchors.bottomMargin: style.current.get("marginBottom")

            font: style.current.get("font")
            color: style.current.get("textColor")
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: style.play(Symbian.BasicItem);
        onClicked: {
            checkable.toggle();
            root.clicked();
            style.play(Symbian.CheckBox);
        }
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
            checkable.toggle();
            root.clicked();
            event.accepted = true;
        }
    }

    Checkable {
        id: checkable
        value: root.text
        enabled: true
    }
}
