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

//! [0]
import com.nokia.symbian 1.1

Window {

    //! [pagestack]
    PageStack {
        id: pageStack
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: sharedToolBar.top
        }

        // the PageStack has a single shared ToolBar
        toolBar: sharedToolBar
    }
    //! [pagestack]

    //! [page1a]
    Page {
        id: page1
        tools: toolBarLayout1
        anchors {
            fill: parent;
            margins: platformStyle.paddingMedium
        }
        //! [page1a]
        Text {
            anchors.fill: parent
            text: "Page 1's ToolBarLayout contains a Single Textual ToolButton"
            wrapMode: Text.WordWrap;
            color: platformStyle.colorNormalLight
            font.family: platformStyle.fontFamilyRegular
            font.pixelSize: platformStyle.fontSizeMedium
        }
        //! [page1b]
    }
    //! [page1b]

    Page {
        id: page2
        tools: toolBarLayout2
        anchors {
            fill: parent;
            margins: platformStyle.paddingMedium
        }
        Text {
            anchors.fill: parent
            text: "Page 2's ToolBarLayout contains two Textual ToolButtons"
            wrapMode: Text.WordWrap;
            color: platformStyle.colorNormalLight
            font.family: platformStyle.fontFamilyRegular
            font.pixelSize: platformStyle.fontSizeMedium
        }
    }

    Page {
        id: page3
        tools: toolBarLayout3
        anchors {
            fill: parent;
            margins: platformStyle.paddingMedium
        }
        Text {
            anchors.fill: parent
            text: "Page 3's ToolBarLayout contains a ButtonRow with three Textual ToolButtons"
            wrapMode: Text.WordWrap;
            color: platformStyle.colorNormalLight
            font.family: platformStyle.fontFamilyRegular
            font.pixelSize: platformStyle.fontSizeMedium
        }
    }

    // this shared ToolBar hosts the current ToolBarLayout instance
    //! [sharedtoolbar]
    ToolBar {
        id: sharedToolBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
    //! [sharedtoolbar]

    //! [toolbarlayout1a]
    ToolBarLayout {
        id: toolBarLayout1
        //! [toolbarlayout1a]
        ToolButton {
            flat: true
            text: "One"
        }
        ToolButton {
            iconSource: "toolbar-next"
            onClicked: pageStack.push(page2)
        }
        //! [toolbarlayout1b]
    }
    //! [toolbarlayout1b]

    //! [toolbarlayout2]
    ToolBarLayout {
        id: toolBarLayout2

        ToolButton {
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
        ToolButton {
            text: "One"
        }
        ToolButton {
            text: "Two"
        }
        ToolButton {
            iconSource: "toolbar-next"
            onClicked: pageStack.push(page3)
        }
    }
    //! [toolbarlayout2]

    //! [toolbarlayout3]
    ToolBarLayout {
        id: toolBarLayout3

        ToolButton {
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
        ButtonRow {
            ToolButton {
                text: "One"
            }
            ToolButton {
                text: "Two"
            }
            ToolButton {
                text: "Three"
            }
        }
    }
    //! [toolbarlayout3]

    // ensure that the first page is pushed to start with

    //! [oncompleted]
    Component.onCompleted: {
        pageStack.push(page1)
    }
    //! [oncompleted]
}
//! [0]
