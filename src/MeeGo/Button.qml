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

Item {
    id: button

    property alias checked: model.checked
    property alias checkable: model.checkable
    property alias autoExclusive: model.autoExclusive

    property alias text: label.text
    property string iconSource //: "images/sample-icon.svg"
    property string checkedIconSource //: "images/sample-icon-2.svg"

    property bool groupButton: false;
    property alias groupPosition: background.tilePosition

    property alias iconVisible: iconImage.visible
    property alias textVisible: label.visible

    signal clicked

//    Component.onCompleted: {
//        // XXX This is not nice
//        // What I wanted to do was to bind the root item "implicitSize" to the
//        // size provided by meegostyle. However:
//        //  - Implicit size is not exported to QML, that's C++ accessible only
//        //  - Binding actual width and height to the sizes provided by meegostyle
//        //    is not OK. It can cause sizes set externally to be overriden if
//        //    the binding is re-evaluated.
//        width = meegostyle.preferredWidth
//        height = meegostyle.preferredHeight
//    }

    Style {
        id: meegostyle
        styleType: button.groupButton ? Style.GroupButton : Style.Button
        mode: {
            if (model.down)
                return "pressed"
            else if (model.checked)
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

            source: (checkedIconSource == "" || !model.checked) ? iconSource : checkedIconSource
        }

        Text {
            id: label
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            horizontalAlignment: "AlignHCenter"
            verticalAlignment: "AlignVCenter"

            font.family: "Nokia Sans"
            font.pixelSize: 24
            color: meegostyle.textColor

            text: "Effect"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    ButtonModel {
        id: model

        pressed: mouseArea.pressed
        highlighted: mouseArea.containsMouse
        onClicked: button.clicked()

        checkable: true
    }
}
