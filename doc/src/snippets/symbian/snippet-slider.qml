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

Rectangle {
    width: 800
    height: 400
    color: "grey"

    Grid {
        id: sliderGrid
        anchors.topMargin: 50
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        //! [0]
        Text {
            text: "Horizontal slider (" + slider1.value + ")"
            color: "white"
        }
        //! [0]

        //! [1]
        Slider {
            id: slider1
            maximumValue: 150
            minimumValue: -150
            value: 150
            stepSize: 50
            valueIndicatorVisible: valueIndToggle.checked
            inverted: inversionToggle.checked
            onValueChanged: {
                txtLog.text = txtLog.text + "The 'valueChanged' signal. <br>"
            }
        }
        //! [1]

        //! [2]
        CheckBox {
            id: valueIndToggle
            text: "Value indicator"
            checked: true
        }

        CheckBox {
            id: inversionToggle
            text: "Inverted"
        }
        //! [2]

        Text {
            text: "Vertical slider (" + slider2.value + ")"
            width: 120
            color: "white"
        }

        Slider {
            id: slider2
            orientation: Qt.Vertical
            maximumValue: 25
            stepSize: 1
            value: 25
            valueIndicatorVisible: valueIndToggle2.checked
            valueIndicatorText: "Volume"
            inverted: inversionToggle2.checked
            onValueChanged: {
                txtLog.text = txtLog.text + "The 'valueChanged' signal. <br>"
            }
        }

        CheckBox {
            id: valueIndToggle2
            text: "Value indicator"
            checked: true
        }

        CheckBox {
            id: inversionToggle2
            text: "Inverted"
        }
        Button {
            id: btnClearText
            text: "Clear text"
            onClicked: txtLog.text = ""
        }

        Text {
            id: txtLog
        }
    }
}

