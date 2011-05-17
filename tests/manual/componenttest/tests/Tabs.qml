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
        var newItem = component.createObject(tabGroup)
        newItem.titleString = title
        priv.newItem = newItem

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

    function inPortrait() {
        return screen.height > screen.width
    }

    function updateOrientationLockValue(lockPortrait, lockLandscape) {
        if (lockPortrait && lockLandscape)
            priv.lockNewPageOrientation = PageOrientation.LockPrevious
        else if (lockPortrait && !lockLandscape)
            priv.lockNewPageOrientation = PageOrientation.LockPortrait
        else if (!lockPortrait && lockLandscape)
            priv.lockNewPageOrientation = PageOrientation.LockLandscape
        else
            priv.lockNewPageOrientation = PageOrientation.Automatic
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
        property int numberOfTabsOnNewPage
        property int lockNewPageOrientation
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
                font.pixelSize: platformStyle.fontSizeLarge * 1.5
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
                Rectangle { color: "red"; height: parent.height; width: parent.width / 2; opacity: page ? (page.status == PageStatus.Deactivating ? 1 : 0.1) : 0}
                Rectangle { color: "blue"; height: parent.height; width: parent.width / 2; opacity: page ? (page.status == PageStatus.Activating ? 1 : 0.1) : 0 }
            }

            Grid {
                id: editTabButtonGrid
                height: inPortrait() ? 90 : 60
                width: parent.width
                columns: inPortrait() ? 1 : 2
                spacing: 2

                Text {
                    id: editTabButtonLabel
                    width: inPortrait() ? parent.width : parent.width / 3
                    text: "Edit TabButton:"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: platformStyle.fontSizeLarge
                    color: "white"
                }

                Row {
                    id: editTabButtonContent
                    height: tabButtonIconSelectionListItem.preferredHeight
                    width: inPortrait() ? parent.width : 2 * parent.width / 3
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

                    SelectionListItem {
                        id: tabButtonIconSelectionListItem
                        objectName: "tabButtonIconSelectionListItem"
                        title: tabButtonIconSelectionDialog ? tabButtonIconSelectionDialog.model.get(tabButtonIconSelectionDialog.selectedIndex).name : "<none>"
                        width: parent.width / 2
                        property SelectionDialog tabButtonIconSelectionDialog

                        onTitleChanged: {
                            var tabButton = findButtonByContent(column.parent)
                            if (tabButton)
                                tabButton.iconSource = title != "<none>" ? "image://theme/:/" + title : ""
                        }

                        onClicked: {
                            if (!tabButtonIconSelectionDialog)
                                tabButtonIconSelectionDialog = tabButtonIconSelectionComponent.createObject(column)
                            tabButtonIconSelectionDialog.open()
                        }

                        Component {
                            id: tabButtonIconSelectionComponent
                            SelectionDialog {
                                titleText: "Select TabButton image"
                                selectedIndex: 0
                                model: ListModel {
                                    ListElement { name: "<none>" }
                                    ListElement { name: "list1.png" }
                                    ListElement { name: "list2.png" }
                                    ListElement { name: "qtg_graf_drill_down_indicator.svg" }
                                }
                            }
                        }
                    }
                }
            }

            Grid {
                id: newPageGrid
                height: inPortrait() ? 90 : 60
                width: parent.width
                columns: inPortrait() ? 1 : 2
                spacing: 2

                Text {
                    id: newPageLabel
                    width: inPortrait() ? parent.width : parent.width / 3
                    text: "New page tabs:"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: platformStyle.fontSizeLarge
                    color: "white"
                }

                Row {
                    height: 40
                    width: inPortrait() ? parent.width : 2 * parent.width / 3

                    SelectionListItem {
                        title: numberOfTabsSelectionDialog ? numberOfTabsSelectionDialog.model.get(numberOfTabsSelectionDialog.selectedIndex).name : "2"
                        width: parent.width / 2
                        property SelectionDialog numberOfTabsSelectionDialog

                        onTitleChanged: {
                            priv.numberOfTabsOnNewPage = parseInt(title)
                        }

                        onClicked: {
                            if (!numberOfTabsSelectionDialog)
                                numberOfTabsSelectionDialog = numberOfTabsSelectionComponent.createObject(column)
                            numberOfTabsSelectionDialog.open()
                        }

                        Component {
                            id: numberOfTabsSelectionComponent
                            SelectionDialog {
                                titleText: "Select number of tabs"
                                selectedIndex: 2
                                model: ListModel {
                                    ListElement { name: "0" }
                                    ListElement { name: "1" }
                                    ListElement { name: "2" }
                                    ListElement { name: "3" }
                                    ListElement { name: "4" }
                                    ListElement { name: "5" }
                                    ListElement { name: "6" }
                                }
                            }
                        }
                    }

                    Button {
                        width: parent.width / 2
                        text: "Launch new page"
                        onClicked: startNewPage()
                    }
                }
            }

            Row {
                height: 40
                width: parent.width

                CheckBox {
                    id: lockInPortrait;

                    width: parent.width / 2;
                    text: "lock in portrait"

                    onClicked: {
                        updateOrientationLockValue(lockInPortrait.checked, lockInLandscape.checked)
                    }
                }

                CheckBox {
                    id: lockInLandscape;

                    width: parent.width / 2;
                    text: "lock in landscape"

                    onClicked: {
                        updateOrientationLockValue(lockInPortrait.checked, lockInLandscape.checked)
                    }
                }
            }
        }
    }

    // function startNewPage() crates a tabbed page described below. The
    // content is created dynamically based on the tab amount requested.
/*
import QtQuick 1.0;
import com.nokia.symbian 1.0;
Page {
    id: page;
    TabBar {
        id: tabBar;
        TabButton { tab: tab1content; text: "1" }
        TabButton { tab: tab2content; text: "2" }
        TabButton { tab: tab3content; text: "3" }
    }

    TabGroup {
        height: 100
        width: 100
        Text { id: tab1content; text: "tab1"; color: "white" }
        Text { id: tab2content; text: "tab2"; color: "white" }
        Text { id: tab3content; text: "tab3"; color: "white" }
    }
}
*/

    function startNewPage() {
        var filePrefix =       "import QtQuick 1.0; import com.nokia.symbian 1.0; Page { id: page; "
        var tabBarPrefix =         "TabBar { id: tabBar; "
        var tabBarContent =            ""
        var tabBarPostfix =        "} "
        var tabGroupPrefix =       "TabGroup { anchors.top: tabBar.bottom; width: 100; height: 100; "
        var tabGroupContent =          ""
        var tabGroupPostfix =      "} "
        var filePostfix =      "}"

        for (var i = 1; i <= priv.numberOfTabsOnNewPage; ++i) {
            // create tab button
            // TabButton { tab: tab1content; text: \"1\" } "
            tabBarContent += "TabButton { tab: tab"
            tabBarContent += i.toString()
            tabBarContent += "content; text: \""
            tabBarContent += i.toString()
            tabBarContent += "\" } "

            // create group content
            // Text { id: tab1content; text: \"tab1\"; color: \"white\" } "
            tabGroupContent += "Text { id: tab"
            tabGroupContent += i.toString()
            tabGroupContent += "content; text: \"tab"
            tabGroupContent += i.toString()
            tabGroupContent += "\"; color: \"white\" } "

        }

        // combine as one long string
        var fullQml = filePrefix + tabBarPrefix + tabBarContent + tabBarPostfix + tabGroupPrefix + tabGroupContent + tabGroupPostfix + filePostfix

        var newObject = Qt.createQmlObject(fullQml, root);
        if (newObject) {
            newObject.orientationLock = priv.lockNewPageOrientation
            testPage.pageStack.push(newObject)
        }
    }
}
