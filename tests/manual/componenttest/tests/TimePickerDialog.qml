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

    property int index: -1
    property TimePickerDialog timeDialog
    property bool platformInverted: false

    Column {
        anchors { margins: 16; top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom }

        Button {
            objectName: "dialog"
            text: "Normal dialog"
            width: parent.width
            onClicked: {
                launchDialog()
                signalValue.text = "None"
            }
        }

        Button {
            objectName: "noSecondsButton"
            text: "No Seconds: 8:35PM"
            width: parent.width
            onClicked: {
                launchDialog(20, 35, 0, DateTime.TwelveHours)
                signalValue.text = "None"
            }
        }

        Button {
            objectName: "noHoursButton"
            text: "No Hours: 35:02"
            width: parent.width
            onClicked: {
                launchDialog(1, 35, 2, DateTime.TwentyFourHours, DateTime.Seconds | DateTime.Minutes)
                signalValue.text = "None"
            }
        }

        Button {
            objectName: "12HourButton"
            text: "12-hour: 4:12:35PM"
            width: parent.width
            onClicked: {
                launchDialog(16, 12, 35, DateTime.TwelveHours, DateTime.All)
                signalValue.text = "None"
            }
        }

        Button {
            objectName: "24HourButton"
            text: "24-hour: 18:27:39"
            width: parent.width
            onClicked: {
                launchDialog(18, 27, 39, DateTime.TwentyFourHours, DateTime.All)
                signalValue.text = "None"
            }
        }

        Button {
            text: "Rotate"
            objectName: "rotateButton"
            onClicked: {
                if (screen.currentOrientation == Screen.Landscape)
                    screen.allowedOrientations = Screen.Portrait
                else if (screen.currentOrientation == Screen.Portrait)
                    screen.allowedOrientations = Screen.LandscapeInverted
                else if (screen.currentOrientation == Screen.LandscapeInverted)
                    screen.allowedOrientations = Screen.PortraitInverted
                else if (screen.currentOrientation == Screen.PortraitInverted)
                    screen.allowedOrientations = Screen.Landscape
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
            Label {
                text : "dialogValue: "
            }
            Label {
                id: dialogValue
                objectName: "dialogValue"
                text: ""
            }
        }
    }

    function launchDialog(hour, minute, seconds, hoursMode, visibleColumns) {
        if (!timeDialog)
            timeDialog = component.createObject(root)
        timeDialog.hourMode = (hoursMode != undefined ? hoursMode : dateTime.hourMode());
        // Do not show seconds by default
        timeDialog.fields = (visibleColumns != undefined ? visibleColumns : DateTime.Hours | DateTime.Minutes);
        if (hour) {
            timeDialog.hour = hour;
            timeDialog.minute = minute;
            timeDialog.second = seconds;
        }
        timeDialog.open();
        console.log("timeDialog.hourMode: " +timeDialog.hourMode)
    }

    Component {
        id: component
        TimePickerDialog {
            id: dialog
            titleText: "Time"
            acceptButtonText: "Ok"
            rejectButtonText: "Cancel"
            platformInverted: root.platformInverted
            onAccepted: {
                signalValue.text = "Accept"
                dialogValue.text = (dialog.fields & DateTime.Hours ? (dialog.hourMode & DateTime.TwentyFourHours ? dialog.hour : (dialog.hour > 12 ? dialog.hour - 12 : dialog.hour)) : "") +
                        (dialog.fields & DateTime.Minutes ? ":" + (dialog.minute < 10 ? "0" : "" ) + dialog.minute : "") +
                        (dialog.fields & DateTime.Seconds ? ":" + (dialog.second < 10 ? "0" : "") + dialog.second : "") +
                        (dialog.hourMode & DateTime.TwentyFourHours ? "" : (dialog.hour > 11 ? " pm" : " am"))
                }
            onRejected: {
                signalValue.text = "Reject"
                dialogValue.text = (dialog.fields & DateTime.Hours ? (dialog.hourMode & DateTime.TwentyFourHours ? dialog.hour : (dialog.hour > 12 ? dialog.hour - 12 : dialog.hour)) : "") +
                        (dialog.fields & DateTime.Minutes ? ":" + (dialog.minute < 10 ? "0" : "" ) + dialog.minute : "") +
                        (dialog.fields & DateTime.Seconds ? ":" + (dialog.second < 10 ? "0" : "") + dialog.second : "") +
                        (dialog.hourMode & DateTime.TwentyFourHours ? "" : (dialog.hour > 11 ? " pm" : " am"))
            }
        }
    }
}
