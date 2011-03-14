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
import "AppManager.js" as AppView

ImplicitSizeItem {
    id: toolTip

    //API
    property alias font: text.font
    property alias text: text.text
    property variant target: null

    implicitWidth: {
        var preferredWidth = text.paintedWidth + privy.horizontalMargin * 2
        return Math.min(preferredWidth, privy.maxWidth)
    }

    implicitHeight: text.paintedHeight + privy.verticalMargin * 2

    onVisibleChanged: {
        if (visible) {
            toolTip.parent = AppView.rootObject()
            privy.calculatePosition()
        }
    }

    QtObject {
        id: privy

        property real verticalMargin: style.current.get("verticalMargin")
        property real horizontalMargin: style.current.get("horizontalMargin")
        property real spacing: style.current.get("spacing")
        property real maxWidth: screen.width - spacing * 2

        function calculatePosition() {
            if (!target)
                return

            // Determine and set the main position for the ToolTip, order: top, right, left, bottom
            var targetPos = toolTip.parent.mapFromItem(target, 0, 0)

            // Top
            if (targetPos.y >= (toolTip.height + privy.verticalMargin + privy.spacing)) {
                toolTip.x = targetPos.x + (target.width / 2) - (toolTip.width / 2)
                toolTip.y = targetPos.y - toolTip.height - privy.verticalMargin

            // Right
            } else if (targetPos.x <= (screen.width - target.width - privy.horizontalMargin - toolTip.width - privy.spacing)) {
                toolTip.x = targetPos.x + target.width + privy.horizontalMargin;
                toolTip.y = targetPos.y + (target.height / 2) - (toolTip.height / 2)

            // Left
            } else if (targetPos.x >= (toolTip.width + privy.horizontalMargin + privy.spacing)) {
                toolTip.x = targetPos.x - toolTip.width - privy.horizontalMargin
                toolTip.y = targetPos.y + (target.height / 2) - (toolTip.height / 2)

            // Bottom
            } else {
                toolTip.x = targetPos.x + (target.width / 2) - (toolTip.width / 2)
                toolTip.y = targetPos.y + target.height + privy.verticalMargin
            }

            // Fine-tune the ToolTip position based on the screen borders
            if (toolTip.x > (screen.width - privy.spacing - toolTip.width))
                toolTip.x = screen.width - toolTip.width - privy.spacing

            if (toolTip.x < privy.spacing)
                toolTip.x = privy.spacing;

            if (toolTip.y > (screen.height - toolTip.height - privy.spacing))
                toolTip.y = screen.height - toolTip.height - privy.spacing

            if (toolTip.y < privy.spacing)
                toolTip.y = privy.spacing
        }
    }

    Style { id: style; styleClass: "ToolTip" }

    BorderImage {
        id: frame
        anchors.fill: parent
        source: style.current.get("background")
        border { left: 20; top: 20; right: 20; bottom: 20 }
    }

    Text {
       id: text
       clip: true
       color: style.current.get("color")
       // TODO: See http://bugreports.qt.nokia.com/browse/QTBUG-16093
       // Enable when Qt Quick 1.1 hit Qt.
       // elide: Text.ElideRight
       font: style.current.get("font")
       verticalAlignment: Text.AlignVCenter

       x: privy.horizontalMargin
       y: privy.verticalMargin
       width: privy.maxWidth
    }
}
