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
    id: testItem

    width: 200
    height: 400

    // This tab group is used to test different button properties
    TabGroup {
        id: testGroup
        x: 10
        y: 130
        objectName: "testGroup"
    }

    TabGroup {
        id: tabGroup1
        objectName: "tabGroup1"
        Item { id: group1tab1; objectName: "group1tab1" }
        Item { id: group1tab2; objectName: "group1tab2"
        }
    }

    TabGroup {
        id: tabGroup2
        objectName: "tabGroup2"

        Component.onCompleted: {
            var tab1 = tabPageComponent.createObject(tabGroup2)
            tab1.objectName = "group2tab1"

            var tab2 = tabPageComponent.createObject(tabGroup2)
            tab2.objectName = "group2tab2"

            currentTab = tab2
        }
    }

    TabGroup {
        id: tabGroup3
        objectName: "tabGroup3"
        currentTab: group3tab2
        Item { id: group3tab1; objectName: "group3tab1" }
        Item { id: group3tab2; objectName: "group3tab2" }
    }

    TabGroup {
        id: tabGroup4
        objectName: "tabGroup4"
        property int orientation: screen.currentOrientation

        Component.onCompleted: {
            var tab1 = tabPageComponent.createObject(tabGroup4)
            tab1.objectName = "group4tab1"
            tab1.orientationLock = PageOrientation.LockPortrait

            var tab2 = tabPageComponent.createObject(tabGroup4)
            tab2.objectName = "group4tab2"
            tab2.orientationLock = PageOrientation.LockLandscape

            currentTab = tab1
        }
    }

    Component {
        id: tabPageComponent
        Page {
        }
    }
}











