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

// ToolButton is a push-button style button intended for use with toolbars.

import Qt 4.7
import com.meego.themebridge 1.0

Item {
    property string text
    width: 150; height: 64
    anchors.verticalCenter: parent ? parent.verticalCenter : undefined
    signal clicked

    BorderImage {
        id: name
        width: 150; height: 40
        border.left: 15; border.top: 15
        border.right: 15; border.bottom: 15

        source: mouseArea.pressed ?
                    "image://theme/meegotouch-button-navigationbar-button-background-pressed" :
                    "image://theme/meegotouch-button-navigationbar-button-background";
        anchors.centerIn: parent

        Text {
            text: parent.parent.text
            anchors.centerIn: parent
            font.bold: true
            font.capitalization: Font.AllUppercase
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
