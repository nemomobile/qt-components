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
import Settings 1.0
import "../components"

Item {
    id: root
    property bool platformInverted: false

    QtObject {
        id: privy
        property bool landscape:
            screen.currentOrientation == Screen.Landscape
            || screen.currentOrientation == Screen.LandscapeInverted
    }


    Settings {
        id: settings
        property bool toggleUsbIndicator: false
        property bool toggleEmailIndicator: false
        property bool toggleBluetoothIndicator: false
        property bool toggleSecureConnectionIndicator: false
    }

    Menu {
        id: optionsMenu

        content: MenuLayout {
            MenuItem {
                text: settings.toggleUsbIndicator ? "Turn off USB" : "Turn on USB"
                onClicked: {
                    settings.toggleUsbIndicator = !settings.toggleUsbIndicator
                    settings.setIndicatorState(Settings.IndicatorUsb, settings.toggleUsbIndicator)
                    }
                }

            MenuItem {
                text: settings.toggleEmailIndicator ? "Turn off email" : "Turn on email"
                onClicked: {
                    settings.toggleEmailIndicator = !settings.toggleEmailIndicator
                    settings.setIndicatorState(Settings.IndicatorEMail, settings.toggleEmailIndicator)
                    }
                }

            MenuItem {
                text: settings.toggleBluetoothIndicator ? "Turn off BT" : "Turn on BT"
                onClicked: {
                    settings.toggleBluetoothIndicator = !settings.toggleBluetoothIndicator
                    settings.setIndicatorState(Settings.IndicatorBluetooth, settings.toggleBluetoothIndicator)
                    }
                }

            MenuItem {
                text: settings.toggleSecureConnectionIndicator ? "Turn off secure" : "Turn on secure"
                onClicked: {
                    settings.toggleSecureConnectionIndicator = !settings.toggleSecureConnectionIndicator
                    settings.setIndicatorState(
                        Settings.IndicatorSecuredConnection,
                        settings.toggleSecureConnectionIndicator)
                    }
                }
            }
    }

    Connections {
        target: optionsButton
        onClicked: optionsMenu.open()
    }

    Grid {
        id: grid

        anchors.fill: parent
        columns: privy.landscape ? 2 : 1

        Item {
            width: privy.landscape ? grid.width / 2 : grid.width
            height: privy.landscape ? grid.height - 20 : grid.height / 2 - 20

            Rectangle {
                width: indicators.implicitWidth
                height: indicators.implicitHeight
                anchors.centerIn: indicators
                color: "#00000000"
                border.color: "blue"
            }

            UniversalIndicators {
                id: indicators

                objectName: "indicators"
                anchors.centerIn: parent
            }

        }

        Column {
            width: privy.landscape ? grid.width / 2 : grid.width
            height: privy.landscape ? grid.height : grid.height / 2

            SelectionListItem {
                id: indicatorColorSetting

                property SelectionDialog colorSelectionDialog

                title: {
                    if (colorSelectionDialog) {
                        if (colorSelectionDialog.selectedIndex >= 0)
                            return colorSelectionDialog.model.get(colorSelectionDialog.selectedIndex).name
                    }
                    return "White"
                }
                width: parent.width

                onClicked: {
                    if (!colorSelectionDialog)
                        colorSelectionDialog = colorSelectionDialogComponent.createObject(indicatorColorSetting)
                    colorSelectionDialog.open()
                }

                Component {
                    id: colorSelectionDialogComponent

                    SelectionDialog {
                        titleText: "Select color"
                        selectedIndex: 5
                        platformInverted: root.platformInverted

                        model: ListModel {
                            id: colorModel

                            ListElement { name: "Red" }
                            ListElement { name: "Blue" }
                            ListElement { name: "Green" }
                            ListElement { name: "Yellow" }
                            ListElement { name: "Black" }
                            ListElement { name: "White" }
                            ListElement { name: "Grey" }
                            ListElement { name: "Orange" }
                            ListElement { name: "Pink" }
                        }

                        onAccepted: { indicators.indicatorColor = colorModel.get(selectedIndex).name }
                    }
                }
            }

            Label {
                width: parent.width
                font.pixelSize: platformStyle.fontSizeSmall * 2 / 3
                text: "Indicator size"
            }

            Slider {
                id: indicatorSizeSetting

                width: parent.width
                height: 40
                maximumValue: 100
                stepSize: 1
                valueIndicatorVisible: true

                Component.onCompleted: value = indicators.indicatorWidth
                onValueChanged: {
                    indicators.indicatorWidth = value
                    indicators.indicatorHeight = value
                }
            }

            Label {
                width: parent.width
                font.pixelSize: platformStyle.fontSizeSmall * 2 / 3
                text: "Indicator padding"
            }

            Slider {
                id: indicatorPaddingSetting

                width: parent.width
                height: 40
                maximumValue: 10
                stepSize: 1
                valueIndicatorVisible: true

                Component.onCompleted: value = indicators.indicatorPadding
                onValueChanged: indicators.indicatorPadding = value
            }

            Label {
                width: parent.width
                font.pixelSize: platformStyle.fontSizeSmall * 2 / 3
                text: "Max indicator count"
            }

            Slider {
                id: indicatorMaxCountSetting

                width: parent.width
                height: 40
                minimumValue: -1
                maximumValue: 11
                stepSize: 1
                valueIndicatorVisible: true

                Component.onCompleted: value = indicators.maxIndicatorCount
                onValueChanged: indicators.maxIndicatorCount = value
            }

            Button {
                id: resetButton

                width: parent.width
                text: "Reset"

                Component.onCompleted: clicked()

                onClicked: {
                    indicators.indicatorColor = root.platformInverted ? platformStyle.colorNormalLightInverted
                                                                      : platformStyle.colorNormalLight
                    indicatorSizeSetting.value = Math.round(privateStyle.statusBarHeight * 18 / 26)
                    indicatorPaddingSetting.value = Math.round(platformStyle.paddingSmall / 4)
                    indicatorMaxCountSetting.value = 3
                }
            }
        }
    }
}
