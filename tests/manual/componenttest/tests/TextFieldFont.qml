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
import Qt.labs.components 1.0
import "../components"

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
        property real itemHeight: (height - spacing * (rows - 1)) / rows
        property real itemWidth: (width - spacing * (columns - 1)) / columns
        anchors {
            left: parent.left
            top: privy.portrait ? textField.bottom : parent.top
            margins: privy.margin
        }
        columns: privy.portrait ? 4 : 1
        rows: privy.portrait ? 1 : 4
        height: parent.height * (privy.portrait ? 1/10 : 1)
        width: parent.width * (privy.portrait ? 1 : 1/10)
        spacing: privy.margin

        Button {
            id: bold; objectName: "bold"
            height: parent.itemHeight; width: parent.itemWidth
            checkable: true; text: "B"

        }

        Button {
            id: italic; objectName: "italic"
            height: parent.itemHeight; width: parent.itemWidth
            checkable: true; text: "I"
        }

        Button {
            id: strikeout; objectName: "strikeout"
            height: parent.itemHeight; width: parent.itemWidth
            checkable: true; text: "S"
        }

        Button {
            id: underline; objectName: "underline"
            height: parent.itemHeight; width: parent.itemWidth
            checkable: true; text: "U"
        }
    }

    Grid {
        id: selectors
        property real itemHeight: (height - spacing * (rows - 1)) / rows
        property real itemWidth: (width - spacing * (columns - 1)) / columns
        anchors {
            left: privy.portrait ? parent.left : style.right
            right: parent.right
            top: privy.portrait ? style.bottom : textField.bottom
            margins: privy.margin
        }
        columns: privy.portrait ? 1 : 3
        spacing: privy.margin

        SelectionListItem {
            id: weight
            objectName: "weight"
            width: parent.itemWidth
            title: "Weight"
            subTitle: weightDialog.selectedIndex >= 0
                      ? weightDialog.model[weightDialog.selectedIndex] : "default"
            onClicked: weightDialog.open()

            SelectionDialog {
                id: weightDialog
                titleText: "Select weight"
                selectedIndex: -1
                model: ["Light", "Normal", "DemiBold", "Bold", "Black"]
                onAccepted: {
                    if (selectedIndex == 0)
                        textField.font.weight = Font.Light
                    else if (selectedIndex == 1)
                        textField.font.weight = Font.Normal
                    else if (selectedIndex == 2)
                        textField.font.weight = Font.DemiBold
                    else if (selectedIndex == 3)
                        textField.font.weight = Font.Bold
                    else if (selectedIndex == 4)
                        textField.font.weight = Font.Black
                }
            }
        }

        SelectionListItem {
            id: capitalization
            objectName: "capitalization"
            width: parent.itemWidth
            title: "Capitalization"
            subTitle: capitalizationDialog.selectedIndex >= 0
                      ? capitalizationDialog.model[capitalizationDialog.selectedIndex] : "default"
            onClicked: capitalizationDialog.open()

            SelectionDialog {
                id: capitalizationDialog
                titleText: "Select capitalization"
                selectedIndex: -1
                model: ["MixedCase", "AllUppercase", "AllLowercase", "SmallCaps", "Capitalize"]
                onAccepted: {
                    if (selectedIndex == 0)
                        textField.font.capitalization = Font.MixedCase
                    else if (selectedIndex == 1)
                        textField.font.capitalization = Font.AllUppercase
                    else if (selectedIndex == 2)
                        textField.font.capitalization = Font.AllLowercase
                    else if (selectedIndex == 3)
                        textField.font.capitalization = Font.SmallCaps
                    else if (selectedIndex == 4)
                        textField.font.capitalization = Font.Capitalize
                }
            }
        }

        SelectionListItem {
            id: family
            objectName: "family"
            width: parent.itemWidth
            title: "Font"
            subTitle: textField.font.family
            onClicked: fontSelector.open()

            FontSelectionDialog {
                id: fontSelector
                onAccepted: textField.font.family = fontSelector.fontFamily
            }
        }
    }

    Grid {
        id: sliders
        property real itemWidth: (width - spacing * (columns - 1)) / columns
        property real sliderHeight: 30
        anchors {
            left: privy.portrait ? parent.left : style.right
            right: parent.right
            top: selectors.bottom
            margins: privy.margin
        }
        columns: 2
        spacing: privy.margin
        flow: Grid.TopToBottom

        Text {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Letter spacing"
            color: "white"
            font { pixelSize: platformStyle.fontSizeSmall }
        }

        Slider {
            id: letterSpacing; objectName: "letterSpacing"
            width: parent.itemWidth
            height: parent.sliderHeight
            minimumValue: -10; maximumValue: 10; stepSize: 1
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.letterSpacing  = value
            Component.onCompleted: value = textField.font.letterSpacing
        }

        Text {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Point size"
            color: "white"
            font { pixelSize: platformStyle.fontSizeSmall }
        }

        Slider {
            id: pointSize; objectName: "pointSize"
            width: parent.itemWidth
            height: parent.sliderHeight
            minimumValue: 0; maximumValue: 25; stepSize: 1
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.pointSize = value
            Component.onCompleted: value = textField.font.pointSize
        }

        Text {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Word spacing"
            color: "white"
            font { pixelSize: platformStyle.fontSizeSmall }
        }

        Slider {
            id: wordSpacing; objectName: "wordSpacing"
            width: parent.itemWidth
            height: parent.sliderHeight
            minimumValue: -10; maximumValue: 10; stepSize: 1
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.wordSpacing  = value
            Component.onCompleted: value = textField.font.wordSpacing
        }

        Text {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Pixel Size"
            color: "white"
            font { pixelSize: platformStyle.fontSizeSmall }
        }

        Slider {
            id: pixelSize; objectName: "pixelSize"
            width: parent.itemWidth
            height: parent.sliderHeight
            minimumValue: 0; maximumValue: 25; stepSize: 1
            valueIndicatorVisible: true; anchors.margins: privy.margin
            onValueChanged: if (pressed) textField.font.pixelSize = value
            Component.onCompleted: value = textField.font.pixelSize
        }
    }
}
