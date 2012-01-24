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
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Item {
    width: 600
    height: 600

    DatePickerDialog {
        id: dd
        titleText: "titleText"
        year: 2011
        month: 2
        day: 3
        minimumYear: 2000
        maximumYear: 3000
        acceptButtonText: "accept"
        rejectButtonText: "reject"
    }

    DatePickerDialog {
        id: dd2
        titleText: "titleText2"
    }

    DatePickerDialog {
        id: dd3
        minimumYear: 2000
        titleText: "titleText3"
    }

    DatePickerDialog {
        id: dd4
        maximumYear: 3000
        titleText: "titleText4"
    }

    DatePickerDialog {
        id: dd5
        titleText: "titleText5"
        minimumYear: 2000
    }

    TestCase {
        id: test1
        name: "DatePickerDialog"

        function init() {
            dd.open();
            dd2.open();
            dd3.open();
            dd4.open();
            dd5.open();
        }

        function test_property_titleText() {
            compare(dd.titleText, "titleText");
            dd.titleText = "tt"
            compare(dd.titleText, "tt");
        }

        function test_property_year() {
            compare(dd.year, 2011);
            dd.year = 2050;
            compare(dd.year, 2050);

            // out of bound case
            dd.year = 3030;
            // TODO: better way to force it to be in open state
            // when this line is executed the dialog is still in opening state
            // due to fade in animation
            dd.status = DialogStatus.Open;
            dd.close();
            compare(dd.year, 3000);
            
            // TODO: better way to force it to be in open state
            // when this line is executed the dialog is still in closing state
            // due to fade in animation
            dd.status = DialogStatus.Closed;
            dd.year = 1900;
            dd.open();
            // TODO: better way to force it to be in open state
            // when this line is executed the dialog is still in opening state
            // due to fade in animation
            dd.status = DialogStatus.Open;
            dd.close();
            compare(dd.year, 2000);

            // check default year
            var currentYear = new Date().getFullYear();
            var min = currentYear - 1;
            var max = currentYear + 20;

            compare(dd2.year, min + 1);
            compare(dd2.minimumYear, min);
            compare(dd2.maximumYear, max);
        }

        function test_property_month() {
            compare(dd.month, 2);
            dd.month = 8;
            compare(dd.month, 8);

            // out of bound case
            dd.month = 30;
            compare(dd.month, 12);

            dd.month = 0;
            compare(dd.month, 1);
        }

        function test_property_day() {
            compare(dd.day, 3);
            dd.day = 9;
            compare(dd.day, 9);

            // out of bound case
            dd5.minimumYear = 1990
            dd5.year = 2000;
            dd5.month = 2;

            dd5.day = 30;
            compare(dd5.day, 29);

            dd5.day = 0;
            compare(dd5.day, 1);
        }

        function test_property_minimumYear() {
            // change min/max year results in reset
            compare(dd3.minimumYear, 2000);
            dd3.minimumYear = 2050;
            // TODO: better way to force it to be in open state
            // when this line is executed the dialog is still in opening state
            // due to fade in animation
            dd3.status = DialogStatus.Open;
            dd3.close();
            compare(dd3.minimumYear, 2031);
        }

        function test_property_maximumYear() {
            // change min/max year results in reset
            compare(dd4.maximumYear, 3000);
            dd4.maximumYear = 3050;
            compare(dd4.maximumYear, 3050);
        }

        function test_property_acceptButtonText() {
            compare(dd.acceptButtonText, "accept");
            dd.acceptButtonText = "a";
            compare(dd.acceptButtonText, "a");
        }

        function test_property_rejectButtonText() {
            compare(dd.rejectButtonText, "reject");
            dd.rejectButtonText = "r";
            compare(dd.rejectButtonText, "r");
        }
    }
}
