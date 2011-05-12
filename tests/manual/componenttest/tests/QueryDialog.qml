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
import com.nokia.symbian 1.0

Item {
    id: root
    anchors.fill: parent

    CheckBox {
        id: sizerCheckBox
        text: "Enable dialog sizers"
        anchors {
            top: parent.top; topMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
    }

    CheckBox {
        id: iconCheckBox
        text: "Custom Icon"
        anchors {
            top: sizerCheckBox.bottom; bottomMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
    }

    CheckBox {
        id: oneButtonCheckBox
        text: "One Button Only"
        anchors {
            top: iconCheckBox.bottom; bottomMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
    }

    Button {
        id: button
        anchors {
            top: oneButtonCheckBox.bottom; bottomMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width - parent.spacing
        text: "Open Query Dialog"
        onClicked: queryDialog.open()
    }

    Text {
        id: text
        color: "white"
        anchors { top: button.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
        font.pixelSize: 30
    }

    Slider {
        id: heightSlider
        x: 60
        y: screen.height - height - 120
        maximumValue: screen.height
        minimumValue: 1
        orientation: Qt.Vertical
        value: 100
        stepSize: 1
        valueIndicatorVisible: true
        visible: sizerCheckBox.checked
    }

    Slider {
        id: widthSlider
        x: screen.width < screen.height ? heightSlider.x + heightSlider.width + 20 : 60
        y: screen.width < screen.height ? heightSlider.y + heightSlider.height - 75 : heightSlider.y - height
        maximumValue: screen.width
        minimumValue: 1
        orientation: Qt.Horizontal
        value: 100
        stepSize: 1
        valueIndicatorVisible: true
        visible: sizerCheckBox.checked
    }

    QueryDialog {
        id: queryDialog
        height: sizerCheckBox.checked ? heightSlider.value : undefined
        width: sizerCheckBox.checked ? widthSlider.value : undefined

        icon: iconCheckBox.checked ? "qrc:tb_plus.png" : ""
        titleText: "Query Dialog"
        message: "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."
                 + "This is a test. This is a Test. This is a Test. This is a Test. This is a Test."

        acceptButtonText: "Ok"
        rejectButtonText: oneButtonCheckBox.checked ? "" : "Cancel"

        onAccepted: { text.text = "Ok clicked" }
        onRejected: { text.text = "Cancel clicked" }
    }
}
