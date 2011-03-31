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
import "." 1.0

Window {
    id: root

    property bool fullScreen: false
    default property alias content: contentItem.data
    property alias pageStack: stack

    Component.onCompleted: console.log("warning: ApplicationWindow is an experimental component. Use Window instead.")

    Item {
        anchors.top: sbar.visible ? sbar.bottom : parent.top
        anchors.bottom: tbar.visible ? tbar.top : parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        Item {
            id: contentItem
            anchors.fill: parent
            PageStack {
                id: stack
                anchors.fill: parent
                toolBar: tbar
            }
        }
    }

    StatusBar {
        id: sbar
        anchors.top: parent.top
        width: parent.width
        visible: !root.fullScreen
    }

    ToolBar {
        id: tbar
        anchors.bottom: parent.bottom
        width: parent.width
        visible: !root.fullScreen
    }

    // event preventer when page transition is active
    MouseArea {
        anchors.fill: parent
        enabled: pageStack.busy
    }
}
