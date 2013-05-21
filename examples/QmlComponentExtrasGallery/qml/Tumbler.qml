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

    Row {
        Item {
            width: 420
            height: 360

            Tumbler {
                id: tumblerWidget
                anchors { top: parent.top; topMargin: 16; horizontalCenter: parent.horizontalCenter }
                columns: [ dayColumn, monthColumn, yearColumn ]
            }
        }

        Flickable {
            width: col.width
            height: parent.height
            contentWidth: col.width
            contentHeight: col.height

            Column {
                id: col

                ButtonRow {
                    exclusive: false
                    Button {
                        text: "Show 4 columns"
                        onClicked: {
                            tumblerWidget.columns = [ dayColumn, monthColumn, yearColumn, extraColumn ]
                        }
                    }
                    Button {
                        text: "Show 2 columns"
                        onClicked: {
                            tumblerWidget.columns = [ monthColumn, yearColumn ]
                        }
                    }
                }
                Button {
                    text: "Fix width"
                    onClicked: {
                        dayColumn.width = 80
                    }
                }
                Button {
                    text: "Set today"
                    onClicked: {
                        var d = new Date();
                        dayColumn.selectedIndex = d.getDate() - 1;
                        monthColumn.selectedIndex = d.getMonth();
                        yearColumn.selectedIndex = d.getFullYear() - 2000;
                    }
                }

                CheckBox {
                    text: "Day column visible?"
                    checked: true
                    onCheckedChanged: dayColumn.visible = checked
                }
                CheckBox {
                    text: "Month column visible?"
                    checked: true
                    onCheckedChanged: monthColumn.visible = checked
                }
                CheckBox {
                    text: "Year column visible?"
                    checked: true
                    onCheckedChanged: yearColumn.visible = checked
                }

                CheckBox {
                    text: "Day column enabled?"
                    checked: true
                    onCheckedChanged: dayColumn.enabled = checked
                }
                CheckBox {
                    text: "Month column enabled?"
                    checked: true
                    onCheckedChanged: monthColumn.enabled = checked
                }
                CheckBox {
                    text: "Year column enabled?"
                    checked: true
                    onCheckedChanged: yearColumn.enabled = checked
                }
            }
        }
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
        label: "DAY"
        selectedIndex: 21
    }

    TumblerColumn {
        id: monthColumn
        items: ListModel {
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
        label: "MONTH"
        selectedIndex: 9
    }

    TumblerColumn {
        id: yearColumn
        items: ListModel { id: yearList }
        label: "YEAR"
        selectedIndex: 10
    }

    TumblerColumn {
        id: extraColumn
        items: ListModel {
            ListElement { value: "Mon" }
            ListElement { value: "Tue" }
            ListElement { value: "Wed" }
            ListElement { value: "Thurs" }
            ListElement { value: "Fri" }
            ListElement { value: "Sat" }
            ListElement { value: "Sun" }
        }
        label: "DATE"
    }
}
