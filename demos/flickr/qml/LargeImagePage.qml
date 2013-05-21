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
