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

    property bool checked: false

    property alias pressed: mouseArea.pressed
    signal clicked

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MCheckboxStyle"
        mode: {
            if (mouseArea.containsMouse && mouseArea.pressed)
                return "pressed"
            else if (button.checked)
                return "selected"
            else
                return "default"
        }
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle

        Pixmap {
            anchors.centerIn: parent
            style: meegostyle
            imageProperty: "checkmarkImage"
            visible: button.checked
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: {
            if (button.checked)
                meegostyle.feedback("pressOnFeedback");
            else
                meegostyle.feedback("pressOffFeedback");
        }

        onClicked: {
            button.checked = !button.checked;
            button.clicked();

            if (button.checked)
                meegostyle.feedback("releaseOnFeedback");
            else
                meegostyle.feedback("releaseOffFeedback");
        }
    }
}
