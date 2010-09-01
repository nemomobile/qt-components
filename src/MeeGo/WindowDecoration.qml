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
import com.meego.themebridge 1.0

Item {
    id: root
    property int orientation
    property alias showBackButton: escapeButton.back
    property bool statusbarVisible
    property bool titlebarVisible
    property real topDecorationHeight: titlebar.y + titlebar.height
    property real bottomDecorationHeight: 0

    signal minimize()
    signal quit()
    signal backClicked()

    StatusBar {
        id: statusbar

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        orientation: root.orientation

        z: 100

        MouseArea {
            anchors.fill: parent
            onClicked: queryDialog.appear()
        }
    }

    Background {
        id: titlebar
        anchors.top: statusbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: titlebarStyle.preferredHeight

        style: titlebarStyle

        Style {
            id: titlebarStyle
            styleClass: "MNavigationBarStyle"
        }

        ScalableImage {
            id: dropShadow
            anchors.top: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            style: titlebarStyle
            imageProperty: "dropShadowImage"
        }

        EscapeButton {
            id: escapeButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            onBackClicked: root.backClicked()
            onCloseClicked: root.quit()
        }
    }

    QueryDialog {
        id: queryDialog
        anchors.fill: parent

        onButtonClicked: { console.log("button clicked " + button); }

        dialogTitle: "Query Dialog"
        dialogText: "Is this working ok ?"
    }

    HomeButton {
        anchors.left: titlebar.left
        anchors.verticalCenter: titlebar.verticalCenter
        onClicked: root.minimize()
    }

    states: [
        State {
            name: "statusbarOnly"
            when: !titlebarVisible && statusbarVisible
            AnchorChanges { target: titlebar; anchors.top: undefined; anchors.bottom: statusbar.bottom }
        },
        State {
            name: "titlebarOnly"
            when: titlebarVisible && !statusbarVisible
            AnchorChanges { target: statusbar; anchors.top: undefined; anchors.bottom: root.bottom }
        },
        State {
            name: "fullscreen"
            when: !titlebarVisible && !statusbarVisible
            AnchorChanges { target: statusbar; anchors.top: undefined; anchors.bottom: titlebar.top }
            AnchorChanges { target: titlebar; anchors.top: undefined; anchors.bottom: root.top }
        }
    ]

    transitions: Transition {
        AnchorAnimation { duration: 300; easing.type: Easing.InOutQuad }
    }
}