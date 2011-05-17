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
    id: root
    anchors.fill: parent
    anchors.margins: privy.margin

    QtObject {
        id: privy
        property bool portrait:    screen.currentOrientation == Screen.Portrait
                                || screen.currentOrientation == Screen.PortraitInverted
        property int margin: 5
    }

    TextField {
        id: textField; objectName: "textField"
        anchors {
            left: privy.portrait ? parent.left : style.right
            right: parent.right
            margins: privy.margin
        }
        text: "Lorem ipsum dolor sit amet."
        font.bold: bold.checked
        font.italic: italic.checked
        font.strikeout: strikeout.checked
        font.underline: underline.checked
    }

    Grid {
        id: style
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors {
            left: parent.left
            top: privy.portrait ? textField.bottom : parent.top
            margins: privy.margin
        }
        columns: privy.portrait ? 4 : 1
        rows: privy.portrait ? 1 : 4
        height: parent.height * (privy.portrait ? 1/6 : 1)
        width: parent.width * (privy.portrait ? 1 : 1/6)
        spacing: privy.margin

        Button {
            id: bold; objectName: "bold"
            height: parent.h; width: parent.w
            checkable: true; text: "B"

        }

        Button {
            id: italic; objectName: "italic"
            height: parent.h; width: parent.w
            checkable: true; text: "I"
        }

        Button {
            id: strikeout; objectName: "strikeout"
            height: parent.h; width: parent.w
            checkable: true; text: "S"
        }

        Button {
            id: underline; objectName: "underline"
            height: parent.h; width: parent.w
            checkable: true; text: "U"
        }
    }

    ChoiceList {
        id: family; objectName: "family"
        anchors {
            left: privy.portrait ? parent.left : style.right
            top: privy.portrait ? style.bottom : textField.bottom
            right: parent.right
            margins: privy.margin
        }
        height: parent.height * (privy.portrait ? 1/10 : 1/6)
        currentIndex: 0
        model: Qt.fontFamilies()
        onCurrentIndexChanged: textField.font.family = model[currentIndex]
        Component.onCompleted: {
            var found = false
            for (var i = 0; i < model.length && !found; ++i) {
                if (model[i] == textField.font.family) {
                    found = true
                    currentIndex = i
                }
            }
        }
    }

    Row {
        id: weightAndCapitalization
        property real w: (width - spacing * (children.length - 1)) / children.length
        anchors {
            left: privy.portrait ? parent.left : style.right
            right: parent.right
            top: family.bottom
            margins: privy.margin
        }
        height: parent.height * (privy.portrait ? 1/10 : 1/6)
        spacing: privy.margin

        ChoiceList {
            id: weight; objectName: "weight"
            height: parent.h; width: parent.w
            currentIndex: 1
            model: ["Light", "Normal", "DemiBold", "Bold", "Black"]

            onCurrentIndexChanged: {
                if (currentIndex == 0)
                    textField.font.weight = Font.Light
                else if (currentIndex == 1)
                    textField.font.weight = Font.Normal
                else if (currentIndex == 2)
                    textField.font.weight = Font.DemiBold
                else if (currentIndex == 3)
                    textField.font.weight = Font.Bold
                else if (currentIndex == 4)
                    textField.font.weight = Font.Black
            }
        }

        ChoiceList {
            id: capitalization; objectName: "capitalization"
            height: parent.h; width: parent.w
            currentIndex: 0
            model: ["MixedCase", "AllUppercase", "AllLowercase", "SmallCaps", "Capitalize"]

            onCurrentIndexChanged: {
                if (currentIndex == 0)
                    textField.font.capitalization = Font.MixedCase
                else if (currentIndex == 1)
                    textField.font.capitalization = Font.AllUppercase
                else if (currentIndex == 2)
                    textField.font.capitalization = Font.AllLowercase
                else if (currentIndex == 3)
                    textField.font.capitalization = Font.SmallCaps
                else if (currentIndex == 4)
                    textField.font.capitalization = Font.Capitalize
            }
        }
    }

    Grid {
        id: sliders
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors {
            left: privy.portrait ? parent.left : style.right
            right: parent.right
            top: weightAndCapitalization.bottom
            bottom: parent.bottom
            margins: privy.margin
        }
        columns: privy.portrait ? 1 : 2
        rows: privy.portrait ? 4 : 2
        height: parent.height * (privy.portrait ? 1/2 : 1/6)
        spacing: privy.margin

        Slider {
            id: letterSpacing; objectName: "letterSpacing"
            height: parent.h; width: parent.w;
            minimumValue: -10; maximumValue: 10; stepSize: 1;
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.letterSpacing  = value
            Component.onCompleted: value = textField.font.letterSpacing

            Text {
                text: "Letter spacing"
                color: "white"
                font { pointSize: 5; bold: true }
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
            }
        }

        Slider {
            id: wordSpacing; objectName: "wordSpacing"
            height: parent.h; width: parent.w;
            minimumValue: -10; maximumValue: 10; stepSize: 1;
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.wordSpacing  = value
            Component.onCompleted: value = textField.font.wordSpacing

            Text {
                text: "Word spacing"
                color: "white"
                font { pointSize: 5; bold: true }
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
            }
        }

        Slider {
            id: pointSize; objectName: "pointSize"
            height: parent.h; width: parent.w;
            minimumValue: 0; maximumValue: 25; stepSize: 1;
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.pointSize = value
            Component.onCompleted: value = textField.font.pointSize

            Text {
                text: "Point size"
                color: "white"
                font { pointSize: 5; bold: true }
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
            }
        }

        Slider {
            id: pixelSize; objectName: "pixelSize"
            height: parent.h; width: parent.w;
            minimumValue: 0; maximumValue: 25; stepSize: 1;
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.pixelSize = value
            Component.onCompleted: value = textField.font.pixelSize

            Text {
                text: "Pixel Size"
                color: "white"
                font { pointSize: 5; bold: true }
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
            }
        }
    }
}
