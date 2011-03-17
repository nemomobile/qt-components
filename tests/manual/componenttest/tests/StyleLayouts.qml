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
            border.color: "blue"
            border.width: 2
            color: "black"
            height: text1.height + m_size + 2 * platformStyle.paddingLarge + platformStyle.paddingMedium
            width: listView.width
            Text {
                id: text1
                anchors.left: parent.left
                anchors.leftMargin: platformStyle.paddingLarge
                anchors.top: parent.top
                anchors.topMargin: platformStyle.paddingLarge
                text: m_text
                color: platformStyle.colorNormalLight
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: platformStyle.paddingLarge
                anchors.top: parent.top
                anchors.topMargin: platformStyle.paddingLarge
                text: m_size
                color: platformStyle.colorNormalLight
                font: text1.font
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
                listModel.setProperty(18, "m_size", privateStyle.dialogMinSize)
                listModel.setProperty(19, "m_size", privateStyle.dialogMaxSize)
            }
        }
    }
}
