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
            color: "black"
            height: text1.height + 2 * platformStyle.paddingMedium
            width: listView.width
            Rectangle {
                color: platformStyle.colorNormalLight
                opacity: 0.3
                anchors.fill: text1
            }
            Text {
                id: text1
                anchors.left: parent.left
                anchors.leftMargin: platformStyle.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                text: m_text
                color: platformStyle.colorNormalLight
                font { family: platformStyle.fontFamilyRegular; pixelSize: m_size; weight: m_weight }
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: platformStyle.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                text: m_size + " (" + height + ")"
                color: platformStyle.colorNormalLight
                font: text1.font
            }
        }
    }

    function initializeModel() {
        listModel.append( {
            "m_size": platformStyle.fontSizeLarge,
            "m_weight": Font.Normal,
            "m_text": "Large, Normal"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeLarge,
            "m_weight": Font.DemiBold,
            "m_text": "Large, DemiBold"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeLarge,
            "m_weight": Font.DemiBold,
            "m_text": "Large, Bold"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeMedium,
            "m_weight": Font.Normal,
            "m_text": "Medium, Normal"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeMedium,
            "m_weight": Font.DemiBold,
            "m_text": "Medium, DemiBold"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeMedium,
            "m_weight": Font.DemiBold,
            "m_text": "Medium, Bold"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeSmall,
            "m_weight": Font.Normal,
            "m_text": "Small, Normal"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeSmall,
            "m_weight": Font.DemiBold,
            "m_text": "Small, DemiBold"
        } )
        listModel.append( {
            "m_size": platformStyle.fontSizeSmall,
            "m_weight": Font.DemiBold,
            "m_text": "Small, Bold"
        } )
    }

    Connections {
        target: platformStyle
        onLayoutParametersChanged: {
            if ( listModel.count ) {
                listModel.setProperty(0, "m_size", platformStyle.fontSizeLarge)
                listModel.setProperty(1, "m_size", platformStyle.fontSizeLarge)
                listModel.setProperty(2, "m_size", platformStyle.fontSizeLarge)
                listModel.setProperty(3, "m_size", platformStyle.fontSizeMedium)
                listModel.setProperty(4, "m_size", platformStyle.fontSizeMedium)
                listModel.setProperty(5, "m_size", platformStyle.fontSizeMedium)
                listModel.setProperty(6, "m_size", platformStyle.fontSizeSmall)
                listModel.setProperty(7, "m_size", platformStyle.fontSizeSmall)
                listModel.setProperty(8, "m_size", platformStyle.fontSizeSmall)
            }
        }
    }
}
