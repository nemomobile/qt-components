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
import Qt.labs.components 1.1
import "../TestUtils.js" as Utils
import "../components"

Item {
    id: root
    anchors.fill: parent
    property string replaceMode: "set"
    property bool flat: false
    property Item testToolBarLayout : null
    property Item previousToolBarLayout : null
    property bool platformInverted: false

    visible: true
    Component.onCompleted: {
        stack.push(firstPage)
    }

    onFlatChanged: {
        flatter1.checked = flat
        flatter2.checked = flat
    }


    QtObject {
        id: internal

        function createToolBarLayout(layoutComponent) {
            if (root.previousToolBarLayout)
                root.previousToolBarLayout.destroy()
            root.previousToolBarLayout = root.testToolBarLayout
            root.testToolBarLayout = layoutComponent.createObject(root, {})
            // manually set inversion mode for dynamically created item tree
            Utils.setItemTreeInversion(root.testToolBarLayout, root.platformInverted)
            bar1.setTools(root.testToolBarLayout, root.replaceMode)
        }
    }

    PageStack {
        id: stack
        anchors.fill: parent
        clip: true
    }

    //
    // Pages
    //
    Page {
        id: firstPage
        tools: bar1
        ToolBar {
            id: bar1
            objectName: "toolBar1"
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            //make sure the content of the page does not hide the toolbar
            z: firstPage.z+1
        }
        Flickable {
            contentWidth: width; contentHeight: columni.height
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: bar1.top
            }
            Column {
                id: columni
                width: firstPage.width
                Row {
                    width: firstPage.width
                    CheckBox {
                        id: flatter1
                        text: "flat"
                        checked: root.flat
                        onClicked: root.flat = !root.flat
                        Component.onCompleted: checked = root.flat
                    }

                    Button {
                        text: "Portrait"
                        onClicked: {
                            screen.allowedOrientations = Screen.Portrait
                        }
                    }
                    Button {
                        text: "Landscape"
                        onClicked: {
                            screen.allowedOrientations = Screen.Landscape
                        }
                    }
                }
                ButtonRow {
                    width: firstPage.width
                    exclusive: true
                    Button {
                        id: rb1
                        text: "set"
                        onClicked: root.replaceMode = "set"
                    }
                    Button {
                        id: rb2
                        text: "push"
                        onClicked: root.replaceMode = "push"
                    }
                    Button {
                        id: rb3
                        text: "pop"
                        onClicked: root.replaceMode = "pop"
                    }
                    Button {
                        id: rb4
                        text: "replace"
                        onClicked: root.replaceMode = "replace"
                    }
                }
                ListHeading {
                    Label {
                        text: "General";
                        anchors { left: parent.left; leftMargin: platformStyle.paddingMedium; verticalCenter: parent.verticalCenter }
                        font.pixelSize: Math.round(platformStyle.fontSizeSmall * 3 / 4)
                    }
                }
                Flow {
                    width: firstPage.width
                    spacing: platformStyle.paddingSmall

                    Button { text: "All (Text)"; onClicked: { allToolBarLayoutsPage.sourceComponent = allTextPageComponent; stack.push(allToolBarLayoutsPage) } }
                    Button { text: "All (Icon)"; onClicked: { allToolBarLayoutsPage.sourceComponent = allIconPageComponent; stack.push(allToolBarLayoutsPage) } }
                    Button { text: "Back+BR(3TabB)"; onClicked: stack.push(buttonRowTabButtonPage) }

                    Button { text: "B"; onClicked: internal.createToolBarLayout(toolBarLayout0a) }
                    Button { text: "B+M"; onClicked: internal.createToolBarLayout(toolBarLayout0b) }
                    Button { text: "B+3B_T"; onClicked: internal.createToolBarLayout(toolBarLayout1a) }
                    Button { text: "B+4B_T [irreg]"; onClicked: internal.createToolBarLayout(toolBarLayout1b) }
                    Button { text: "4Switch"; onClicked: internal.createToolBarLayout(toolBarLayout1c) }
                }
                ListHeading {
                    Label {
                        text: "ToolButton > Text"
                        anchors { left: parent.left; leftMargin: platformStyle.paddingMedium; verticalCenter: parent.verticalCenter }
                        font.pixelSize: Math.round(platformStyle.fontSizeSmall * 3 / 4)
                    }
                }
                Flow {
                    width: firstPage.width
                    spacing: platformStyle.paddingSmall

                    Button { text: "B+TB_empty+M"; onClicked: internal.createToolBarLayout(toolBarLayout2a) }
                    Button { text: "B+TB_T+M"; onClicked: internal.createToolBarLayout(toolBarLayout2b) }
                    Button { text: "B+TB_T&I+M"; onClicked: internal.createToolBarLayout(toolBarLayout2c) }
                    Button { text: "B+2TB_T+M"; onClicked: internal.createToolBarLayout(toolBarLayout3a) }
                    Button { text: "B+2TB_T"; onClicked: internal.createToolBarLayout(toolBarLayout3b) }
                    Button { text: "2TB_T+M"; onClicked: internal.createToolBarLayout(toolBarLayout3c) }
                }
                ListHeading {
                    Label {
                        text: "ButtonRow > ToolButton > Text"
                        anchors { left: parent.left; leftMargin: platformStyle.paddingMedium; verticalCenter: parent.verticalCenter }
                        font.pixelSize: Math.round(platformStyle.fontSizeSmall * 3 / 4)
                    }
                }
                Flow {
                    width: firstPage.width
                    spacing: platformStyle.paddingSmall

                    Button { text: "BR(TB_T)"; onClicked: internal.createToolBarLayout(toolBarLayout13a) }
                    Button { text: "BR(2TB_T)"; onClicked: internal.createToolBarLayout(toolBarLayout13b) }
                    Button { text: "BR(3TB_T)"; onClicked: internal.createToolBarLayout(toolBarLayout13c) }
                    Button { text: "B+BR(TB_T)"; onClicked: internal.createToolBarLayout(toolBarLayout14a) }
                    Button { text: "B+BR(2TB_T)"; onClicked: internal.createToolBarLayout(toolBarLayout14b) }
                    Button { text: "B+BR(3TB_T)"; onClicked: internal.createToolBarLayout(toolBarLayout14c) }
                    Button { text: "BR(TB_T)+M"; onClicked: internal.createToolBarLayout(toolBarLayout15a) }
                    Button { text: "BR(2TB_T)+M"; onClicked: internal.createToolBarLayout(toolBarLayout15b) }
                    Button { text: "BR(3TB_T)+M"; onClicked: internal.createToolBarLayout(toolBarLayout15c) }
                    Button { text: "B+BR(TB_T)+M"; onClicked: internal.createToolBarLayout(toolBarLayout19a) }
                    Button { text: "B+BR(2TB_T)+M"; onClicked: internal.createToolBarLayout(toolBarLayout19b) }
                    Button { text: "B+BR(3TB_T)+M"; onClicked: internal.createToolBarLayout(toolBarLayout19c) }
                }
                ListHeading {
                    Label {
                        text: "ToolButton  > Icon"
                        anchors { left: parent.left; leftMargin: platformStyle.paddingMedium; verticalCenter: parent.verticalCenter }
                        font.pixelSize: Math.round(platformStyle.fontSizeSmall * 3 / 4)
                    }
                }
                Flow {
                    width: firstPage.width
                    spacing: platformStyle.paddingSmall

                    Button { text: "0+TB_I+0"; onClicked: internal.createToolBarLayout(toolBarLayout5) }
                    Button { text: "B+TB_I+M"; onClicked: internal.createToolBarLayout(toolBarLayout6) }
                    Button { text: "B+2TB_I+M"; onClicked: internal.createToolBarLayout(toolBarLayout7) }
                    Button { text: "B+3TB_I+M"; onClicked: internal.createToolBarLayout(toolBarLayout8) }
                    Button { text: "B+4TB_I+M"; onClicked: internal.createToolBarLayout(toolBarLayout9) }
                    Button { text: "B+5TB_I+M"; onClicked: internal.createToolBarLayout(toolBarLayout10) }
                    Button { text: "B+5TB_I+0"; onClicked: internal.createToolBarLayout(toolBarLayout11a) }
                    Button { text: "0+5TB_I+M"; onClicked: internal.createToolBarLayout(toolBarLayout11b) }
                }
                ListHeading {
                    Label {
                        text: "ButtonRow > ToolButton > Icon"
                        anchors { left: parent.left; leftMargin: platformStyle.paddingMedium; verticalCenter: parent.verticalCenter }
                        font.pixelSize: Math.round(platformStyle.fontSizeSmall * 3 / 4)
                    }
                }
                Flow {
                    width: firstPage.width
                    spacing: platformStyle.paddingSmall

                    Button { text: "BR(TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout16a) }
                    Button { text: "BR(2TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout16b) }
                    Button { text: "BR(3TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout16c) }
                    Button { text: "BR(4TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout16d) }
                    Button { text: "B+BR(TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout17a) }
                    Button { text: "B+BR(2TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout17b) }
                    Button { text: "B+BR(3TB_I)_0"; onClicked: internal.createToolBarLayout(toolBarLayout17c) }
                    Button { text: "B+BR(3TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout17d) }
                    Button { text: "B+BR(4TB_I)"; onClicked: internal.createToolBarLayout(toolBarLayout17e) }
                    Button { text: "B+BR(TB_I)+M"; onClicked: internal.createToolBarLayout(toolBarLayout18a) }
                    Button { text: "B+BR(2TB_I)+M"; onClicked: internal.createToolBarLayout(toolBarLayout18b) }
                    Button { text: "B+BR(3TB_I)+0"; onClicked: internal.createToolBarLayout(toolBarLayout18c) }
                    Button { text: "B+BR(3TB_I)+M"; onClicked: internal.createToolBarLayout(toolBarLayout18d) }
                    Button { text: "B+BR(4TB_I)+M"; onClicked: internal.createToolBarLayout(toolBarLayout18e) }
                }
            }
        }
    }

    Page {
        id: secondPage
        tools: bar2
        Column {
            Label {
                text: "Page2\n click next to go to \nthird page and back to return to first"
            }
        }
        ToolBar {
            id: bar2
            objectName: "toolBar2"
            anchors.bottom: parent.bottom
            tools: ToolBarLayout {
                ToolButton {
                    flat: root.flat
                    iconSource: "toolbar-back"
                    onClicked: stack.pop()
                }
                ToolButton {
                    flat: root.flat
                    text: "next"
                    onClicked: stack.push(thirdPage)
                }
            }
        }
    }

    Page {
        id: thirdPage
        Column {
            Label {
                text: "Page3\n no toolbar"
            }
            Button {
                text: "back"
                onClicked: stack.pop();
            }
        }
    }

    Page {
        id: buttonRowTabButtonPage
        tools: buttonRowTabButtonToolBar

        Text {
            text: "Current tab: " + tabGroup.currentTab.objectName
            color: "red"
        }

        TabGroup {
            id: tabGroup
            anchors { bottom: buttonRowTabButtonToolBar.top; top: parent.top; left: parent.left; right: parent.right }

            Item {
                id: tab1
                objectName: "tab1"
                anchors.fill: parent
                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    width: 60
                    height: 60
                }
            }

            Item {
                id: tab2
                objectName: "tab2"
                anchors.fill: parent
                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    width: 120
                    height: 120
                }
            }

            Item {
                id: tab3
                objectName: "tab3"
                anchors.fill: parent
                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    width: 240
                    height: 240
                }
            }
        }

        ToolBar {
            id: buttonRowTabButtonToolBar
            objectName: "toolBar3"
            anchors.bottom: parent.bottom
            //make sure the content of the page does not hide the toolbar
            z: buttonRowTabButtonPage.z + 1
            tools: ToolBarLayout {
                ToolButton {
                    id: backButton
                    iconSource: "toolbar-back"
                    onClicked: stack.pop()
                }
                ButtonRow {
                    TabButton { id: tabButton1; tab: tab1; text: "Tab1"; }
                    TabButton { id: tabButton2; tab: tab2; text: "Tab2"; }
                    TabButton { id: tabButton3; tab: tab3; text: "Tab3"; }
                }
            }
        }
    }

    Page {
        id: allToolBarLayoutsPage
        property Component sourceComponent : null

        onStatusChanged: {
            if (allToolBarLayoutsPage.status == PageStatus.Activating) {
                allToolBarLayoutsPageBusy.running = true
            } else if (allToolBarLayoutsPage.status == PageStatus.Active) {
                allToolBarLayoutsPageLoader.sourceComponent = allToolBarLayoutsPage.sourceComponent
            } else if (allToolBarLayoutsPage.status == PageStatus.Deactivating) {
                allToolBarLayoutsPageBusy.running = true
            } else if (allToolBarLayoutsPage.status == PageStatus.Inactive) {
                allToolBarLayoutsPageLoader.sourceComponent = undefined
            }
        }

        Row {
            id: controls2
            height: childrenRect.height
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            Button {
                iconSource: "image://theme/toolbar-back"
                onClicked: stack.pop();
            }
            CheckBox {
                id: flatter2
                text: "flat"
                checked: root.flat
                onClicked: root.flat = !root.flat
                Component.onCompleted: checked = root.flat
            }

            Button {
                text: "Portrait"
                onClicked: {
                    screen.allowedOrientations = Screen.Portrait
                }
            }
            Button {
                text: "Landscape"
                onClicked: {
                    screen.allowedOrientations = Screen.Landscape
                }
            }
        }

        BusyIndicator {
            id: allToolBarLayoutsPageBusy
            anchors.centerIn: parent
            width: 120
            height: 120
        }

        Loader {
            id: allToolBarLayoutsPageLoader
            anchors {
                top: controls2.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            sourceComponent: undefined
            opacity: (status == Loader.Ready && sourceComponent != undefined) ? 1 : 0
            Behavior on opacity {
                NumberAnimation { from: 0; to: 1; duration: 200 }
            }
            onLoaded: {
                allToolBarLayoutsPageBusy.running = false
                // manually set inversion mode for dynamically loaded item tree
                Utils.setItemTreeInversion(allToolBarLayoutsPageLoader, root.platformInverted)
            }
        }
    }

    //
    // Components
    //
    Component {
        id: allTextPageComponent

        Item {
            Flickable {
                anchors.fill: parent
                contentWidth: parent.width; contentHeight: allTextColumn.height
                clip: true

                Column {
                    id: allTextColumn
                    spacing: platformStyle.paddingSmall

                    // tool buttons
                    ToolBar { Component.onCompleted: setTools(toolBarLayout0a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout0b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout1a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout1b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout1c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout2a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout2b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout2c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout3a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout3b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout3c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }

                    // button rows
                    ToolBar { Component.onCompleted: setTools(toolBarLayout13a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout13b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout13c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout14a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout14b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout14c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout15a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout15b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout15c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout19a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout19b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout19c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                }
            }
        }
    }


    Component {
        id: allIconPageComponent

        Item {
            Flickable {
                anchors.fill: parent
                contentWidth: parent.width; contentHeight: allIconColumn.height
                clip: true

                Column {
                    id: allIconColumn
                    spacing: platformStyle.paddingSmall

                    // tool buttons
                    ToolBar { Component.onCompleted: setTools(toolBarLayout5.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout6.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout7.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout8.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout9.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout10.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout11a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout11b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }

                    // button rows
                    ToolBar { Component.onCompleted: setTools(toolBarLayout16a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout16b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout16c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout16d.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout17a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout17b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout17c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout17d.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout17e.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout18a.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout18b.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout18c.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout18d.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                    ToolBar { Component.onCompleted: setTools(toolBarLayout18e.createObject(root, {} ), "set"); Component.onDestruction: { tools.destroy() } }
                }
            }
        }
    }

    Component {
        id: toolBarLayout0a
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }
        }
    }

    Component {
        id: toolBarLayout0b
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }
            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout1a
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }
            Button {
                text: "Button"
            }
            Button {
                text: "Button"
            }
            Button {
                text: "Button"
            }
        }
    }

    Component {
        id: toolBarLayout1b
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }
            Button {
                text: "one"
            }
            Button {
                text: "two"
            }
            Button {
                text: "three"
            }
            Button {
                text: "four"
            }
        }
    }

    Component {
        id: toolBarLayout1c
        ToolBarLayout {

            Switch { }
            Switch { }
            Switch { }
            Switch { }
        }
    }

    Component {
        id: toolBarLayout2a
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }
            ToolButton {
                flat: root.flat
                text: ""
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout2b
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tb1"
                flat: root.flat
                text: "ToolBtn"
            }

            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout2c
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tb1"
                flat: root.flat
                text: "ToolBtn"
                iconSource: "toolbar-home"
            }

            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout3a
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tb1"
                flat: root.flat
                text: "ToolBtn"
            }
            ToolButton {
                objectName: "tb2"
                flat: root.flat
                text: "ToolBtn"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout3b
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tb1"
                flat: root.flat
                text: "ToolBtn"
            }
            ToolButton {
                objectName: "tb2"
                flat: root.flat
                text: "ToolBtn"
            }
        }
    }

    Component {
        id: toolBarLayout3c
        ToolBarLayout {
            ToolButton {
                objectName: "tb1"
                flat: root.flat
                text: "ToolBtn"
            }
            ToolButton {
                objectName: "tb2"
                flat: root.flat
                text: "ToolBtn"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout5
        ToolBarLayout {
            ToolButton {
                visible: false
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                visible: false
            }
        }
    }

    Component {
        id: toolBarLayout6
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack1"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout7
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack1"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack3"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout8
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack1"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack3"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack4"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout9
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack1"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack3"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack4"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack5"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout10
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack1"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack3"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack4"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack5"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack6"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout11a
        ToolBarLayout {
            ToolButton {
                objectName: "tbBack1"
                iconSource: "toolbar-back"
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack3"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack4"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack5"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack6"
                iconSource: "toolbar-home"
            }
            ToolButton {
                visible: false
            }
        }
    }

    Component {
        id: toolBarLayout11b
        ToolBarLayout {
            ToolButton {
                visible: false
            }
            ToolButton {
                objectName: "tbBack2"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack3"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack4"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack5"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbBack6"
                iconSource: "toolbar-home"
            }
            ToolButton {
                objectName: "tbOptions"
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout13a
        ToolBarLayout {
            ButtonRow {
                id: buttonRow13a
                objectName: "buttonRow13a"

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }
        }
    }

    Component {
        id: toolBarLayout13b
        ToolBarLayout {
            ButtonRow {
                id: buttonRow13b
                objectName: "buttonRow13b"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }

                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }
        }
    }

    Component {
        id: toolBarLayout13c
        ToolBarLayout {
            ButtonRow {
                id: buttonRow13c
                objectName: "buttonRow13c"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }
        }
    }

    Component {
        id: toolBarLayout14a
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow14a
                objectName: "buttonRow14a"

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }
        }
    }

    Component {
        id: toolBarLayout14b
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow14b
                objectName: "buttonRow14b"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }

                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }
        }
    }

    Component {
        id: toolBarLayout14c
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow14c
                objectName: "buttonRow14c"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }
        }
    }

    Component {
        id: toolBarLayout15a
        ToolBarLayout {
            ButtonRow {
                id: buttonRow15a
                objectName: "buttonRow15a"

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }

        }
    }

    Component {
        id: toolBarLayout15b
        ToolBarLayout {
            ButtonRow {
                id: buttonRow15b
                objectName: "buttonRow15b"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }

                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }

        }
    }

    Component {
        id: toolBarLayout15c
        ToolBarLayout {
            ButtonRow {
                id: buttonRow15c
                objectName: "buttonRow15c"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }

        }
    }

    Component {
        id: toolBarLayout19a
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow19a
                objectName: "buttonRow19a"

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }

        }
    }

    Component {
        id: toolBarLayout19b
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow19b
                objectName: "buttonRow19b"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }

                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }

        }
    }

    Component {
        id: toolBarLayout19c
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow19c
                objectName: "buttonRow19c"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    text: "BR ToolBtn"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }

        }
    }

    Component {
        id: toolBarLayout16a
        ToolBarLayout {
            ButtonRow {
                id: buttonRow16a
                objectName: "buttonRow16a"

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout16b
        ToolBarLayout {
            ButtonRow {
                id: buttonRow16b
                objectName: "buttonRow16b"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout16c
        ToolBarLayout {
            ButtonRow {
                id: buttonRow16c
                objectName: "buttonRow16c"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout16d
        ToolBarLayout {
            ButtonRow {
                id: buttonRow16d
                objectName: "buttonRow16d"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb4
                    objectName: "tb4"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout17a
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow17a
                objectName: "buttonRow17a"

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout17b
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow17b
                objectName: "buttonRow17b"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout17c
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow17c
                objectName: "buttonRow17c"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }

            ToolButton {
                visible: false
            }
        }
    }

    Component {
        id: toolBarLayout17d
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow17d
                objectName: "buttonRow17d"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout17e
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow17e
                objectName: "buttonRow17e"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb4
                    objectName: "tb4"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }
        }
    }

    Component {
        id: toolBarLayout18a
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow18a
                objectName: "buttonRow18a"

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout18b
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow18b
                objectName: "buttonRow18b"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout18c
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow18c
                objectName: "buttonRow18c"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }

            ToolButton {
                visible: false
            }
        }
    }

    Component {
        id: toolBarLayout18d
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow18d
                objectName: "buttonRow18d"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

    Component {
        id: toolBarLayout18e
        ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
            }

            ButtonRow {
                id: buttonRow18e
                objectName: "buttonRow18e"
                exclusive: true
                checkedButton: tb1

                ToolButton {
                    id: tb1
                    objectName: "tb1"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb2
                    objectName: "tb2"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb3
                    objectName: "tb3"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
                ToolButton {
                    id: tb4
                    objectName: "tb4"
                    flat: root.flat
                    iconSource: "toolbar-home"
                }
            }

            ToolButton {
                onClicked: stack.push(secondPage)
                iconSource: "toolbar-menu"
            }
        }
    }

}
