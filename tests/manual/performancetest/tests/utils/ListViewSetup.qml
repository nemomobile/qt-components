/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
    anchors.fill: parent
    property int itemCount: 2000
    property int sectionInterval: 0
    property string heading: "Test"
    property alias cacheBuffer: listView.cacheBuffer

    Rectangle {
        anchors.fill: parent
        color: "Black"
    }

    ListView {
        id: listView
        anchors.fill: parent
        focus: true
        clip: true
        model: ListModel { id: model }
        delegate: textDelegate
        header: heading != "" ? listHeader : null
        section.property: sectionInterval > 0 ? "sectionIdentifier" : ""
        section.delegate: sectionInterval > 0 ? sectionDelegate : null
    }

    ScrollBar {
        flickableItem: listView
        anchors { top: listView.top; right: listView.right }
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

    ObjectMenu {
           id: objectmenu
           actions: ["Set disabled", "Add item", "Delete item"]
           onTriggered: {
               switch (index) {
                   case 0: {
                       // Switch state
                       listView.model.set(listView.currentIndex, {"disabled": true})
                       break
                   }
                   case 1: {
                       createItemDialog.open()
                       break
                   }
                   case 2: {
                       if (listView.currentIndex >= 0)
                           listView.model.remove(listView.currentIndex)
                       break
                   }
                   default:
                       break
               }
           }
       }

    Dialog {
        id: createItemDialog
        title: Text {
            text: "Create new list item"
            font { bold: true; pixelSize: 16 }
            color: "white"
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        buttons: Row {
            height: 60
            width: parent.width

            Button {
                text: "Ok"
                width: parent.width / 2
                height: parent.height
                onClicked: createItemDialog.accept()
            }

            Button {
                text: "Cancel"
                width: parent.width / 2
                height: parent.height
                onClicked: createItemDialog.reject()
            }
        }
        content: Column {
            anchors.fill: parent

            Text {
                text: "Enter title"
            }

            TextField {
                id: titleField
                text: "New item - Title"
                width: parent.width
            }

            Text {
                text: "Enter subtitle"
            }

            TextField {
                id: subTitleField
                text: "New item - SubTitle"
                width: parent.width
            }

            Text {
                text: "Select image size"
            }

            ChoiceList {
                id: imageSizeChoiceList
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - parent.spacing
                currentIndex: 0
                model: ["Undefined", "Small", "Medium", "Large", "ImagePortrait"]
            }
        }
        onAccepted: {
            listView.model.insert(listView.currentIndex + 1, {
                "title": titleField.text,
                "subTitle": subTitleField.text,
                "imageSize": imageSizeChoiceList.currentIndex,
                "image": imageSizeChoiceList.currentIndex > 0 ? "image://theme/:/list1.png" : "",
                "disabled": false,
                "selected": false,
                "sectionIdentifier": 0} )
        }
    }

    Component {
        id: listHeader

        ListHeading {
            id: listHeader
            width: listView.width

            ListItemText {
                id: txtHeading
                anchors.fill: listHeader.padding
                style: listHeader.style
                role: "Heading"
                text: root.heading
            }
        }
    }

    Component {
        id: sectionDelegate

        ListHeading {
            width: listView.width
            id: sectionHeader

            ListItemText {
                id: txtHeading
                anchors.fill: sectionHeader.padding
                style: sectionHeader.style
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
            enabled: !disabled // State from model

            Column {
                anchors.fill: parent.padding
                spacing: parent.verticalSpacing

                ListItemText {
                    style: listItem2.style
                    role: "Title"
                    text: title // Title from model
                }

                ListItemText {
                    style: listItem2.style
                    role: "SubTitle"
                    text: subTitle // SubTitle from model
                }
            }
            onPressAndHold: objectmenu.open()
        }
    }

    Component {
        id: imageDelegate

        ListItem {
            width: listView.width
            enabled: !disabled // State from model

            Image {
                anchors { left: parent.padding.left; top: parent.padding.top }
                sourceSize.height: parent.preferredImageHeight(imageSize)
                sourceSize.width: parent.preferredImageWidth(imageSize)
                source: image
            }
            onPressAndHold: objectmenu.open()
        }
    }

    Component {
        id: imageAndTextDelegate

        ListItem {
            id: listItem3
            objectName: title
            width: listView.width
            enabled: !disabled // State from model

            Row {
                anchors.fill: listItem3.padding
                spacing: listItem3.horizontalSpacing

                Image {
                    sourceSize.height: listItem3.preferredImageHeight(imageSize)
                    sourceSize.width: listItem3.preferredImageWidth(imageSize)
                    source: image
                }

                Column {
                    spacing: listItem3.verticalSpacing

                    ListItemText {
                        style: listItem3.style
                        role: "Title"
                        text: title // Title from model
                    }

                    ListItemText {
                        style: listItem3.style
                        role: "SubTitle"
                        text: subTitle // SubTitle from model
                    }
                }
            }
            onPressAndHold: objectmenu.open()
        }
    }

    Component {
        id: selectableDelegate

        ListItem {
            id: listItem4
            objectName: title
            width: listView.width
            enabled: !disabled // State from model

            Row {
                anchors.fill: listItem4.padding
                spacing: listItem4.horizontalSpacing

                Image {
                    sourceSize.height: listItem4.preferredImageHeight(imageSize)
                    sourceSize.width: listItem4.preferredImageWidth(imageSize)
                    source: image
                }

                Column {
                    spacing: listItem4.verticalSpacing

                    ListItemText {
                        style: listItem4.style
                        role: "Title"
                        text: title // Title from model
                    }

                    ListItemText {
                        style: listItem4.style
                        role: "SubTitle"
                        text: subTitle // SubTitle from model
                    }
                }
            }

            CheckBox {
                id: checkbox
                enabled: listItem4.enabled
                checked: selected
                anchors { right: listItem4.padding.right; verticalCenter: listItem4.verticalCenter }
                onClicked: listView.model.set(index, { "selected": checkbox.checked })
            }
            onClicked: {
                checkbox.checked = !checkbox.checked
                listView.model.set(index, { "selected": checkbox.checked })
            }
            onPressAndHold: objectmenu.open()
        }
    }


    function initializeModel() {
        listView.model.clear()
        for (var i = 0; i < itemCount; i++) {
            listView.model.append( {
                "title": "Title text - " + i,
                "subTitle": i,
                "imageSize": Symbian.Large,
                "image": "image://theme/:/list1.png",
                "disabled": false,
                "selected": false,
                "sectionIdentifier": Math.floor(i/sectionInterval)
            } )
        }
    }
}

