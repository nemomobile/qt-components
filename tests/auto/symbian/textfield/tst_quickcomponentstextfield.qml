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

Column {
    id: column
    anchors.leftMargin: 5
    anchors.topMargin: 5
    anchors.rightMargin: 5
    anchors.bottomMargin: 5
    spacing: 5
    height: 360
    width: 640

    Component.onCompleted: screen.allowedOrientations = Screen.Portrait

    TextField {
        id: textField
        objectName: "textField"
        text: "test"
        width: parent.width
    }

    TextField {
        id: textFieldWithValidator
        objectName: "textFieldWithValidator"
        text: "50"
        width: parent.width
        validator: IntValidator {bottom: 11; top: 31;}
    }

    TextField {
        id: textFieldImpSize
        objectName: "textFieldImpSize"
    }

    Button {
        id: testButton
        objectName: "testButton"
        text: "Click Me!"
        width: parent.width
        onClicked: forceActiveFocus()
    }
}
