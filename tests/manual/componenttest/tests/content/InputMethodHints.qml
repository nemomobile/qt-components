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

ListModel {
    ListElement {
        title: "HiddenText"
        //Qt.ImhHiddenText
        flag: 1
        selected: false
        category: "Altering behavior"
    }

    ListElement {
        title: "NoAutoUppercase"
        //Qt.ImhNoAutoUppercase
        flag: 2
        selected: false
        category: "Altering behavior"
    }

    ListElement {
        title: "PreferNumbers"
        // Qt.ImhPreferNumbers
        flag: 4
        selected: false
        category: "Altering behavior"
    }

    ListElement {
        title: "PreferUppercase"
        // Qt.ImhPreferUppercase
        flag: 8
        selected: false
        category: "Altering behavior"
    }

    ListElement {
        title: "PreferLowercase"
        // Qt.ImhPreferLowercase
        flag: 16
        selected: false
        category: "Altering behavior"
    }

    ListElement {
        title: "NoPredictiveText"
        // Qt.ImhNoPredictiveText
        flag: 32
        selected: false
        category: "Altering behavior"
    }

    ListElement {
        title: "DigitsOnly"
        // Qt.ImhDigitsOnly
        flag: 65536
        selected: false
        category: "Restricting input"
    }

    ListElement {
        title: "FormattedNumbersOnly"
        // Qt.ImhFormattedNumbersOnly
        flag: 131072
        selected: false
        category: "Restricting input"
    }

    ListElement {
        title: "UppercaseOnly"
        // Qt.ImhUppercaseOnly
        flag: 262144
        selected: false
        category: "Restricting input"
    }

    ListElement {
        title: "LowercaseOnly"
        // Qt.ImhLowercaseOnly
        flag: 524288
        selected: false
        category: "Restricting input"
    }

    ListElement {
        title: "DialableCharactersOnly"
        // Qt.ImhDialableCharactersOnly
        flag: 1048576
        selected: false
        category: "Restricting input"
    }

    ListElement {
        title: "EmailCharactersOnly"
        // Qt.ImhEmailCharactersOnly
        flag: 2097152
        selected: false
        category: "Restricting input"
    }

    ListElement {
        title: "UrlCharactersOnly"
        // Qt.ImhUrlCharactersOnly
        flag: 4194304
        selected: false
        category: "Restricting input"
    }
}

