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

// ### Display Entered / Exited! Pause animation when not "on display".
// ### LayoutDirection

ImplicitSizeItem {
    id: root

    property bool running: false
    property alias __styleParentClass: meegostyle.styleParentClass

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MSpinnerStyle"
    }

    Pixmap {
        anchors.fill: parent
        style: meegostyle
        imageProperty: "bgPixmap"
    }

    PiePixmap {
        id: pie
        anchors.fill: parent
        style: meegostyle
        imageProperty: "progressPixmap"

        NumberAnimation {
            id: unknownAnimation
            running: root.running
            target: pie
            property: "startAngle"
            duration: meegostyle.current.get("period")
            // PiePixmap follows QPainter::drawPie() API, 0 is at 3 o'clock
            // and grows counter-clockwise. So we shift to start at 9 o'clock
            // and go clockwise.
            from: 540
            to: 180
            loops: Animation.Infinite
        }
    }
}
