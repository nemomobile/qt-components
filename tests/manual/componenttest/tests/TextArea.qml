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

FocusScope {
    id: root

    QtObject {
        id: privy
        property bool portrait:    screen.currentOrientation == Screen.Portrait
                                || screen.currentOrientation == Screen.PortraitInverted

        property real contentHeight: parent.height - buttons.height
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

        anchors { top: buttons.bottom; left: parent.left; bottom: privy.portrait ? settings.top : vbk.top }
        width: visible ? h : 0
        visible: !inputContext.visible

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
            top: buttons.bottom
            left: style.right
            right: privy.portrait ? parent.right : settings.left
            bottom: privy.portrait ? settings.top : vbk.top
        }

        placeholderText: "Enter text here"
        platformMaxImplicitWidth: root.width - style.width - (privy.portrait ? 0 : settings.width)
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
            top: privy.portrait ? undefined : buttons.bottom
            bottom: vbk.top
            right: parent.right
            topMargin: platformStyle.paddingSmall
            bottomMargin: platformStyle.paddingSmall
        }
        height: visible ? privy.contentHeight * 1/3 : 0
        width: privy.portrait ? parent.width : parent.width * 2/5
        visible: !privy.portrait || !inputContext.visible
    }

    Item {
        id: vbk
        anchors {
            bottom: root.bottom
            right: parent.right
            left: parent.left
        }
        height: inputContext.visible ? inputContext.height : 0
    }

    Menu {
        id: optionMenu
        content: MenuLayout {
            MenuItem {text: "Clear"; onClicked: textArea.text = ""}
            MenuItem {text: "Long text"; onClicked: textArea.text = "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Sed porttitor tempus nunc a porta. Etiam sit amet sem eros, quis sollicitudin est. Nullam commodo, augue et pulvinar tristique, leo purus consequat mi, commodo tempor ligula orci id enim. Donec varius urna et lacus consequat in placerat orci vestibulum. Etiam semper, velit sed tincidunt hendrerit, augue tortor gravida velit, a aliquam elit nulla vitae risus. In vitae enim eget est molestie dignissim at at turpis. Nam id purus at orci convallis ultrices. Vivamus at nisl id lacus tempor varius et nec tellus. Mauris posuere lorem ut mauris rutrum porttitor. Nunc at metus turpis, non volutpat erat. Sed bibendum imperdiet tellus, sit amet imperdiet augue tincidunt id. Morbi ultrices dapibus augue, at placerat magna luctus in.This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Sed porttitor tempus nunc a porta. Etiam sit amet sem eros, quis sollicitudin est. Nullam commodo, augue et pulvinar tristique, leo purus consequat mi, commodo tempor ligula orci id enim. Donec varius urna et lacus consequat in placerat orci vestibulum. Etiam semper, velit sed tincidunt hendrerit, augue tortor gravida velit, a aliquam elit nulla vitae risus. In vitae enim eget est molestie dignissim at at turpis. Nam id purus at orci convallis ultrices. Vivamus at nisl id lacus tempor varius et nec tellus. Mauris posuere lorem ut mauris rutrum porttitor. Nunc at metus turpis, non volutpat erat. Sed bibendum imperdiet tellus, sit amet imperdiet augue tincidunt id. Morbi ultrices dapibus augue, at placerat magna luctus in."}
            MenuItem {text: "Rich text"; onClicked: textArea.text = "<p align=center><b>Lorem, Lorem</b></p><p><b>Lorem ipsum dolor sit amet, <i>consectetur adipisicing elit</i>, <u>sed do</u></b> eiusmod tempor <i><u style=color:'red'>incididunt</i><img src=\":/memory_card\"></img></u> ut labore et dolore <big style=color:'green'>magna</big> aliqua.<p>"}
        }
    }

    Connections {
        target: optionsButton
        onClicked: optionMenu.open()
    }
}
