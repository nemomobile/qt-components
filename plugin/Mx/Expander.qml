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

Item{
    id: container
    Common{id:mx}
    height: 34
    width: Math.max(containee.width,label.width+label.x) + 16
    default property Item containee
    onContaineeChanged: {//Setup containee
        //does NOT deal with the case of setting it a second time
        containee.parent = container;
        containee.opacity = 0;
        containee.x = 8;
        containee.y = 34;
    }
    property alias text: label.text
    property alias tooltipText: tooltip.text

    children:[//not the default property anymore
    Image {
        id: button
        property string subStr: 'up'
        anchors.left: image.left
        anchors.leftMargin: 8
        source: Qt.resolvedUrl('images/expander-arrow-' + subStr + (mr.containsMouse?'-hover':'') + '.png')
        y: 6
        z:2
    },
    Text {
        id: label
        color: mx.fontColor
        font.bold: true
        anchors.left: button.right
        anchors.leftMargin: 8
        y:8 //doesn't look to be verticalCenter aligned to button (but should be)
        z:2
    },
    MouseArea {
        id:mr
        anchors.fill: parent
        hoverEnabled: true
        onClicked: if(container.state=="open"){container.state="";}else{container.state="open";}
        onPressed: tooltip.pressDismiss = true;
        onExited: tooltip.pressDismiss = false;
    },
    BorderImage {
        id:image
        anchors.fill: parent
        source: Qt.resolvedUrl('images/expander-'+ (mr.containsMouse?'hover':'closed') + '.png')
        border.bottom: 8
        border.left: 8
        border.right: 8
        border.top: 8
        z:0
    },
    TooltipLoader {
        id: tooltip;
        anchors.fill: parent;

        property bool pressDismiss: false;
        shown: !pressDismiss && mr.containsMouse && (container.state == "");
    }
    ]
    states: [
        State{
            name: "open" //Closed is the default state
            PropertyChanges{target: container; height: 34 + containee.height + 7;}
            PropertyChanges{target: container.containee; parent: container; opacity: 1}
            PropertyChanges{target: button; subStr: 'down';}
            PropertyChanges{target: image; source: 'images/expander-open.png'; border.top: 33}
        }
    ]
    transitions: Transition {
        from: ""; to: "open";
        reversible: true;
        SequentialAnimation{
            PropertyAction{target: button; property: 'subStr'}
            PropertyAction{property: 'border.top'}
            NumberAnimation{properties:'height'; duration:256}
            NumberAnimation{properties:'opacity'; duration:120}
        }
    }
}
