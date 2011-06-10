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

import QtQuick 1.0

Item {
    Item {
        id: helper
        anchors { fill: parent; margins: appStyle.marginSize }
    }

    RadioConsole {
        id: infoArea
    }

    FrequencyStrip {
        id: frequencyArea
    }

    RadioControls {
        id: controlArea
    }

    //![stategroup_default_a]
    StateGroup {
        states: [
            State {
                when: (true)
                AnchorChanges { target: infoArea;  anchors.left: helper.left; anchors.top: helper.top }
                AnchorChanges { target: frequencyArea; anchors.left: helper.left; anchors.right: helper.right }
                AnchorChanges { target: controlArea; anchors.right: helper.right }
                //![stategroup_default_a]

                PropertyChanges { target: infoArea;  anchors.bottomMargin: appStyle.spacingSize }
                //![stategroup_default_b]
            }
        ]
    }
    //![stategroup_default_b]
    //![stategroup_a]
    StateGroup {
        states: [
            State {
                when: (container.width / container.height) > 1.20
                AnchorChanges { target: infoArea; anchors.right: undefined; anchors.bottom: frequencyArea.top }
                AnchorChanges { target: frequencyArea; anchors.top: undefined; anchors.bottom: parent.bottom }
                AnchorChanges { target: controlArea; anchors.left: infoArea.right; anchors.top: helper.top; anchors.bottom: frequencyArea.top }
                //![stategroup_a]

                //![stategroup_b]
                PropertyChanges { target: infoArea; width: Math.round(helper.width * appStyle.consoleRatio) }
                PropertyChanges { target: frequencyArea; anchors.bottomMargin: appStyle.marginSize }
                PropertyChanges { target: controlArea; anchors.leftMargin: appStyle.spacingSize; anchors.topMargin: 0; anchors.bottomMargin: appStyle.spacingSize }
                //![stategroup_b]
                //![stategroup_c]
            },
            State {
                when: (true)
                AnchorChanges { target: infoArea;  anchors.right: helper.right; anchors.bottom: undefined }
                AnchorChanges { target: frequencyArea; anchors.top: infoArea.bottom; anchors.bottom: undefined }
                AnchorChanges { target: controlArea; anchors.left: helper.left; anchors.top: frequencyArea.bottom; anchors.bottom: helper.bottom }
                //![stategroup_c]

                PropertyChanges { target: infoArea; height: Math.round(helper.height * appStyle.consoleRatio) }
                PropertyChanges { target: frequencyArea; anchors.topMargin: appStyle.spacingSize; anchors.bottomMargin: 0 }
                PropertyChanges { target: controlArea; anchors.leftMargin: 0; anchors.topMargin: appStyle.spacingSize; anchors.bottomMargin: 0 }
                //![stategroup_d]
            }
        ]
    }
    //![stategroup_d]
}
