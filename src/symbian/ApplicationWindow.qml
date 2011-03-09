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

Window {
    id: window

    property bool fullScreen: false
    default property alias content: contentItem.data
    property alias pageStack: stack

    Item {
        anchors.fill: parent
        Item {
            id: contentItem
            anchors.fill: parent
            anchors.topMargin: decoration.topDecorationHeight
            anchors.bottomMargin: decoration.bottomDecorationHeight

            PageStack {
                id: stack
                anchors.fill: parent
            }
        }
    }

    // To be replaced by StatusBar and ToolBar components
    WindowDecoration {
        id: decoration
        anchors.fill: parent
        orientation: screen.orientation
        statusBarVisible: !window.fullScreen
        titleBarVisible: !window.fullScreen
        backButtonVisible: pageStack.depth > 1
        onQuit: Qt.quit()
        onBackClicked: pageStack.pop()
    }
}
