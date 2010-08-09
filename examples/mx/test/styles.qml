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
import Qt.labs.Mx 1.0

Item {
    id: styles;

    function setupBorders(item) {
        item.backgroundImageBorder.top = 0;
        item.backgroundImageBorder.bottom = 0;
        item.backgroundImageBorder.left = 0;
        item.backgroundImageBorder.right = 0;

        item.activeImageBorder.top = 0;
        item.activeImageBorder.bottom = 0;
        item.activeImageBorder.left = 0;
        item.activeImageBorder.right = 0;

        item.hoverImageBorder.top = 0;
        item.hoverImageBorder.bottom = 0;
        item.hoverImageBorder.left = 0;
        item.hoverImageBorder.right = 0;
    }

    Column {
        x: 10;
        y: 10;
        spacing: 40;

        Row {
            spacing: 20;
            Button {
                width: 150;
                height: 100;
                text: "Default Style";
            }
            Button {
                id: green;
                width: 150;
                height: 100;
                text: "Green Style";
                backgroundImage: "images/green-button.png";
                activeImage: "images/green-button.png";
                hoverImage: "images/green-button.png";
                Component.onCompleted: styles.setupBorders(green);
            }
        }

        Row {
            spacing: 40;
            Button {
                id: red;
                width: 150;
                height: 100;
                text: "<b>Red Style</b>";
                backgroundImage: "images/red-button.png";
                activeImage: "images/red-button.png";
                hoverImage: "images/red-button.png";
                Component.onCompleted: styles.setupBorders(red);
            }
            Button {
                id: blue;
                width: 150;
                height: 100;
                text: "<b>Blue Style</b>";
                backgroundImage: "images/cyan-button.png";
                activeImage: "images/cyan-button.png";
                hoverImage: "images/cyan-button.png";
                Component.onCompleted: styles.setupBorders(blue);
            }
        }

        Rectangle {
            width: 200;
            height: 80;
            color: "lightgrey";

            BorderImage {
                anchors.fill: parent;
                source: "images/rounded-corner.png";
                border.top: 10;
                border.bottom: 10;
                border.left: 10;
                border.right: 10;
            }

            Button {
                id: container;
                width: 180;
                height: 60;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                text: "<b><font color='blue'>Container Test</font></b>";
                backgroundImage: "images/border-image.png";
                activeImage: "images/border-image-dark.png";
                hoverImage: "images/border-image-light.png";
            }
        }
    }
}
