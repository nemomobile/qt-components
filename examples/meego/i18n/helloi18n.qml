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

Item {
    id: wrapper

    Component.onCompleted: {
        console.log("**********************************************************************************");

        console.log("Set turkish locale");
        locale.setLocale("tr_TR");

        console.log("MLocaleTest:: locale.isValid: " + locale.valid);
        console.log("MLocaleTest:: locale.calendarType: " + locale.calendarType);
        console.log("\nSwitch to BuddhistCalendar\n");
        locale.calendarType = Locale.BuddhistCalendar;
        console.log("MLocaleTest:: locale.calendarType: " + locale.calendarType);
        console.log("MLocaleTest:: locale.defaultTimeFormat24h: " + locale.defaultTimeFormat24h);
        console.log("MLocaleTest:: locale.timeFormat24h: " + locale.timeFormat24h);
        console.log("\nSwitch to TwelveHourTimeFormat24h\n");
        locale.timeFormat24h = Locale.TwelveHourTimeFormat24h;
        console.log("MLocaleTest:: locale.timeFormat24h: " + locale.timeFormat24h);
        console.log("MLocaleTest:: locale.toLower(): " + locale.toLower("THE big BROWN fox JUMPS over THE lazy DOG"));
        console.log("MLocaleTest:: locale.toUpper(): " + locale.toUpper("THE big BROWN fox JUMPS over THE lazy DOG"));
        console.log("MLocaleTest:: locale.languageEndonym: " + locale.languageEndonym);
        console.log("MLocaleTest:: locale.countryEndonym: " + locale.countryEndonym);
        console.log("MLocaleTest:: locale.decimalPoint: " + locale.decimalPoint);
        console.log("MLocaleTest:: locale.language: " + locale.language);
        console.log("MLocaleTest:: locale.country: " + locale.country);
        console.log("MLocaleTest:: locale.script: " + locale.script);
        console.log("MLocaleTest:: locale.variant: " + locale.variant);
        console.log("MLocaleTest:: locale.name: " + locale.name);
        console.log("MLocaleTest:: locale.categoryLanguage(): " + locale.categoryLanguage(Locale.MLcName));
        console.log("MLocaleTest:: locale.categoryCountry(): " + locale.categoryCountry(Locale.MLcName));
        console.log("MLocaleTest:: locale.categoryScript(): " + locale.categoryScript(Locale.MLcName));
        console.log("MLocaleTest:: locale.categoryVariant(): " + locale.categoryVariant(Locale.MLcName));
        console.log("MLocaleTest:: locale.categoryName(): " + locale.categoryName(Locale.MLcName));
        console.log("MLocaleTest:: locale.formatNumber(): " + locale.formatNumber(12345));
        console.log("MLocaleTest:: locale.toInt(): " + locale.toInt("6789"));
        console.log("MLocaleTest:: locale.formatDouble(): - default " + locale.formatDouble(123.456));
        console.log("MLocaleTest:: locale.formatDouble(): - maxPrecision " + locale.formatDouble(123.456, 1));
        console.log("MLocaleTest:: locale.formatDouble(): - maxMinPrecision " + locale.formatDouble(123.456, 9, 9));
        console.log("MLocaleTest:: locale.toDouble(): " + locale.toDouble("987.654"));
        console.log("MLocaleTest:: locale.formatPercent(): " + locale.formatPercent(123.456));

        console.log("MLocaleTest:: locale.directionForText(\"latin\"): " + (locale.directionForText("latin") == Qt.LeftToRight ? "Qt.LeftToRight" : "Qt.RightToLeft"));
        console.log("MLocaleTest:: locale.directionForText(\"اختبار عربية\"): " + (locale.directionForText("اختبار عربية") == Qt.LeftToRight ? "Qt.LeftToRight" : "Qt.RightToLeft"));

        console.log("\nMLocaleTest:: locale.translationPaths:" + locale.translationPaths);
        console.log("Date/Time functionality:\n");
        
        var myDate = new Date("October 13, 1975 11:13:00")
        //var myDate = new Date(80, 3, 24);
        console.log("myDate is " + myDate);
        console.log("MLocaleTest:: locale.formatDateTime(): " + locale.formatDateTime(myDate));

        var newDate = locale.parseDateTime("13 Ekim 1975", Locale.DateLong, Locale.TimeNone);
        console.log("MLocaleTest:: locale.parseDateTime: Date \"13 Ekim 1975\" has been parsed to: " + newDate);

        console.log("MLocaleTest:: locale.installTrCatalog()");
        locale.installTrCatalog("common");
        console.log("MLocaleTest:: locale.isInstalledTrCatalog(): " + locale.isInstalledTrCatalog("common"));

        console.log("MLocaleTest:: locale.setDataPaths()");
        console.log("MLocaleTest:: locale.dataPaths: " + locale.dataPaths + "\n");

        console.log("\nFinally switching to arabic locale:");
        locale.setLocale("ar_AE");
        console.log("MLocaleTest:: locale.toLocalizedNumbers(): " + locale.toLocalizedNumbers("0123456789"));
        console.log("MLocaleTest:: locale.toLocalizedNumbers(): - static: " + locale.toLocalizedNumbers("0123456789", "٠١٢٣٤٥٦٧٨٩"));
        console.log("MLocaleTest:: locale.toLatinNumbers(): " + locale.toLatinNumbers("٠١٢٣٤٥٦٧٨٩"));
        console.log();
 
        Qt.quit();
        
    }
}

