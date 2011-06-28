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
import "../components"

Item {
    id: root
    property bool platformInverted: false

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
            border.color: "blue"
            border.width: 2
            color: root.platformInverted ? platformStyle.colorBackgroundInverted
                                         : platformStyle.colorBackground
            height: text1.height + m_size + 2 * platformStyle.paddingLarge + platformStyle.paddingMedium
            width: listView.width
            Label {
                id: text1
                anchors.left: parent.left
                anchors.leftMargin: platformStyle.paddingLarge
                anchors.top: parent.top
                anchors.topMargin: platformStyle.paddingLarge
                text: m_text
                platformInverted: root.platformInverted
            }
            Label {
                anchors.right: parent.right
                anchors.rightMargin: platformStyle.paddingLarge
                anchors.top: parent.top
                anchors.topMargin: platformStyle.paddingLarge
                text: m_size
                platformInverted: root.platformInverted
            }
            Rectangle {
                width: {
                    if (m_type == 0)
                        m_size
                    else if (m_type == 1)
                        2
                    else
                        parent.width - 2 * platformStyle.paddingLarge
                }
                height: m_size
                color: m_color
                anchors.left: parent.left
                anchors.leftMargin: platformStyle.paddingLarge
                anchors.top: text1.bottom
                anchors.topMargin: platformStyle.paddingMedium
            }
        }
    }

    function initializeModel() {
        listModel.append( {
            "m_size": platformStyle.graphicSizeLarge,
            "m_text": "graphicSizeLarge",
            "m_color": "red",
            "m_type": 0
        } )
        listModel.append( {
            "m_size": platformStyle.graphicSizeMedium,
            "m_text": "graphicSizeMedium",
            "m_color": "red",
            "m_type": 0
        } )
        listModel.append( {
            "m_size": platformStyle.graphicSizeSmall,
            "m_text": "graphicSizeSmall",
            "m_color": "red",
            "m_type": 0
        } )
        listModel.append( {
            "m_size": platformStyle.graphicSizeTiny,
            "m_text": "graphicSizeTiny",
            "m_color": "red",
            "m_type": 0
        } )
        listModel.append( {
            "m_size": platformStyle.paddingLarge,
            "m_text": "paddingLarge",
            "m_color": "orange",
            "m_type": 1
        } )
        listModel.append( {
            "m_size": platformStyle.paddingMedium,
            "m_text": "paddingMedium",
            "m_color": "orange",
            "m_type": 1
        } )
        listModel.append( {
            "m_size": platformStyle.paddingSmall,
            "m_text": "paddingSmall",
            "m_color": "orange",
            "m_type": 1
        } )
        listModel.append( {
            "m_size": platformStyle.borderSizeMedium,
            "m_text": "borderSizeMedium",
            "m_color": "yellow",
            "m_type": 1
        } )
        listModel.append( {
            "m_size": privateStyle.statusBarHeight,
            "m_text": "statusBarHeight",
            "m_color": "yellowgreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.tabBarHeightPortrait,
            "m_text": "tabBarHeightPortrait",
            "m_color": "yellowgreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.tabBarHeightLandscape,
            "m_text": "tabBarHeightLandscape",
            "m_color": "yellowgreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.toolBarHeightPortrait,
            "m_text": "toolBarHeightPortrait",
            "m_color": "yellowgreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.toolBarHeightLandscape,
            "m_text": "toolBarHeightLandscape",
            "m_color": "yellowgreen",
            "m_type": 2
        } )

        listModel.append( {
            "m_size": privateStyle.scrollBarThickness,
            "m_text": "scrollBarThickness",
            "m_color": "green",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.sliderThickness,
            "m_text": "sliderThickness",
            "m_color": "green",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.menuItemHeight,
            "m_text": "menuItemHeight",
            "m_color": "darkgreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.textFieldHeight,
            "m_text": "textFieldHeight",
            "m_color": "darkgreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.switchButtonHeight,
            "m_text": "switchButtonHeight",
            "m_color": "darkgreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.buttonSize,
            "m_text": "buttonSize",
            "m_color": "red",
            "m_type": 0
        } )
        listModel.append( {
            "m_size": privateStyle.dialogMinSize,
            "m_text": "dialogMinSize",
            "m_color": "seagreen",
            "m_type": 2
        } )
        listModel.append( {
            "m_size": privateStyle.dialogMaxSize,
            "m_text": "dialogMaxSize",
            "m_color": "seagreen",
            "m_type": 2
        } )
    }

    Connections {
        target: platformStyle
        onLayoutParametersChanged: {
            if ( listModel.count ) {
                listModel.setProperty(0, "m_size", platformStyle.graphicSizeLarge)
                listModel.setProperty(1, "m_size", platformStyle.graphicSizeMedium)
                listModel.setProperty(2, "m_size", platformStyle.graphicSizeSmall)
                listModel.setProperty(3, "m_size", platformStyle.graphicSizeTiny)
                listModel.setProperty(4, "m_size", platformStyle.paddingLarge)
                listModel.setProperty(5, "m_size", platformStyle.paddingMedium)
                listModel.setProperty(6, "m_size", platformStyle.paddingSmall)
                listModel.setProperty(7, "m_size", platformStyle.borderSizeMedium)
            }
        }
    }
    Connections {
        target: privateStyle
        onLayoutParametersChanged: {
            if ( listModel.count ) {
                listModel.setProperty(8, "m_size", privateStyle.statusBarHeight)
                listModel.setProperty(9, "m_size", privateStyle.tabBarHeightPortrait)
                listModel.setProperty(10, "m_size", privateStyle.tabBarHeightLandscape)
                listModel.setProperty(11, "m_size", privateStyle.toolBarHeightPortrait)
                listModel.setProperty(12, "m_size", privateStyle.toolBarHeightLandscape)
                listModel.setProperty(13, "m_size", privateStyle.scrollBarThickness)
                listModel.setProperty(14, "m_size", privateStyle.sliderThickness)
                listModel.setProperty(15, "m_size", privateStyle.menuItemHeight)
                listModel.setProperty(16, "m_size", privateStyle.textFieldHeight)
                listModel.setProperty(17, "m_size", privateStyle.switchButtonHeight)
                listModel.setProperty(18, "m_size", privateStyle.buttonSize)
                listModel.setProperty(19, "m_size", privateStyle.dialogMinSize)
                listModel.setProperty(20, "m_size", privateStyle.dialogMaxSize)
            }
        }
    }
}
