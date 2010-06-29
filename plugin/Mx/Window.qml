/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
import Qt.labs.components 1.0

Item{//This should be ROOT element in a QML/MX app
    property int headerBottom: header.height //Children should have at least this y
    property alias comboModel: combo.model
    property alias comboCurrent: combo.current
    WindowModel{id:wm}
    Rectangle{
        id:background
        anchors.fill: parent
        anchors.rightMargin: 1
        anchors.bottomMargin: 1
        color: "white"
        border.color: "black"
    }
    Item {
        width: parent.width;
        height: 64;

        Rectangle{
            id:header
            width: parent.width
            height:64
            color:'#333333'
            MouseArea{
                anchors.fill: parent
                onReleased: wm.resetMove();
                onPositionChanged: wm.dragPointMove(mouse.x, mouse.y);
            }
            TitlebarCombobox{
                id:combo;//API redesign required after I figure out what
                //on earth this this is SUPPOSED to do.
                anchors.centerIn: parent
                current: 'Select'
                model: ListModel{
                    ListElement{
                        content: 'Hello'
                    }
                    ListElement{
                        content: 'World'
                    }
                }
            }
            Image{
                id:closeImg
                anchors.right:parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                source: Qt.resolvedUrl('images/close' + (closeMA.containsMouse?'-hover':'') + '.png')
                MouseArea{
                    id: closeMA
                    anchors.fill: parent
                    hoverEnabled:true
                    onClicked: Qt.quit()
                }
            }
        }
    }

    TopLevelItem {
        id: resizeArea;
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 1

        Image{
            id:resizeImg
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 1
            source: 'images/resize-grip.png'
            MouseArea {
                id: resizeMA
                anchors.fill: parent
                onReleased: wm.resetResize();
                onPositionChanged: wm.dragPointResize(mouse.x, mouse.y);
            }
        }
    }
}
