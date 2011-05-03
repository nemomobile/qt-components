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

Item {
    id: listItem
    QtObject {
        id: internal
        property int halfSpacingSize: Math.round(appStyle.spacingSize / 4.0)
    }

    Rectangle {
        id: gradient
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#3574A6" }
            GradientStop { position: 0.5; color: "black" }
            GradientStop { position: 1.0; color: "#3574A6" }
        }
        opacity: mouseArea.pressed ? 1.0 : 0.0
    }
    height: text1.height + text2.height + appStyle.marginSize + (internal.halfSpacingSize * 2)
    width: parent.width
    Text {
        id: text1
        color: "white"
        font: appStyle.largeFont
        text: Name
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.verticalCenter
            leftMargin: appStyle.marginSize
            rightMargin: appStyle.marginSize
            bottomMargin: internal.halfSpacingSize
        }
    }
    Text {
        id: text2
        color: "white"
        font: appStyle.mediumFont
        text: Description
        anchors {
            left: parent.left
            right: parent.right
            top: parent.verticalCenter
            leftMargin: appStyle.marginSize
            rightMargin: appStyle.marginSize
            topMargin: internal.halfSpacingSize
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if (!window.pageStack.busy) pageStack.pop(stationView)
        }
    }
}
