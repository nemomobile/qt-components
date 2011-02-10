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

import QtQuick 1.0
import com.nokia.symbian 1.0

Item {

    ViewMenu {
        id: menu

        actions: ["item0", "item1", "item2", "item3", "item4", "item5", "item6"]
        onTriggered: signalLabel.text = "itemClicked(" + index + ")"
        onCanceled: signalLabel.text = "canceled"
    }

    Button {
        id: menuButton

        x: 10
        y: 50
        width: 160
        height: 50
        text: "Show menu"

        onClicked: menu.open()
    }

    Button {
        id: menuButton2

        x: 180
        y: 50
        width: 170
        height: 50
        text: "Menu, ext dismiss"

        onClicked: {
            menu.open()
            menuDismissTimer.start()
        }
    }

    Timer {
        id: menuDismissTimer

        interval: 3000
        onTriggered: menu.close()
    }

    Row {
        anchors { top: menuButton.bottom; topMargin: 20 }
        x: 50

        Text {
            font { bold: true; pixelSize: 14 }
            color: "white"
            text: "Signal: "
        }
        Text {
            id: signalLabel

            font { bold: true; pixelSize: 14 }
            color: "white"
        }
    }
}
