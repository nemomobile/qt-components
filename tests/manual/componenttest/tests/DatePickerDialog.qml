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
import com.nokia.extras 1.1
import "../components"

Item {
    id: root
    anchors.fill: parent

    property DatePickerDialog dateDialog
    property bool platformInverted: false

    Column {
        anchors { margins: 16; top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom }

        Button {
            id: dialogButton
            objectName: "dialogButton"
            width:  parent.width
            text: "Launch Dialog"
            onClicked: {
                if (!dateDialog)
                    dateDialog = component.createObject(root)
                dateDialog.open()
                signalValue.text = "None"
            }
        }

        Button {
            id: todayButton
            objectName: "todayButton"
            width:  parent.width
            text: "Launch Today"
            onClicked: {
                if (!dateDialog)
                    dateDialog = component.createObject(root)
                var d = new Date()
                setDate(d.getFullYear(), d.getMonth() + 1, d.getDate())
                dateDialog.open()
                signalValue.text = "None"
            }
        }

        Button {
            id: dateButton
            objectName: "dateButton"
            width:  parent.width
            text: "Launch Feb 28, 2012"
            onClicked: {
                if (!dateDialog)
                    dateDialog = component.createObject(root)
                setDate(2012, 2, 28)
                dateDialog.minimumYear = dateDialog.year - 10;
                dateDialog.maximumYear = dateDialog.year + 10;
                dateDialog.open()
                signalValue.text = "None"
            }
        }

        Button {
            objectName: "rotateButton"
            text: "Rotate"
            onClicked: {
                if (screen.currentOrientation == Screen.Landscape)
                    screen.allowedOrientations = Screen.Portrait;
                else if (screen.currentOrientation == Screen.Portrait)
                    screen.allowedOrientations = Screen.LandscapeInverted;
                else if (screen.currentOrientation == Screen.LandscapeInverted)
                    screen.allowedOrientations = Screen.PortraitInverted;
                else if (screen.currentOrientation == Screen.PortraitInverted)
                    screen.allowedOrientations = Screen.Landscape;
            }
        }

        Row {
            Label {
                id: signalName
                text : "Signal / Func: "
            }
            Label {
                id: signalValue
                objectName: "signalValue"
                text : "None"
            }
        }

        Row {
            LayoutMirroring.enabled: false
            Label {
                text: "Dialog Value (m/d/yyyy): "
            }
            Label {
                id: dialogValue
                objectName: "dialogValue"
                text: ""
            }
        }
    }

    function setDate(year, month, day) {
        dateDialog.year = year
        dateDialog.month = month
        dateDialog.day = day
    }

    Component {
        id: component
        DatePickerDialog {
            id: dialog
            titleText: "Date"
            acceptButtonText: "Ok"
            rejectButtonText: "Cancel"
            platformInverted: root.platformInverted
            onAccepted: {
                signalValue.text = "Accept"
                dialogValue.text = dialog.month + " " + dialog.day + " " + dialog.year
            }
            onRejected: {
                signalValue.text = "Reject"
                dialogValue.text = dialog.month + " " + dialog.day + " " + dialog.year
            }
        }
    }
}
