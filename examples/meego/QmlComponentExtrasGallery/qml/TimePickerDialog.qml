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
import com.nokia.meego 1.1
import com.nokia.extras 1.1

Page {
    id: container
    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    property int index: -1

    Item {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Button {
            id: tumblerButton
            anchors.top: parent.top
            anchors.topMargin: 10
            text: "Default Time Picker"
            width: 300
            height: 35
            onClicked: launchDialog()
        }

        Button {
            id: todayButton
            anchors.top: tumblerButton.bottom
            anchors.topMargin: 10
            text: "4:05pm (12)"
            width: 300
            height: 35
            onClicked: launchDialog(16, 5, DateTime.TwelveHours, DateTime.All)
        }

        Button {
            id: dateButton
            anchors.top: todayButton.bottom
            anchors.topMargin: 10
            text: "16:10 (24)"
            width: 300
            height: 35
            onClicked: launchDialog(16, 10, DateTime.TwentyFourHours, DateTime.All)
        }

        Button {
            id: noSecondsButton
            anchors.top: dateButton.bottom
            anchors.topMargin: 10
            text: "13:18 (No seconds)"
            width: 300
            height: 35
            onClicked: launchDialog(13, 18, DateTime.TwentyFourHours, DateTime.Hours | DateTime.Minutes)
        }

        Label {
            id: result
            anchors.top: noSecondsButton.bottom
            anchors.topMargin: 10
        }
    }

    function launchDialog(hour, minute, hoursMode, fields) {
        if (hour) {
            tDialog.hour = hour;
            tDialog.minute = minute;
            tDialog.second = 0;
        }
        if (hoursMode != undefined) {
            tDialog.hourMode = hoursMode;
        }
        if (fields != undefined) {
            tDialog.fields = fields;
        }
        tDialog.open();
    }

    function callbackFunction() {
        result.text = tDialog.hour + ":" + (tDialog.minute < 10 ? "0" : "" ) + tDialog.minute  + ":" + (tDialog.second < 10 ? "0" : "") + tDialog.second;
    }

    TimePickerDialog {
        id: tDialog
        titleText: "Time"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        onAccepted: callbackFunction()
    }
}
