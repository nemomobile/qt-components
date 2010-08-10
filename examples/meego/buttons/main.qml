/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import Qt 4.7
import com.meego 1.0

Item {
    width: 800
    height: 300

    ButtonRow {
        id: buttons
        groupType: typeSelection.checkedButton.text
        x: 30
        y: 30
        property int count: 3

        Button {
            width: 100
            height: 50
            text: "Button 1"
        }

        Button {
            width: 100
            height: 50
            text: "Button 2"
            checked: true
        }

        Button {
            width: 100
            height: 50
            text: "Button 3"
        }
    }

    Button {
        id: advanced
        width: 100
        height: 50
        anchors {
            top: buttons.bottom
            topMargin: 40
            left: buttons.left
        }
        text: "More..."
        checkable: true
    }

    Button {
        id: another
        visible: advanced.checked
        width: 200
        height: 50
        text: "Add another!"

        anchors {
            left: advanced.right
            leftMargin: 20
            top: advanced.top
        }

        onClicked: {
            buttons.count += 1;
            var b = Qt.createQmlObject("import com.meego 1.0; Button { checkable: true; width: 100; height: 50 }", buttons);
            b.text = "Button " + buttons.count;
        }
    }

    Text {
        visible: advanced.checked
        anchors {
            verticalCenter: advanced.verticalCenter
            left: another.right
            leftMargin: 20
        }
        text: {
            if (typeSelection.checkedButton) {
                if (typeSelection.checkedButton.text === "One")
                    return "The checked is '" + buttons.checkedButton.text + "'";
                if (typeSelection.checkedButton.text === "Many")
                    return "More than one option can be selected.";
            }
            return "Just clickable buttons in a row...";
        }
    }

    ButtonColumn {
        id: typeSelection
        visible: advanced.checked
        anchors {
            left: advanced.left
            top: advanced.bottom
            topMargin: 20
        }

        Button {
            text: "None"
            width: 100
            height: 50
        }

        Button {
            text: "One"
            width: 100
            height: 50
            checked: true
        }

        Button {
            text: "Many"
            width: 100
            height: 50
        }
    }
}
