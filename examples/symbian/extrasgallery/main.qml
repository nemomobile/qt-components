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

import Qt 4.7
import com.nokia.symbian 1.0
import com.nokia.extras 1.0

ApplicationWindow {
    id: root

    ToolBarLayout {
        id: commonTools
        ToolButton {
            flat: true
            iconSource: "image://theme/qtg_toolbar_back"
            onClicked: root.pageStack.depth <= 1 ? Qt.quit() : root.pageStack.pop()
        }
        ToolButton {
            flat: true
            iconSource: "image://theme/qtg_toolbar_options"
        }
    }

    Component.onCompleted: {
        root.pageStack.push(pageComponent)
        // clear the toolBar pointer, prevents subpages from
        // accidentally removing common application tools
        root.pageStack.toolBar = null
    }

    Component {
        id: pageComponent

        Page {
            id: testPage
            tools: commonTools

            Item {
                id: testList
                anchors.fill: parent

                function openFile(file) {
                    var component = Qt.createComponent(file)

                    if (component.status == Component.Ready) {
                        root.pageStack.push(component);
                        console.log("Loading component okay");
                    }
                    else {
                        console.log("Error loading component:", component.errorString());
                    }
                }

                ListModel {
                    id: pagesModel

                    ListElement {
                        page: "RatingIndicator.qml"
                        title: "RatingIndicator"
                        subtitle: "Indicates ratings"
                    }
                    ListElement {
                        page: "Tumbler.qml"
                        title: "Tumbler"
                        subtitle: "Show tumbler"
                    }
                    ListElement {
                        page: "TumblerDialog.qml"
                        title: "TumblerDialog"
                        subtitle: "Show tumbler dialog"
                    }
                    ListElement {
                        page: "DatePickerDialog.qml"
                        title: "DatePickerDialog"
                        subtitle: "Show date picker dialog"
                    }
                    ListElement {
                        page: "TimePickerDialog.qml"
                        title: "TimePickerDialog"
                        subtitle: "Show time picker dialog"
                    }
                }

                ListView {
                    id: list
                    model: pagesModel
                    anchors.fill: parent

                    delegate: ListItem {
                        id: listItem
                        height: 68

                        Row {
                            id: listItemRow
                            anchors.fill: parent
                            anchors.leftMargin: 18

                            Column {
                                anchors.verticalCenter: parent.verticalCenter

                                ListItemText {
                                    id: mainText
                                    style: listItem.style
                                    role: "Title"
                                    text: title
                                }

                                ListItemText {
                                    id: subText
                                    style: listItem.style
                                    role: "SubTitle"
                                    text: subtitle
                                    visible: text != ""
                                }
                            }
                        }

                        onClicked: { testList.openFile(page); }
                    } // listItem
                } // listView

                ScrollDecorator {
                    flickableItem: list
                }
            } // item
        } // page
    } // component
}
