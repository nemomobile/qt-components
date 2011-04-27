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
* you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/
import Qt 4.7
import com.nokia.symbian 1.0
import "Constants.js" as CONSTANTS

ImplicitSizeItem {
    id: root

    property alias iconSource: image.source
    property alias text: text.text
    property alias timeout : timer.interval

    function open() {
        animationShow.running = true;
        if (root.timerEnabled) //for backward compability
            timer.restart();
    }
    function close() {
        animationHide.running = true;
    }

    // Deprecated ->
    property bool timerEnabled: true
    onTimerEnabledChanged: {
        console.log("InfoBanner.timerEnablerd is deprecated. Use timeout property instead. InfoBanner stays pop-up if timeout value is 0.");
    }
    property int timerShowTime
    onTimerShowTimeChanged: {
        console.log("InfoBanner.timerShowTime is deprecated. Use timeout property instead.");
        root.timeout = root.timerShowTime
    }
    property int topMargin
    onTopMarginChanged: console.log("InfoBanner.topMargin is deprecated. The margin is a fixed value")
    property int leftMargin
    onLeftMarginChanged: console.log("InfoBanner.leftMargin is deprecated. The margin is a fixed value")

    function show() {
        console.log("InfoBanner.show is deprecated. Use open instead");
        open()
    }
    function hide() {
        console.log("InfoBanner.hide is deprecated. Use close instead");
        close()
    }
    // <- Deprecated

    x: platformStyle.paddingMedium;
    y: platformStyle.paddingMedium;
    implicitHeight: Math.max(image.height, text.paintedHeight) + platformStyle.paddingLarge * 2
    implicitWidth: parent ? parent.width - platformStyle.paddingMedium * 2 : screen.width - platformStyle.paddingMedium * 2
    scale: 0

    BorderImage {
        id: background

        source: privateStyle.imagePath("qtg_fr_popup_infobanner")
        anchors.fill: parent
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        border { left: CONSTANTS.INFOBANNER_BORDER_MARGIN; top: CONSTANTS.INFOBANNER_BORDER_MARGIN;
                 right: CONSTANTS.INFOBANNER_BORDER_MARGIN; bottom: CONSTANTS.INFOBANNER_BORDER_MARGIN }
        opacity: CONSTANTS.INFOBANNER_OPACITY

        Row {
            anchors { fill: parent; margins: platformStyle.paddingLarge; }
            spacing: platformStyle.paddingMedium
            Image {
                id: image

                sourceSize { width: platformStyle.graphicSizeSmall; height: platformStyle.graphicSizeSmall }
                visible: source != ""
            }
            Text {
                id: text

                width: image.visible ? (root.width - image.width - platformStyle.paddingLarge * 2 - platformStyle.paddingMedium)
                                     : (root.width - platformStyle.paddingLarge * 2);
                verticalAlignment: Text.AlignVCenter
                color: platformStyle.colorNormalLight
                wrapMode: Text.Wrap
                font {
                    pixelSize: platformStyle.fontSizeMedium
                    family: platformStyle.fontFamilyRegular
                    letterSpacing: CONSTANTS.INFOBANNER_LETTER_SPACING
                }
            }
        }
    }

    QtObject {
        id: internal

        function getScaleValue() {
            return platformStyle.paddingMedium * 2 / root.width + 1;
        }
    }

    Timer {
        id: timer
        interval: CONSTANTS.INFOBANNER_DISMISS_TIMER
        onTriggered: close()
    }

    MouseArea {
        anchors.fill: parent
        onClicked: close()
    }

    SequentialAnimation {
        id: animationShow
        NumberAnimation { target: root; property: "scale"; from: 0; to: internal.getScaleValue();
                          duration: CONSTANTS.INFOBANNER_ANIMATION_DURATION; easing.type: Easing.OutQuad }
        NumberAnimation { target: root; property: "scale"; from: internal.getScaleValue(); to: 1;
                          duration: CONSTANTS.INFOBANNER_ANIMATION_DURATION }
    }

    NumberAnimation {
        id: animationHide
        target: root; property: "scale"; to: 0; duration: CONSTANTS.INFOBANNER_ANIMATION_DURATION; easing.type: Easing.InExpo
    }
}
