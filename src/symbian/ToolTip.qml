/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
import "." 1.0
import "AppManager.js" as AppView

ImplicitSizeItem {
    id: root

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
            root.parent = AppView.rootObject()
            privy.calculatePosition()
        }
    }

    QtObject {
        id: privy

        property real verticalMargin: platformStyle.paddingMedium
        property real horizontalMargin: platformStyle.paddingMedium
        property real spacing: platformStyle.paddingLarge
        property real maxWidth: screen.width - spacing * 2

        function calculatePosition() {
            if (!target)
                return

            // Determine and set the main position for the ToolTip, order: top, right, left, bottom
            var targetPos = root.parent.mapFromItem(target, 0, 0)

            // Top
            if (targetPos.y >= (root.height + privy.verticalMargin + privy.spacing)) {
                root.x = targetPos.x + (target.width / 2) - (root.width / 2)
                root.y = targetPos.y - root.height - privy.verticalMargin

            // Right
            } else if (targetPos.x <= (screen.width - target.width - privy.horizontalMargin - root.width - privy.spacing)) {
                root.x = targetPos.x + target.width + privy.horizontalMargin;
                root.y = targetPos.y + (target.height / 2) - (root.height / 2)

            // Left
            } else if (targetPos.x >= (root.width + privy.horizontalMargin + privy.spacing)) {
                root.x = targetPos.x - root.width - privy.horizontalMargin
                root.y = targetPos.y + (target.height / 2) - (root.height / 2)

            // Bottom
            } else {
                root.x = targetPos.x + (target.width / 2) - (root.width / 2)
                root.y = targetPos.y + target.height + privy.verticalMargin
            }

            // Fine-tune the ToolTip position based on the screen borders
            if (root.x > (screen.width - privy.spacing - root.width))
                root.x = screen.width - root.width - privy.spacing

            if (root.x < privy.spacing)
                root.x = privy.spacing;

            if (root.y > (screen.height - root.height - privy.spacing))
                root.y = screen.height - root.height - privy.spacing

            if (root.y < privy.spacing)
                root.y = privy.spacing
        }
    }

    BorderImage {
        id: frame
        anchors.fill: parent
        source: privateStyle.imagePath("qtg_fr_popup")
        border { left: 20; top: 20; right: 20; bottom: 20 }
    }

    Text {
       id: text
       clip: true
       color: platformStyle.colorNormalLight
       // TODO: See http://bugreports.qt.nokia.com/browse/QTBUG-16093
       // Enable when Qt Quick 1.1 hit Qt.
       // elide: Text.ElideRight
       font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall }
       verticalAlignment: Text.AlignVCenter

       x: privy.horizontalMargin
       y: privy.verticalMargin
       width: privy.maxWidth
    }
}
