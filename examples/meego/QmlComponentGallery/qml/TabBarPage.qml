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
import com.nokia.meego 1.1

Page {
    id: tabbarPage

    property bool busy: Boolean(tabGroup.currentTab.busy)
    anchors.margins: UiConstants.DefaultMargin

    tools: ToolBarLayout {
      
        ToolIcon { 
          id: toolIcon 
          iconId: "toolbar-back" 
          onClicked: pageStack.pop()
        }
                 
        ButtonRow {
          anchors {left: toolIcon.right; right: parent.right}
            
            platformStyle: TabButtonStyle { }
            TabButton {
                text: "Tab1"
                tab: tab1
            }
            TabButton {
                text: "Tab2"
                tab: tab2
            }
            TabButton {
                text: "Tab3"
                tab: tab3
            }
        }
    }

    TabGroup {
        id: tabGroup

        currentTab: tab1

        PageStack {
            id: tab1
        }
        PageStack {
            id: tab2
        }
        Page {
            id: tab3
            Column {
                spacing: 10

                Text {
                    text: "This is a single page"
                }
            }
        }
    }

    Component {
        id: stackedPage
        Page {
            property int index
            Column {
                anchors.horizontalCenter:parent.horizontalCenter
                spacing: 10

                Text {
                    text: "This is a stacked page for tab: " + index + "\nCurrent depth: " + tabGroup.currentTab.depth
                }
                Button {
                    text: "Push another page"
                    onClicked: { tabGroup.currentTab.push(stackedPage, { index : tabGroup.currentTab == tab1 ? 1 : 2 } ); }
                }
                Button {
                     text: "Change mode"
                     onClicked: {
                         for (var i = 0, l = tabbarPage.tools.children.length; i < l; i++) {
                             var row = tabbarPage.tools.children[i];
                             if (row.hasOwnProperty("checkedButton")) {
                                 for (var j = 0, l2 = row.children.length; j < l2; j++) {
                                     var child = row.children[j];
                                     child.text = child.text ? "" : "Tab" + (j+1);
                                     child.iconSource = child.iconSource != "" ? "" : "image://theme/icon-m-toolbar-search";
                                 }
                             }
                         }
                     }
                 }
             }
        }
    }

    Component.onCompleted: {
        tab1.push(stackedPage, { index: 1 });
        tab2.push(stackedPage, { index: 2 });
    }
}
