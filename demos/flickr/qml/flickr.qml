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
import Qt.labs.components.native 1.0

//![0]
Window {
    id: window

    PageStack {
        id: pageStack

        anchors.fill: parent
        toolBar: toolBar
        onDepthChanged: searchBar.close();
    }

    StatusBar {
        id: statusBar

        anchors { top: parent.top; left: parent.left; right: parent.right }
        opacity: largeImagePage.chromeOpacity
    }

    SearchBar {
        id: searchBar

        anchors.top: statusBar.bottom
        width: statusBar.width
        onSearchTagChanged: photoFeedModel.tags = searchTag
    }

    ToolBar {
        id: toolBar

        anchors { bottom: parent.bottom; left: parent.left; right: parent.right }
        opacity: largeImagePage.chromeOpacity
    }

    ThumbnailPage {
        id: thumbnailPage

        anchors { fill: parent; topMargin: statusBar.height; bottomMargin: toolBar.height }
        inPortrait: window.inPortrait
        model: PhotoFeedModel {
            id: photoFeedModel
        }
        tools: ToolBarLayout {
            ToolButton {
                iconSource: "images/tb_back.svg"
                onClicked: Qt.quit();
            }
            ToolButton {
                iconSource: "images/tb_reload.svg"
                onClicked: {
                    photoFeedModel.reload();
                    searchBar.close();
                }
            }
            ToolButton {
                iconSource: "images/tb_search.svg"
                onClicked: searchBar.toggle();
            }
        }
        onPhotoClicked: {
            largeImagePage.setPhotoData(url, photoWidth, photoHeight);
            detailsPage.setPhotoData(author, date, description, tags, title,
                                     photoWidth, photoHeight);
            pageStack.push(largeImagePage);
        }
    }

    LargeImagePage {
        id: largeImagePage

        tools: ToolBarLayout {
            ToolButton {
                iconSource: "images/tb_back.svg"
                onClicked: pageStack.pop();
            }
            ToolButton {
                iconSource: "images/tb_info.svg"
                checked: false
                onClicked: pageStack.replace(detailsPage);
            }
        }
    }

    DetailsPage {
        id: detailsPage

        anchors { fill: parent; topMargin: statusBar.height; bottomMargin: toolBar.height }

        tools: ToolBarLayout {
            ToolButton {
                iconSource: "images/tb_back.svg"
                onClicked: pageStack.pop();
            }
            ToolButton {
                iconSource: "images/tb_info.svg"
                checked: true
                onClicked: pageStack.replace(largeImagePage);
            }
        }
    }

    Splash {
        id: splash

        image : "images/splash.png"
        timeout: 1000
        fadeout: 700
        Component.onCompleted: splash.activate();
        onFinished: splash.destroy();
    }

    Component.onCompleted: pageStack.push(thumbnailPage);
}
//![0]
