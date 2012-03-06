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
import com.nokia.meego 1.1

Page {
    id: root
    tools: tabTools
    anchors.margins: UiConstants.DefaultMargin

    TabGroup {
        id: tabGroup
        anchors.fill: parent
        currentTab: showcaseTab

        // SHOWCASE TAB
        Item {
            id: showcaseTab
            anchors.fill: parent
            Flickable {
                id: container
                anchors.fill: parent
                contentWidth: col.width
                contentHeight: col.height
                flickableDirection: Flickable.VerticalFlick
                pressDelay: 100
                Column {
                    id: col
                    spacing: 10
                    width: container.width

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Default text field:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        placeholderText: "Default text"
                        maximumLength: 80

                        onAccepted: { console.log("accepted signal triggered") }

                        Keys.onReturnPressed: {
                            text = "Return key pressed";
                            parent.focus = true;
                        }
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Custom text field:"
                    }

                    Item {
                        anchors {left: parent.left; right: parent.right}
                        height: custom.height

                        Rectangle {
                            id: suggestionBubble
                            radius: 10
                            color: "lightgrey"
                            anchors {left: parent.left; right: custom.right; bottom: custom.top; bottomMargin:5}
                            height: 100

                            visible: custom.activeFocus

                            Flickable {
                                anchors.fill: parent
                                anchors.margins: 10
                                clip: true
                                contentWidth: suggestionBubble.width
                                contentHeight: suggestionBubble.height
                                contentY:  Math.max(0, suggestionBubbleText.height - height)
                                flickableDirection: Flickable.VerticalFlick
                                Text {
                                    id: suggestionBubbleText
                                    width: parent.width
                                    font.pointSize: 14
                                    color: "darkgrey"
                                    onLineHeightChanged: console.log(lineHeight);
                                    wrapMode: Text.WordWrap
                                    text: "You can show elements on TextField activeFocus change: \n" + custom.text
                                }
                            }
                        }

                        TextField {
                            id: custom
                            anchors {left: parent.left; right: activateButton.left; rightMargin: 10}

                            platformSipAttributes: SipAttributes { actionKeyHighlighted: true }

                            placeholderText: "Single-line field with button to clear"
                            platformStyle: TextFieldStyle { paddingRight: clearButton.width }
                            Image {
                                id: clearButton
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                source: "image://theme/icon-m-input-clear"
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        inputContext.reset();
                                        custom.text = "";
                                    }
                                }
                            }
                        }
                        Button {
                            id: activateButton
                            anchors.right: parent.right
                            width: 200
                            text: "activate"
                            onClicked: custom.forceActiveFocus()
                        }
                    }

                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        text: "read only text field"
                        readOnly:  true
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Multiline text area:"
                    }
                    TextArea {
                        anchors {left: parent.left; right: parent.right;}
                        placeholderText: "Multi-line text area"
                        text: "This is a\nmultiline\ntext area"
                        // Set the minimum height to be 200
                        height: 200
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Read only multiline text area:"
                    }

                    TextArea {
                        anchors {left: parent.left; right: parent.right;}
                        placeholderText: "Multi-line text area"
                        text: "This is a\nmultiline\ntext area.\nSet as Read only"
                        // Set the minimum height to be 200
                        readOnly: true
                        height: Math.max (200, implicitHeight)
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Bottom text field:"
                    }

                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Page ends here"
                    }
                }
            }
            ScrollDecorator {
                flickableItem: container
            }
        } // showcaseTab page

        // SPECIAL FIELDS TAB
        Item {
            id: specialFieldsTab
            anchors.fill: parent
            property int toolBarHeight: rootWindow.platformToolBarHeight
            onToolBarHeightChanged: {
                anchors.bottomMargin = toolBarHeight
                ? toolBarHeight: anchors.bottomMargin
            }
            Flickable {
                id: specialFieldsContainer
                anchors.fill: parent
                contentWidth: specialFieldsCol.width
                contentHeight: specialFieldsCol.height
                flickableDirection: Flickable.VerticalFlick
                pressDelay: 100
                Column {
                    id: specialFieldsCol
                    spacing: 10
                    width: specialFieldsContainer.width
                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Password:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        placeholderText: "Password field"
                        echoMode: TextInput.Password
                        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Text field using int validator:"
                    }

                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        placeholderText: "Value between 0 and 100 (mandatory field)"
                        validator: IntValidator{bottom: 0; top: 100;}
                        inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                    }

                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        placeholderText: "Value between 1980 and 2020 (optional field)"
                        validator: IntValidator { bottom: 1980; top: 2020 }
                        errorHighlight: text ? !acceptableInput : false
                        inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Text area using error highlight"
                    }

                    TextArea {
                        anchors {left: parent.left; right: parent.right;}
                        placeholderText: "Error highlight when more than 5 characters"
                        errorHighlight: text.length > 5
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "No preedit:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        inputMethodHints: Qt.ImhNoPredictiveText
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Digits only:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Phone number:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        inputMethodHints: Qt.ImhDialableCharactersOnly | Qt.ImhNoPredictiveText
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "E-mail:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhEmailCharactersOnly | Qt.ImhNoPredictiveText
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Url:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhUrlCharactersOnly | Qt.ImhNoPredictiveText
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "Uppercase only:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        inputMethodHints: Qt.ImhUppercaseOnly
                    }

                    Label {
                        font: UiConstants.FieldLabelFont
                        color: UiConstants.FieldLabelColor
                        text: "No edit bubble:"
                    }
                    TextField {
                        anchors {left: parent.left; right: parent.right;}
                        platformEnableEditBubble: false
                    }
                }
            }
            ScrollDecorator {
                flickableItem: specialFieldsContainer
            }
        }
        Page {
            id: sipAttributesTab
            anchors.fill: parent
            Column {
                id: sipAttributesCol
                spacing: 10
                ButtonRow {
                    exclusive: false
                    Button {
                        id: labelButton
                        text:"Label"
                        checkable: true
                        onClicked: sipAttributesTextField.forceActiveFocus()
                    }
                    Button {
                        id: iconButton
                        text:"Icon"
                        checkable: true
                        onClicked: sipAttributesTextField.forceActiveFocus()
                    }
                    Button {
                        id: highlightButton
                        text:"Highlight"
                        checkable: true
                        onClicked: sipAttributesTextField.forceActiveFocus()
                    }
                    Button {
                        id: disableButton
                        text:"Disable"
                        checkable: true
                        onClicked: sipAttributesTextField.forceActiveFocus()
                    }
                }
                TextField {
                    id: sipAttributesTextField
                    anchors {left:parent.left; right:parent.right}
                    platformSipAttributes: SipAttributes {
                        actionKeyLabel: labelButton.checked?"custom":""
                        actionKeyIcon: iconButton.checked?"/usr/share/widgetsgallery/imtoolbar/icons/icon-m-messaging-smiley-cool.svg":""
                        actionKeyHighlighted: highlightButton.checked
                        actionKeyEnabled: !disableButton.checked
                    }
                    Keys.onReturnPressed: {
                        sipAttributesTextField2.forceActiveFocus();
                    }
                }
                ButtonRow {
                    exclusive: false
                    Button {
                        id: labelButton2
                        text:"Label"
                        checkable: true
                        onClicked: sipAttributesTextField2.forceActiveFocus()
                    }
                    Button {
                        id: iconButton2
                        text:"Icon"
                        checkable: true
                        onClicked: sipAttributesTextField2.forceActiveFocus()
                    }
                    Button {
                        id: highlightButton2
                        text:"Highlight"
                        checkable: true
                        onClicked: sipAttributesTextField2.forceActiveFocus()
                    }
                    Button {
                        id: disableButton2
                        text:"Disable"
                        checkable: true
                        onClicked: sipAttributesTextField2.forceActiveFocus()
                    }
                }
                SipAttributes {
                    id: sipAttributes
                    actionKeyLabel: labelButton2.checked?"custom":""
                    actionKeyIcon: iconButton2.checked?"/usr/share/widgetsgallery/imtoolbar/icons/icon-m-messaging-smiley-cool.svg":""
                    actionKeyHighlighted: highlightButton2.checked
                    actionKeyEnabled: !disableButton2.checked
                }
                TextField {
                    id: sipAttributesTextField2
                    anchors {left:parent.left; right:parent.right}
                    platformSipAttributes: sipAttributes
                }
                TextArea {
                    id: sipAttributesTextArea
                    anchors {left:parent.left; right:parent.right}
                    platformSipAttributes: sipAttributes
                }
            }
        }
        ToolBarLayout {
            id: tabTools
            ToolIcon {
                id: toolIcon 
                iconId: "toolbar-back"
                onClicked: tabGroup.currentTab.depth > 1 ? tabGroup.currentTab.pop() : pageStack.pop()
            }
            ButtonRow {
                anchors {left: toolIcon.right; right: parent.right}
                TabButton {
                    text: "Showcase"
                    tab: showcaseTab
                }
                TabButton {
                    text: "Special fields"
                    tab: specialFieldsTab
                }
                TabButton {
                    text: "Sip Attributes"
                    tab: sipAttributesTab
                }
            }
        }
    } //TabGroup
}
