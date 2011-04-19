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
import com.nokia.symbian 1.0
import com.nokia.extras 1.0


Page {
    id: root
    anchors.fill: parent

    property DatePickerDialog dateDialog

    Column {
        anchors { margins: 16; top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom }

        Button {
            id: dialogButton
            objectName: "dialogButton"
            text: "Launch Date Picker"
            onClicked: {
                if (!dateDialog)
                    dateDialog = component.createObject(root)
                dateDialog.open()
            }
        }

        Row {
            Text {
                font { bold: true; pixelSize: 16 }
                color: "white"
                text: "Dialog Value (d/m/yyyy): "
            }
            Text {
                id: dialogValue
                objectName: "dialogValue"
                font.pixelSize: 16
                color: "white"
                text: ""
            }
        }
    }

    Component {
        id: component
        DatePickerDialog {
            id: dialog
            titleText: "Date of birth"
            acceptButtonText: "Ok"
            rejectButtonText: "Cancel"
            onAccepted: {
                dialogValue.text = dialog.day + " " + dialog.month + " " + dialog.year
            }
            onRejected: {
                dialogValue.text = dialog.day + " " + dialog.month + " " + dialog.year
            }
        }
    }
}
