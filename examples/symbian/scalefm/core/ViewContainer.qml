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
import Qt.labs.components.native 1.0
import "StyleLoader.js" as StyleLoader

Page {
    id: container

    property bool magnifyGlass: false
    scale: magnifyGlass ? (50 / magnifyTool.width) : 1

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    //![loader_a]
    Loader {
        source: isTablet ? "TabletView.qml" : "MobileView.qml"
        anchors.fill: parent
    //![loader_a]

        MagnifyingTool {
            id: magnifyTool
            anchors.right: parent.right; anchors.rightMargin: 2
            anchors.top: parent.top; anchors.topMargin: 2
        }
    //![loader_b]
    }
    //![loader_b]
}
