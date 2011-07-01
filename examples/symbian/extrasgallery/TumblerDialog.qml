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

Item {
    id: container
    anchors.fill: parent
    property bool childrenInverted: mainWindow.childrenInverted

    Button {
        id: button
        anchors.top: parent.top
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: 206
        text: "Date"
        platformInverted: container.childrenInverted
        onClicked: dialogBirth.open()
    }
    Button {
        id: button2
        anchors.top: button.bottom
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: 206
        text: "Restaurant"
        platformInverted: container.childrenInverted
        onClicked: dialogRestaurant.open()
    }

    function displayBirthday() {
        button.text =
            monthList.get(monthColumn.selectedIndex).value + " " +
            dayList.get(dayColumn.selectedIndex).value + " " +
            yearList.get(yearColumn.selectedIndex).value;
    }

    TumblerDialog {
        id: dialogBirth
        titleText: "Date of birth"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        columns: [ monthColumn, dayColumn, yearColumn ]
        platformInverted: container.childrenInverted
        onAccepted: displayBirthday()
    }

    TumblerDialog {
        id: dialogRestaurant
        titleText: "Search restaurant"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        columns: [ cuisineColumn, mealColumn, priceColumn ]
        platformInverted: container.childrenInverted
    }

    function initializeDataModels() {
        for (var year = 2000; year <= 2020; year++) {
            yearList.append({"value" : year});
        }

        for (var day = 1; day <= 31; day++) {
            dayList.append({"value" : day});
        }
    }

    Component.onCompleted: {
        initializeDataModels();
    }

    TumblerColumn {
        id: dayColumn
        items: ListModel { id: dayList }
        selectedIndex: 21
    }

    TumblerColumn {
        id: monthColumn
        items: ListModel {
            id: monthList
            ListElement { value: "Jan" }
            ListElement { value: "Feb" }
            ListElement { value: "Mar" }
            ListElement { value: "Apr" }
            ListElement { value: "May" }
            ListElement { value: "Jun" }
            ListElement { value: "Jul" }
            ListElement { value: "Aug" }
            ListElement { value: "Sep" }
            ListElement { value: "Oct" }
            ListElement { value: "Nov" }
            ListElement { value: "Dec" }
        }
        selectedIndex: 9
    }

    TumblerColumn {
        id: yearColumn
        items: ListModel { id: yearList }
        selectedIndex: 10
    }

    TumblerColumn {
        id: cuisineColumn
        items: ListModel {
            id: cuisineList
            ListElement { value: "African" }
            ListElement { value: "American" }
            ListElement { value: "Asian" }
            ListElement { value: "Australian" }
            ListElement { value: "Brazilian" }
            ListElement { value: "French" }
            ListElement { value: "German" }
            ListElement { value: "Greek" }
            ListElement { value: "Indian" }
            ListElement { value: "Italian" }
            ListElement { value: "Japanese" }
            ListElement { value: "Korean" }
            ListElement { value: "Mexican" }
            ListElement { value: "Other" }
        }
        selectedIndex: 1
    }

    TumblerColumn {
        id: mealColumn
        items: ListModel {
               id: mealList
               ListElement { value: "Appetizers" }
               ListElement { value: "Soup" }
               ListElement { value: "Salad" }
               ListElement { value: "Entree" }
               ListElement { value: "Dessert" }
               ListElement { value: "Wine" }
               ListElement { value: "Breakfast" }
               ListElement { value: "Lunch" }
               ListElement { value: "Dinner" }
        }
        selectedIndex: 1
    }

    TumblerColumn {
        id: priceColumn
        items: ListModel {
            id: priceList
            ListElement { value: "< 10" }
            ListElement { value: "10-20" }
            ListElement { value: "20-30" }
            ListElement { value: "30-50" }
            ListElement { value: "50-80" }
            ListElement { value: "80-100" }
            ListElement { value: "> 100" }
        }
        selectedIndex: 1
    }
}
