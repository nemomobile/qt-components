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
import "." 1.0

ImplicitSizeItem {
    id: root
    implicitWidth: screen.width
    implicitHeight: privateStyle.statusBarHeight

    Component.onCompleted: priv.updateBatteryStatus()

    QtObject {
        id: priv
        objectName: "priv"

        property int imageHeight: privateStyle.statusBarHeight - 2 * platformStyle.paddingSmall
        function batteryImage(level) {
            if (level == 100)
                return privateStyle.imagePath("qtg_graf_battery_level_full")
            else if (level > 20)
                return privateStyle.imagePath("qtg_graf_battery_level_medium")
            else
                return privateStyle.imagePath("qtg_graf_battery_level_low")
        }

        function updateBatteryStatus() {
            if (batteryInfo.powerState == BatteryInfo.WallPowerChargingBattery) {
                batteryBackground.state = "Charging";
                batteryChargingAnimation.start();
            } else if (batteryInfo.batteryStatus == BatteryInfo.BatteryCritical) {
                batteryBackground.state = "PowerSave";
                batteryChargingAnimation.stop();
            } else {
                batteryBackground.state = "Normal";
                batteryChargingAnimation.stop();
            }
            updateBatteryImage();
        }

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

        function animationStarted() {
            batteryLevelImage.source = privateStyle.imagePath("qtg_graf_battery_level_full")
        }

        function updateBatteryImage() {
            if (batteryInfo.powerState != BatteryInfo.WallPowerChargingBattery) {
                if (batteryInfo.batteryStatus >= BatteryInfo.BatteryVeryLow) {
                    batteryLevelItem.width = batteryBackground.width;
                    batteryLevelImage.width = batteryBackground.width;
                    batteryChargingAnimation.stop();
                    batteryLevelImage.source = priv.batteryImage(batteryInfo.batteryLevel);
                } else {
                    batteryLevelImage.source = "";
                }
            }
        }
    }

    BorderImage {
        source: privateStyle.imagePath("qtg_fr_statusbar")
        anchors.fill: parent
        width: parent.width

        Connections {
            target: batteryInfo
            onPowerStateChanged: priv.updateBatteryStatus()
            onBatteryLevelChanged: priv.updateBatteryStatus()
            onBatteryStatusChanged: priv.updateBatteryStatus()
        }

        // icon for network signal type e.g. 3G, GPRS etc
        Image {
            id: networkMode
            sourceSize.height: priv.imageHeight
            sourceSize.width: Symbian.UndefinedSourceDimension
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: signalBackground.left
            anchors.rightMargin: platformStyle.paddingSmall
            fillMode: Image.PreserveAspectFit
            source: privateStyle.imagePath(networkInfo.networkMode == NetworkInfo.GsmMode
                ? "qtg_graf_signal_gprs_att_icon" : "qtg_graf_signal_icon")
        }
        Image {
            id: signalBackground
            sourceSize.height: priv.imageHeight
            sourceSize.width: Symbian.UndefinedSourceDimension
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: batteryBackground.left
            anchors.rightMargin: platformStyle.paddingSmall
            fillMode: Image.PreserveAspectFit
            source: privateStyle.imagePath("qtg_graf_signal_level_bg")
            Item {
                id: signalLevelItem
                height: parent.height
                width: priv.signalWidthPercentage(networkInfo.networkSignalStrength) * parent.width
                clip: true
                Image {
                    sourceSize.height: priv.imageHeight
                    sourceSize.width: Symbian.UndefinedSourceDimension
                    anchors.left: parent.left
                    anchors.top: parent.top
                    fillMode: Image.PreserveAspectFit
                    source: privateStyle.imagePath("qtg_graf_signal_level_full")
                }
            }
        }
        Image {
            id: batteryBackground
            sourceSize.height: priv.imageHeight
            sourceSize.width: Symbian.UndefinedSourceDimension
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: timeItem.left
            anchors.rightMargin: platformStyle.paddingSmall
            fillMode: Image.PreserveAspectFit
            source: privateStyle.imagePath("qtg_graf_battery_level_bg")
            smooth: true
            state: "Normal"
            Item {
                id: batteryLevelItem
                anchors.right: parent.right
                clip: true
                width: parent.width
                height: parent.height
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                Image {
                    id: batteryLevelImage
                    fillMode: Image.PreserveAspectCrop
                    smooth: true
                    clip:  true
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    source: privateStyle.imagePath(batteryInfo.batteryStatus >= BatteryInfo.BatteryVeryLow
                        ? "qtg_graf_battery_level_full" : "qtg_graf_battery_level_low")
                }
            }
            states: [
                State {
                    name:"Charging"
                    PropertyChanges { target: batteryLevelImage; source: privateStyle.imagePath("qtg_graf_battery_level_full") }
                },
                State {
                    name:"Normal"
                    PropertyChanges { target: batteryLevelImage; width: batteryBackground.width; source: priv.batteryImage(batteryInfo.batteryLevel); }
                },
                State {
                    name:"PowerSave"
                    PropertyChanges { target: batteryLevelImage; source: ""; width: 0 }
                }
            ]
            SequentialAnimation {
                id: batteryChargingAnimation
                loops: Animation.Infinite
                ScriptAction { script: priv.animationStarted(); }
                NumberAnimation {
                    target: batteryLevelItem
                    property: "width"
                    from: 0.2 * batteryBackground.width
                    to: 0.5 * batteryBackground.width
                    duration: 2000
                }
                NumberAnimation {
                    target: batteryLevelItem
                    property: "width"
                    from: 0.5 * batteryBackground.width
                    to: batteryBackground.width * .8
                    duration: 2000
                }
                ScriptAction { script: priv.updateBatteryImage(); }
            }
        }
        Text {
            id: timeItem
            // TODO: what's the correct way of defining the width?
            width: privateStyle.textWidth("23:59", timeItem.font)
            color: platformStyle.colorNormalLight
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: platformStyle.paddingSmall
            horizontalAlignment: Text.AlignRight
            text: symbian.currentTime
            font { family: platformStyle.fontFamilyRegular; pixelSize: 0.8 * priv.imageHeight }
        }
    }
}

