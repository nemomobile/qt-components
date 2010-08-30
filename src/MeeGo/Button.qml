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
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: button

    property alias checked: checkable.checked
    property alias checkable: checkable.enabled

    // Internal property, to be used by ButtonRow and other button containers.
    property alias __exclusiveGroup: checkable.exclusiveGroup

    property alias text: label.text
    property string iconSource //: "images/sample-icon.svg"
    property string checkedIconSource //: "images/sample-icon-2.svg"

    // Defines the button viewtype. Usually this is automatically set by
    // specialized containers like the ButtonRow or the QueryDialog
    property alias buttonType: meegostyle.styleType
    property alias groupPosition: background.tilePosition
    property alias styleObjectName: meegostyle.styleObjectName

    property alias iconVisible: iconImage.visible
    property alias textVisible: label.visible

    signal clicked

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MButtonStyle"
        mode: {
            if (mouseArea.containsMouse && mouseArea.pressed)
                return "pressed"
            else if (checkable.checked)
                return "selected"
            else
                return "default"
        }
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle
    }

    Item {
        id: centeredContainer
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: calculateWidth()

        function calculateWidth() {
            var width = 0
            var hasIcon = (iconVisible && iconImage.source != "")

            if (textVisible && hasIcon) {
                width = label.width + iconImage.width + 10
            } else if (textVisible) {
                width += label.width
            } else if (hasIcon) {
                width += iconImage.width
            }

            return width
        }

        Image {
            id: iconImage
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48

            source: (checkedIconSource == "" || !button.checked) ? iconSource : checkedIconSource
        }

        Text {
            id: label
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.family: "Nokia Sans"
            font.pixelSize: 24
            color: meegostyle.current.get("textColor")

            text: "Effect"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            checkable.toggle();
            button.clicked();
        }
    }

    Checkable {
        id: checkable
        value: button
    }
}
