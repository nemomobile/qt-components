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
import com.nokia.meego 1.0

Item {
    id: container
    property int dynPageCount: 0

    property variant pages: [pageStatic, pageStatic2, pageStatic3]

    property alias depth: pageStack.depth

    property bool busyChangedTriggered: false

    property string currentPageName: pageStack.currentPage==undefined? "0" : pageStack.currentPage.objectName

    PageStack {
        id: pageStack
        onBusyChanged: {
            container.busyChangedTriggered = true;
        }
    }

    Component {
        id: pageComponent
        Page {
            id: page
            objectName: "dynamicpage"
            property int testparam1: 0
        }
    }

    Page {
        id: pageStatic
        objectName: "page1"
        property int testparam2: 0
    }

    Page {
        id: pageStatic2
        objectName: "page2"
    }

    Page {
        id: pageStatic3
        objectName: "page3"
    }


    function pushPage()
    {
        pageStack.push(pageComponent);
    }

    function replacePage()
    {
        pageStack.replace(pageStatic3);
    }

    function pushPageStatic()
    {
        pageStack.push(pageStatic);
    }

    function popPage()
    {
        return pageStack.pop(undefined, true);
    }

    function clearStack()
    {
        return pageStack.clear();
    }

    function pushPageArray()
    {
        return pageStack.push(pages, null, true);
    }

    function pushPagesForFind()
    {
        pageStack.push(pageStatic);
        pageStack.push(pageStatic2);
    }

    function unwind()
    {
        pageStack.pop(pageStatic);
    }

    function unwindToBottom()
    {
        pageStack.pop(null);
    }

    function pushPageWithProperties()
    {
        pageStack.push(pageComponent, { testparam1: 1 });
    }

    function checkForPageProperties()
    {
        if (pageStack.currentPage.testparam1==1) return true;
        return false;
    }

    function pushPageStaticWithProperties()
    {
        pageStack.push(pageStatic, { testparam2: 2 });
    }

    function checkForPageStaticProperties()
    {
        if (pageStack.currentPage.testparam2==2) return true;
        return false;
    }

    function findPage()
    {
        if (pageStack.find(findFunc)!=null) return true;
        return false;
    }

    function findFunc(page)
    {
        if (page==pageStatic2) return true;
        return false;
    }
}
