/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

import QtQuick 1.0
import Qt.labs.components.native 1.0

//![0]
Window {
    id: window

    PageStack {
        id: pageStack

        anchors.fill: parent
        toolBar: toolBar
        onDepthChanged: searchBar.hideSearch();
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
        onSearchTagChanged: thumbnailPage.tags = searchTag
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
        onBackClicked: Qt.quit();
        onSearchClicked: searchBar.searching ? searchBar.hideSearch() : searchBar.search();
        onReloadClicked: if (searchBar.searching) searchBar.hideSearch();
        onPhotoClicked: {
            largeImagePage.setPhotoData(url, photoWidth, photoHeight);
            detailsPage.setPhotoData(author, date, description, tags, title,
                                     photoWidth, photoHeight);
            pageStack.push(largeImagePage);
        }
    }

    LargeImagePage {
        id: largeImagePage

        onBackClicked: pageStack.pop();
        onInfoClicked: pageStack.replace(detailsPage);
    }

    DetailsPage {
        id: detailsPage

        anchors { fill: parent; topMargin: statusBar.height; bottomMargin: toolBar.height }
        onBackClicked: pageStack.pop();
        onInfoClicked: pageStack.replace(largeImagePage);
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
