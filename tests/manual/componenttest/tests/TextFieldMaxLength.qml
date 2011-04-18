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

Item {
    id: root

    QtObject {
        id: privy

        function generateContent(length) {
            var content = ""
            for (var i = 0; i < length; i++) {
                var code = Math.round(Math.random()* (122 - 97) + 97)
                content += String.fromCharCode(code)
            }
            textField.text = content
        }
    }


    Column {
        anchors.fill: parent

        TextField {
            id: textField
            objectName: "textField"
            width: parent.width

            KeyNavigation.down: charsToGenerate
        }

        Text {
            id: lengthIndicator
            anchors.horizontalCenter: textField.horizontalCenter
            color: "white"
            text: textField.text.length + "/" + textField.platformMaximumLength
        }

        Row {
            width: parent.width
            height: parent.height / 6

            TextField {
                id: charsToGenerate
                objectName: "charsToGenerate"
                width: parent.width * 2/3
                placeholderText: "Number of chars to gen."
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1 }

                KeyNavigation.up: textField
                KeyNavigation.down: adjustMaxLength
                KeyNavigation.right: generate
            }

            Button {
                id: generate
                objectName: "generate"
                width: parent.width * 1/3
                height: parent.height
                text: "Generate"

                KeyNavigation.up: textField
                KeyNavigation.down: adjustMaxLength
                KeyNavigation.left: charsToGenerate

                onClicked: privy.generateContent(charsToGenerate.text)
            }
        }

        Slider {
            Text {
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
                text: "Max length"
                color: "white"
            }

            id: adjustMaxLength
            objectName: "adjustMaxLength"
            width: parent.width
            focus: true
            minimumValue: 0
            maximumValue: 32767
            stepSize: 1
            value: 32767
            valueIndicatorVisible: true

            KeyNavigation.up: charsToGenerate

            onValueChanged: textField.platformMaximumLength = value
        }
    }
}
