/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
import Qt.labs.components 1.0

Rectangle {
    width: 500
    height: 130
    color: "white"

    Label {
        id: songLabel
        anchors.top: parent.top
        anchors.left: parent.left
        text: view.title
    }

    Label {
        id: artistLabel
        anchors.top: songLabel.bottom
        anchors.left: parent.left
        text: view.artist
    }

    Slider {
        id: volumeSlider
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: artistLabel.right
        anchors.leftMargin: 200
        anchors.right: parent.right

        value: view.volume
    }

    ProgressBar {
        id: progressbar
        anchors.top: artistLabel.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        value: view.currentTime / view.totalTime
    }

    Row {
        id: row
        spacing: 15
        anchors.top: progressbar.bottom
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: previous
            text: "previous"
            onClicked: view.previousClicked()
        }
        Button {
            id: playpause
            text: (view.state == 3 ? "pause" : "play")
            onClicked: {
                if (view.state == 3)
                    view.pauseClicked()
                else
                    view.playClicked()
            }
        }
        Button {
            id: stop
            text: "stop"
            onClicked: view.stopClicked()
        }
        Button {
            id: next
            text: "next"
            onClicked: view.nextClicked()
        }
    }
}
