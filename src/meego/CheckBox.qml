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

import QtQuick 1.0
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

import "UIConstants.js" as UI

ImplicitSizeItem {
    id: checkbox

    property bool checked: false
    property bool pressed
    signal clicked

    implicitWidth: 32
    implicitHeight: 32

    Binding {
        target: checkbox
        property: "pressed"
        value: mouseArea.pressed && mouseArea.containsMouse
    }

    BorderImage {
        anchors.fill: parent
        smooth: true

        source: !checkbox.enabled ? "image://theme/meegotouch-button-checkbox-background-disabled" :
                checkbox.pressed ? "image://theme/meegotouch-button-checkbox-background-pressed" :
                checkbox.checked ? "image://theme/meegotouch-button-checkbox-background-selected" :
                "image://theme/meegotouch-button-checkbox-background"

        border {
            left: 4
            top: 4
            right: 4
            bottom: 4
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        anchors.margins: pressed ? -UI.RELEASE_MISS_DELTA : 0

        onPressed: {
            // TODO: enable feedback without old themebridge
            // if (checkbox.checked)
            //     meegostyle.feedback("pressOnFeedback");
            // else
            //     meegostyle.feedback("pressOffFeedback");
        }

        onClicked: {
            checkbox.checked = !checkbox.checked;
            checkbox.clicked();

            // TODO: enable feedback without old themebridge
            // if (checkbox.checked)
            //     meegostyle.feedback("releaseOnFeedback");
            // else
            //     meegostyle.feedback("releaseOffFeedback");
        }
    }
}
