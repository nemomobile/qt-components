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

Page {
    id: root
    anchors.fill: parent
    property bool childrenInverted: mainWindow.childrenInverted

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
            width: inPortrait() ? screen.width - 6 * platformStyle.paddingMedium
                     : screen.width - 18 * platformStyle.paddingLarge
            columns: [monthColumn, dayColumn, yearColumn]
            platformInverted: root.childrenInverted
        }
    }

    Column {
        id: col
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: inPortrait() ? parent.width : parent.width - tumbler.width

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
                platformInverted: root.childrenInverted
            }

            Button {
                id: monthButton
                text: "Month"
                width: parent.buttonWidth
                checkable: true
                checked: true
                platformInverted: root.childrenInverted
            }

            Button {
                id: yearButton
                text: "Year"
                width: parent.buttonWidth
                checkable: true
                checked: true
                platformInverted: root.childrenInverted
            }

            Button {
                text: "Set Today"
                width: parent.buttonWidth
                platformInverted: root.childrenInverted
                onClicked: {
                    var d = new Date();
                    dayColumn.selectedIndex = d.getDate() - 1;
                    monthColumn.selectedIndex = d.getMonth();
                    yearColumn.selectedIndex = d.getFullYear() - 2000;
                    extraColumn.selectedIndex = d.getDay();
                }
            }

            Button {
                checkable: true
                text: "Enabled"
                checked: true
                width: parent.buttonWidth
                platformInverted: root.childrenInverted
                onCheckedChanged: {
                    dayColumn.enabled = !dayColumn.enabled;
                    monthColumn.enabled = !monthColumn.enabled;
                    yearColumn.enabled = !yearColumn.enabled;
                    extraColumn.enabled = !extraColumn.enabled;
                }
            }

            Button {
                id: col4
                checkable: true
                text: "4 columns"
                checked: false
                width: parent.buttonWidth
                platformInverted: root.childrenInverted
                onClicked: {
                    if (checked) {
                        tumbler.columns = [monthColumn, dayColumn, yearColumn, extraColumn];
                        col2.checked = false;
                    } else {
                        tumbler.columns = [monthColumn, dayColumn, yearColumn]
                    }
                }
            }

            Button {
                id: col2
                checkable: true
                text: "2 columns"
                checked: false
                width: parent.buttonWidth
                platformInverted: root.childrenInverted
                onClicked: {
                    if (checked) {
                        tumbler.columns = [dayColumn, yearColumn];
                        col4.checked = false;
                    } else {
                        tumbler.columns = [monthColumn, dayColumn, yearColumn]
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
        selectedIndex: 9
        items: daysList
        visible: dayButton.checked

        onSelectedIndexChanged: {
            console.log("dayColumn index changed: " + selectedIndex )
        }
    }

    TumblerColumn {
        id: monthColumn

        selectedIndex: 4
        items: monthsList
        visible: monthButton.checked

        onSelectedIndexChanged: {
            console.log("monthColumn index changed: " + selectedIndex )
        }
    }

    TumblerColumn {
        id: yearColumn

        selectedIndex: 10
        items: yearsList
        visible: yearButton.checked

        onSelectedIndexChanged: {
            console.log("yearColumn index changed: " + selectedIndex )
        }
    }

    TumblerColumn {
        id: extraColumn

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
