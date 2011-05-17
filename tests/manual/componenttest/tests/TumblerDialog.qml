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
    id: container

    Button {
        id: button
        anchors.top: parent.top
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: 206
        text: "Open Tumbler"
        onClicked: dialog.open()
    }

    function displaySelections() {
        button.text =
            monthList.get(monthColumn.selectedIndex).value + " " +
            dayList.get(dayColumn.selectedIndex).value + " " +
            yearList.get(yearColumn.selectedIndex).value;
    }

    TumblerDialog {
        id: dialog
        titleText: "Date of birth"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        columns: [ monthColumn, dayColumn, yearColumn ]
        onAccepted: displaySelections()
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
}
