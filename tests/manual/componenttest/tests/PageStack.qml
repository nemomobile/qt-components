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

import Qt 4.7
import com.nokia.symbian 1.0

Item {
    id: root

    property variant pages: [pageStatic2, pageStatic3, pageStatic4]
    property string currentPageName: pageStack.currentPage == undefined? "0" : pageStack.currentPage.objectName

    function inPortrait() {
        return screen.height > screen.width
    }

    PageStack {
        id: pageStack
    }

    Page {
        id: pageStatic1
        objectName: "page1"
    }

    Component {
        id: pageStatic2
        Page { objectName: "page2" }
    }

    Component {
        id: pageStatic3
        Page { objectName: "page3" }
    }

    Page {
        id: pageStatic4
        objectName: "page4"
    }

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Row {
            spacing: parent.spacing / 2
            Text { color: "white"; font.pointSize: 10; text: "depth: [ " + pageStack.depth + " ]" }
            Text { color: "white"; font.pointSize: 10; text: "|  page: [ " + currentPageName + " ]" }
        }

        Rectangle { color: "blue"; height: 40; width: parent.width; opacity: pageStack.busy ? 1 : 0.1 }

        Grid {
            height: inPortrait() ? parent.height / 5 : parent.height / 6
            width: parent.width
            columns: inPortrait() ? 1 : 2
            spacing: parent.spacing / 3

            Row {
                spacing: parent.spacing
                width: inPortrait() ? parent.width : parent.width / 2
                Button { text: "Page 1"; width: parent.width / 2; onClicked: pageStack.push(pageStatic1); }
                Button { text: "Page 2"; width: parent.width / 2; onClicked: pageStack.push(pageStatic2); }
            }

            Row {
                spacing: parent.spacing
                width: inPortrait() ? parent.width : parent.width / 2
                Button { text: "Page 3"; width: parent.width / 2; onClicked: pageStack.push(pageStatic3); }
                Button { text: "Page 4"; width: parent.width / 2; onClicked: pageStack.push(pageStatic4); }
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
                Button { text: "Push 2,3,4"; width: parent.width / 2; onClicked: pageStack.push(pages); }
                Button { text: "Replace 2,3,4"; width: parent.width / 2; onClicked: pageStack.replace(pages); }
            }

            Row {
                width: inPortrait() ? parent.width : parent.width / 2
                spacing: parent.spacing
                Button { text: "Rewind to 3"; width: parent.width / 2; onClicked: pageStack.pop(pageStack.find(function(page) { return page.objectName == "page3" })); }
                Button { text: "Find non-existent"; width: parent.width / 2; onClicked: pageStack.pop(pageStack.find(function(page) { return page.objectName == "unknown" })); }
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
                Button { text: "Replace 2"; width: parent.width / 2; onClicked: pageStack.replace(pageStatic2); }
                Button { text: "Replace 3"; width: parent.width / 2; onClicked: pageStack.replace(pageStatic3); }
            }

            Row {
                width: inPortrait() ? parent.width : parent.width / 2
                spacing:  parent.spacing
                Button { text: "Back"; width: parent.width / 2; onClicked: pageStack.pop(); }
                Button { text: "Clear stack"; width: parent.width / 2; onClicked: pageStack.clear(); }
            }
        }
    }
}
