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
import "Constants.js" as C
import "TumblerIndexHelper.js" as TH

CommonDialog {
    id: root

    property int year: 0
    property int month: 1
    property int day: 1
    property int minimumYear: 0
    property int maximumYear: 0
    property string acceptButtonText
    property string rejectButtonText

    // TODO do not dismiss the dialog when empty area is clicked

    content: Item {
        id: dialogContent
        height: tumbler.height + platformStyle.paddingLarge * 2
        width: parent.width

        Tumbler {
            id: tumbler

            function _handleTumblerChanges(index) {
                if (index == 0 || index == 2) {
                    var curYear = yearColumn.selectedIndex + yearList.get(0).value;
                    var curMonth = monthColumn.selectedIndex + 1;

                    var d = dateTime.daysInMonth(curYear, curMonth);

                    if (dayColumn.selectedIndex >= d)
                        dayColumn.selectedIndex = d - 1
                    while (dayList.count > d)
                        dayList.remove(dayList.count - 1)
                    while (dayList.count < d)
                        dayList.append({"value" : dayList.count + 1})
                }
            }

            columns:  [monthColumn, dayColumn, yearColumn]
            onChanged: {
                _handleTumblerChanges(index);
            }
            anchors.centerIn: parent
            height: privateStyle.menuItemHeight * 4
            width: parent.width - platformStyle.paddingMedium * 4
            privateDelayInit: true
            platformInverted: root.platformInverted
            states: State {
                when: screen.currentOrientation == Screen.Landscape || screen.currentOrientation == Screen.LandscapeInverted
                PropertyChanges {
                    target: tumbler; height: privateStyle.menuItemHeight * 3
                }
            }

            TumblerColumn {
                id: dayColumn
                width: privateStyle.menuItemHeight
                items: ListModel {
                    id: dayList
                }
                selectedIndex: 0
            }

            TumblerColumn {
                id: monthColumn
                privateTextAlignment: Text.AlignLeft
                items: ListModel {
                    id: monthList
                }
                selectedIndex: 0
            }

            TumblerColumn {
                id: yearColumn
                items: ListModel {
                    id: yearList
                }
                selectedIndex: yearList.length > 0 ? internal.year - yearList.get(0).value : 0
                privateResizeToFit: true
            }
        }
    }

    onAcceptButtonTextChanged: internal.updateButtonTexts()
    onRejectButtonTextChanged: internal.updateButtonTexts()

    onButtonClicked: (acceptButtonText && index == 0) ? accept() : reject()

    onMinimumYearChanged: {
        if (!internal.surpassUpdate) {
            internal.year = root.year
            internal.minYear = root.minimumYear
            
            if (internal.minYear < 0)
                internal.minYear = internal.currentYear;
            else if (internal.minYear > root.maximumYear)
                internal.minYear = root.maximumYear;

            internal.updateYearList()
            internal.validateDate()
            internal.year = internal.year < internal.minYear ? internal.minYear : 
                            (internal.year > root.maximumYear ? root.maximumYear :internal.year) 
        }
    }
    onMaximumYearChanged: {
        internal.minYear = root.minimumYear
        
        if (root.maximumYear < 0)
            root.maximumYear = internal.currentYear + 20;
        else if (root.maximumYear < internal.minYear)
            root.maximumYear = internal.minYear;

        internal.updateYearList()
        internal.validateDate()
        internal.year = internal.year > root.maximumYear ? root.maximumYear : 
                        (internal.year < internal.minYear ? internal.minYear : internal.year)
        if (internal.minYear < 0)
            root.minimumYear = internal.currentYear
    }
    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            TH.saveIndex(tumbler);
            if (!internal.initialised)
                internal.initializeDataModels();
            internal.resetYear()
            internal.resetMonth()
            internal.resetDay()
            TH.saveIndex(tumbler)
            TH.restoreIndex(tumbler)
            if (internal.year > 0)
                yearColumn.selectedIndex = internal.year - yearList.get(0).value;
            tumbler._handleTumblerChanges(2);
            dayColumn.selectedIndex = root.day - 1;
        }
        if (status == DialogStatus.Closing) {
            internal.surpassUpdate = true
            if (internal.surpassUpdate) {
                root.year = internal.year
                root.minimumYear = internal.minYear
            }
            internal.surpassUpdate = false
        }
    }
    onDayChanged: {
        internal.validateDate()
        if (dayColumn.items.length > root.day - 1)
            dayColumn.selectedIndex = root.day - 1
    }
    onMonthChanged: {
        internal.validateDate()
        monthColumn.selectedIndex = root.month - 1
    }
    onYearChanged: {
        if (!internal.surpassUpdate) {
            internal.year = root.year
            internal.validateDate()
            internal.year = internal.year < internal.minYear ? internal.minYear : 
                                  (internal.year > root.maximumYear ? root.maximumYear : internal.year)
            if (internal.initialised)
                yearColumn.selectedIndex = internal.year - yearList.get(0).value  
        }
    }
    onAccepted: {
        tumbler.privateForceUpdate();
        root.year = yearColumn.selectedIndex + yearList.get(0).value;
        root.month = monthColumn.selectedIndex + 1;
        root.day = dayColumn.selectedIndex + 1;
    }
    onRejected: {
        internal.resetYear()
        internal.resetMonth()
        internal.resetDay()
        TH.saveIndex(tumbler)
        TH.restoreIndex(tumbler)
    }

    QtObject {
        id: internal

        property variant initialised: false
        property int year
        property int minYear
        property bool surpassUpdate: false
        property int currentYear: new Date().getFullYear();

        function initializeDataModels() {
            minimumYear = minimumYear ? minimumYear : internal.currentYear;
            maximumYear = maximumYear ? maximumYear : internal.currentYear + 20;

            // ensure sane minimum & maximum year
            if (maximumYear < minimumYear)
                year = maximumYear = minimumYear = 0;  // use default values

            for (var y = minimumYear; y <= maximumYear; ++y)
                yearList.append({"value" : y}) // year

            var nDays = dateTime.daysInMonth(internal.year, root.month);
            for (var d = 1; d <= nDays; ++d)
                dayList.append({"value" : d})  // day
            for (var m = 1; m <= 12; ++m)
                monthList.append({"value" : dateTime.longMonthName(m)});

            tumbler.privateInitialize()

            if (year < minimumYear) year = minimumYear;
            if (year > maximumYear) year = maximumYear;

            internal.initialised = true;
        }

        function updateYearList() {
            if (internal.initialised) {
                var tmp = yearColumn.selectedIndex;
                yearList.clear();
                for (var i = internal.minYear; i <= root.maximumYear; ++i)
                    yearList.append({"value" : i})
                if (tmp < yearList.count) {
                    yearColumn.selectedIndex = 0;
                    yearColumn.selectedIndex = tmp;
                }
            }
        }

        function validateDate() {
            if (internal.year < 0){
                internal.year = new Date().getFullYear()
                if (maximumYear < internal.year)
                    root.maximumYear = internal.currentYear + 20;
                if (minimumYear > internal.year)
                    internal.minYear = internal.currentYear;
            }
            
            root.month = Math.max(1, Math.min(12, root.month))
            var d = dateTime.daysInMonth(internal.year, root.month);
            root.day = Math.max(1, Math.min(d, root.day))
        }

        function updateButtonTexts() {
            var newButtonTexts = []
            if (acceptButtonText)
                newButtonTexts.push(acceptButtonText)
            if (rejectButtonText)
                newButtonTexts.push(rejectButtonText)
            root.buttonTexts = newButtonTexts
        }

        function resetDay() {
            if (internal.initialised)
                dayColumn.selectedIndex = root.day - (root.day > 0 ?  1 : 0);
            tumbler._handleTumblerChanges(1);
        }

        function resetMonth() {
            if (internal.initialised)
                monthColumn.selectedIndex = root.month - (root.month > 0 ?  1 : 0)
            tumbler._handleTumblerChanges(0);
        }

        function resetYear() {
            if (internal.initialised)
                yearColumn.selectedIndex = root.year - yearList.get(0).value
            tumbler._handleTumblerChanges(2);
        }
    }
}
