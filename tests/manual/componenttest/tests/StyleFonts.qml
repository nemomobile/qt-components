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
    property bool platformInverted: false
    property color labelColor: platformInverted ? platformStyle.colorNormalLightInverted
                                                : platformStyle.colorNormalLight

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
            color: root.platformInverted ? platformStyle.colorBackgroundInverted
                                         : platformStyle.colorBackground
            height: text1.height + 2 * platformStyle.paddingMedium
            width: listView.width
            Rectangle {
                color: root.labelColor
                opacity: 0.3
                anchors.fill: text1
            }
            Text {
                id: text1
                anchors.left: parent.left
                anchors.leftMargin: platformStyle.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                text: m_text
                color: root.labelColor
                font { family: platformStyle.fontFamilyRegular; pixelSize: m_size; weight: m_weight }
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: platformStyle.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                text: m_size + " (" + height + ")"
                color: root.labelColor
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
