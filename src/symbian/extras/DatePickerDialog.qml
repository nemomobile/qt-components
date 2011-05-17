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
import "Constants.js" as C
import "TumblerIndexHelper.js" as TH

CommonDialog {
    id: root

    property int year: 0
    property int month: 1
    property int day: 1
    property int minimumYear: 0
    property int maximumYear: 0
    property alias acceptButtonText: confirmButton.text
    property alias rejectButtonText: rejectButton.text

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
                selectedIndex: root.day - (root.day > 0 ?  1 : 0)
            }

            TumblerColumn {
                id: monthColumn
                textAlignment: "AlignLeft"
                items: ListModel {
                    id: monthList
                }
                selectedIndex: root.month - (root.month > 0 ?  1 : 0)
            }

            TumblerColumn {
                id: yearColumn
                items: ListModel {
                    id: yearList
                }
                selectedIndex: yearList.length > 0 ? root.year - yearList.get(0).value : 0
                privateResizeToFit: true
            }
        }
    }
    buttons: ToolBar {
        id: buttons
        width: parent.width
        height: privateStyle.toolBarHeightLandscape + 2 * platformStyle.paddingSmall
        Row {
            id: buttonRow
            anchors.centerIn: parent
            spacing: platformStyle.paddingMedium

            ToolButton {
                id: confirmButton
                width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                onClicked: accept()
                visible: text != ""
            }
            ToolButton {
                id: rejectButton
                width: (buttons.width - 3 * platformStyle.paddingMedium) / 2
                onClicked: reject()
                visible: text != ""
            }
        }
    }
    onMinimumYearChanged: {
        internal.updateYearList()
    }
    onMaximumYearChanged: {
        internal.updateYearList()
    }
    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            if (!internal.initialised)
                internal.initializeDataModels();
            if (year > 0)
                yearColumn.selectedIndex = root.year - yearList.get(0).value;
            tumbler._handleTumblerChanges(2);
            TH.saveIndex(tumbler);
            dayColumn.selectedIndex = root.day - 1;
        }
    }
    onDayChanged: {
        if (dayColumn.items.length > root.day - 1)
            dayColumn.selectedIndex = root.day - 1
    }
    onMonthChanged: {
        monthColumn.selectedIndex = root.month - 1
    }
    onYearChanged: {
        if (internal.initialised)
            yearColumn.selectedIndex = root.year - yearList.get(0).value
    }
    onAccepted: {
        tumbler.privateForceUpdate();
        root.year = yearColumn.selectedIndex + yearList.get(0).value;
        root.month = monthColumn.selectedIndex + 1;
        root.day = dayColumn.selectedIndex + 1;
    }
    onRejected: {
        TH.restoreIndex(tumbler);
    }

    QtObject {
        id: internal

        property variant initialised: false

        function initializeDataModels() {
            var currentYear = new Date().getFullYear();
            minimumYear = minimumYear ? minimumYear : currentYear;
            maximumYear = maximumYear ? maximumYear : currentYear + 20;

            // ensure sane minimum & maximum year
            if (maximumYear < minimumYear)
                year = maximumYear = minimumYear = 0;  // use default values

            for (var y = minimumYear; y <= maximumYear; ++y)
                yearList.append({"value" : y}) // year

            var nDays = dateTime.daysInMonth(root.year, root.month);
            for (var d = 1; d <= nDays; ++d)
                dayList.append({"value" : d})  // day
            for (var m = 1; m <= 12; ++m)
                monthList.append({"value" : dateTime.longMonthName(m)});

            tumbler.privateInitialize()
            internal.initialised = true;
        }

        function updateYearList() {
            if (internal.initialised) {
                var tmp = yearColumn.selectedIndex;
                yearList.clear();
                for (var i = root.minimumYear; i <= root.maximumYear; ++i)
                    yearList.append({"value" : i})
                yearColumn.selectedIndex = tmp;
            }
        }
    }
}
