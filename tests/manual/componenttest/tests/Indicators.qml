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
import Settings 1.0

Item {
    id: root

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
            height: privy.landscape ? grid.height : grid.height / 2

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

            Text {
                width: parent.width
                color: platformStyle.colorNormalLight
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall * 2 / 3 }
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

            Text {
                width: parent.width
                color: platformStyle.colorNormalLight
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall * 2 / 3 }
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

            Text {
                width: parent.width
                color: platformStyle.colorNormalLight
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall * 2 / 3 }
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
                    indicators.indicatorColor = platformStyle.colorNormalLight
                    indicatorSizeSetting.value = Math.round(privateStyle.statusBarHeight * 18 / 26)
                    indicatorPaddingSetting.value = Math.round(platformStyle.paddingSmall / 4)
                    indicatorMaxCountSetting.value = 3
                }
            }
        }
    }
}
