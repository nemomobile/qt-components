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

import Qt 4.7
import com.nokia.symbian 1.0

Item {
    id: root
    anchors.fill: parent
    anchors.margins: privy.margin

    QtObject {
        id: privy
        property bool portrait:    screen.currentOrientation == Screen.Portrait
                                || screen.currentOrientation == Screen.PortraitInverted
        property int margin: 5
    }

    Row {
        id: wrapAndContent
        property real w: (width - spacing * (children.length - 1)) / children.length
        anchors.top: parent.top; anchors.left: parent.left
        anchors.bottomMargin: privy.margin
        height: parent.height * 1/9
        width: parent.width * (privy.portrait ? 1 : 1/2)
        spacing: privy.margin

        ChoiceList {
            id: wrap; objectName: "wrap"
            height: parent.height; width: wrapAndContent.w
            currentIndex: 0
            model: ["Wrap", "NoWrap", "WordWrap", "Anywhere"]
            onCurrentIndexChanged: {
                if (currentIndex == 0)
                    textArea.wrapMode = TextEdit.Wrap
                else if (currentIndex == 1)
                    textArea.wrapMode = TextEdit.NoWrap
                else if (currentIndex == 2)
                    textArea.wrapMode = TextEdit.WordWrap
                else if (currentIndex == 3)
                    textArea.wrapMode = TextEdit.WrapAnywhere
            }
        }

        ChoiceList {
            id: content; objectName: "content"
            height: parent.height; width: wrapAndContent.w
            currentIndex: 0
            model: ["Clean", "Long text", "Rich text"]
            onClosed: {
                if (currentValue == "Clean")
                    textArea.text = ""
                else if (currentValue == "Long text")
                    textArea.text = "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Sed porttitor tempus nunc a porta. Etiam sit amet sem eros, quis sollicitudin est. Nullam commodo, augue et pulvinar tristique, leo purus consequat mi, commodo tempor ligula orci id enim. Donec varius urna et lacus consequat in placerat orci vestibulum. Etiam semper, velit sed tincidunt hendrerit, augue tortor gravida velit, a aliquam elit nulla vitae risus. In vitae enim eget est molestie dignissim at at turpis. Nam id purus at orci convallis ultrices. Vivamus at nisl id lacus tempor varius et nec tellus. Mauris posuere lorem ut mauris rutrum porttitor. Nunc at metus turpis, non volutpat erat. Sed bibendum imperdiet tellus, sit amet imperdiet augue tincidunt id. Morbi ultrices dapibus augue, at placerat magna luctus in.This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Sed porttitor tempus nunc a porta. Etiam sit amet sem eros, quis sollicitudin est. Nullam commodo, augue et pulvinar tristique, leo purus consequat mi, commodo tempor ligula orci id enim. Donec varius urna et lacus consequat in placerat orci vestibulum. Etiam semper, velit sed tincidunt hendrerit, augue tortor gravida velit, a aliquam elit nulla vitae risus. In vitae enim eget est molestie dignissim at at turpis. Nam id purus at orci convallis ultrices. Vivamus at nisl id lacus tempor varius et nec tellus. Mauris posuere lorem ut mauris rutrum porttitor. Nunc at metus turpis, non volutpat erat. Sed bibendum imperdiet tellus, sit amet imperdiet augue tincidunt id. Morbi ultrices dapibus augue, at placerat magna luctus in."
                else if (currentValue == "Rich text")
                    textArea.text = "<p align=center><b>Lorem, Lorem</b></p><p><b>Lorem ipsum dolor sit amet, <i>consectetur adipisicing elit</i>, <u>sed do</u></b> eiusmod tempor <i><u style=color:'red'>incididunt</i><img src=\":/richtexteditor\"></img></u> ut labore et dolore <big style=color:'green'>magna</big> aliqua.<p>"
            }
        }
    }

    Row {
        id: alignment
        property real w: (width - spacing * (children.length - 1)) / children.length
        anchors.top: privy.portrait ? wrapAndContent.bottom : parent.top
        anchors.left: privy.portrait ? parent.left : wrapAndContent.right
        anchors.right: parent.right
        anchors.topMargin: privy.portrait ? privy.margin : 0
        anchors.leftMargin: privy.portrait ? 0 : privy.margin
        height: parent.height * 1/9
        width: parent.width * (privy.portrait ? 1 : 1/2)
        spacing: privy.margin

        ChoiceList {
            id: hAlignment; objectName: "hAlignment"
            height: parent.height; width: alignment.w
            currentIndex: 0
            model: ["Left", "Center", "Right", "Justify"]

            onCurrentIndexChanged: {
                if (currentIndex == 0)
                    textArea.horizontalAlignment = TextEdit.AlignLeft
                else if (currentIndex == 1)
                    textArea.horizontalAlignment = TextEdit.AlignHCenter
                else if (currentIndex == 2)
                    textArea.horizontalAlignment = TextEdit.AlignRight
                else if (currentIndex == 3)
                    textArea.horizontalAlignment = TextEdit.AlignJustify
            }
        }

        ChoiceList {
            id: vAlignment; objectName: "vAlignment"
            height: parent.height; width: alignment.w
            currentIndex: 0
            model: ["Top", "Center", "Bottom"]
            onCurrentIndexChanged: {
                if (currentIndex == 0)
                    textArea.verticalAlignment  = TextEdit.AlignTop
                else if (currentIndex == 1)
                    textArea.verticalAlignment  = TextEdit.AlignVCenter
                else if (currentIndex == 2)
                    textArea.verticalAlignment  = TextEdit.AlignBottom
            }
        }
    }

    Grid {
        id: buttons
        property real h: (height - spacing * (rows - 1)) / rows
        property real w: (width - spacing * (columns - 1)) / columns
        anchors.top: privy.portrait ? alignment.bottom : wrapAndContent.bottom
        anchors.left: parent.left
        anchors.topMargin: privy.margin
        columns: privy.portrait ? 3 : 1
        rows: privy.portrait ? 1 : 3
        height: parent.height * (privy.portrait ? 1/9 : 8/9)
        width: parent.width * (privy.portrait ? 1 : 1/5)
        spacing: privy.margin

        Button {
            id: readOnly; objectName: "readOnly"
            checkable: true;
            height: buttons.h; width: buttons.w
            text: "Editable"
            onClicked: {
                textArea.readOnly = checked ? true : false
                text = checked ? "ReadOnly" : "Editable"
            }
        }

        Button {
            id: enable; objectName: "enable"
            checkable: true
            height: buttons.h; width: buttons.w
            text: "Enabled"
            onClicked: {
                textArea.enabled = checked ? false : true
                text = checked ? "Disabled" : "Enabled"
            }
        }

        Button {
            id: defocus; objectName: "defocus"
            height: buttons.h; width: buttons.w
            text: "Defocus"
            onClicked: forceActiveFocus()
        }
    }

    TextArea {
        id: textArea; objectName: "textArea"
        anchors.top: privy.portrait ? buttons.bottom : wrapAndContent.bottom
        anchors.left: privy.portrait ? parent.left : buttons.right
        anchors.right: parent.right; anchors.bottom: parent.bottom
        anchors.leftMargin: privy.portrait ? 0 : privy.margin
        anchors.topMargin: privy.margin
        height: parent.height * (privy.portrait ? 2/3 : 8/9)
        width: parent.width
        placeholderText: "Enter text here"
    }
}
