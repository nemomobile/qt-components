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
    id: testItem
    property int dynPageCount: 0

    property variant pages: [pageStatic, pageStatic2, pageStatic3]

    property alias depth: pageStack.depth

    property bool busy: pageStack.busy

    property bool busyChangedTriggered: false

    property string currentPageName: pageStack.currentPage==undefined? "0" : pageStack.currentPage.objectName

    PageStack {
        id: pageStack
        onBusyChanged:
            testItem.busyChangedTriggered = true;
    }

    Component {
        id: pageComponent
        Page {
            id: page
            objectName: "dynamicpage"
        }
    }

    Page {
        id: pageStatic
        objectName: "page1"
        property int testparam1: 0
    }

    Page {
        id: pageStatic2
        objectName: "page2"
    }

    Page {
        id: pageStatic3
        objectName: "page3"
    }


    function pushPage() {
        pageStack.push(pageComponent);
    }

    function replacePage() {
        pageStack.replace(pageStatic3);
    }

    function pushPageStatic() {
        pageStack.push(pageStatic);
    }

    function popPage() {
        return pageStack.pop();
    }

    function clearStack() {
        return pageStack.clear();
    }

    function pushPageArray() {
        return pageStack.push(pages);
    }

    function pushPagesForFind() {
        pageStack.push(pageStatic);
        pageStack.push(pageStatic2);
    }

    function unwind() {
        pageStack.pop(pageStatic);
    }

    function unwindToBottom() {
        pageStack.pop(null);
    }

    function pushPageWithProperties() {
        pageStack.push(pageStatic, { testparam1: 2 });
    }
    function checkForPageProperties() {
        if (pageStack.currentPage.testparam1 == 2) return true;
        return false;
    }

    function findPage() {
        if (pageStack.find(findFunc) != null) return true;
        return false;
    }

    function findFunc(page) {
        if (page == pageStatic2) return true;
        return false;
    }
}
