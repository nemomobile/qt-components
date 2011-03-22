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

Item {
    focus: true
    onFocusChanged: normalChoiceList.forceActiveFocus()

    ChoiceList {
        id: normalChoiceList

        x: 10; y: 10; width: 300
        currentIndex: 2
        model: ["One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"]
        onClosed: parent.focus = true
        onCurrentIndexChanged: console.log("normalChoiceList " + currentIndex + " option selected")
    }

    ChoiceList {
        id: disabledChoiceList

        enabled: false
        x: 10; y: 70; width: 300
        currentIndex: 0
        model: ["Disabled"]
        onClosed: parent.focus = true;
        onCurrentIndexChanged: console.log("disabledChoiceList " + currentIndex + " option selected")
    }

    ChoiceList {
        id: editChoiceList

        x: 10; y: 130; width: 300
        currentIndex: 2
        model: ["Option 1", "Option 2", "Option 3"]
        onClosed: parent.focus = true
        onCurrentIndexChanged: console.log("editChoiceList " + currentIndex + " option selected")
    }

    ChoiceList {
        id: upperSideChoiceList

        x: 10; y: 400; width: 300
        currentIndex: 0
        model: ["A", "B", "C", "D", "E"]
        onClosed: parent.focus = true
        onCurrentIndexChanged: console.log("upperSideChoiceList " + currentIndex + " option selected")
    }
}
