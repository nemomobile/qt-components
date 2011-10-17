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
// Note: This example imports the “Qt.labs.components.native 1.0" module that allows the same
// Qt Quick Components example code to run as is in both MeeGo 1.2 Harmattan and Symbian platforms
// during the application development. However, real published applications should not import this
// module but one of the following platform-specific modules instead:
// import com.nokia.symbian 1.1    // Symbian components
// import com.nokia.meego 1.1      // MeeGo components
import Qt.labs.components.native 1.0

//! [page_a]
Page {
    id: lists
//! [page_a]

    Column {
        id: col1
        spacing: 20
        anchors.margins: 15
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        Text {
            width: parent.width
            text: "The first list demonstrates the effect of not rounding the height of each delegate item, and also the effect of not rounding the width of the ListView."
            wrapMode: Text.WordWrap;
            color: "white"
            font.pixelSize: 15
        }
        Text {
            width: parent.width
            text: "The second list illustrates the benefits of rounding. Notice that when you resize the container (with the slider), the right hand edge is always the same width."
            wrapMode: Text.WordWrap;
            color: "white"
            font.pixelSize: 15
        }
    }
//! [row_a]
    Row {
        id: row2
        property real itemWidth: (row2.width - 50) / 2
        width: slider.value
//! [row_a]
        spacing: 20
        anchors.margins: 15
        anchors {
            top: col1.bottom
            bottom: slider.top
            left: parent.left
        }

//! [row_b]
        ListView {
            property bool rounding: false
            clip: true
            maximumFlickVelocity: 100
            flickDeceleration: 0.01
            width: row2.itemWidth
            height: parent.height
            delegate: listItem
            model: 20
        }
        ListView {
            property bool rounding: true
            clip: true
            maximumFlickVelocity: 100
            flickDeceleration: 0.01
            width: Math.floor(row2.itemWidth)
            height: parent.height
            delegate: listItem
            model: 20
        }
    }
//! [row_b]

    Slider {
        id: slider
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        maximumValue : col1.width
        minimumValue : 30
        value: maximumValue
    }


    //! [listitem_a]
    Component {
        id: listItem

        Rectangle {
            color: "black"
            border.color: "white"
            border.width: 2

            width: parent.width
            height: ListView.view.rounding ? 32 : 32.5
    //! [listitem_a]

            Text {
                id: text1
                anchors.centerIn: parent
                color: "white"
            }
            Component.onCompleted: text1.text = ListView.view.rounding ? "rounded" : "!rounded"
    //! [listitem_b]
        }
    }
    //! [listitem_b]
//! [page_b]
}
//! [page_b]
