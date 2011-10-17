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
    id: gaps
//! [page_a]

    Column {
        id: col1
        spacing: 20
        width: parent.width
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 15
        }
        Text {
            width: parent.width
            text: "The first row demonstrates the effect of not rounding the spacing values between each item"
            wrapMode: Text.WordWrap;
            color: "white"
            font.pixelSize: 15
        }
        Text {
            width: parent.width
            text: "The second row illustrates how to evenly distribute a number of items so that the spacing between them is the exact same number of pixels, and any rounding errors are shared into the margins."
            wrapMode: Text.WordWrap;
            color: "white"
            font.pixelSize: 15
        }
        Text {
            width: parent.width
            text: "Drag the slider to resize the container. Look at the gap sizes as they approach zero. The second row always looks consistent, although there is a variable margin at the edge."
            wrapMode: Text.WordWrap;
            color: "white"
            font.pixelSize: 15
        }
    }
//! [column_a]
    Column {
        id: col2
        width: slider.value
//! [column_a]
        anchors {
            left: parent.left
            top: col1.bottom
            bottom: slider.top
            margins: 20
        }
        spacing: 20

//! [loaders]
        Loader {
            width: parent.width
            sourceComponent: rounder
            Component.onCompleted: item.evenlyDistributed = false
        }
        Loader {
            width: parent.width
            sourceComponent: rounder
            Component.onCompleted: item.evenlyDistributed = true
        }
//! [loaders]
//! [column_b]
    }
//! [column_b]

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

    //! [rounder_a]
    Component {
        id: rounder

        Item {
            id: container
            property bool evenlyDistributed: true
    //! [rounder_a]

            height: 30

            //! [layoutChildren]
            function layoutChildren() {
                if (parent == null || parent.width == 0 || parent.height == 0 || children.length == 0)
                    return;

                var remainingSpace = width
                var spacingNotRounded = remainingSpace

                // in this example, we have to subtract one for the Repeater, which inserts the remaining
                // children before its position
                var childrenRemaining = children.length - 1
                for (var p = 0; p < childrenRemaining; p++) {
                    spacingNotRounded -= children[p].width
                }
                spacingNotRounded /= (childrenRemaining + 1)
                var spacing = evenlyDistributed ? Math.floor(spacingNotRounded) : spacingNotRounded
                var totalRoundingError = (spacingNotRounded - spacing) * (childrenRemaining + 1)
                var curPos = Math.floor(totalRoundingError / 2.0)

                for (var q = 0; q < childrenRemaining; q++) {
                    var nextChild = children[q]
                    curPos += spacing
                    nextChild.x = curPos
                    curPos += nextChild.width
                }
            }
            //! [layoutChildren]

    //! [repeater]
            Repeater {
                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    radius: height / 9
                    width:  10 * (2 + index)
                    height: parent.height
                    color: "white"
                }
                model: 6
            }
    //! [repeater]

    //! [rounder_b]
            Component.onCompleted: container.layoutChildren()
            onParentChanged: container.layoutChildren()
            onChildrenChanged: container.layoutChildren()
            onWidthChanged: container.layoutChildren()
            onHeightChanged: container.layoutChildren()
        }
    //! [rounder_b]
    }

//! [page_b]
}
//! [page_b]
