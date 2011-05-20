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
import "." 1.0

ImplicitSizeItem {
    id: root
    implicitWidth: screen.width
    implicitHeight: privateStyle.statusBarHeight

    QtObject {
        id: priv
        objectName: "priv"

        property int contentHeight: Math.round(privateStyle.statusBarHeight * 18 / 26)
        property int paddingSmallOneQuarter: Math.round(platformStyle.paddingSmall / 4)
        property int paddingSmallThreeQuarters: Math.round(platformStyle.paddingSmall * 3 / 4)

        function signalWidthPercentage(signalStrength) {
            if (signalStrength < 10)
                return 0;
            else if (signalStrength < 20)
                return 1/5;
            else if (signalStrength < 30)
                return 2/5;
            else if (signalStrength < 60)
                return 3/5;
            else if (signalStrength < 100)
                return 4/5;
            else
                return 1;
        }

        function convertedBatteryLevel(level) {
            // Convert scale 1-100 to 1-7
            return Math.floor(1 + (level * 6.9999 / 100))
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            privateStyle.play(Symbian.PopUp)
            platformPopupManager.privateShowIndicatorPopup()
        }
    }

    BorderImage {
        source: privateStyle.imagePath("qtg_fr_statusbar")
        anchors.fill: parent
        width: parent.width

        UniversalIndicators {
            height: indicatorHeight
            anchors {
                right: networkMode.left
                rightMargin: priv.paddingSmallOneQuarter
                verticalCenter: parent.verticalCenter
                }

            width: priv.paddingSmallOneQuarter * 2 + 3 * indicatorWidth

            indicatorColor: platformStyle.colorNormalLight
            indicatorWidth: priv.contentHeight // same as height
            indicatorHeight: priv.contentHeight
            indicatorPadding: priv.paddingSmallOneQuarter
            maxIndicatorCount: 3
        }

        // icon for network signal type e.g. 3G, GPRS etc
        NetworkIndicator {
            id: networkMode
            height: priv.contentHeight
            width: priv.contentHeight // same as height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: signalBackground.left
            anchors.rightMargin: priv.paddingSmallThreeQuarters
            color: platformStyle.colorNormalLight
        }
        // signal strength
        Image {
            id: signalBackground
            sourceSize.height: priv.contentHeight
            sourceSize.width: Math.round(privateStyle.statusBarHeight * 19 / 26)
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: batteryBackground.left
            anchors.rightMargin: priv.paddingSmallThreeQuarters
            fillMode: Image.PreserveAspectFit
            source: privateStyle.imagePath("qtg_graf_signal_level_bg")
            Item {
                id: signalLevelItem
                anchors.left: parent.left
                anchors.top: parent.top
                height: parent.height
                width: priv.signalWidthPercentage(networkInfo.networkSignalStrength) * parent.width
                clip: true
                Image {
                    sourceSize.width: signalBackground.sourceSize.width
                    sourceSize.height: signalBackground.sourceSize.height
                    fillMode: Image.PreserveAspectFit
                    source: privateStyle.imagePath("qtg_graf_signal_level_full")
                }
            }
        }
        // battery indicator
        Image {
            id: batteryBackground
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: timeItem.left
            anchors.rightMargin: priv.paddingSmallThreeQuarters
            sourceSize.height: priv.contentHeight
            sourceSize.width: Math.round(privateStyle.statusBarHeight * 24 / 26)
            fillMode: Image.PreserveAspectFit
            source: privateStyle.imagePath("qtg_graf_battery_level_bg")

            Item {
                id: batteryLevel

                property int animatedLevel

                anchors.left: parent.left
                anchors.top: parent.top
                width: Math.round(privateStyle.statusBarHeight
                    * (priv.convertedBatteryLevel(batteryInfo.powerState == BatteryInfo.WallPowerChargingBattery
                    ? animatedLevel : batteryInfo.batteryLevel) + 2) / 13)
                height: parent.height
                clip: true

                Image {
                    sourceSize.width: batteryBackground.sourceSize.width
                    sourceSize.height: batteryBackground.sourceSize.height

                    fillMode: Image.PreserveAspectFit

                    // Battery state mappings: Levels 0 and 1 are low, 2-4 are medium, 5-7 are full.
                    // Currently all levels use same graphics with white color.

                    source: privateStyle.imagePath("qtg_graf_battery_level_full")
                }
            }

            NumberAnimation {
                id: batteryChargingAnimation
                loops: Animation.Infinite
                running: batteryInfo.powerState == BatteryInfo.WallPowerChargingBattery
                target: batteryLevel
                property: "animatedLevel"
                from: 1
                to: 100
                duration: 3500
            }
        }
        // clock
        Text {
            id: timeItem
            width: Math.round(privateStyle.statusBarHeight * 44 / 26)
            color: platformStyle.colorNormalLight
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: platformStyle.paddingSmall
            horizontalAlignment: Text.AlignRight
            text: symbian.currentTime
            font {
                family: platformStyle.fontFamilyRegular;
                pixelSize: priv.contentHeight
                weight: Font.Light
            }
        }
    }
}

