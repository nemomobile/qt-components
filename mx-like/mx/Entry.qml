/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
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
/*This won't work without mouse proxy support. It was/will be recently pushed*/

Item {
    id:lineedit
    property string hint: "Entry"
    property alias text: textInp.text
    property bool secret: false
    property alias leftIconSource: leftIcon.source 
    property alias rightIconSource: rightIcon.source 
    signal rightIconClicked
    signal leftIconClicked

    width: (hintText.state=='hinting'?hintText.width:textInp.width) + 11 + leftIcon.width + rightIcon.width
    height: 13 + 11 

    BorderImage {
        source: "images/entry.png"
        anchors.fill:parent
        border.left:5; border.top:5;
        border.right:5; border.bottom:5;
    }
    BorderImage {
        id: focusframe
        opacity:0
        source: "images/entry-active.png"
        anchors.fill:parent
        border.left:5; border.top:5;
        border.right:5; border.bottom:5;
    }
    clip: true
    Text{
        id:hintText
        anchors.centerIn: parent
        anchors.topMargin:5
        anchors.rightMargin:6+rightIcon.width
        anchors.bottomMargin:6
        anchors.leftMargin:5+leftIcon.width
        font.pixelSize:13
        font.family: droidSansBold.name
        color: '#A2A2A2'
        text: ''
        states: State{
            name: 'hinting'
            when: textInp.text=="" && textInp.focus==false
            PropertyChanges{target: hintText; text: lineedit.hint}
        }//No cool animated transition, because mx doesn't do that
        
    }
    TextInput{
        id:textInp
        cursorDelegate: Item{
            Rectangle{
                visible: parent.parent.focus
                color: "#009BCE"
                height: 13
                width: 2
                y: 1
            }
        }
        /*
        anchors.topMargin:5
        anchors.rightMargin:6
        anchors.bottomMargin:6
        anchors.leftMargin:5
        */
        property int leftMargin: 6 + leftIcon.width
        property int rightMargin: 6 + rightIcon.width
        x: leftMargin
        y: 5
        //Below function implements all scrolling logic
        onCursorPositionChanged: {
            if(cursorRect.x < leftMargin - textInp.x){//Cursor went off the front
                textInp.x = leftMargin - Math.max(0, cursorRect.x);
            }else if(cursorRect.x > parent.width - leftMargin - rightMargin - textInp.x){//Cusor went off the end
                textInp.x = leftMargin - Math.max(0, cursorRect.x - (parent.width - leftMargin - rightMargin));
            }
        }

        text:""
        horizontalAlignment: TextInput.AlignLeft
        font.pixelSize:13
        font.family: droidSansBold.name
        focusOnPress:true
        echoMode: secret?TextInput.Password:TextInput.Normal
    }
    MouseArea{
        //Forward events to TextInput, and claim focus for the LineEdit
        anchors.fill: parent; forwardTo: textInp;
        onPressed: {mouse.accepted=false;}
    }

    Image{id:rightIcon
        anchors.right: parent.right
        anchors.rightMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        MouseArea{
            anchors.fill: parent
            onClicked: lineedit.rightIconClicked();
        }
    }
    Image{id:leftIcon
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        MouseArea{
            anchors.fill: parent
            onClicked: lineedit.leftIconClicked();
        }
    }

    states: [
        State {
            name: "Highlighted"
            when: textInp.focus
            PropertyChanges { target: focusframe; opacity: 1.0}
        },
        State { name: "Disabled" }
    ]
    transitions: [
        Transition {
            from: ""; to: "Highlighted"
            NumberAnimation { properties: "opacity"; duration: 150 }
        },
        Transition {
            from: "Highlighted"; to: ""
            NumberAnimation { properties: "opacity"; duration: 150 }
        }
    ]
}
