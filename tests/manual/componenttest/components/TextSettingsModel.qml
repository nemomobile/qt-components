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
