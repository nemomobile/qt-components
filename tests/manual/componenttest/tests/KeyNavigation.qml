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

import QtQuick 1.0
import com.nokia.symbian 1.1
import Qt.labs.components 1.1

FocusScope {
    id: root
    anchors.fill: parent
    focus: true

    Component {
        id: highlight
        Rectangle {
            visible: GridView.view.activeFocus
            border {color: "steelblue"; width: 5}
            color: "#00000000"; radius: 5;
            z: 5
        }
    }

    ListModel {
        id: buttonsModel
        ListElement {
            objectName: "addText"
            title: "Add text"
            action: "Add"
            checkableButton: false
            buttonChecked: false
        }
        ListElement {
            objectName: "clearText"
            title: "Clear text"
            action: "Clear"
            checkableButton: false
            buttonChecked: false
        }
        ListElement {
            objectName: "preText"
            title: "Toggle start"
            action: ""
            checkableButton: true
            buttonChecked: false
        }
        ListElement {
            objectName: "postText"
            title: "Toggle end"
            action: ""
            checkableButton: true
            buttonChecked: false
        }
    }

    Component {
        id: buttonsDelegate
        Button {
            objectName: objectName
            width: buttons.cellWidth; height: buttons.cellHeight
            text: title
            checkable: checkableButton
            onClicked: {
                if (action == "Add")
                    editor.text = (buttonsModel.get(2).buttonChecked ? "Toggled:" : "") + ("Lorem ipsum") + (buttonsModel.get(3).buttonChecked ? ":Toggled\n" : "\n") + (editor.text)
                else if (action == "Clear")
                    editor.text = ""
                buttonsModel.set(index, {"buttonChecked": checked})
            }
        }
    }

    GridView {
        id: buttons
        anchors {top: parent.top; horizontalCenter: parent.horizontalCenter; margins: platformStyle.paddingSmall }
        width: parent.width - anchors.margins * 2; height: parent.height * 2 / 6 - anchors.margins * 2
        cellWidth: width / 2 - anchors.margins; cellHeight: height / 2 - anchors.margins
        highlight: highlight
        focus: true
        model: buttonsModel
        delegate: buttonsDelegate
        KeyNavigation.down: radioButtons
    }

    TextArea {
        id: editor
        objectName: "editor"
        anchors { top: buttons.bottom; horizontalCenter: parent.horizontalCenter; margins: platformStyle.paddingSmall }
        width: parent.width - anchors.margins * 2; height: parent.height * 2 / 6 - anchors.margins * 2
        readOnly: true
        enabled: false
    }

    ListModel {
        id: radioButtonsModel
        ListElement {
            objectName: "one"
            title: "one"
        }
        ListElement {
            objectName: "two"
            title: "two"
        }
        ListElement {
            objectName: "three"
            title: "three"
        }
    }

    Component {
        id: radioButtonsDelegate
        RadioButton {
            objectName: objectName
            width: radioButtons.cellWidth; height: radioButtons.cellHeight
            platformExclusiveGroup: group
            text: title
        }
    }

    CheckableGroup { id: group }

    GridView {
        id: radioButtons
        anchors { top: editor.bottom; horizontalCenter: parent.horizontalCenter; margins: platformStyle.paddingSmall; }
        width: parent.width - anchors.margins * 2; height: parent.height  * 1 / 6 - anchors.margins * 2
        cellWidth: width / 3 - anchors.margins; cellHeight: height - anchors.margins
        highlight: highlight
        model: radioButtonsModel
        delegate: radioButtonsDelegate
        KeyNavigation.up: buttons
        KeyNavigation.down: checkBoxes
    }

    ListModel {
        id: checkBoxesModel
        ListElement {
            objectName: "one"
            title: "one"
        }
        ListElement {
            objectName: "two"
            title: "two"
        }
        ListElement {
            objectName: "three"
            title: "three"
        }
    }

    Component {
        id: checkBoxesDelegate
        CheckBox {
            objectName: objectName
            width: checkBoxes.cellWidth; height: checkBoxes.cellHeight
            text: title
        }
    }

    GridView {
        id: checkBoxes
        anchors { top: radioButtons.bottom; horizontalCenter: parent.horizontalCenter; margins: platformStyle.paddingSmall; }
        width: parent.width - anchors.margins * 2; height: parent.height  * 1 / 6 - anchors.margins * 2
        cellWidth: width / 3 - anchors.margins; cellHeight: height - anchors.margins
        highlight: highlight
        model: checkBoxesModel
        delegate: checkBoxesDelegate
        KeyNavigation.up: radioButtons
    }
}
