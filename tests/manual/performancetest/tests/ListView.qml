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

Item {
    id: root

    anchors.fill: parent
    property Component testPage: Qt.createComponent("utils/ListViewSetup.qml")

    ListView {
        id: listView
        anchors.fill: parent
        focus: true
        clip: true
        model: model
        delegate: delegate
    }

    ListModel {
        id: model
        ListElement {
            name: "ListView performance test 1"
            description: "ListItems with texts"
        }
        ListElement {
            name: "ListView performance test 2"
            description: "ListItems with image"
        }
        ListElement {
            name: "ListView performance test 3"
            description: "ListItems with image and texts"
        }
        ListElement {
            name: "ListView performance test 4"
            description: "ListView with multiselection"
        }
        ListElement {
            name: "ListView performance test 5"
            description: "ListView with sections"
        }
    }


    Component {
        id: delegate

        ListItem {
            id: listItem
            width: listView.width

            Column {
                anchors.fill: parent.paddingItem

                ListItemText {
                    mode: listItem.mode
                    role: "Title"
                    text: name // Title from model
                }

                ListItemText {
                    mode: listItem.mode
                    role: "SubTitle"
                    text: description // SubTitle from model
                }
            }
            onClicked: {
                if (testPage.status == Component.Ready) {
                    var page = testPage.createObject(root)
                    // Select test setup based on clicked item
                    page.setupTest(index)
                    mainWindow.pageStack.push(page)
                }
            }
        }
    }
}
