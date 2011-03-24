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

Window {
    id: root

    property int rowSpacing: 24
    property int columnSpacing: 14

    StatusBar {
        id: statusBar
        anchors.top: parent.top
        width: parent.width
    }

    Flickable {
        id: flickable
        anchors { 
            left: parent.left
            right: parent.right
            top: statusBar.visible ? statusBar.bottom: parent.top
            bottom: toolBar.visible ? toolBar.top: parent.bottom
        }
        contentHeight: defaultColumn.height + 2 * root.rowSpacing
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        Component.onCompleted: {
            var cWidth = 0;
            for (var i = 0; i < content.children.length; ++i) {
                cWidth += content.children[i].width;
            }
            flickable.contentWidth = cWidth;
        }

        Row {
            id: content
            anchors.fill: parent

            Rectangle {
                width: defaultColumn.width + 2 * root.rowSpacing; height: defaultColumn.height + 2 * root.rowSpacing
                anchors.topMargin: root.rowSpacing

                SampleColumn {
                    id: defaultColumn
                    spacing: root.columnSpacing
                    anchors.centerIn: parent
                    title: "Default:"; titleColor: "#333"; titleStyleColor: "white"
                }
            }

            Item {
                width: disabledColumn.width + 2 * root.rowSpacing; height: disabledColumn.height + 2 * root.rowSpacing

                SampleColumn {
                    id: disabledColumn
                    enabled: false
                    spacing: root.columnSpacing
                    anchors.centerIn: parent
                    title: "Disabled:"; titleColor: "#333"; titleStyleColor: "white"
                }
            }

            Rectangle {
                width: coloredColumn.width + 2 * root.rowSpacing; height: coloredColumn.height + 2 * root.rowSpacing
                color: "#666"; border.color: "#444"

                SampleColumn {
                    id: coloredColumn
                    spacing: root.columnSpacing
                    anchors.centerIn: parent
                    title: "Colored:"; titleColor: "white"; titleStyleColor: "#333"
                }
            }

            Rectangle {
                width: customColumn.width + 2 * root.rowSpacing; height: customColumn.height + 2 * root.rowSpacing
                color: "#ccc"; border.color: "#444"

                SampleColumn {
                    id: customColumn
                    spacing: root.columnSpacing
                    anchors.centerIn: parent
                    title: "Custom:"; titleColor: "#333"; titleStyleColor: "white"
                }
            }
        }
    }

    ScrollDecorator {
        focus: true
        flickableItem: flickable
        anchors { top: flickable.top; right: flickable.right }
    }

    ToolBar {
        id: toolBar
        tools: toolBarlayout
        anchors.bottom: parent.bottom
    }

    Component {
        id: textButton
        ToolButton {
            text:"button"
        }
    }

    ToolBarLayout {
        id: toolBarlayout
        ToolButton {
            flat: true
            iconSource: "qrc:tb_back.svg"
            onClicked: Qt.quit()
        }

        ToolButton {
            iconSource: "qrc:tb_plus.png"
            onClicked: {
                if (toolBarlayout.children.length >= 4){
                    for (var p = 1; p < toolBarlayout.children.length - 1; ++p){
                        toolBarlayout.children[p].destroy()
                    }
                }
                else {
                    var button = textButton.createObject(null)
                    var len = toolBarlayout.children.length
                    var temp = toolBarlayout.children[len-1]
                    temp.parent = null
                    button.parent = toolBarlayout
                    temp.parent = toolBarlayout
                }
            }
        }
    }
}

