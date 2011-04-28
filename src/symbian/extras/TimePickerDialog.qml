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
import "Constants.js" as C
import "TumblerIndexHelper.js" as TH

Dialog {
    id: root

    property alias titleText: title.text
    property int hour: 0
    property int minute: 0
    property int second: 0
    property int fields: DateTime.All
    property int hourMode: dateTime.hourMode()
    property alias acceptButtonText: acceptButton.text
    property alias rejectButtonText: rejectButton.text

    // TODO do not dismiss the dialog when empty area is clicked

    title: Text {
        id: title
        objectName: "title"
        visible: text.length > 0
        color: platformStyle.colorNormalLight
        font { pixelSize: platformStyle.fontSizeSmall; family: platformStyle.fontFamilyRegular }
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: platformStyle.paddingLarge }
        elide: Text.ElideRight
    }
    content: Item {
        id: content
        height: tumbler.height + platformStyle.paddingLarge * 2
        width: parent.width

        Tumbler {
            id: tumbler

            columns: [hourColumn, minuteColumn, secondColumn, meridiemColumn]
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
                id: hourColumn
                items: ListModel {
                    id: hourList
                }
                selectedIndex: root.hour - ((root.hourMode == DateTime.TwelveHours && root.hour > 11) ? 12 : 0)
                visible: fields & DateTime.Hours
            }

            TumblerColumn {
                id: minuteColumn
                items: ListModel {
                    id: minuteList
                }
                selectedIndex: root.minute
                visible: fields & DateTime.Minutes
            }

            TumblerColumn {
                id: secondColumn
                items: ListModel {
                    id: secondList
                }
                selectedIndex: root.second
                visible: fields & DateTime.Seconds
            }

            TumblerColumn {
                id: meridiemColumn
                items: ListModel {
                    id: meridiemList
                }
                selectedIndex: root.hour > 11 ? 1: 0
                visible: root.hourMode == DateTime.TwelveHours
            }
        }
    }
    buttons: ToolBar {
        id: buttons
        width: parent.width

        Row {
            anchors.centerIn: parent

            ToolButton {
                id: acceptButton
                width: buttons.width / 2
                onClicked: accept()
                visible: text != ""
            }
            ToolButton {
                id: rejectButton
                width: buttons.width / 2
                onClicked: reject()
                visible: text != ""
            }
        }
    }
    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            TH.saveIndex(tumbler);
            if (!internal.initialised)
                internal.initializeDataModels();
        }
    }
    onAccepted: {
        tumbler.privateForceUpdate();
        if (root.hourMode == DateTime.TwelveHours)
            root.hour = hourColumn.selectedIndex + (meridiemColumn.selectedIndex > 0 ? 12 : 0);
        else
            root.hour = hourColumn.selectedIndex;
        root.minute = minuteColumn.selectedIndex;
        root.second = secondColumn.selectedIndex;
    }
    onRejected: {
        TH.restoreIndex(tumbler);
    }
    onHourModeChanged: {
        hourList.clear();
        var tmp = hourColumn.selectedIndex;
        if (root.hourMode == DateTime.TwentyFourHours) {
            tmp = (root.hour < 12 ? tmp : tmp + 12)
            for (var i = 0; i < 24; ++i)
                hourList.append({"value" : (i < 10 ? "0" : "") + i});
        } else {
            tmp = (root.hour < 12 ? tmp : tmp - 12)
            hourList.append({"value" : 12 + ""});
            for (var i = 1; i < 12; ++i)
                hourList.append({"value" : i + ""});
        }
        hourColumn.selectedIndex = -1;
        hourColumn.selectedIndex = tmp;
    }
    onHourChanged: {
        internal.validateTime()
        hourColumn.selectedIndex = root.hour - ((root.hourMode == DateTime.TwelveHours && root.hour > 11) ? 12 : 0)
        meridiemColumn.selectedIndex = root.hour > 11 ? 1: 0
    }
    onMinuteChanged: {
        internal.validateTime()
        minuteColumn.selectedIndex = root.minute
    }
    onSecondChanged: {
        internal.validateTime()
        secondColumn.selectedIndex = root.second
    }

    QtObject {
        id: internal

        property variant initialised: false

        function initializeDataModels() {
            if (root.hourMode == DateTime.TwelveHours) {
                hourList.append({"value" : 12 + ""});
                for (var i = 1; i < 12; ++i)
                    hourList.append({"value" : i + ""});
            }
            for (var i = 0; i < 60; ++i) {
                minuteList.append({"value" : (i < 10 ? "0" : "") + i });
                secondList.append({"value" : (i < 10 ? "0" : "") + i });
            }
            meridiemList.append({"value" : dateTime.amText()});
            meridiemList.append({"value" : dateTime.pmText()});

            tumbler.privateInitialize();
            internal.initialised = true;
        }

        function validateTime() {
            root.hour = Math.max(0, Math.min(23, root.hour))
            root.minute = Math.max(0, Math.min(59, root.minute))
            root.second = Math.max(0, Math.min(59, root.second))
        }
    }
}
