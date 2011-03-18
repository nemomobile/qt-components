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

Dialog {
    id: root
    property string fontFamily

    title: Text {
        text: "Select font"
        font { bold: true; pixelSize: 16 }
        color: "white"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    content: ListView {
        id: listView
        anchors.fill: parent
        focus: true
        clip: true
        model: Qt.fontFamilies()
        delegate: defaultDelegate
    }

    Component {
        id: defaultDelegate

        ListItem {
            id: listItem
            width: listView.width
            height: 30

            Text {
                id: listItemText
                text: modelData
                font.family: modelData
                color: textStyle.current.get("textColor")
                Style {
                    id: textStyle
                    styleClass: "ListItemText"
                    mode: listItem.style.mode
                    styleObjectName: "Title"
                }
            }

            onClicked: {
                fontFamily = modelData
                root.close()
            }
        }
    }

    buttons: Row {
        height: 60
        width: parent.width

        Button {
            text: "Cancel"
            width: parent.width / 2
            height: parent.height
            x: parent.width / 2
            onClicked: root.close()
        }
    }
}
