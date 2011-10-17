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
import com.nokia.symbian 1.1

Window {
    // setup the window
    id: mainWindow
    objectName: "mainWindow"
    height: 350
    width: 350

    // page stack for the pages
    PageStack {
        id: pageStack
    }

    // title text
    Text {
        id: title
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "PageStack/Page snippets"
        font.pointSize: 20
        color: "white"
    }

    // last action information text
    Text {
        id: lastAction
        anchors.top: title.bottom
        text: setLastAction("")
        font.pointSize: 10
        color: "white"
    }

    // calculate the text for the last action
    function setLastAction(actionName) {
        lastAction.text = "Last action: " + actionName
    }

    // push initial pages when application is ready
    Component.onCompleted: {
        //! [5]
        pageStack.push(componentPage);
        //! [5]
        //! [4]
        pageStack.push(Qt.resolvedUrl("FilePage.qml"));
        //! [4]
        //! [3]
        pageStack.push(itemPage)
        //! [3]
        setLastAction("Initial pages pushed")
    }

    // define a toolbar for the options
    ToolBar {
        id: bar
        anchors.bottom: parent.bottom
        Row {
            // pop a page
            Button {
                text: "<-"
                enabled: pageStack.depth != 1
                onClicked: {
                    pageStack.pop()
                    setLastAction("<- 'Back', pop a page")
                }
            }
            // push a page
            Button {
                text: "+"
                onClicked: {
                //! [7]
                    pageStack.push( componentPage, {message: pageStack.depth+1});
                //! [7]
                    setLastAction("+ Push a page")
                }
            }
            // push multiple pages at once
            Button {
                text: "+++"
                onClicked: {
                //! [8]
                    pageStack.push([
                        {page: componentPage, properties: {message: pageStack.depth+1}},
                        {page: componentPage, properties: {message: pageStack.depth+2}},
                        {page: componentPage, properties: {message: pageStack.depth+3}}])
                //! [8]
                    setLastAction("+++ Push multiple pages")
                }
            }
            // replace the current page with another
            Button {
                text: "-+"
                onClicked: {
                    pageStack.replace( componentPage, {message: pageStack.depth+"r"})
                    setLastAction("-+ Replace current page with another")
                }
            }
            // replace the current page with several pages
            Button {
                text: "-+++"
                onClicked: {
                    pageStack.replace([
                        {page: componentPage, properties: {message: pageStack.depth+"r"}},
                        {page: componentPage, properties: {message: pageStack.depth+1}},
                        {page: componentPage, properties: {message: pageStack.depth+2}}])
                    setLastAction("-+++ Replace current page with multiple pages")
                }
            }
            // pop and rewind to the page with '2' as the text if it exists
            Button {
                text: "<-'2'"
                onClicked: {
                //! [6]
                    pageStack.pop(pageStack.find(function(page) {
                                                     return page.message == "2";
                                                 }));
                //! [6]
                    setLastAction("Back to '2' if it exists, otherwise first page")
                }
            }
            // clear all pages and push a new start page
            Button {
                text: "---"
                onClicked: {
                    pageStack.clear();
                    pageStack.push( componentPage, {message: pageStack.depth+1})
                    setLastAction("--- Clear all pages and start fresh")
                }
            }
        }
    }

    //! [1]
    // a page item
    Page {
        id: itemPage
        property alias message: pageText.text
        Text {
            id: pageText
            anchors.centerIn: parent
            text: "item page"
            font.pointSize: 25
            color: "white"
        }
    }
    //! [1]

    //! [0]
    // a page component
    Component {
        id: componentPage
        Page {
            id: myComponentPage
            property alias message: pageText.text
            Text {
                id: pageText
                anchors.centerIn: parent
                text: "component page"
                font.pointSize: 25
                color: "White"
            }
        }
    }
    //! [0]
}

