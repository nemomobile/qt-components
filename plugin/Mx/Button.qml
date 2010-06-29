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

Item {
    id: pushbutton
    Common {id: mx}

    property string text: "Text"
    property bool checkable: false
    property bool checked: false
    property url backgroundImage: "";
    property alias backgroundImageBorder: buttonImage.border;
    property url activeImage: "";
    property alias activeImageBorder: buttonActiveImage.border;
    property url hoverImage: "";
    property alias hoverImageBorder: buttonHoverImage.border;
    property alias tooltipText: tooltip.text;
    signal clicked

    width: Math.max(text.width + 20, 110)
    height: Math.max(text.height + 12, 23)

    BorderImage {
        id: buttonImage
        anchors.fill:parent
        source: Qt.resolvedUrl(backgroundImage == "" ? "images/button.png" : backgroundImage);
        border.left:10;
        border.top:10;
        border.right:10;
        border.bottom:10;
    }

    BorderImage {
        id: buttonActiveImage
        source: Qt.resolvedUrl(activeImage == "" ? "images/button-active.png" : activeImage);
        opacity: 0

        anchors.fill: parent
        border.left: 10
        border.top: 10
        border.right: 10
        border.bottom: 10
    }

    BorderImage {
        id: buttonHoverImage
        source: Qt.resolvedUrl(hoverImage == "" ? "images/button-hover.png" : hoverImage);
        opacity: 0

        anchors.fill: parent
        border.left: 10
        border.top: 10
        border.right: 10
        border.bottom: 10
    }

    Text {
        id: text
        font.pixelSize: mx.fontSize
        anchors.verticalCenter:parent.verticalCenter
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.verticalCenterOffset: pushbutton.state === "pressed" ? 1 : 0
        anchors.horizontalCenterOffset: pushbutton.state === "pressed" ? 1 : 0
        text: pushbutton.text
        color: "#616261";
    }

    TooltipLoader {
        id: tooltip;
        anchors.fill: parent;

        property bool pressDismiss: false;
        shown: !pressDismiss && mouseRegion.containsMouse;
    }

    MouseArea {
        id: mouseRegion
        hoverEnabled: true
        anchors.fill: parent
        onPressed: { tooltip.pressDismiss = true; }
        onExited: { tooltip.pressDismiss = false; }
        onClicked: {
            if (pushbutton.checkable) {
                pushbutton.checked = !pushbutton.checked;
            }
            pushbutton.clicked();
        }
    }

    states: [
        State {
            name: "pressed"
            when: mouseRegion.pressed || pushbutton.checked
            PropertyChanges { target: buttonImage; opacity: 0 }
            PropertyChanges { target: buttonActiveImage; opacity: 1 }
            PropertyChanges { target: buttonHoverImage; opacity: 0 }
        },
        State {
            name: "highlighted"
            when: mouseRegion.containsMouse && !pushbutton.checked
            PropertyChanges { target: buttonImage; opacity: 0 }
            PropertyChanges { target: buttonActiveImage; opacity: 0 }
            PropertyChanges { target: buttonHoverImage; opacity: 1 }
        }
    ]

    transitions: [
        /*Transition {
            from: "";
            to: "Pressed"
            NumberAnimation { properties: "opacity"; duration: 30 }
            ColorAnimation { properties: "color"; duration: 30 }
        },*/
        Transition {
            from: "highlighted";
            to: ""
            NumberAnimation { properties: "opacity"; duration: 130 }
        }
    ]

}
