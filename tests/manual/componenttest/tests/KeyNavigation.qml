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
import Qt.labs.components 1.0

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
        width: parent.width - anchors.margins * 2; height: parent.height * 3 / 6 - anchors.margins * 2
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
    }
}
