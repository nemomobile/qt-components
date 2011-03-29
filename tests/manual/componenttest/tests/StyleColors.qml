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

Item {
    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        model: ListModel { id: listModel }
        delegate: listDelegate
        Component.onCompleted: {
            initializeModel()
        }
        ScrollBar {
            flickableItem: listView
            anchors { top: listView.top; right: listView.right }
        }
    }
    Component {
        id: listDelegate
        Rectangle {
            color: m_inverse ? "white" : "black"
            height: text1.height + 2 * platformStyle.paddingLarge
            width: listView.width
            Text {
                id: text1
                anchors.left: parent.left
                anchors.leftMargin: platformStyle.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                text: m_text
                color: m_color
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
            }
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: platformStyle.paddingMedium
                width: height
                color: m_color
            }
        }
    }

    function initializeModel() {
        listView.model.append( {
            "m_color": platformStyle.colorNormalLight,
            "m_text": "colorNormalLight",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalMid,
            "m_text": "colorNormalMid",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalDark,
            "m_text": "colorNormalDark",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalLink,
            "m_text": "colorNormalLink",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorPressed,
            "m_text": "colorPressed",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorChecked,
            "m_text": "colorChecked",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorHighlighted,
            "m_text": "colorHighlighted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledLight,
            "m_text": "colorDisabledLight",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledMid,
            "m_text": "colorDisabledMid",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledDark,
            "m_text": "colorDisabledDark",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorTextSelection,
            "m_text": "colorTextSelection",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": privateStyle.listItemSeparatorColor,
            "m_text": "listItemSeparatorColor",
            "m_inverse": false
        } )
    }
}
