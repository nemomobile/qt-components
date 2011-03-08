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
import "." 1.0

Item {
    id: root

    property alias text: textArea.text

    signal clicked

    width: parent.width; height: style.current.get("itemHeight")

    Style {
        id: style
        styleClass: "MenuContent"
        mode: mouseArea.pressed && mouseArea.containsMouse ? "pressed" : "default"
    }

    BorderImage {
        source: style.current.get("itemBackground")
        border { left: 20; top: 20; right: 20; bottom: 20 }
        anchors.fill: parent
    }

    Text {
        id: textArea
        anchors {
            fill: parent
            topMargin: style.current.get("itemMarginTop")
            bottomMargin: style.current.get("itemMarginBottom")
            leftMargin: style.current.get("itemMarginLeft")
            rightMargin: style.current.get("itemMarginRight")
        }
        font: style.current.get("font")
        color: style.current.get("color")
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: style.play(Symbian.BasicItem)
        onClicked: root.clicked()
        onReleased: style.play(Symbian.PopUpClose)
    }
}
