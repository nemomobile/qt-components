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

Page {
    anchors.fill: parent

    function inPortrait() {
        return screen.height > screen.width
    }

    Item {
        id: item
        anchors.left: parent.left
        anchors.right: inPortrait() ? parent.right : col.left
        anchors.top: parent.top
        anchors.bottom: inPortrait() ? col.top : parent.bottom

        Tumbler {
            id: tumbler
            anchors { top: parent.top; topMargin: 32; horizontalCenter: parent.horizontalCenter }
            columns: [dayColumn, monthColumn, yearColumn]
        }
    }

    Column {
        id: col
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: inPortrait() ? parent.width : parent.width / 3

        Grid {
            id: grid
            rows: inPortrait() ? 4 : 6
            columns: inPortrait() ? 3 : 2
            property real buttonWidth: inPortrait() ? parent.width / 3 : parent.width / 2

            Button {
                id: dayButton
                text: "Day"
                width: parent.buttonWidth
                checkable: true
                checked: true
            }

            Button {
                id: monthButton
                text: "Month"
                width: parent.buttonWidth
                checkable: true
                checked: true
            }

            Button {
                id: yearButton
                text: "Year"
                width: parent.buttonWidth
                checkable: true
                checked: true
            }

            Button {
                text: "Set Today"
                width: parent.buttonWidth
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var d = new Date();
                        dayColumn.selectedIndex = d.getDate() - 1;
                        monthColumn.selectedIndex = d.getMonth();
                        yearColumn.selectedIndex = d.getFullYear() - 2000;
                        extraColumn.selectedIndex = d.getDay();
                    }
                }
            }

            Button {
                text: "Set label"
                width: parent.buttonWidth
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        dayColumn.label = "JOUR";
                        monthColumn.label = "MOIS";
                        yearColumn.label = "ANNEE";
                        extraColumn.label = "DATTE";
                    }
                }
            }

            Button {
                text: "Clear label"
                width: parent.buttonWidth
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        dayColumn.label = "";
                        monthColumn.label = "";
                        yearColumn.label = "";
                        extraColumn.label = "";
                    }
                }
            }

            Button {
                text: "Set model"
                width: parent.buttonWidth
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        dayColumn.items = yearsList;
                    }
                }
            }

            Button {
                checkable: true
                text: "Enabled"
                checked: true
                width: parent.buttonWidth
                onCheckedChanged: {
                    dayColumn.enabled = !dayColumn.enabled;
                    monthColumn.enabled = !monthColumn.enabled;
                    yearColumn.enabled = !yearColumn.enabled;
                    extraColumn.enabled = !extraColumn.enabled;
                }
            }

            Button {
                text: "Set width"
                width: parent.buttonWidth
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        dayColumn.width = 200;
                        yearColumn.width = 80;
                    }
                }
            }

            Button {
                text: "4 columns"
                width: parent.buttonWidth
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        tumbler.columns = [dayColumn, monthColumn, yearColumn, extraColumn]
                    }
                }
            }

            Button {
                text: "2 columns"
                width: parent.buttonWidth
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        tumbler.columns = [dayColumn, yearColumn]
                    }
                }
            }
        }
    }

    function initializeDataModels() {
        for (var year = 2000; year <= 2020; year++)
            yearsList.append({"value" : year});

        for (var day = 1; day <= 31; day++)
            daysList.append({"value" : day});
    }

    Component.onCompleted: {
        initializeDataModels();
    }

    ListModel { id: yearsList }
    ListModel { id: daysList }
    ListModel {
        id: monthsList
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

    TumblerColumn {
        id: dayColumn

        width: 100
        label: "DAY"
        selectedIndex: 9
        items: daysList
        visible: dayButton.checked

        onSelectedIndexChanged: {
            console.log("dayColumn index changed: " + selectedIndex )
        }
    }

    TumblerColumn {
        id: monthColumn

        label: "MONTH"
        selectedIndex: 4
        items: monthsList
        visible: monthButton.checked

        onSelectedIndexChanged: {
            console.log("monthColumn index changed: " + selectedIndex )
        }
    }

    TumblerColumn {
        id: yearColumn

        label: "YEAR"
        selectedIndex: 10
        items: yearsList
        visible: yearButton.checked

        onSelectedIndexChanged: {
            console.log("yearColumn index changed: " + selectedIndex )
        }
    }

    TumblerColumn {
        id: extraColumn

        label: "DATE"
        selectedIndex: 2
        items: ListModel {
            ListElement { value: "Sun" }
            ListElement { value: "Mon" }
            ListElement { value: "Tue" }
            ListElement { value: "Wed" }
            ListElement { value: "Thr" }
            ListElement { value: "Fri" }
            ListElement { value: "Sat" }
        }

        onSelectedIndexChanged: {
            console.log("extraColumn index changed: " + selectedIndex )
        }
    }
}
