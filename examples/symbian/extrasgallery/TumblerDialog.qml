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
    id: container

    Button {
        id: button
        anchors.top: parent.top
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: 206
        text: "Date"
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
        onClicked: dialogRestaurant.open()
    }
    Row {
        id: row
        anchors.top: button2.bottom
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 16
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
    Text {
        id: text
        anchors.top: row.bottom
        anchors.left: parent.left
        anchors.leftMargin: 16
        font { bold: true; pixelSize: 16 }
        color: "white"
        text: "Search category: "
    }
    Text {
        id: searchCat1
        objectName: "searchCat1"
        anchors.top: text.bottom
        anchors.left: parent.left
        anchors.leftMargin: 16
        font.pixelSize: 16
        color: "white"
        text: ""
    }
    Text {
        id: searchCat2
        objectName: "searchCat2"
        anchors.top: searchCat1.bottom
        anchors.left: parent.left
        anchors.leftMargin: 16
        font.pixelSize: 16
        color: "white"
        text: ""
    }
    Text {
        id: searchCat3
        objectName: "searchCat3"
        anchors.top: searchCat2.bottom
        anchors.left: parent.left
        anchors.leftMargin: 16
        font.pixelSize: 16
        color: "white"
        text: ""
    }

    function displayBirthday() {
        button.text =
            monthList.get(monthColumn.selectedIndex).value + " " +
            dayList.get(dayColumn.selectedIndex).value + " " +
            yearList.get(yearColumn.selectedIndex).value;
    }

    function displayRestaurant() {
        signalValue.text = "Accept"
        searchCat1.text = cuisineList.get(cuisineColumn.selectedIndex).value;
        searchCat2.text = mealList.get(mealColumn.selectedIndex).value;
        searchCat3.text = priceList.get(priceColumn.selectedIndex).value;
    }

    function displayRejectResult() {
        signalValue.text = "Reject"
        searchCat1.text = 'None';
        searchCat2.text = 'None';
        searchCat3.text = 'None';
    }

    TumblerDialog {
        id: dialogBirth
        titleText: "Date of birth"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        columns: [ monthColumn, dayColumn, yearColumn ]
        onAccepted: displayBirthday()
    }

    TumblerDialog {
        id: dialogRestaurant
        titleText: "Search restaurant"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        columns: [ cuisineColumn, mealColumn, priceColumn ]
        onAccepted: displayRestaurant()
        onRejected: displayRejectResult()
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
