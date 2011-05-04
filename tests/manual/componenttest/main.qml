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

        // Diretories from where qmlfiles can be loaded
        property variant qmlPaths: []
    }

    ToolBarLayout {
        id: commonTools
        ToolButton {
            flat: true
            iconSource: "image://theme/qtg_toolbar_back"
            onClicked: pageStack.depth <= 1 ? Qt.quit() : pageStack.pop()
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
                if (buttons.checkedButton
                    && buttons.checkedButton.text == "Load chosen file"
                    && internal.fullPath.length > 0)
                    return "file:///" + internal.fullPath
                else if (buttons.checkedButton
                         && buttons.checkedButton.text != "Load chosen file") {
                    // All tests are under tests folder
                    var file = buttons.checkedButton.text.replace(" ", "") + ".qml"
                    return internal.testFilesPath + file
                } else
                    return ""
            }

            onStatusChanged: {
                if (status == PageStatus.Activating)
                    screen.allowedOrientations = startupOrientationButton.orientation
            }

            Flickable {
                id: flickable

                anchors.fill: parent
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
                        id: pickFromFileButton

                        width: mainWindow.width
                        text: {
                            // Append qmlpaths to the button label
                            var buttonLabel = "Choose a file from "
                            for (var i = 0; i < internal.qmlPaths.length; i++) {
                                buttonLabel += internal.qmlPaths[i];
                                if (!((i+1) == internal.qmlPaths.length))
                                    buttonLabel += " or "
                            }
                            return buttonLabel
                        }

                        onClicked: mainWindow.pageStack.push(listPageComponent)
                    }
                    Text {
                        id: currentFileLabel

                        width: mainWindow.width
                        color: "red"
                        text: "Picked file: " + internal.fullPath
                        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall }
                    }

                    Button {
                        id: toggleFullscreenButton

                        width: mainWindow.width
                        text: "Toggle Fullscreen"
                        onClicked: mainWindow.fullScreen = mainWindow.fullScreen ? false : true
                    }
                    CheckBox {
                        id: flickableSetting
                        text: "Flickable"
                        checked: false
                    }
                    CheckBox {
                        id: dragSetting
                        text: "Drag-able"
                        checked: false
                    }
                    CheckBox {
                        id: fillSetting
                        text: "Fill area"
                        checked: true
                    }
                    Repeater {
                        id: componentsmodel

                        // Model is populated with the qml-files found from
                        // the tests folder
                        model: {
                            var entry = new Array("Load chosen file")
                            var components = new Array()
                            components = fileAccess.qmlFileNames(internal.testFilesPath)
                            return entry.concat(components)
                        }

                        Button {
                            id: testButton

                            text: modelData
                            width: mainWindow.width

                            onClicked: {
                                buttons.checkedButton = testButton
                                if (flickableSetting.checked == true)
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
                                        buttons.checkedButton  = testButton
                                        if (flickableSetting.checked)
                                            mainWindow.pageStack.push(flickableTestComponent, null, true)
                                        else
                                            mainWindow.pageStack.push(testComponent, null, true)
                                    }
                                componentName = ""
                                }
                            }
                            // <- for TDriver

                            Component {
                                id: flickableTestComponent

                                Page {
                                    id: testPage
                                    orientationLock: PageOrientation.Automatic

                                    Flickable {
                                        id: testPageGroup

                                        anchors.fill: parent
                                        contentWidth: width
                                        contentHeight: 1.2 * screen.height
                                        flickableDirection:Flickable.AutoFlickDirection

                                        Loader {
                                            id: loader

                                            width: testPageGroup.contentWidth
                                            height: testPageGroup.contentHeight
                                            visible: loader.status !== Loader.Error
                                            source: mainPage.resolveSource()
                                        }
                                    }
                                }
                            }// flickableTestComponent
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

                                            visible: dragSetting.checked
                                        }
                                        Loader {
                                            id: loader

                                            anchors { left: parent.left; top: parent.top }
                                            width: if (fillSetting.checked) parent.width
                                            height: if (fillSetting.checked) parent.height
                                            focus: true
                                            visible: loader.status !== Loader.Error
                                            source: mainPage.resolveSource()
                                        }// Loader
                                    }
                                }
                            } // testComponent
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
    Component {
        id: listPageComponent

        Page {
            id: listPage
            ListModel {
                id: testfileModel
            }

            // fill the model based on files in C or E-drives
            Component.onCompleted: {
                console.log("listpage completed")
                var fileNames = new Array()
                var fileFullPaths = new Array()
                for (var i = 0; i < internal.qmlPaths.length; i++) {
                    fileNames = fileAccess.qmlFileNames(internal.qmlPaths[i])
                    fileFullPaths = fileAccess.qmlFilePaths(internal.qmlPaths[i])
                    for (var j = 0; j < fileNames.length; j++) {
                       console.log("adding file from disk: " + fileNames[j])
                        testfileModel.append({"fileName": fileNames[j] + ".qml", "fileFullPath": fileFullPaths[j]})
                    }
                }
            }

            ListView {
                id: listView

                model: testfileModel
                pressDelay: 50
                anchors.fill: parent
                delegate:
                ListItem {
                    onClicked: {
                        internal.fullPath = listTextId.fullPath
                        mainWindow.pageStack.pop(listPageComponent)
                    }
                    ListItemText {
                        id: listTextId

                        property string fullPath: fileFullPath

                        text: fileName
                    }
                }
            }// listView
        }// listPage
    }// listPageComponent
}
