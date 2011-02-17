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
import com.nokia.symbian 1.0

Item {
    id: root

    Grid {
        id: sliderGrid
        anchors.fill: parent
        anchors.margins: 20
        columns: screen.orientation == Qt.Horizontal ? sliderGrid.columns = 2 : sliderGrid.columns = 4;
        spacing: 10

        Text {
            text: "Horizontal slider (" + slider1.value + ")"
            width: 120
            color: "white"
        }

        Slider {
            id: slider1
            objectName: "slider1"
            width: parent.width / 3
            height: 50
            focus: true
            maximumValue: 125
            minimumValue: -125
            value: 150
            stepSize: 5
            toolTipVisible: toolTipToggle.checked
            inverted: inversionToggle.checked
        }

        CheckBox {
            id: toolTipToggle
            text: "Tool tip"
            checked: true
        }

        CheckBox {
            id: inversionToggle
            text: "Inverted"
        }

        Text {
            text: "Vertical slider (" + slider2.value + ")"
            width: 120
            color: "white"
        }

        Slider {
            id: slider2
            objectName: "slider2"
            width: 50
            height: parent.height / 2
            orientation: Qt.Vertical
            maximumValue: 25
            stepSize: 1
            value: 25
            toolTipVisible: toolTipToggle2.checked
            inverted: inversionToggle2.checked
        }

        CheckBox {
            id: toolTipToggle2
            text: "Tool tip"
            checked: true
        }

        CheckBox {
            id: inversionToggle2
            text: "Inverted"
        }
    }
}
