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
import FileAccess 1.0
import Settings 1.0

ApplicationWindow {
    id: mainWindow

    objectName: "mainWindow"

    property Menu memoryToolsMenu

    // For TDriver tests - setting component name to this property will open the corresponding
    // component page "automatically"
    property string componentName

    FileAccess {
        id: fileAccess
    }

    Settings {
        id: settings
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

    Loader {
        id: memoryDisplay
        visible: false
        source: visible ? "qrc:/MemoryDisplay.qml" : ""
    }

    ToolBarLayout {
        id: commonTools
        ToolButton {
            flat: true
            iconSource: "image://theme/qtg_toolbar_back"
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
            iconSource: "image://theme/qtg_toolbar_options"
        }
    }

    Component.onCompleted: {
        internal.qmlPaths = fileAccess.qmlPaths()
        screen.allowedOrientations = settings.orientation()
        mainWindow.pageStack.push(component)
        // clear the toolBar pointer, prevents subpages from
        // accidentally removing common application tools
        mainWindow.pageStack.toolBar = null
    }

    Component {
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
                    }

                    Button {
                        id: toggleFullscreenButton

                        width: mainWindow.width
                        text: "Toggle Fullscreen"
                        onClicked: mainWindow.fullScreen = mainWindow.fullScreen ? false : true
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
                            width: mainWindow.width

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

                                    Rectangle {
                                        anchors { left: parent.left; top: parent.top }
                                        width: parent.width
                                        height: parent.height
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
                MenuItem { text: "Clear icon caches"; onClicked: symbian.privateClearIconCaches() }
                MenuItem { text: "Clear component cache"; onClicked: symbian.privateClearComponentCache() }
                MenuItem { text: "Run garbage collector"; onClicked: gc() }
                MenuItem { text: "Toggle memory display"; onClicked: { memoryDisplay.visible = !memoryDisplay.visible }}
            }
        }
    }
}
