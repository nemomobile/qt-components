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
import com.meego 1.0
import Qt.labs.components 1.0

Item {
    id: root
    width: 500
    height: 800

    Text {
        id: text
        anchors.top: root.top
        anchors.left: root.left
        anchors.topMargin: 30
        anchors.leftMargin: 30
        color: Qt.rgba(red.value/255, 0, 0, 1)
        font.letterSpacing: letterSpacing.value
        text: "Current slider value: " + red.value;

        function set(value){
            text.text = "Current slider value: " + value;
        }
    }

    Grid {
        id: sliderGrid
        anchors.top: text.bottom
        anchors.left: text.left
        anchors.right: letterSpacing.left
        anchors.topMargin: 30
        anchors.rightMargin: 30

        columns: 2
        spacing: 100

        Text { text: "Normal"}
        Slider { id: red; maximumValue: 255; value: 150; onValueChanged: text.set(value); onPressed: text.set(value)}
        Text { text: "Steps"}
        Slider { maximumValue: 10; stepSize:5; onValueChanged: text.set(value); onPressed: text.set(value)}
        Text { text: "Progress" }
        Slider { id: stream; maximumValue: 300; onValueChanged: text.set(value); onPressed: text.set(value)           
            function formatTime(timeValue)
            {
                var sec = '' + value%60
                if (sec.length == 1)
                    sec = "0" + sec;
                return Math.floor(value/60) + ":" + sec
            }
            indicatorLabel: formatTime(value)
            Timer {
                interval: 100; running: true; repeat: true
                onTriggered: if (++stream.progress == stream.maximumValue) stream.progress = 0
            }
        }
    }

    Slider {
        id: letterSpacing
        anchors.top: sliderGrid.top
        anchors.bottom: sliderGrid.bottom
        anchors.right: parent.right
        anchors.rightMargin: 30

        minimumValue: -2
        maximumValue: 5
        orientation: Qt.Vertical
        stepSize: 0
        progress: minimumValue
        onValueChanged: text.set(value)
        onPressed: text.set(value)
    }

}
