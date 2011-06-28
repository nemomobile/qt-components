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
import com.nokia.symbian 1.1
import Qt.labs.components 1.1
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

        Label {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Letter spacing"
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

        Label {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Point size"
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

        Label {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Word spacing"
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

        Label {
            width: parent.itemWidth
            horizontalAlignment: Text.AlignHCenter
            text: "Pixel Size"
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
