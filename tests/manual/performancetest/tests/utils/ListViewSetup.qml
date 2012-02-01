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

Item {
    id: root
    anchors.fill: parent
    property int testId: 0
    property int itemCount: 2000
    property int sectionInterval: 0
    property string heading: "Test"
    property alias cacheBuffer: listView.cacheBuffer
    property bool scrollBar: false
    // For TDriver ->
    // Triggers the InitializeModel()
    property bool reInitModel: false

    Component.onCompleted: setupTest(testId)
    onReInitModelChanged: initializeModel()

    Loader {
        id: memoryDisplay
        visible: displayMemory
        source: visible ? "qrc:/MemoryDisplay.qml" : ""
    }

    BusyIndicator {
        id: busyIndicator
        visible: !listModel.initialized
        running: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: 150
        width: 150
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: memoryDisplay.visible ? memoryDisplay.height : 0
        opacity: !listModel.initialized ? 0 : 1
        focus: true
        clip: true
        model: ListModel { id: model }
        delegate: textDelegate
        header: heading != "" ? listHeader : null
        section.property: sectionInterval > 0 ? "sectionIdentifier" : ""
        section.delegate: sectionInterval > 0 ? sectionDelegate : null
    }

    // Creates Scrollbar dynamically based on state of scrollBar flag
    Loader {
        sourceComponent: root.scrollBar ? scrollBar : undefined
        anchors { top: listView.top; right: listView.right }
    }

    Component {
        id: scrollBar
        ScrollBar {
            flickableItem: listView
        }
    }

    function setupTest(testid) {
        switch (testid) {
            case 0: {
                root.heading = "ListItems with text"
                listView.delegate = textDelegate
                break
            }
            case 1: {
                root.heading = "ListItems with image"
                listView.delegate = imageDelegate
                break
            }
            case 2: {
                root.heading = "ListItems with image and text"
                listView.delegate = imageAndTextDelegate
                break
            }
            case 3: {
                root.heading = "ListView with multiselection"
                listView.delegate = selectableDelegate
                break
            }
            case 4: {
                root.heading = "ListView with sections"
                root.sectionInterval = 10
                listView.delegate = imageAndTextDelegate
                break
            }
            default:
                console.log("Invalid test setup")
        }
        initializeModel()
    }

    Component {
        id: listHeader

        ListHeading {
            id: listHeader
            width: listView.width

            ListItemText {
                id: txtHeading
                anchors.fill: listHeader.paddingItem
                role: "Heading"
                text: root.heading
            }
        }
    }

    Component {
        id: sectionDelegate

        ListHeading {
            id: sectionHeader
            width: listView.width

            ListItemText {
                id: txtHeading
                anchors.fill: sectionHeader.paddingItem
                role: "Heading"
                text: "Section separator"
            }
        }
    }


    Component {
        id: textDelegate

        ListItem {
            id: listItem2
            objectName: title
            width: listView.width

            Column {
                anchors.fill: parent.paddingItem

                ListItemText {
                    width: parent.width
                    mode: listItem2.mode
                    role: "Title"
                    text: title // Title from model
                }

                ListItemText {
                    width: parent.width
                    mode: listItem2.mode
                    role: "SubTitle"
                    text: subTitle // SubTitle from model
                }
            }
        }
    }

    Component {
        id: imageDelegate

        ListItem {
            width: listView.width

            Image {
                anchors {
                    left: imageSize == platformStyle.graphicSizeLarge ? parent.left : parent.paddingItem.left
                    top: imageSize == platformStyle.graphicSizeLarge ? parent.top : parent.paddingItem.top
                }
                sourceSize.height: imageSize
                sourceSize.width: imageSize
                source: image
            }
        }
    }

    Component {
        id: imageAndTextDelegate

        ListItem {
            id: listItem3
            objectName: title
            width: listView.width

            Image {
                id: imageItem
                anchors {
                    left: imageSize == platformStyle.graphicSizeLarge ? listItem3.left : listItem3.paddingItem.left
                    top: imageSize == platformStyle.graphicSizeLarge ? listItem3.top : listItem3.paddingItem.top
                }
                sourceSize.height: imageSize
                sourceSize.width: imageSize
                source: imageSize == 0 ? "" : image
            }
            Column {
                anchors {
                    top: listItem3.paddingItem.top
                    left: imageItem.right
                    leftMargin: platformStyle.paddingMedium
                    right: listItem3.paddingItem.right
                }

                ListItemText {
                    width: parent.width
                    mode: listItem3.mode
                    role: "Title"
                    text: title // Title from model
                }

                ListItemText {
                    width: parent.width
                    mode: listItem3.mode
                    role: "SubTitle"
                    text: subTitle // SubTitle from model
                }
            }
        }
    }

    Component {
        id: selectableDelegate

        ListItem {
            id: listItem4
            objectName: title
            width: listView.width

            Row {
                anchors.fill: listItem4.paddingItem
                spacing: platformStyle.paddingMedium

                Image {
                    sourceSize.height: imageSize
                    sourceSize.width: imageSize
                    source: image
                }

                Column {

                    ListItemText {
                        anchors.left: parent.left
                        mode: listItem4.mode
                        role: "Title"
                        text: title // Title from model
                    }

                    ListItemText {
                        anchors.left: parent.left
                        mode: listItem4.mode
                        role: "SubTitle"
                        text: subTitle // SubTitle from model
                    }
                }
            }

            CheckBox {
                id: checkbox
                enabled: listItem4.enabled
                checked: selected
                anchors { right: listItem4.paddingItem.right; verticalCenter: listItem4.verticalCenter }
                onClicked: listView.model.set(index, { "selected": checkbox.checked })
            }
            onClicked: {
                checkbox.checked = !checkbox.checked
                listView.model.set(index, { "selected": checkbox.checked })
            }
        }
    }


    ListModel {
        id: listModel
        property bool initialized: false
    }

    WorkerScript {
        id: worker
        source: "listmodelloader.js"
        onMessage: listModel.initialized = true
    }

    function initializeModel() {
        listModel.clear()
        listView.model = listModel
        var msg = { 'listModel': listModel,
            'imageSize': platformStyle.graphicSizeMedium,
            'image': "image://theme/:/list1.svg",
            'sectionInterval': sectionInterval,
            'itemCount': root.itemCount
            };
        worker.sendMessage(msg)
    }
}
