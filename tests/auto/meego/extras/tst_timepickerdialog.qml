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
import QtQuickTest 1.0
import com.nokia.extras 1.1

Item {
    width: 600
    height: 600

    TimePickerDialog {
        id: td
        titleText: "titleText"
        hour: 3
        minute: 4
        second: 5
        mode24Hour: true
        showSeconds: true
        acceptButtonText: "accept"
        rejectButtonText: "reject"
    }

    TimePickerDialog {
        id: td2
        titleText: "titleText2"
        hour: 3
        minute: 4
        second: 5
        mode24Hour: false
        showSeconds: true
        acceptButtonText: "accept"
        rejectButtonText: "reject"
    }

    TimePickerDialog {
        id: td3
        titleText: "titleText3"
        hour: 6
        minute: 7
        second: 8
        mode24Hour: true
    }

    TimePickerDialog {
        id: td4
        titleText: "titleText4"
        hour: 9
        minute: 10
        second: 11
        showSeconds: false
    }

    TestCase {
        id: test1
        name: "TimePickerDialog"

        function init() {
            td.open();
            td2.open();
            td3.open();
            td4.open();
        }

        function test_property_titleText() {
            compare(td.titleText, "titleText");
            td.titleText = "tt"
            compare(td.titleText, "tt");
        }

        function test_property_hour() {
            compare(td.hour, 3);
            td.hour = 6;
            compare(td.hour, 6);

            // out of bound cases
            td.hour = 24;
            compare(td.hour, 23);

            td.hour = -1;
            compare(td.hour, 0);
        }

        function test_property_minute() {
            compare(td.minute, 4);
            td.minute = 8;
            compare(td.minute, 8);

            // out of bound cases
            td.minute = 60;
            compare(td.minute, 59);

            td.minute = -1;
            compare(td.minute, 0);
        }

        function test_property_second() {
            compare(td.second, 5);
            td.second = 10;
            compare(td.second, 10);

            // out of bound cases
            td.second = 60;
            compare(td.second, 59);

            td.second = -1;
            compare(td.second, 0);
        }

        function test_property_mode24Hour() {
            compare(td3.mode24Hour, true);
            td3.mode24Hour = false;
            compare(td3.mode24Hour, false);
            compare(td3.hour, 6);
            compare(td3.minute, 7);
            compare(td3.second, 8);
        }

        function test_property_showSeconds() {
            compare(td4.showSeconds, false);
            td4.showSeconds = true;
            compare(td4.showSeconds, true);
            compare(td4.hour, 9);
            compare(td4.minute, 10);
            compare(td4.second, 11);
        }

        function test_property_acceptButtonText() {
            compare(td.acceptButtonText, "accept");
            td.acceptButtonText = "a";
            compare(td.acceptButtonText, "a");
        }

        function test_property_rejectButtonText() {
            compare(td.rejectButtonText, "reject");
            td.rejectButtonText = "r";
            compare(td.rejectButtonText, "r");
        }
    }
}
