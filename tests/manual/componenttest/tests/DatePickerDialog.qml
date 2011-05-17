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

import QtQuick 1.0
import com.nokia.symbian 1.0
import com.nokia.extras 1.0


Item {
    id: root
    anchors.fill: parent

    property DatePickerDialog dateDialog

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
            Text {
                id: signalName
                font { bold: true; pixelSize: 16 }
                color: "white"
                text : "Signal / Func: "
            }
            Text {
                id: signalValue
                objectName: "signalValue"
                font.pixelSize: 16
                color: "white"
                text : "None"
            }
        }

        Row {
            Text {
                font { bold: true; pixelSize: 16 }
                color: "white"
                text: "Dialog Value (m/d/yyyy): "
            }
            Text {
                id: dialogValue
                objectName: "dialogValue"
                font.pixelSize: 16
                color: "white"
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
