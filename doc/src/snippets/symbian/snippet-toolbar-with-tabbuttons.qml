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

    QtObject {
        id: internal
        property int busyIndicatorSize : Math.min(tabGroup.width, tabGroup.height) / 4.0
    }

    Text {
        id: label1
        text: "When a TabButton is placed in a ButtonRow, which is inside a ToolBar, it automatically looks like a ToolButton, but the TabGroup operations function as normal."
        anchors {
            left: parent.left;
            right: parent.right
            top: parent.top
            margins: platformStyle.paddingMedium
        }
        wrapMode: Text.WordWrap;
        color: platformStyle.colorNormalLight
        font.family: platformStyle.fontFamilyRegular
        font.pixelSize: platformStyle.fontSizeMedium
    }

    Text {
        id: label2
        anchors {
            left: parent.left;
            right: parent.right
            top: label1.bottom
            margins: platformStyle.paddingMedium
        }
        text: "Current tab: " + tabGroup.currentTab.objectName
        color: "red"
        font.family: platformStyle.fontFamilyRegular
        font.pixelSize: platformStyle.fontSizeSmall
    }

    //! [tabgroupa]
    TabGroup {
        id: tabGroup
        anchors {
            //! [tabgroupa]
            top: label2.bottom
            //! [tabgroupb]
            bottom: sharedToolBar.top
            left: parent.left
            right: parent.right
        }

        // tab definitions
        //! [tabgroupb]

        //! [tab1a]
        Item {
            id: tab1
            anchors.fill: parent

            // tab contents
            //! [tab1a]
            objectName: "Tab 1"
            BusyIndicator {
                anchors.centerIn: parent
                running: true
                width: internal.busyIndicatorSize
                height: internal.busyIndicatorSize
            }
            //! [tab1b]
        }
        //! [tab1b]

        Item {
            id: tab2
            objectName: "Tab 2"
            anchors.fill: parent
            BusyIndicator {
                anchors.centerIn: parent
                running: true
                width: internal.busyIndicatorSize * 2
                height: internal.busyIndicatorSize * 2
            }
        }

        Item {
            id: tab3
            objectName: "Tab 3"
            anchors.fill: parent
            BusyIndicator {
                anchors.centerIn: parent
                running: true
                width: internal.busyIndicatorSize * 3
                height: internal.busyIndicatorSize * 3
            }
        }
        //! [tabgroupc]
    }
    //! [tabgroupc]

    //! [sharedtoolbar]
    ToolBar {
        id: sharedToolBar
        anchors.bottom: parent.bottom
        tools: toolBarLayout1
    }
    //! [sharedtoolbar]

    //! [toolbarlayout1]
    ToolBarLayout {
        id: toolBarLayout1
        ToolButton {
            iconSource: "toolbar-back"
        }
        ButtonRow {
            TabButton {
                id: tabButton1
                tab: tab1
                text: "Tab1"
            }
            TabButton {
                id: tabButton2
                tab: tab2
                text: "Tab2"
            }
            TabButton {
                id: tabButton3
                tab: tab3
                text: "Tab3"
            }
        }
    }
    //! [toolbarlayout1]
}
//! [0]
