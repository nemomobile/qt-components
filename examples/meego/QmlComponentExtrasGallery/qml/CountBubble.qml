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
import com.nokia.meego 1.1
import com.nokia.extras 1.1

Page {
    id: container
    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    Text {
        id: txt1
        text:"Small count bubbles"
        font.pixelSize: 22
    }

    Text {
        id: txt2
        anchors.top: column1.bottom; anchors.topMargin: 40
        text:"Large count bubbles"
        font.pixelSize: 22
    }

    Text {
        id: txt3
        anchors.left: txt1.right; anchors.leftMargin: 60
        text:"Use slider to change count bubble value"
        font.pixelSize: 22
    }

    Column {
        id: column1
        spacing: 5

        anchors.top: txt1.bottom

        CountBubble{
            objectName: "countBubbleObject"
            value: 9
        }

        CountBubble{
            id: cb3
            value: 99
        }

        CountBubble{
            value: 999
        }
    }

    Column {
        id: column2
        spacing: 5
        x: txt2.x

        anchors.top: txt2.bottom

        CountBubble{
            value: 9
            largeSized: true
        }

        CountBubble{
            id: cb4
            value: 99
            largeSized: true
        }

        CountBubble{
            value: 999
            largeSized: true
        }
    }

    Column {
        id: column3
        anchors.left: txt3.left
        y: column1.y
        spacing: 5
        CountBubble{
            id: cb1
            value: slider1.value
        }

        CountBubble{
            id: cb2
            value: cb1.value
            largeSized: true
        }
    }

    Slider {
        id: slider1
        width: 400
        x: column3.x + 45
        anchors {top: txt3.bottom; topMargin: 40}
        stepSize: 1
        maximumValue: 999
    }
}
