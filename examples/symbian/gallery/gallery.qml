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

Window {
    id: root

    property Menu menu

    StatusBar {
        id: statusBar
        anchors.top: root.top
    }

    Flickable {
        id: flickable
        clip: true
        anchors { 
            left: root.left
            right: root.right
            top: statusBar.visible ? statusBar.bottom: root.top
            bottom: toolBar.visible ? toolBar.top: root.bottom
        }
        contentHeight: column.height

        SampleColumn {
            id: column
            anchors {
                left: parent.left
                right: parent.right
                margins: column.spacing
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    ToolBar {
        id: toolBar
        anchors.bottom: root.bottom
        tools: ToolBarLayout {
            id: toolBarlayout
            ToolButton {
                flat: true
                iconSource: "image://theme/qtg_toolbar_back"
                onClicked: Qt.quit()
            }
            ToolButton {
                iconSource: "image://theme/qtg_toolbar_options"
                onClicked: {
                    if (!menu)
                        menu = menuComponent.createObject(root)
                    menu.open()
                }
            }
        }
    }

    Component {
        id: menuComponent
        Menu {
            content: MenuLayout {
                MenuItem {
                    text: column.enabled ? "Disable" : "Enable"
                    onClicked: column.enabled = !column.enabled
                }
                MenuItem { text: "Quit"; onClicked: Qt.quit() }
            }
        }
    }
}
