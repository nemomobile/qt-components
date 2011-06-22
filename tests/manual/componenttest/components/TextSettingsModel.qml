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

ListModel {
    ListElement {
        setting: "Wrap mode"
        multiSelection: false
        initialIndex: 0
        dialogModel: [
            ListElement { name: "Wrap"; value: 4 },
            ListElement { name: "NoWrap"; value: 0 },
            ListElement { name: "WordWrap"; value: 1 },
            ListElement { name: "Anywhere"; value: 3 }
        ]
    }

    ListElement {
        setting: "Horizontal alignment"
        multiSelection: false
        initialIndex: 0
        dialogModel: [
            ListElement { name: "Left"; value: 1 },
            ListElement { name: "Center"; value: 4 },
            ListElement { name: "Right"; value: 2 }
        ]
    }

    ListElement {
        setting: "Vertical alignment"
        multiSelection: false
        initialIndex: 0
        dialogModel: [
            ListElement { name: "Top"; value: 32 },
            ListElement { name: "Center"; value: 128 },
            ListElement { name: "Bottom"; value: 64 }
        ]
    }
    ListElement {
        setting: "Font family"
        multiSelection: false
        initialIndex: 0
        dialogModel: [] // model content added dynamically
    }

    ListElement {
        setting: "Font capitalization"
        multiSelection: false
        initialIndex: 0
        dialogModel: [
            ListElement { name: "MixedCase"; value: 0 },
            ListElement { name: "AllUppercase"; value: 1 },
            ListElement { name: "AllLowercase"; value: 2 },
            ListElement { name: "SmallCaps"; value: 3 },
            ListElement { name: "Capitalize"; value: 4 }
        ]
    }

    ListElement {
        setting: "Font letter spacing"
        multiSelection: false
        initialIndex: 0
        dialogModel: [
            ListElement { name: "0"; value: 0 },
            ListElement { name: "1"; value: 1 },
            ListElement { name: "2"; value: 2 },
            ListElement { name: "3"; value: 3 },
            ListElement { name: "4"; value: 4 },
            ListElement { name: "5"; value: 5 },
            ListElement { name: "6"; value: 6 },
            ListElement { name: "7"; value: 7 },
            ListElement { name: "8"; value: 8 },
            ListElement { name: "9"; value: 9 },
            ListElement { name: "10"; value: 10 }
        ]
    }

    ListElement {
        setting: "Font pixel size"
        multiSelection: false
        initialIndex: 8
        dialogModel: [
            ListElement { name: "8"; value: 8 },
            ListElement { name: "9"; value: 9 },
            ListElement { name: "10"; value: 10 },
            ListElement { name: "11"; value: 11 },
            ListElement { name: "12"; value: 12 },
            ListElement { name: "14"; value: 14 },
            ListElement { name: "16"; value: 16 },
            ListElement { name: "18"; value: 18 },
            ListElement { name: "20"; value: 20 },
            ListElement { name: "22"; value: 22 },
            ListElement { name: "24"; value: 24 },
            ListElement { name: "26"; value: 26 },
            ListElement { name: "28"; value: 28 },
            ListElement { name: "36"; value: 36 },
            ListElement { name: "48"; value: 48 },
            ListElement { name: "72"; value: 72 }
        ]
    }

    ListElement {
        setting: "Font weight"
        multiSelection: false
        initialIndex: 1
        dialogModel: [
            ListElement { name: "Light"; value: 25 },
            ListElement { name: "Normal"; value: 50 },
            ListElement { name: "DemiBold"; value: 63 },
            ListElement { name: "Bold"; value: 75 },
            ListElement { name: "Black"; value: 87 }
        ]
    }

    ListElement {
        setting: "Input method hints"
        multiSelection: true
        values: 0 // for multiSelection values ^= selected values from dialogModel
        initialIndex: 0
        dialogModel: [
            ListElement { name: "HiddenText"; value: 1; selected: false },
            ListElement { name: "NoAutoUppercase"; value: 2; selected: false },
            ListElement { name: "PreferNumbers"; value: 4; selected: false },
            ListElement { name: "PreferUppercase"; value: 8; selected: false },
            ListElement { name: "PreferLowercase"; value: 16; selected: false },
            ListElement { name: "NoPredictiveText"; value: 32; selected: false },
            ListElement { name: "DigitsOnly"; value: 65536; selected: false },
            ListElement { name: "FormattedNumbersOnly"; value: 131072; selected: false },
            ListElement { name: "UppercaseOnly"; value: 262144; selected: false },
            ListElement { name: "LowercaseOnly"; value: 524288; selected: false },
            ListElement { name: "DialableCharactersOnly"; value: 1048576; selected: false },
            ListElement { name: "EmailCharactersOnly"; value: 2097152; selected: false },
            ListElement { name: "UrlCharactersOnly"; value: 4194304; selected: false }
        ]
    }
}
