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
import com.nokia.extras 1.0

Item {
    id: root
    anchors.fill: parent

    property int index: -1
    property TimePickerDialog timeDialog

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
                launchDialog(20, 35, 0, DateTime.TwelveHours, DateTime.Hours | DateTime.Minutes)
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
                launchDialog(16, 12, 35, DateTime.TwelveHours)
                signalValue.text = "None"
            }
        }

        Button {
            objectName: "24HourButton"
            text: "24-hour: 18:27:39"
            width: parent.width
            onClicked: {
                launchDialog(18, 27, 39, DateTime.TwentyFourHours)
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
            Text {
                id: signalName
                color: "white"
                font { bold: true; pixelSize: 16}
                text : "Signal / Func: "
            }
            Text {
                id: signalValue
                objectName: "signalValue"
                color: "white"
                font.pixelSize: 16
                text : "None"
            }
        }

        Row {
            Text {
                color: "white"
                font { bold: true; pixelSize: 16}
                text : "dialogValue: "
            }
            Text {
                id: dialogValue
                objectName: "dialogValue"
                color: "white"
                font.pixelSize: 16
                text: ""
            }
        }
    }

    function launchDialog(hour, minute, seconds, hoursMode, visibleColumns) {
        if (!timeDialog)
            timeDialog = component.createObject(root)
        timeDialog.hourMode = (hoursMode != undefined ? hoursMode : dateTime.hourMode());
        timeDialog.fields = (visibleColumns != undefined ? visibleColumns : DateTime.All);
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
