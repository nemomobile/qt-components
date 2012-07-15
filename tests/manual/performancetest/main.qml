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

PageStackWindow {
    id: mainWindow

    objectName: "mainWindow"

    property Menu memoryToolsMenu
    property bool displayMemory: false

    // For TDriver tests - setting component name to this property will open the corresponding
    // component page "automatically"
    property string componentName

    LayoutMirroring.enabled: Qt.application.layoutDirection == Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

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

        // Directories from where qmlfiles can be loaded
        property variant qmlPaths: []
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
                if (!memoryToolsMenu)
                    memoryToolsMenu = memToolsMenuComponent.createObject(mainWindow)
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

    initialPage: Component {
        id: component

        Page {
            id: mainPage
            tools: commonTools
            orientationLock: PageOrientation.Manual

            anchors.fill: parent

            function resolveSource() {
                // All tests are under tests folder
                var file = buttons.checkedButton.text.replace(" ", "") + ".qml"
                return internal.testFilesPath + file
            }

            Loader {
                id: memoryDisplay
                visible: displayMemory
                source: visible ? "qrc:/MemoryDisplay.qml" : ""
            }

            Flickable {
                id: flickable

                anchors.fill: parent
                anchors.topMargin: memoryDisplay.visible ? memoryDisplay.height : 0
                contentHeight: buttons.height
                contentWidth: parent.width
                flickableDirection: Flickable.VerticalFlick
                clip: true

                Column {
                    id: buttons

                    width: parent.width

                    // Holds the text of the pressed button in the main view
                    property variant checkedButton: null

                    Row {
                        id: orientationButtons

                        property real buttonWidth: parent.width / children.length

                        Button {
                            id: portraitButton

                            text: "Port."
                            width: parent.buttonWidth

                            onClicked: {
                                screen.allowedOrientations = Screen.Portrait
                            }
                        }
                        Button {
                            id: landscapeButton

                            text: "Land."
                            width: parent.buttonWidth

                            onClicked: {
                                screen.allowedOrientations = Screen.Landscape
                            }
                        }
                        Button {
                            id: automaticOrientationButton

                            text: "Auto"
                            width: parent.buttonWidth

                            onClicked: {
                                screen.allowedOrientations = Screen.Default
                            }
                        }
                    }

                    Button {
                        id: toggleFullscreenButton

                        width: screen.width
                        text: "Toggle Fullscreen"
                        onClicked: {
                            mainWindow.showStatusBar = !mainWindow.showStatusBar
                            mainWindow.showToolBar = !mainWindow.showToolBar
                        }
                    }

                    Repeater {
                        id: componentsmodel

                        // Model is populated with the qml-files found from
                        // the tests folder
                        model: {
                            var components = new Array()
                            components = fileAccess.qmlFileNames(internal.testFilesPath)
                            return components
                        }

                        Button {
                            id: testButton

                            text: modelData
                            width: screen.width

                            onClicked: {
                                buttons.checkedButton = testButton
                                mainWindow.pageStack.push(testComponent)
                            }

                            // For TDriver ->
                            // Triggers the page openining based on the componentName property
                            Connections {
                                target: mainWindow
                                onComponentNameChanged: {
                                    if (componentName) {
                                        testButton.text = mainWindow.componentName
                                        buttons.checkedButton  = testButton
                                        mainWindow.pageStack.push(testComponent)
                                    }
                                componentName = ""
                                }
                            }
                            // <- for TDriver

                            Component {
                                id: testComponent

                                Page {
                                    id: testPage

                                    orientationLock: PageOrientation.Automatic

                                    Loader {
                                        id: memoryDisplay
                                        visible: displayMemory
                                        source: visible ? "qrc:/MemoryDisplay.qml" : ""
                                    }

                                    Rectangle {
                                        anchors { left: parent.left; top: parent.top }
                                        anchors.topMargin: memoryDisplay.visible ? memoryDisplay.height : 0
                                        width: parent.width
                                        height: parent.height - (memoryDisplay.visible ? memoryDisplay.height : 0)
                                        color: "#1000FF00"

                                        Loader {
                                            id: loader
                                            anchors.fill: parent
                                            visible: loader.status !== Loader.Error
                                            source: mainPage.resolveSource()
                                        }// Loader
                                    }
                                }
                            } // testComponent
                        }
                   } // Repeater
                }
            }
        }
    } // main page component

    Component {
        id: memToolsMenuComponent
        Menu {
            content: MenuLayout {
                MenuItem { text: "Set layout dir"; platformSubItemIndicator: true; onClicked: layoutDirectionSubMenu.open() }
                MenuItem { text: "Clear icon caches"; onClicked: symbian.privateClearIconCaches() }
                MenuItem { text: "Clear component cache"; onClicked: symbian.privateClearComponentCache() }
                MenuItem { text: "Run garbage collector"; onClicked: gc() }
                MenuItem { text: "Toggle memory display"; onClicked: { displayMemory = !displayMemory } }
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
