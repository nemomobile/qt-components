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
    id: root
    width: 350
    height: 350

    //! [0]
    // create a tab bar layout and anchor it to the top of the window
    TabBarLayout {
        id: tabBarLayout
        anchors { left: parent.left; right: parent.right; top: parent.top }
    }

    // create the content area for the tabs and fill the remaining window space
    TabGroup {
        id: tabGroup
        anchors { left: parent.left; right: parent.right; top: tabBarLayout.bottom; bottom: parent.bottom }
    }
    //! [0]

    // A simple toolbar with code to add and remove a particular tab
    ToolBar {
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom}
        tools: ToolBarLayout {
            ToolButton {
                text: "+'2'"
                onClicked:  {
                    // add a component page to the tab bar
                    var i = findTabIndexByButtonText("2")
                    if (i == -1) {
                        // create a tab button and page and add them
                        var page = pageComponent.createObject(tabGroup)
                        page.message = "New"
                        var button = tabButtonComponent.createObject(tabBarLayout)
                        button.text = "2"
                        button.tab = page
                    }
                }
            }
            ToolButton {
                text: "-'2'"
                onClicked: {
                    removeTabByButtonText("2")
                }
            }
        }
    }

    //! [1]
    // an item page to add to the tab bar
    Page {
        id: myPage
        Text {
            anchors.centerIn: parent
            text: "Item page"
            font.pointSize: 20
            color: "white"
        }
    }
    //! [1]

    //! [2]
    // a component page to add to the tab bar
    Component {
        id: pageComponent
        Page {
            property alias message: pageText.text
            Text {
                id: pageText
                anchors.centerIn: parent
                text: "Component page"
                font.pointSize: 20
                color: "white"
            }
        }
    }
    //! [2]

    //! [3]
    // utility component so can add buttons to the tab bar
    Component {
        id: tabButtonComponent
        TabButton {}
    }
    //! [3]

    //! [5]
    // find the tab with the given text in the button and return the index
    function findTabIndexByButtonText(buttonText) {
        // find the button in the children of the tab bar layout item
        for (var i=0; i<tabBarLayout.data.length; i++) {
            if (tabBarLayout.data[i].hasOwnProperty("text")
                    && tabBarLayout.data[i].text == buttonText) {
                return i
            }
        }
        return -1 // button text not found
    }
    //! [5]

    //! [6]
    // remove a tab by the text on the button
    function removeTabByButtonText(buttonText) {
        var index = findTabIndexByButtonText(buttonText)
        if (index != -1) {
            tabBarLayout.data[index].tab.destroy()  // remove the tab content
            tabBarLayout.data[index].destroy()      // remove the tab button
        }
        // TODO: If the removed page was the current page, choose a new current page
    }
    //! [6]

    //! [7]
    // create some tabs for the tab bar
    Component.onCompleted: {
        var page = pageComponent.createObject(tabGroup)
        var button = tabButtonComponent.createObject(tabBarLayout);
        button.text = "1"
        button.tab = page

        // add a component page to the tab bar
        page = pageComponent.createObject(tabGroup)
        page.message = "Original"
        button = tabButtonComponent.createObject(tabBarLayout);
        button.text = "2"
        button.tab = page

        // add an item page to the tab bar
        myPage.parent = tabGroup
        button = tabButtonComponent.createObject(tabBarLayout);
        button.text = "3"
        button.tab = myPage
    }
    //! [7]
}
