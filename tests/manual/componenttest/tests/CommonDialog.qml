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

Item {
    id: root

    property bool showTitleIcon: false

    Grid {
        id: grid

        property int itemWidth: width / columns

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: platformStyle.paddingSmall
        columns: (screen.width < screen.height) ? 1 : 2

        SelectionListItem {
            id: titleTextSelection

            objectName: "titleTextSelection"
            width: grid.itemWidth
            title: "Title text"
            subTitle: titleTextEditor.text

            onClicked: titleTextDialog.open()

            CommonDialog {
                id: titleTextDialog
                titleText: "Title text"
                buttonTexts: ["OK"]
                content: Item {
                    width: titleTextDialog.platformContentMaximumWidth
                    height: titleTextEditor.height + 2 * platformStyle.paddingMedium
                    TextField {
                        id: titleTextEditor
                        objectName: "titleTextEditor"
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: platformStyle.paddingMedium
                        text: "The Title"
                    }
                }
            }
        }

        SelectionListItem {
            id: titleIconSelection

            objectName: "titleIconSelection"
            width: grid.itemWidth
            title: "Title icon"
            subTitle: showTitleIcon ? "Yes" : "No"

            onClicked: showTitleIcon = !showTitleIcon
        }

        SelectionListItem {
            id: contentSelection

            objectName: "contentSelection"
            width: grid.itemWidth
            title: "Content"
            subTitle: contentDialog.model[contentDialog.selectedIndex]

            onClicked: contentDialog.open()

            SelectionDialog {
                id: contentDialog
                titleText: "Select content"
                selectedIndex: 0
                model: ["Simple", "Flickable"]
            }
        }

        SelectionListItem {
            id: numberOfButtonsSelection
            objectName: "numberOfButtonsSelection"

            width: grid.itemWidth
            title: "Number of buttons"
            subTitle: numberOfButtonsDialog.model[numberOfButtonsDialog.selectedIndex]

            onClicked: numberOfButtonsDialog.open()

            SelectionDialog {
                id: numberOfButtonsDialog
                titleText: "Select number of buttons"
                selectedIndex: 0
                model: ["Zero", "One", "Two", "Three"]
            }
        }
    }

    Button {
        id: showDialogButton
        objectName: "showDialogButton"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: grid.bottom
        anchors.margins: platformStyle.paddingMedium
        text: "Show!"

        onClicked: commonDialog.open()
    }

    Label {
        id: clickedText
        objectName: "clickedText"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: showDialogButton.bottom
        anchors.topMargin: platformStyle.paddingLarge
    }

    CommonDialog {
        id: commonDialog

        titleText: titleTextEditor.text
        titleIcon: showTitleIcon ? "qrc:/list1.svg" : ""
        content: Loader {
            anchors.left: parent.left // makes the mirroring work correctly
            sourceComponent: {
                switch (contentDialog.selectedIndex) {
                    case 0: return simpleContent
                    case 1: return flickableContent
                    default: return simpleContent
                }
            }
        }

        buttonTexts: {
            switch (numberOfButtonsDialog.selectedIndex) {
                case 0: return []
                case 1: return ["OK"]
                case 2: return ["OK", "Cancel"]
                case 3: return ["OK", "Retry", "Cancel"]
                default: return []
            }
        }

        onButtonClicked: {
            clickedText.text = "Clicked on button " + index + " (" + commonDialog.buttonTexts[index] + ")"
        }

        onClickedOutside: {
            clickedText.text = "Clicked outside"

            // in case there's no buttons, dialog can be closed by clicking the fader
            if(numberOfButtonsDialog.selectedIndex == 0)
                close()
        }
    }

    Component {
        id: simpleContent

        Rectangle {
            width: 300
            height: 200
            color: "#30ff0000" // this makes the margins visible on the ui
            Grid {
                id: grid

                property int itemWidth: (width - platformStyle.paddingSmall) / 2
                property int itemHeight: (height - platformStyle.paddingSmall) / 2

                anchors.fill: parent
                anchors.margins: platformStyle.paddingMedium
                spacing: platformStyle.paddingSmall
                columns: 2

                Rectangle { color: "red"; width: grid.itemWidth; height: grid.itemHeight }
                Rectangle { color: "blue"; width: grid.itemWidth; height: grid.itemHeight }
                Rectangle { color: "green"; width: grid.itemWidth; height: grid.itemHeight }
                Rectangle { color: "yellow"; width: grid.itemWidth; height: grid.itemHeight }
            }
        }
    }

    Component {
        id: flickableContent
        Item {
            height: commonDialog.platformContentMaximumHeight
            width: commonDialog.platformContentMaximumWidth

            Flickable {
                id: flickableArea

                anchors.fill: parent
                contentWidth: colorfulSquare.width
                contentHeight: colorfulSquare.height
                clip: true

                Rectangle {
                    id: colorfulSquare

                    width: 1000
                    height: 1000
                    opacity: 0.5
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "blue" }
                        GradientStop { position: 1.0; color: "yellow" }
                    }
                }
                Rectangle {
                    height: colorfulSquare.height
                    width: colorfulSquare.width
                    opacity: 0.5
                    rotation: -90
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "red" }
                        GradientStop { position: 1.0; color: "green" }
                    }
                }
                Text {
                    anchors.centerIn: parent
                    text: "Flick around!"
                    color: "white"
                    font.pixelSize: 50
                    opacity: 0.85
                }
            }

            ScrollDecorator {
                flickableItem: flickableArea
            }
        }
    }
}
