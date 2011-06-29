/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import com.nokia.symbian 1.1
import "../components"

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

    Label {
        id: text
        anchors { top: button.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
    }

    Slider {
        id: heightSlider
        objectName: 'heightSlider'
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
        objectName: 'widthSlider'
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

        icon: iconCheckBox.checked ? "qrc:/list8.svg" : ""
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
