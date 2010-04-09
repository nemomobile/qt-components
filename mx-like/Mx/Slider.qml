/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
import Qt 4.6

Item {
    id: basicSlider;
    property var sliderEdgeOffset: 6;
    property real value: nob.x/(sliderBase.width-nob.width)
    height: 22
    width: 108
    Rectangle {
        id: sliderBase
        color: '#dadada'
        width: parent.width - 12
        x:6
        anchors.verticalCenter: parent.verticalCenter
        height: 8
        Rectangle {
            id: sliderBar
            color: '#84dbf5'
            anchors.left: parent.left
            anchors.right : nob.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: 8
        }
        Button {
            id: nob
            x: parent.width/2 - nob.width/2
            anchors.verticalCenter: parent.verticalCenter
            text:""
            width: 22
            height: 16
        }
        MouseArea {
            function handleRelease(x) {nob.x = x}
            anchors.verticalCenter:parent.verticalCenter
            anchors.left:parent.left;
            anchors.right:parent.right;
            height:20
            onPressed: { handleRelease(mouseX);}
        }
        MouseArea {
            id:mouseRegion
            anchors.verticalCenter:nob.verticalCenter
            anchors.horizontalCenter:nob.horizontalCenter
            width:50
            height:50
            drag.target: nob
            drag.axis: "XAxis"
            drag.minimumX:-sliderEdgeOffset
            drag.maximumX:sliderBase.width-nob.width/2 - sliderEdgeOffset
        }
    }
}
