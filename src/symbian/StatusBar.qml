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

    QtObject {
        id: priv
        objectName: "priv"

        property int imageHeight: privateStyle.statusBarHeight - 2 * platformStyle.paddingSmall

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

        function batteryWidthPercentage(batteryLevel) {
            return 0.2 + (0.6 * batteryLevel / 100)
        }
    }

    BorderImage {
        source: privateStyle.imagePath("qtg_fr_statusbar")
        anchors.fill: parent
        width: parent.width

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
            Item {
                id: batteryLevelItem
                property int animatedLevel
                anchors.right: parent.right
                anchors.top: parent.top
                height: parent.height
                width: parent.width * priv.batteryWidthPercentage(
                    batteryInfo.powerState == BatteryInfo.WallPowerChargingBattery
                    ? animatedLevel : batteryInfo.batteryLevel)
                clip: true
                Image {
                    sourceSize.height: priv.imageHeight
                    sourceSize.width: Symbian.UndefinedSourceDimension
                    anchors.right: parent.right
                    anchors.top: parent.top
                    fillMode: Image.PreserveAspectFit
                    source: privateStyle.imagePath("qtg_graf_battery_level_full")
                }
            }
            SequentialAnimation {
                id: batteryChargingAnimation
                loops: Animation.Infinite
                running: batteryInfo.powerState == BatteryInfo.WallPowerChargingBattery
                NumberAnimation {
                    target: batteryLevelItem
                    property: "animatedLevel"
                    from: 0
                    to: 100
                    duration: 4000
                }
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

