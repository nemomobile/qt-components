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
import "../components"

FocusScope {
    id: root

    QtObject {
        id: privy
        property bool portrait:    screen.currentOrientation == Screen.Portrait
                                || screen.currentOrientation == Screen.PortraitInverted

        property real contentHeight: parent.height * (privy.portrait ? 7/8 : 5/6) - platformStyle.paddingSmall * 3
    }

    ButtonRow {
        id: buttons

        property real h: parent.height * (privy.portrait ? 1/8 : 1/6)

        anchors { top: parent.top; left: parent.left; right: parent.right }
        exclusive: false

        Button {
            id: editable; objectName: "readOnly"
            checked: true; checkable: true
            height: parent.h; text: checked ? "Editable" : "ReadOnly"
        }

        Button {
            id: enable; objectName: "enable"
            checked: true; checkable: true
            height: parent.h; text: checked ? "Enabled": "Disabled"
        }

        Button {
            id: error; objectName: "error"
            checkable: true; height: parent.h
            text: "Error"
        }
    }

    Column {
        id: style

        property real h: (height - spacing * (children.length - 1)) / children.length

        anchors { top: buttons.bottom; left: parent.left }
        height: privy.contentHeight * (privy.portrait ? 2/3 : 1)
        width: parent.height * (privy.portrait ? 1/8 : 1/6)

        Button {
            id: bold; objectName: "bold"
            height: parent.h; width: parent.width
            checkable: true; text: "B"
        }

        Button {
            id: italic; objectName: "italic"
            height: parent.h; width: parent.width
            checkable: true; text: "I"
        }

        Button {
            id: strikeout; objectName: "strikeout"
            height: parent.h; width: parent.width
            checkable: true; text: "S"
        }

        Button {
            id: underline; objectName: "underline"
            height: parent.h; width: parent.width
            checkable: true; text: "U"
        }
    }

    TextArea {
        id: textArea; objectName: "textArea"
        anchors {
            top: buttons.bottom; left: style.right
            right: privy.portrait ? parent.right : settings.left
        }

        height: privy.contentHeight * (privy.portrait ? 2/3 : 1)
        placeholderText: "Enter text here"

        enabled: enable.checked
        errorHighlight: error.checked
        horizontalAlignment: settings.horizontalAlignment
        inputMethodHints: settings.inputMethodHints
        readOnly: !editable.checked
        verticalAlignment: settings.verticalAlignment
        wrapMode: settings.wrapMode

        font.bold: bold.checked
        font.capitalization: settings.fontCapitalization
        font.family: settings.fontFamily
        font.italic: italic.checked
        font.letterSpacing: settings.fontLetterSpacing
        font.pixelSize: settings.fontPixelSize
        font.strikeout: strikeout.checked
        font.underline: underline.checked
        font.weight: settings.fontWeight
   }

   TextSettings {
        id: settings
        anchors {
            top: privy.portrait ? textArea.bottom : buttons.bottom
            bottom: root.bottom
            right: parent.right
            topMargin: platformStyle.paddingSmall
            bottomMargin: platformStyle.paddingSmall
        }
        width: privy.portrait ? parent.width : parent.width * 2/5
    }

    Menu {
        id: optionMenu
        content: MenuLayout {
            MenuItem {text: "Clear"; onClicked: textArea.text = ""}
            MenuItem {text: "Long text"; onClicked: textArea.text = "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Sed porttitor tempus nunc a porta. Etiam sit amet sem eros, quis sollicitudin est. Nullam commodo, augue et pulvinar tristique, leo purus consequat mi, commodo tempor ligula orci id enim. Donec varius urna et lacus consequat in placerat orci vestibulum. Etiam semper, velit sed tincidunt hendrerit, augue tortor gravida velit, a aliquam elit nulla vitae risus. In vitae enim eget est molestie dignissim at at turpis. Nam id purus at orci convallis ultrices. Vivamus at nisl id lacus tempor varius et nec tellus. Mauris posuere lorem ut mauris rutrum porttitor. Nunc at metus turpis, non volutpat erat. Sed bibendum imperdiet tellus, sit amet imperdiet augue tincidunt id. Morbi ultrices dapibus augue, at placerat magna luctus in.This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Sed porttitor tempus nunc a porta. Etiam sit amet sem eros, quis sollicitudin est. Nullam commodo, augue et pulvinar tristique, leo purus consequat mi, commodo tempor ligula orci id enim. Donec varius urna et lacus consequat in placerat orci vestibulum. Etiam semper, velit sed tincidunt hendrerit, augue tortor gravida velit, a aliquam elit nulla vitae risus. In vitae enim eget est molestie dignissim at at turpis. Nam id purus at orci convallis ultrices. Vivamus at nisl id lacus tempor varius et nec tellus. Mauris posuere lorem ut mauris rutrum porttitor. Nunc at metus turpis, non volutpat erat. Sed bibendum imperdiet tellus, sit amet imperdiet augue tincidunt id. Morbi ultrices dapibus augue, at placerat magna luctus in."}
            MenuItem {text: "Rich text"; onClicked: textArea.text = "<p align=center><b>Lorem, Lorem</b></p><p><b>Lorem ipsum dolor sit amet, <i>consectetur adipisicing elit</i>, <u>sed do</u></b> eiusmod tempor <i><u style=color:'red'>incididunt</i><img src=\":/richtexteditor\"></img></u> ut labore et dolore <big style=color:'green'>magna</big> aliqua.<p>"}
        }
    }

   Connections {
       target: optionsButton
       onClicked: optionMenu.open()
   }
}
