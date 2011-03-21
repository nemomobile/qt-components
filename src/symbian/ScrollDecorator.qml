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

Item {
    id: scrollDecorator

    //Common Public API
    property Flickable flickableItem: null

    anchors.fill: flickableItem
    QtObject {
        id: decorators
        property Item vertical: null
        property Item horizontal: null
        property Component component: null

        function destroyDecorators() {
            if (vertical != null)
                vertical.destroy()
            if (horizontal != null)
                horizontal.destroy()
            if (component != null)
                component.url = ""
        }
        function createDecorators() {
            if (flickableItem != null) {
                component = Qt.createComponent("ScrollBar.qml")
                if (component.status == Component.Ready)
                    finishCreation()
                else
                    console.log("Error creating component")
            }
        }
        function finishCreation() {
            decorators.vertical = component.createObject(scrollDecorator)
            decorators.horizontal = component.createObject(scrollDecorator)
            if (decorators.vertical == null || decorators.horizontal == null) {
                console.log("Error creating objects vertical="+decorators.vertical+" and horizontal="+decorators.horizontal)
                destroyDecorators()
            } else {
                decorators.horizontal.flickableItem = scrollDecorator.flickableItem
                decorators.horizontal.orientation = Qt.Horizontal
                decorators.horizontal.interactive = false
                decorators.horizontal.anchors.bottom = scrollDecorator.bottom
                decorators.horizontal.anchors.left = scrollDecorator.left
                //set margin to make scrollBars not overlap each other
                decorators.horizontal.anchors.rightMargin = decorators.vertical.width
                decorators.vertical.flickableItem = scrollDecorator.flickableItem
                decorators.vertical.orientation = Qt.Vertical
                decorators.vertical.interactive = false
                decorators.vertical.anchors.top = scrollDecorator.top
                decorators.vertical.anchors.right = scrollDecorator.right
            }
        }
    }
    onFlickableItemChanged: {
        decorators.destroyDecorators()
        decorators.createDecorators()
    }
    onFocusChanged: {
        if (decorators.vertical == null)
            decorators.vertical.focus = focus //for key navigation support
    }
}
