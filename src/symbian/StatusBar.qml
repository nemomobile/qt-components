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
    implicitWidth: style.current.preferredWidth
    implicitHeight: style.current.preferredHeight

    Component.onCompleted: priv.updateBatteryStatus()

    QtObject {
        id: priv
        objectName: "priv"
        function batteryImage(level) {
            if (level == 100)
                return style.current.get("batteryFull");
            else if (level > 20)
                return style.current.get("batteryMedium");
            return style.current.get("batteryLow");
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
            batteryLevelImage.source = style.current.get("batteryFull");
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
        source: style.current.get("background")
        anchors.fill: parent
        width: parent.width

        Style {
            id: style
            styleClass: "StatusBar"
        }

        Connections {
            target: batteryInfo
            onPowerStateChanged: priv.updateBatteryStatus()
            onBatteryLevelChanged: priv.updateBatteryStatus()
            onBatteryStatusChanged: priv.updateBatteryStatus()
        }

        Row {
            id: signalRow
            anchors.right: parent.right
            anchors.margins: style.current.get("rowMargins")
            spacing: style.current.get("rowSpacing")
            // icon for network signal type e.g. 3G, GPRS etc
            Image {
                id: networkMode
                sourceSize.width: style.current.get("signalWidth")
                sourceSize.height: style.current.get("signalHeight")
                anchors.verticalCenter: signalRow.verticalCenter
                source: (networkInfo.networkMode == NetworkInfo.GsmMode)
                         ? style.current.get("signalGprs") : style.current.get("signalGeneric")
            }
            Image {
                id: signalBackground
                sourceSize.width: style.current.get("signalLevelWidth")
                sourceSize.height: style.current.get("signalLevelHeight")
                height: style.current.get("signalLevelHeight")
                source: style.current.get("signalBackground")
                Item {
                    id: signalLevelItem
                    height: parent.height
                    width: priv.signalWidthPercentage(networkInfo.networkSignalStrength) * parent.width
                    Image {
                        fillMode: Image.PreserveAspectCrop
                        smooth: true
                        clip: true
                        anchors.fill: parent
                        source: style.current.get("signalFull")
                     }
                 }
             }

            Image {
                id: batteryBackground
                width: style.current.get("batteryLevelWidth")
                height: style.current.get("batteryLevelHeight")
                anchors.verticalCenter: signalRow.verticalCenter
                source: style.current.get("batteryBackground")
                fillMode: Image.PreserveAspectCrop
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
                        source: (batteryInfo.batteryStatus >= BatteryInfo.BatteryVeryLow)
                                 ? style.current.get("batteryFull") : style.current.get("batteryLow")
                    }
                }
                states: [
                    State {
                        name:"Charging"
                        PropertyChanges { target: batteryLevelImage; source: style.current.get("batteryFull") }
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
                width: style.current.get("timeWidth")
                height: style.current.get("timeHeight")
                color: "white"
                anchors.verticalCenter: signalRow.verticalCenter
                text: symbian.currentTime
            }
        }
    }
}

