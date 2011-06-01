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
import Qt.labs.components 1.0

Item {
    id: root
    anchors.fill: parent
    property variant imageSizes: ["Undefined", "Tiny", "Small", "Medium", "Large"]
    Connections {
        target: platformPopupManager
        onPopupStackDepthChanged: if (platformPopupManager.popupStackDepth == 0) listView.forceActiveFocus()
    }

    Column {
        anchors.fill: parent

        ListView {
            id: listView
            height: parent.height - statusBar.height
            width: parent.width
            focus: true
            clip: true
            model: ListModel { id: model }
            delegate: defaultDelegate
            Component.onCompleted: {
                 initializeDefault() // Initial fill of the model
             }
            ScrollBar {
                flickableItem: listView
                anchors { top: listView.top; right: listView.right }
            }
        }
        Rectangle {
            id: statusBar
            height: 40
            width: parent.width
            color: "darkgray"
            border.color: "gray"

            Text {
                font { bold: true; pixelSize: 16 }
                color: "white"
                anchors { left: parent.left; leftMargin: 10; verticalCenter: parent.verticalCenter }
                text: "Current item: " + listView.currentIndex
            }

            Button {
                text: "Menu"
                width: 100
                anchors { right: parent.right; rightMargin: 10; verticalCenter: parent.verticalCenter }
                onClicked: menu.open()
            }
        }
    }

    ContextMenu {
        id: contextMenu

        content: MenuLayout {
            MenuItem {
                text: "Set disabled"
                onClicked: {
                    listView.model.set(listView.currentIndex, {"disabled": true})
                }
            }
            MenuItem {
                text: "Toggle subitem indicator"
                onClicked: {
                    var indicatorState = listView.model.get(listView.currentIndex).indicator
                    listView.model.set(listView.currentIndex, {"indicator": !indicatorState})
                }
            }
            MenuItem {
                text: "Add item"
                onClicked: createItemDialog.open()
            }
            MenuItem {
                text: "Delete item"
                onClicked: {
                    if (listView.currentIndex >= 0)
                        listView.model.remove(listView.currentIndex)
                }
            }
        }
    }

    Menu {
        id: menu

        content: MenuLayout {
            MenuItem {
                text: "Show/Hide List header"
                onClicked: listView.header = listView.header ? null : listHeader
            }
            MenuItem {
                text: "Show/Hide Sections"
                onClicked: {
                    // Models field to group by
                    listView.section.property = "image"
                    // Delegate for section heading
                    listView.section.delegate = listView.section.delegate ? null : sectionDelegate
                }
            }
            MenuItem {
                text: "Back"
                onClicked: pageStack.pop()
            }
        }
    }

    Dialog {
        id: notificationDialog
        property string notificationText: ""
        title: Text {
            text: "Item clicked"
            font { bold: true; pixelSize: 16 }
            color: "white"
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        buttons: Button {
            text: "Ok"
            width: parent.width
            height: 50
            onClicked: notificationDialog.accept()
        }
        content: Text {
            text: notificationDialog.notificationText
            font { bold: true; pixelSize: 16 }
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Dialog {
        id: createItemDialog
        title: Text {
            text: "Create new list item"
            font.pixelSize: platformStyle.fontSizeMedium
            color: platformStyle.colorNormalLight
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
        content: Item {
            id: createItemContainer
            height: createItemGrid.height
            width: createItemDialog.platformContentMaximumWidth
            CheckableGroup { id: imageSizeGroup }

            Grid {
                id: createItemGrid
                columns: 2
                spacing: platformStyle.paddingSmall
                property int cellWidth: (createItemContainer.width / 2) - spacing

                Text {
                    id: titleLabel
                    color: platformStyle.colorNormalLight
                    font.family: platformStyle.fontFamilyRegular
                    font.pixelSize: platformStyle.fontSizeMedium
                    text: "Title:"
                    width: createItemGrid.cellWidth
                }

                TextField {
                    id: titleField
                    text: "New item - Title"
                    width: createItemGrid.cellWidth
                }

                Text {
                    id: subTitleLabel
                    color: platformStyle.colorNormalLight
                    font.family: platformStyle.fontFamilyRegular
                    font.pixelSize: platformStyle.fontSizeMedium
                    text: "Subtitle:"
                    width: createItemGrid.cellWidth
                }

                TextField {
                    id: subTitleField
                    text: "New item - SubTitle"
                    width: createItemGrid.cellWidth
                }

                Text {
                    id: imageSizeLabel
                    color: platformStyle.colorNormalLight
                    font.family: platformStyle.fontFamilyRegular
                    font.pixelSize: platformStyle.fontSizeMedium
                    text: "Image size:"
                    width: createItemGrid.cellWidth
                }

                Column {
                    spacing: platformStyle.paddingSmall
                    RadioButton {
                        text: root.imageSizes[0]
                        platformExclusiveGroup: imageSizeGroup
                        width: createItemGrid.cellWidth
                    }
                    RadioButton {
                        text: root.imageSizes[1]
                        platformExclusiveGroup: imageSizeGroup
                        width: createItemGrid.cellWidth
                    }
                    RadioButton {
                        text: root.imageSizes[2]
                        platformExclusiveGroup: imageSizeGroup
                        width: createItemGrid.cellWidth
                    }
                    RadioButton {
                        text: root.imageSizes[3]
                        platformExclusiveGroup: imageSizeGroup
                        width: createItemGrid.cellWidth
                    }
                    RadioButton {
                        text: root.imageSizes[4]
                        platformExclusiveGroup: imageSizeGroup
                        checked: true
                        width: createItemGrid.cellWidth
                    }
                }
            }
        }
        onAccepted: {
            listView.model.insert(listView.currentIndex + 1, {
                                  "title": titleField.text,
                                  "subTitle": subTitleField.text,
                                  // Fetch actual size in pixels based on index
                                  "imageSize": root.getSize(imageSizeGroup.selectedValue),
                                  "image": "image://theme/:/list1.svg",
                                  "disabled": false,
                                  "selected": false,
                                  "indicator": false
        } )
        }
    }

    function initializeDefault() {
        listView.header = listHeader
        listView.section.property = "image" // Models field to group by
        listView.section.delegate = sectionDelegate // Delegate for section headings
        for (var i = 0; i < 4; i++) {
            for (var j = 0; j < root.imageSizes.length; j++) {
                listView.model.append( {
                                      "title": "Title text - " + (5 * i + j),
                                      "subTitle": "SubTitle " + (5 * i + j),
                                      "imageSize": root.getSize(root.imageSizes[j]), // Fetch actual size in pixels based on index
                                      "image": "image://theme/:/list" + (i + 1) + ".svg",
                                      "disabled": false,
                                      "selected": false,
                                      "indicator": false
            } )
            }
        }
    }

    Component {
        id: defaultDelegate

        ListItem {
            id: listItem
            objectName: title
            width: listView.width
            enabled: !disabled // State from model
            subItemIndicator: indicator

            Image {
                id: imageItem
                anchors {
                    left: imageSize == platformStyle.graphicSizeLarge ? listItem.left : listItem.paddingItem.left
                    top: imageSize == platformStyle.graphicSizeLarge ? listItem.top : listItem.paddingItem.top
                }
                sourceSize.height: imageSize
                sourceSize.width: imageSize
                source: imageSize == 0 ? "" : image
            }
            Column {
                anchors {
                    top: listItem.paddingItem.top
                    left: imageItem.right
                    leftMargin: platformStyle.paddingMedium
                    right: listItem.paddingItem.right
                }

                ListItemText {
                    mode: listItem.mode
                    role: "Title"
                    text: title // Title from model
                }

                ListItemText {
                    mode: listItem.mode
                    role: "SubTitle"
                    text: subTitle // SubTitle from model
                }
            }

            onClicked: {
                notificationDialog.notificationText = "Activated item: " + title
                notificationDialog.open()
            }
            onPressAndHold: contextMenu.open()

            Keys.onPressed: {
                if (event.key == Qt.Key_Backspace ||event.key == Qt.Key_Delete) {
                    if (listView.currentIndex >= 0)
                        listView.model.remove(listView.currentIndex)
                }
            }
        }
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
                text: "Test list"
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
                anchors.fill: sectionHeader.paddingItem
                role: "Heading"
                text: "Section: " + section
            }
        }
    }

    function getSize(size) {
        switch (size) {
            case "Undefined": return 0; break
            case "Tiny": return platformStyle.graphicSizeTiny; break
            case "Small": return platformStyle.graphicSizeSmall; break
            case "Medium": return platformStyle.graphicSizeMedium; break
            case "Large": return platformStyle.graphicSizeLarge; break
            default: return 0
        }
    }
}
