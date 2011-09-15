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

PageStackWindow {
    id: rootWindow
    initialPage: page

    Page {
        id: page

        function createNewLocale(loc) {
            locale.setLocale(loc);

            // we have to update some parts of the model manually :-(

            localeModel[4]['value']   = locale.toLower(sampleTextInput.text);
            localeModel[5]['value']   = locale.toUpper(sampleTextInput.text);

            localeModel[14]['value']  = locale.categoryLanguage(Locale.MLcName);
            localeModel[15]['value']  = locale.categoryCountry(Locale.MLcName);
            localeModel[16]['value']  = locale.categoryScript(Locale.MLcName);
            localeModel[17]['value']  = locale.categoryVariant(Locale.MLcName);
            localeModel[18]['value']  = locale.categoryName(Locale.MLcName);
            localeModel[19]['value']  = locale.formatNumber(12345);
            localeModel[20]['value']  = locale.toInt("6789");
            localeModel[21]['value']  = locale.formatDouble(123.456);
            localeModel[22]['value']  = locale.toDouble("987.654");
            localeModel[23]['value']  = locale.formatPercent(123.456);
        }

        tools: ToolBarLayout {
            id: viewMenuBar
            visible: true
            ToolIcon {
                iconId: "toolbar-view-menu"
                 anchors.right: parent.right
                onClicked: (localeSelector.status == DialogStatus.Closed) ? localeSelector.open() : localeSelector.close()
            }
        }

        Label {
            id: sampleTextLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 30
            text: "Enter sample text:"
        }

        TextField {
            id: sampleTextInput
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: sampleTextLabel.bottom
            anchors.leftMargin: 25
            anchors.rightMargin: 25
            anchors.topMargin: 5
            maximumLength: 45
            text: "THE big BROWN fox JUMPS over THE lazy DOG"

            //we loose the property binding after a locale switch
            onTextChanged: {
                page.localeModel[4]['value']   = locale.toLower(sampleTextInput.text);
                page.localeModel[5]['value']   = locale.toUpper(sampleTextInput.text);
            }

        }

        property list<QtObject>  localeModel: [
            QtObject { property string key: "Locale is Valid:";           property string value: locale.valid; },
            QtObject { property string key: "CalendarType:";              property string value: locale.calendarType; },
            QtObject { property string key: "DefaultTimeFormat24h:";      property string value: locale.defaultTimeFormat24h; },
            QtObject { property string key: "TimeFormat24h:";             property string value: locale.timeFormat24h;},
            QtObject { property string key: "toLower:";                   property string value: locale.toLower(sampleTextInput.text); },
            QtObject { property string key: "toUpper:";                   property string value: locale.toUpper(sampleTextInput.text); },
            QtObject { property string key: "languageEndonym:";           property string value: locale.languageEndonym; },
            QtObject { property string key: "countryEndonym:";            property string value: locale.countryEndonym; },
            QtObject { property string key: "decimalPoint:";              property string value: locale.decimalPoint; },
            QtObject { property string key: "language:";                  property string value: locale.language; },
            QtObject { property string key: "country:";                   property string value: locale.country; },
            QtObject { property string key: "script:";                    property string value: locale.script; },
            QtObject { property string key: "variant:";                   property string value: locale.variant; },
            QtObject { property string key: "name:";                      property string value: locale.name; },
            QtObject { property string key: "categoryLanguage:";          property string value: locale.categoryLanguage(Locale.MLcName); },
            QtObject { property string key: "categoryCountry:";           property string value: locale.categoryCountry(Locale.MLcName); },
            QtObject { property string key: "categoryScript:";            property string value: locale.categoryScript(Locale.MLcName); },
            QtObject { property string key: "categoryVariant:";           property string value: locale.categoryVariant(Locale.MLcName); },
            QtObject { property string key: "categoryName:";              property string value: locale.categoryName(Locale.MLcName); },
            QtObject { property string key: "formatNumber (12345):";      property string value: locale.formatNumber(12345); },
            QtObject { property string key: "toInt (\"6789\"):";          property string value: locale.toInt("6789"); },
            QtObject { property string key: "formatDouble (123.456):";    property string value: locale.formatDouble(123.456); },
            QtObject { property string key: "toDouble (\"987.654\"):";    property string value: locale.toDouble("987.654"); },
            QtObject { property string key: "formatPercent (123.456):";   property string value: locale.formatPercent(123.456); }
        ]


        ListView {
            id: list
            anchors.top: sampleTextInput.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            model: page.localeModel
            clip: true
            anchors.margins: 25
            delegate: Item {
                id: testerItem
                height: 22 + 50
                width: parent.width - 50
                x: 25; y: 25

                Label {anchors.left: parent.left;   text: model.modelData.key}
                Label {anchors.right: parent.right; text: model.modelData.value}

            }
        }

        Menu {
            id: localeSelector
            MenuLayout {
                MenuItem { text: "en_GB"; onClicked: page.createNewLocale("en_GB"); }
                MenuItem { text: "de_DE"; onClicked: page.createNewLocale("de_DE"); }
                MenuItem { text: "tr_TR"; onClicked: page.createNewLocale("tr_TR"); }
            }
        }

    }

}

