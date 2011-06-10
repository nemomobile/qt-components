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
import Qt.labs.components.native 1.0

Item {
    id: listItem
    QtObject {
        id: internal
        property int halfSpacingSize: Math.round(appStyle.spacingSize / 4.0)
    }

    Rectangle {
        id: gradient
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#3574A6" }
            GradientStop { position: 0.5; color: "black" }
            GradientStop { position: 1.0; color: "#3574A6" }
        }
        opacity: mouseArea.pressed ? 1.0 : 0.0
    }
    height: text1.height + text2.height + appStyle.marginSize + (internal.halfSpacingSize * 2)
    width: parent.width
    Text {
        id: text1
        color: "white"
        font: appStyle.largeFont
        text: Name
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.verticalCenter
            leftMargin: appStyle.marginSize
            rightMargin: appStyle.marginSize
            bottomMargin: internal.halfSpacingSize
        }
    }
    Text {
        id: text2
        color: "white"
        font: appStyle.mediumFont
        text: Description
        anchors {
            left: parent.left
            right: parent.right
            top: parent.verticalCenter
            leftMargin: appStyle.marginSize
            rightMargin: appStyle.marginSize
            topMargin: internal.halfSpacingSize
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if (!window.pageStack.busy) pageStack.pop(stationView)
        }
    }
}
