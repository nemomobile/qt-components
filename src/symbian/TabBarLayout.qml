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

import Qt 4.7

ImplicitSizeItem {
    id: root

    Component.onCompleted: priv.layoutChidren()
    onChildrenChanged: priv.layoutChidren()
    onWidthChanged: priv.layoutChidren()
    onHeightChanged: priv.layoutChidren()

    Keys.onPressed: {
        if (event.key == Qt.Key_Right) {
            var oldIndex = priv.currentButtonIndex()
            if (oldIndex != root.children.length - 1) {
                priv.tabGroup.currentTab = root.children[oldIndex + 1].tab
                event.accepted = true
            }
        } else if (event.key == Qt.Key_Left) {
            var oldIndex = priv.currentButtonIndex()
            if (oldIndex != 0) {
                priv.tabGroup.currentTab = root.children[oldIndex - 1].tab
                event.accepted = true
            }
        }
    }

    focus: true

    QtObject {
        id: priv
        property Item firstButton: root.children.length > 0 ? root.children[0] : null
        property Item firstTab: firstButton ? (firstButton.tab != null ? firstButton.tab : null) : null
        property Item tabGroup: firstTab ? (firstTab.parent ? firstTab.parent.parent : null) : null

        function currentButtonIndex() {
            for (var i = 0; i < root.children.length; ++i) {
                if (root.children[i].tab == tabGroup.currentTab)
                    return i
            }
            return -1
        }

        function layoutChidren() {
            var childCount = root.children.length
            var contentWidth = 0
            var contentHeight = 0
            if (childCount != 0) {
                var itemWidth = root.width / childCount
                for (var i = 0; i < childCount; ++i) {
                    var child = root.children[i]
                    child.x = i * itemWidth
                    child.y = 0
                    child.width = itemWidth
                    child.height = root.height

                    if (child.implicitWidth != undefined) {
                        contentWidth = Math.max(contentWidth, child.implicitWidth * childCount)
                        contentHeight = Math.max(contentHeight, child.implicitHeight)
                    }
                }
            }
            root.implicitWidth = contentWidth
            root.implicitHeight = contentHeight
        }
    }
}
