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
    property variant contentPages: []
    property int currentIndex: 1

    onContentPagesChanged: {
        if (priv.newItem != null)
            addButton()
        else
            removeObsoleteButtons()
    }

    function addButton() {
        var newButton = tabButtonComponent.createObject(tabBar.layout)
        newButton.text = priv.newItem.titleString
        newButton.tab = priv.newItem
        priv.newItem = null
    }

    function addTab(component) {
        var title = "tab " + currentIndex.toString()
        currentIndex++
        var newItem = component.createObject(null)
        newItem.titleString = title
        priv.newItem = newItem
        tabGroup.addTab(newItem)

        // Array properties cannot be manipulated. Using temporary array for pushing.
        var tempArray = []
        if (contentPages.length > 0)
            tempArray = contentPages
        tempArray.push(newItem)
        contentPages = tempArray
    }

    function containsPage(title) {
        for (var i = 0; i < contentPages.length; ++i) {
            if (contentPages[i].titleString == title)
                return true
        }
        return false
    }

    function removeObsoleteButtons() {
        for (var i = 0; i < tabBar.layout.children.length; ++i) {
            if (!containsPage(tabBar.layout.children[i].text))
                tabBar.layout.children[i].destroy()
        }
    }

    function removeTab(page) {
        // Array properties cannot be manipulated. Using temporary array for splicing.
        var tempArray = []
        tempArray = contentPages
        var foundIndex = -1
        for (var i = 0; i < tempArray.length && foundIndex == -1; ++i ) {
            if (tempArray[i].titleString == page.titleString)
                foundIndex = i
        }

        if (foundIndex != -1) {
            tempArray.splice(foundIndex,1)
            contentPages = tempArray
        }

        page.destroy()
    }

    function findButtonByContent(content) {
        for (var i = 0; i < tabBar.layout.children.length; ++i) {
            if (tabBar.layout.children[i].tab == content)
                return tabBar.layout.children[i]
        }
        return 0
    }

    TabBar {
        id: tabBar
        anchors.top: parent.top
    }

    TabGroup {
        id: tabGroup
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        width: parent.width

        Component.onCompleted: {
            root.addTab(pageComponent)
            root.addTab(pageComponent)
            root.addTab(pageComponent)
        }
    }

    Component {
        id: itemComponent
        Item {
            id: itemItem // ;)
            property string titleString

            Component.onCompleted: { commonContent.createObject(itemItem) }
        }
    }

    Component {
        id: pageComponent
        Page {
            id: pageItem
            property string titleString

            Component.onCompleted: {
                var newContent = commonContent.createObject(pageItem)
                newContent.page = pageItem
            }
        }
    }

    Component {
        id: tabButtonComponent
        TabButton {}
    }

    QtObject {
        id: priv
        property Item newItem
    }

    Component {
        id: commonContent
        Column {
            id: column
            property Item page

            anchors.fill: parent
            anchors.topMargin: 20
            Text {
                text: (page ? "page: " : "item: ") + column.parent.titleString
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 30
                color: "white"
            }

            Row {
                Button { text: "append page"; onClicked: root.addTab(pageComponent) }
                Button { text: "append item"; onClicked: root.addTab(itemComponent) }
                Button { text: "remove"; onClicked: removeTab(column.parent) }
            }

            Row {
                height: 40
                width: parent.width
                Rectangle { color: "red"; height: parent.height; width: parent.width / 2; opacity: page ? (page.status == Symbian.PageDeactivating ? 1 : 0.1) : 0}
                Rectangle { color: "blue"; height: parent.height; width: parent.width / 2; opacity: page ? (page.status == Symbian.PageActivating ? 1 : 0.1) : 0 }
            }

            Row {
                height: 40
                width: parent.width
                TextField {
                    id: editor
                    width: parent.width / 2
                    placeholderText: column.parent.titleString
                    onTextChanged: {
                        var tabButton = findButtonByContent(column.parent)
                        if (tabButton)
                            tabButton.text = text
                        column.parent.titleString = text
                    }
                }
                ChoiceList {
                    Component.onCompleted: {console.log("ChoiceList::onCompleted: " + currentValue + " " + currentIndex)}

                    id: iconChoicelist
                    width: parent.width / 2
                    onCurrentValueChanged: {
                        var tabButton = findButtonByContent(column.parent)
                        if (tabButton)
                            tabButton.iconSource = currentValue != "<none>" ? "image://theme/:/" + currentValue : ""
                    }
                    model: ["<none>", "list1.png", "list2.png"]
                }
            }
        }
    }
}
