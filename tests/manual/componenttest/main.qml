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
import FileAccess 1.0
import Settings 1.0
import LayoutDirectionSetter 1.0
import "TestUtils.js" as Utils

// This uses ApplicationWindow instead of PageStackWindow so that it remains to be tested by our apps.
ApplicationWindow {
    id: mainWindow

    objectName: "mainWindow"

    property Menu memoryToolsMenu
    property bool platformInverted: false

    // For TDriver tests - setting component name to this property will open the corresponding
    // component page "automatically"
    property string componentName

    LayoutMirroring.enabled: Qt.application.layoutDirection == Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

    initialPage: component

    FileAccess {
        id: fileAccess
    }

    Settings {
        id: settings
    }

    LayoutDirectionSetter {
        id: layoutDirectionSetter
    }

    QtObject {
        id: internal

        // Path to file to be loaded from drive
        property string fullPath: ""

        // Path to test qml-files shown on the main view
        property string testFilesPath: "tests/"

        // Diretories from where qmlfiles can be loaded
        property variant qmlPaths: []

        // Holds the text of the pressed button in the main view
        property variant checkedButton: null

        property bool dragEnabled: false
        property bool flickEnabled: false
        property bool fillEnabled: true

        function resolveSource() {
            if (checkedButton) {
                // All tests are under tests folder
                var file = checkedButton.text.replace(" ", "") + ".qml"
                return testFilesPath + file
            } else if (fullPath) {
                return "file:///" + fullPath
            } else {
                return ""
            }
        }
    }

    Loader {
        id: memoryDisplay
        visible: false
        source: visible ? "qrc:/MemoryDisplay.qml" : ""
    }

    ToolBarLayout {
        id: commonTools
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: pageStack.depth <= 1 ? Qt.quit() : pageStack.pop()
        }
        ToolButton {
            id: memoryToolsButton
            flat: true
            iconSource: "qrc:memory_card.svg"
            onClicked: {
                if (!memoryToolsMenu) {
                    memoryToolsMenu = memToolsMenuComponent.createObject(mainWindow)
                    Utils.setItemTreeInversion(memoryToolsMenu, mainWindow.platformInverted)
                }
                memoryToolsMenu.open()
            }
        }
        ToolButton {
            id: optionsButton
            flat: true
            iconSource: "toolbar-menu"
        }
    }

    Component.onCompleted: {
        internal.qmlPaths = fileAccess.qmlPaths()
        screen.allowedOrientations = settings.orientation()
        // clear the toolBar pointer, prevents subpages from
        // accidentally removing common application tools
        mainWindow.pageStack.toolBar = null
    }

    Component {
        id: component

        Page {
            tools: commonTools
            orientationLock: PageOrientation.Manual

            anchors.fill: parent

            onStatusChanged: {
                if (status == PageStatus.Activating)
                    screen.allowedOrientations = startupOrientationButton.orientation
            }

            Flickable {
                id: flickable
                anchors.fill: parent
                anchors.topMargin: memoryDisplay.visible ? memoryDisplay.height : 0

                contentHeight: buttons.height
                contentWidth: parent.width
                flickableDirection: Flickable.VerticalFlick

                Column {
                    id: buttons

                    width: parent.width

                    Row {
                        id: orientationButtons

                        property real buttonWidth: parent.width / children.length

                        Button {
                            id: portraitButton

                            text: "Port."
                            width: parent.buttonWidth

                            onClicked: {
                                startupOrientationButton.orientation = Screen.Portrait
                                screen.allowedOrientations = Screen.Portrait
                            }
                        }
                        Button {
                            id: landscapeButton

                            text: "Land."
                            width: parent.buttonWidth

                            onClicked: {
                                startupOrientationButton.orientation = Screen.Landscape
                                screen.allowedOrientations = Screen.Landscape
                            }
                        }
                        Button {
                            id: automaticOrientationButton

                            text: "Auto"
                            width: parent.buttonWidth

                            onClicked: {
                                startupOrientationButton.orientation = Screen.Default
                                screen.allowedOrientations = Screen.Default
                            }
                        }
                        Button {
                            id: startupOrientationButton

                            // Holds the numerical value of the orientation
                            property int orientation

                            text: "Save:" + orientation
                            width: parent.buttonWidth

                            Component.onCompleted: orientation = settings.orientation()

                            // save orientation for next startup
                            onClicked: settings.setOrientation(orientation)
                        }
                    }
                    Button {
                        id: toggleFullscreenButton

                        width: screen.width
                        text: "Toggle Fullscreen"
                        onClicked: {
                            mainWindow.fullScreen = !mainWindow.fullScreen
                        }
                    }

                    CheckBox {
                        id: flickableSetting
                        anchors.left: parent.left
                        text: "Flickable"
                        checked: false
                        onCheckedChanged: {
                            internal.flickEnabled = checked
                            if (checked) dragSetting.checked = false
                        }
                    }
                    CheckBox {
                        id: dragSetting
                        anchors.left: parent.left
                        text: "Drag-able"
                        checked: false
                        onCheckedChanged: {
                            internal.dragEnabled = checked
                            if (checked) flickableSetting.checked = false
                        }
                    }
                    CheckBox {
                        id: invertedThemeSetting
                        anchors.left: parent.left
                        text: "Inverted theme"
                        checked: false
                        onCheckedChanged: Utils.setItemTreeInversion(mainWindow, checked)
                    }
                    CheckBox {
                        id: orientationMethodSetting
                        anchors.left: parent.left
                        text: "Next start to resize orientation"
                        checked: false

                        Component.onDestruction: {
                            if (checked)
                                settings.setOrientationMethod(1)
                            else settings.setOrientationMethod(0)
                        }
                    }

                    Button {
                        id: pickFromFileButton

                        width: screen.width
                        text: {
                            // Append qmlpaths to the button label
                            var buttonLabel = "Load from "
                            for (var i = 0; i < internal.qmlPaths.length; i++) {
                                buttonLabel += internal.qmlPaths[i];
                                if (!((i+1) == internal.qmlPaths.length))
                                    buttonLabel += " or "
                            }
                            return buttonLabel
                        }

                        onClicked: { listPageComponent.show() }
                    }

                    Repeater {
                        id: componentsmodel

                        // Model is populated with the qml-files found from
                        // the tests folder
                        model: fileAccess.qmlFileNames(internal.testFilesPath)

                        Button {
                            id: testButton

                            text: modelData
                            width: screen.width

                            onClicked: {
                                internal.checkedButton = testButton
                                internal.fillEnabled = true
                                if (internal.flickEnabled)
                                    mainWindow.pageStack.push(flickableTestComponent)
                                else
                                    mainWindow.pageStack.push(testComponent)
                            }

                            // For TDriver ->
                            // Triggers the page openining based on the componentName property
                            Connections {
                                target: mainWindow
                                onComponentNameChanged: {
                                    if (componentName) {
                                        testButton.text = mainWindow.componentName
                                        internal.checkedButton = testButton
                                        internal.fillEnabled = true
                                        if (internal.flickEnabled)
                                            mainWindow.pageStack.push(flickableTestComponent, null, true)
                                        else
                                            mainWindow.pageStack.push(testComponent, null, true)
                                    }
                                componentName = ""
                                }
                            }
                            // <- for TDriver
                        }
                    }
                }
            }

            ScrollBar {
                id: scrollbar

                anchors { top: flickable.top; right: flickable.right }
                flickableItem: flickable
            }
        }
    } // main page component

    SelectionDialog {
        titleText: "Select file:"
        id: listPageComponent
        model: ListModel { id: testfileModel }
        buttons: Item {
            height: fillSetting.height + 2 * platformStyle.paddingMedium
            width: parent.width
            CheckBox {
                id: fillSetting
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: platformStyle.paddingLarge
                text: "Fill area"
                checked: true
                enabled: !internal.flickEnabled
                onClicked: internal.fillEnabled = checked
            }
        }

        property bool initialized: false

        // fill the model based on files in C or E-drives
        function show() {
            if (!initialized) {
                var fileNames = new Array()
                var fileFullPaths = new Array()
                for (var i = 0; i < internal.qmlPaths.length; i++) {
                    console.log("path #" + i)
                    fileNames = fileAccess.qmlFileNames(internal.qmlPaths[i])
                    fileFullPaths = fileAccess.qmlFilePaths(internal.qmlPaths[i])
                    for (var j = 0; j < fileNames.length; j++) {
                        testfileModel.append({"modelData": fileNames[j] + ".qml", "fileFullPath": fileFullPaths[j]})
                    }
                }
                initialized = true
            }
            if (internal.flickEnabled) fillSetting.checked = true
            open()
        }

        onAccepted: {
            internal.fullPath = testfileModel.get(selectedIndex).fileFullPath
            internal.checkedButton = 0
            if (internal.flickEnabled)
                mainWindow.pageStack.push(flickableTestComponent)
            else
                mainWindow.pageStack.push(testComponent)
        }
    }// listPageComponent

    Component {
        id: flickableTestComponent

        Page {
            id: testPage
            orientationLock: screen.allowedOrientations

            Flickable {
                id: testPageGroup

                anchors.fill: parent
                anchors.topMargin: memoryDisplay.visible ? memoryDisplay.height : 0
                contentWidth: width
                contentHeight: 1.3 * screen.height - (memoryDisplay.visible ? memoryDisplay.height : 0)
                flickableDirection:Flickable.VerticalFlick

                Loader {
                    id: loader

                    width: testPageGroup.contentWidth
                    height: testPageGroup.contentHeight
                    visible: loader.status !== Loader.Error
                    source: internal.resolveSource()
                    onLoaded: Utils.setItemTreeInversion(loader, mainWindow.platformInverted)
                }
            }
        }
    }// flickableTestComponent

    Component {
        id: testComponent

        Page {
            id: testPage
            orientationLock: screen.allowedOrientations

            Rectangle {
                anchors { left: parent.left; top: parent.top }
                anchors.topMargin: memoryDisplay.visible ? memoryDisplay.height : 0
                width: if (internal.fillEnabled) parent.width
                height: if (internal.fillEnabled) parent.height - (memoryDisplay.visible ? memoryDisplay.height : 0)
                color: "transparent"
                border.color: "steelblue"
                border.width: internal.dragEnabled ? 2 : 0

                property int dragStartX
                property int dragStartY
                property int dragStartWidth
                property int dragStartHeight

                MouseArea {
                    anchors.fill: parent

                    onPressed: {
                        parent.dragStartX = mouse.x
                        parent.dragStartY = mouse.y
                        parent.dragStartWidth = parent.width
                        parent.dragStartHeight = parent.height
                    }

                    onPositionChanged: {
                        parent.width = parent.dragStartWidth + mouse.x - parent.dragStartX
                        parent.height = parent.dragStartHeight + mouse.y - parent.dragStartY
                    }

                    visible: internal.dragEnabled
                }
                Loader {
                    id: loader

                    anchors { left: parent.left; top: parent.top }
                    width: parent.width
                    height: parent.height
                    focus: true
                    visible: loader.status !== Loader.Error
                    source: internal.resolveSource()
                    onLoaded: Utils.setItemTreeInversion(loader, mainWindow.platformInverted)
                }// Loader
            }
        }
    } // testComponent

    Component {
        id: memToolsMenuComponent
        Menu {
            content: MenuLayout {
                MenuItem { text: "Set layout dir"; platformSubItemIndicator: true; onClicked: layoutDirectionSubMenu.open() }
                MenuItem { text: "Clear clipboard"; onClicked: { settings.clearClipboard() } }
                MenuItem { text: "Clear icon caches"; onClicked: symbian.privateClearIconCaches() }
                MenuItem { text: "Clear component cache"; onClicked: symbian.privateClearComponentCache() }
                MenuItem { text: "Run garbage collector"; onClicked: gc() }
                MenuItem { text: "Toggle memory display"; onClicked: { memoryDisplay.visible = !memoryDisplay.visible } }
            }
        }
    }

    Menu {
        id: layoutDirectionSubMenu

        MenuLayout {
            MenuItem { text: "LeftToRight"; onClicked: layoutDirectionSetter.setLayoutDirection(Qt.LeftToRight) }
            MenuItem { text: "RightToLeft"; onClicked: layoutDirectionSetter.setLayoutDirection(Qt.RightToLeft) }
            MenuItem { text: "Automatic"; onClicked: layoutDirectionSetter.setLayoutDirection(Qt.LayoutDirectionAuto) }
        }
    }
}
