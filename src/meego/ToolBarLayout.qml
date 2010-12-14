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

// ToolBarLayout is a container for items on a toolbar that automatically
// implements an appropriate layout for its children.

import Qt 4.7

Item {
    id: toolbarLayout

    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    function layoutChildren() {
        if (parent == null)
            return

        var numChildren = children.length

        for (i = 0; i < children.length; ++i) {
            children[i].parent = toolbarLayout;
        }

        // |   X   |
        if (numChildren == 1) {
            children[0].x = width / 2 - children[0].width / 2
            return
        }

        // |X ... X|
        var lastChild = children[numChildren-1] ? children[numChildren-1] : undefined;
        if (lastChild) {
            if (parent) {
                lastChild.x = parent.width - lastChild.width;
            }
        }

        // space to be divided between first and last items
        var toolBox = width - (children[0] ? children[0].width : 0) -
            (lastChild ? lastChild.width : 0);
        var childrenToLayout = numChildren - 2;

        // |X  X  X| etc.
        for (var i = 1; i < numChildren - 1; i++) {
            children[i].x = i * (toolBox / (childrenToLayout+1)) - children[i].width / 2 + children[0].width;
        }
    }

    Component.onCompleted: { layoutChildren() }
    onChildrenChanged: { layoutChildren() }
    onWidthChanged: { layoutChildren() }
}
