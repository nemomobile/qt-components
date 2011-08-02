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
import "../TestUtils.js" as Utils
import "../components"

Item {
    id: root

    property variant pages: [page2, page3, page4]
    property string currentPageName: pageStack.currentPage == undefined? "0" : pageStack.currentPage.objectName
    property Item rootPageStack: pageStack
    property bool platformInverted: false

    function inPortrait() {
        return screen.height > screen.width
    }

    PageStack {
        id: pageStack
    }

    Page {
        id: staticPage1

        objectName: "staticPage1"
        Loader { sourceComponent: pageContent }
    }

    Component {
        id: page2

        Page {
            objectName: "page2"
            Loader { sourceComponent: pageContent }
        }
    }

    Component {
        id: page3

        Page {
            objectName: "page3"
            Loader { sourceComponent: pageContent }
        }
    }

    Component {
        id: page4

        Page {
            objectName: "page4"
            Loader { sourceComponent: pageContent }
        }
    }

    Loader {
        sourceComponent: pageContent
        visible: rootPageStack.depth == 0 ? true : false
    }

    MouseArea {
        anchors.fill: parent
        enabled: rootPageStack.busy
    }

    Component {
        id: pageContent

        Column {
            id: contentColumn
            x: root.x
            y: root.y
            width: root.width
            height: root.height
            spacing: 10

            // manually set inversion mode for dynamically created item tree
            Component.onCompleted: Utils.setItemTreeInversion(contentColumn, root.platformInverted)
            Row {
                spacing: parent.spacing / 2
                Label { text: "depth: [ " + rootPageStack.depth + " ]" }
                Label { text: "|  page: [ " + currentPageName + " ]" }
            }

            Rectangle { color: "blue"; height: 40; width: parent.width; opacity: rootPageStack.busy ? 1 : 0.1 }

            Grid {
                height: inPortrait() ? parent.height / 5 : parent.height / 6
                width: parent.width
                columns: inPortrait() ? 1 : 2
                spacing: parent.spacing / 3

                Row {
                    spacing: parent.spacing
                    width: inPortrait() ? parent.width : parent.width / 2
                    Button { text: "staticPage 1"; width: parent.width / 2; onClicked: rootPageStack.push(staticPage1); }
                    Button { text: "Page 2"; width: parent.width / 2; onClicked: rootPageStack.push(page2); }
                }

                Row {
                    spacing: parent.spacing
                    width: inPortrait() ? parent.width : parent.width / 2
                    Button { text: "Page 3"; width: parent.width / 2; onClicked: rootPageStack.push(page3); }
                    Button { text: "Page 4"; width: parent.width / 2; onClicked: rootPageStack.push(page4); }
                }
            }

            Grid {
                height: inPortrait() ? parent.height / 5 : parent.height / 6
                width: parent.width
                columns: inPortrait() ? 1 : 2
                spacing: parent.spacing / 3

                Row {
                    width: inPortrait() ? parent.width : parent.width / 2
                    spacing: parent.spacing
                    Button { text: "Push 2,3,4"; width: parent.width / 2; onClicked: rootPageStack.push(pages); }
                    Button { text: "Replace 2,3,4"; width: parent.width / 2; onClicked: rootPageStack.replace(pages); }
                }

                Row {
                    width: inPortrait() ? parent.width : parent.width / 2
                    spacing: parent.spacing

                    Button {
                        text: "Rewind to 3";
                        width: parent.width / 2;
                        onClicked: {
                            if (currentPageName == "page3")
                                rootPageStack.pop(undefined,true);
                            rootPageStack.pop(rootPageStack.find(function(page) { return page.objectName == "page3" }));
                        }
                    }

                    Button {
                        text: "Find non-existent";
                        width: parent.width / 2;
                        onClicked: {
                            rootPageStack.pop(rootPageStack.find(function(page) { return page.objectName == "unknown" }));
                        }
                    }
                }
            }

            Grid {
                height: inPortrait() ? parent.height / 5 : parent.height / 6
                width: parent.width
                columns: inPortrait() ? 1 : 2
                spacing: parent.spacing / 3

                Row {
                    width: inPortrait() ? parent.width : parent.width / 2
                    spacing:  parent.spacing
                    Button { text: "Replace 3"; width: parent.width / 2; onClicked: rootPageStack.replace(page3); }
                    Button { text: "Clear stack"; width: parent.width / 2; onClicked: rootPageStack.clear(); }
                }

                Button {
                    text: "Back";
                    width: inPortrait() ? parent.width / 2 : parent.width / 4;
                    onClicked: rootPageStack.pop();
                }
            }

            Grid {
                height: inPortrait() ? parent.height / 5 : parent.height / 6
                width: parent.width
                columns: inPortrait() ? 1 : 2
                spacing: parent.spacing / 3

                Row {
                    width: inPortrait() ? parent.width : parent.width / 2
                    spacing:  parent.spacing
                    Button { text: "init: static"; width: parent.width / 2; onClicked: rootPageStack.initialPage = staticPage1; }
                    Button { text: "init: component"; width: parent.width / 2; onClicked: rootPageStack.initialPage = page2; }
                }

                Button {
                    text: "clean init";
                    width: inPortrait() ? parent.width / 2 : parent.width / 4;
                    onClicked: rootPageStack.initialPage = null;
                }
            }

        }
    }
}
