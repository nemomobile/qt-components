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
import Qt.labs.components.native 1.0
import "UIConstants.js" as UI

FlickrPage {
    id: largeImage

    property string photoUrl
    property int photoHeight
    property int photoWidth
    property real chromeOpacity: 1.0

    function setPhotoData(url, photoWidth, photoHeight) {
        largeImage.photoUrl = url;
        largeImage.photoHeight = photoHeight;
        largeImage.photoWidth = photoWidth;
    }

    function showChrome(show) {
        if (show)
            chromeOpacity = 1.0;
        else
            chromeOpacity = 0;
    }

    onVisibleChanged: {
        if (!visible) {
            if (timer.running)
                timer.stop();
            showChrome(true);
        } else {
            showChrome(false);
        }
    }

    Behavior on chromeOpacity {
        NumberAnimation {
            easing.type: Easing.InOutQuad
            duration: 500
        }
    }

    Timer {
        id: timer

        interval: 2000
        running: false
        repeat: false

        onTriggered: if (visible) showChrome(false);
    }

    //![0]
    ProgressBar {
        anchors.centerIn: parent
        minimumValue: 1
        maximumValue: 100
        value: image.progress * 100
        visible: image.status != Image.Ready

        Text {
            text: Math.floor(parent.value) + qsTr(" %");
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.top
            anchors.bottomMargin: UI.PROGRESSBAR_LABEL_BOTTOM_MARGIN
            font.bold: true
            color: UI.PROGRESSBAR_LABEL_COLOR
        }
    }
    //![0]

    Flickable {
        id: flickable

        anchors.fill: parent
        clip: true
        contentWidth: imageContainer.width
        contentHeight: imageContainer.height

        Item {
            id: imageContainer

            width: Math.max(image.width * image.scale, flickable.width)
            height: Math.max(image.height * image.scale, flickable.height)

            Image {
                id: image

                property real prevScale

                anchors.centerIn: parent
                smooth: !flickable.movingVertically
                source: largeImage.photoUrl
                sourceSize.width: (largeImage.photoWidth > 1024
                                   && largeImage.photoWidth > largeImage.photoHeight) ? 1024 : 0
                sourceSize.height: (largeImage.photoHeight > 1024
                                    && largeImage.photoHeight > largeImage.photoWidth) ? 1024 : 0

                onStatusChanged: {
                    if (status == Image.Loading) {
                        // Hide and reset slider: move slider handle to the left
                        slider.visible = false;
                        slider.value = 0;
                    } else if (status == Image.Ready && width != 0) {
                        // Default scale shows the entire image.
                        scale = Math.min(flickable.width / width, flickable.height / height);
                        prevScale = Math.min(scale, 1);

                        // Prepare and show the Slider if the image can be scaled
                        if (scale < 1) {
                            slider.minimumValue = scale;
                            slider.value = slider.minimumValue;
                            slider.visible = true;
                        }
                    }
                }

                onScaleChanged: {
                    if ((width * scale) > flickable.width) {
                        var xoff = (flickable.width / 2 + flickable.contentX) * scale / prevScale;
                        flickable.contentX = xoff - flickable.width / 2;
                    }
                    if ((height * scale) > flickable.height) {
                        var yoff = (flickable.height / 2 + flickable.contentY) * scale / prevScale;
                        flickable.contentY = yoff - flickable.height / 2;
                    }
                    prevScale = scale;
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    showChrome(true);
                    timer.start();
                }
            }
        }
    }

    //![2]
    ScrollDecorator {
        flickableItem: flickable
    }
    //![2]

    Text {
        text: qsTr("Image Unavailable")
        visible: image.status == Image.Error
        anchors.centerIn: parent
        color: UI.LARGEIMAGE_ERROR_MSG_COLOR
        font.bold: true
    }

    //![1]
    Slider {
        id: slider

        maximumValue: 1
        stepSize: (maximumValue - minimumValue) / 100
        opacity: UI.SLIDER_OPACITY
        anchors {
            bottom: parent.bottom
            bottomMargin: UI.SLIDER_BOTTOM_MARGIN
            left: parent.left
            leftMargin: UI.SLIDER_SIDE_MARGIN
            right: parent.right
            rightMargin: UI.SLIDER_SIDE_MARGIN
        }
    }

    Binding { target: image; property: "scale"; value: slider.value; when: slider.visible }
    //![1]
}
