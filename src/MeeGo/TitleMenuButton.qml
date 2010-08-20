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
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: button

    property alias title: title.text

    signal clicked()

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MApplicationMenuButtonStyle"
        styleObjectName: "NavigationBarMenuButton"
        mode: {
            // Removed the Selected mode as it doesnt seem useful here
            if (mouseArea.containsMouse && mouseArea.pressed)
                return "pressed"
            else
                return "default"
        }
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle
    }

    Label {
        id: title
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        // XXX: settng margins here is not overriding the ones from Label
        //anchors.leftMargin: 10

        width: button.width
        elide: Text.ElideRight

        styleObjectName: "NavigationBarMenuButtonLabel"

        // XXX This does not make sense yet, since the label width is not being set
        // horizontalAlignment: Text.AlignHCenter
        verticalAlignment: meegostyle.current.get("verticalTextAlign")

        text: "Title"
    }

    // XXX: arrow icon is only visibly when there are menu options
    Icon {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        iconId: meegostyle.current.get("arrowIcon")
        width: meegostyle.current.get("arrowIconSize").width
        height: meegostyle.current.get("arrowIconSize").height
        visible: title.text != ""
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: { button.clicked() }
    }
}
