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
import "../TestUtils.js" as Utils
import "../components"

Item {
    id: root
    property variant contentPages: []
    property int currentIndex: 1
    property bool platformInverted: false

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
        return newItem
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
            var page1 = root.addTab(pageComponent)
            page1.orientationLock = PageOrientation.Automatic
            var page2 = root.addTab(pageComponent)
            page2.orientationLock = PageOrientation.LockPortrait
            var page3 = root.addTab(pageComponent)
            page3.orientationLock = PageOrientation.LockLandscape
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
        TabButton {platformInverted: root.platformInverted}
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
            Label {
                text: (page ? "page: " : "item: ") + column.parent.titleString
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: platformStyle.fontSizeLarge * 1.5
                platformInverted: root.platformInverted
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

                Label {
                    id: editTabButtonLabel
                    width: inPortrait() ? parent.width : parent.width / 3
                    text: "Edit TabButton:"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: platformStyle.fontSizeLarge
                    platformInverted: root.platformInverted
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
                        title: tabButtonIconSelectionDialog ? tabButtonIconSelectionDialog.model.get(tabButtonIconSelectionDialog.selectedIndex).name : "None"
                        width: parent.width / 2
                        property SelectionDialog tabButtonIconSelectionDialog

                        onTitleChanged: {
                            var tabButton = findButtonByContent(column.parent)
                            if (tabButton)
                                tabButton.iconSource = title != "None" ? title : ""
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
                                platformInverted: root.platformInverted
                                model: ListModel {
                                    ListElement { name: "None"; }
                                    ListElement { name: "../images/tb_plus.png" }
                                    ListElement { name: "qrc:/tb_plus.png" }
                                    ListElement { name: "qrc:/qtg_graf_drill_down_indicator.svg" }
                                    ListElement { name: "qtg_graf_drill_down_indicator.svg" }
                                    ListElement { name: "qtg_graf_drill_down_indicator" }
                                    ListElement { name: "qrc:/list1.svg" }
                                    ListElement { name: "qrc:list1.svg" }
                                    ListElement { name: "../images/list1.svg" }
                                    ListElement { name: "http://www.nokia.com/NOKIA_COM_1/Microsites/wayfinder/images/logo_new.gif" }
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

                Label {
                    id: newPageLabel
                    width: inPortrait() ? parent.width : parent.width / 3
                    text: "New page tabs:"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: platformStyle.fontSizeLarge
                    platformInverted: root.platformInverted
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
                                platformInverted: root.platformInverted
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

        Component.onCompleted: Utils.setItemTreeInversion(column, root.platformInverted)
        }
    }

    // function startNewPage() crates a tabbed page described below. The
    // content is created dynamically based on the tab amount requested.
/*

import QtQuick 1.1;
import com.nokia.symbian 1.1;

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
        Text { id: tab1content; text: "tab1"; color: "grey" }
        Text { id: tab2content; text: "tab2"; color: "grey" }
        Text { id: tab3content; text: "tab3"; color: "grey" }
    }
}
*/

    function startNewPage() {
        var filePrefix =       "import QtQuick 1.1; import com.nokia.symbian 1.1; Page { id: page; "
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
            // Text { id: tab1content; text: \"tab1\"; color: \"grey\" } "
            tabGroupContent += "Text { id: tab"
            tabGroupContent += i.toString()
            tabGroupContent += "content; text: \"tab"
            tabGroupContent += i.toString()
            tabGroupContent += "\"; color: \"grey\" } "

        }

        // combine as one long string
        var fullQml = filePrefix + tabBarPrefix + tabBarContent + tabBarPostfix + tabGroupPrefix + tabGroupContent + tabGroupPostfix + filePostfix

        var newObject = Qt.createQmlObject(fullQml, root);
        if (newObject) {
            newObject.orientationLock = priv.lockNewPageOrientation
            testPage.pageStack.push(newObject)
        }
    }

    Menu {
        id: optionsMenu

        content: MenuLayout {
            MenuItem {
                text: tabGroup.platformAnimated ? "Disable animation" : "Enable animation"
                onClicked: tabGroup.platformAnimated = !tabGroup.platformAnimated
            }
        }
    }

    Connections {
        target: optionsButton
        onClicked: optionsMenu.open()
    }
}
