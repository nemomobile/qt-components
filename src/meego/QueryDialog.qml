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
import "." 1.0
import "UIConstants.js" as UI

Dialog {
    id: root
    objectName: "queryDialog"

    property string titleText
    property string message

    //are they necessary?
    property alias acceptButtonText: acceptButton.text
    property alias rejectButtonText: rejectButton.text

    //ToDo
    property alias icon: iconImage.source

    property Style platformStyle: QueryDialogStyle {}

    //__centerContentField: true

    __dim: platformStyle.dim
    __fadeInDuration:  platformStyle.fadeInDuration
    __fadeOutDuration: platformStyle.fadeOutDuration
    __fadeInDelay:     platformStyle.fadeInDelay
    __fadeOutDelay:    platformStyle.fadeOutDelay

    __animationChief: "queryDialog"

    // the default is a modal QueryDialog, but don't make it modal when no buttons are shown
    __platformModal: !(acceptButtonText === "" && rejectButtonText === "")

    //Deprecated, TODO Remove this on w13
    property alias style: root.platformStyle

    signal linkActivated(string link)

    // the title field consists of the following parts: title string and
    // a close button (which is in fact an image)
    // it can additionally have an icon
    title: Item {
        id: titleField
        width: parent.width
        height: titleText == "" ? titleBarIconField.height :
                    titleBarIconField.height + titleLabel.height + root.platformStyle.titleColumnSpacing
        Column {
            id: titleFieldCol
            spacing: root.platformStyle.titleColumnSpacing

            anchors.left:  parent.left
            anchors.right:  parent.right
            anchors.top:  parent.top

            width: root.width

            Item {
                id: titleBarIconField
                height: iconImage.height
                width: parent.width
                Image {
                    id: iconImage
                    anchors.horizontalCenter: titleBarIconField.horizontalCenter
                    source: ""
                }

            }


            Item {
                id: titleBarTextField
                height: titleLabel.height
                width: parent.width

                Text {
                    id: titleLabel
                    width: parent.width

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter

                    font.family: root.platformStyle.titleFontFamily
                    font.pixelSize: root.platformStyle.titleFontPixelSize
                    font.bold:  root.platformStyle.titleFontBold
                    font.capitalization: root.platformStyle.titleFontCapitalization
                    elide: root.platformStyle.titleElideMode
                    wrapMode: elide == Text.ElideNone ? Text.Wrap : Text.NoWrap
                    color: root.platformStyle.titleTextColor
                    text: root.titleText

                }
            }

            // needed for animation
            transform: Scale {
                id: titleScale
                xScale: 1.0; yScale: 1.0
                origin.x: mapFromItem(queryContent, queryContent.width / 2, queryContent.height / 2).x
                origin.y: mapFromItem(queryContent, queryContent.width / 2, queryContent.height / 2).y
            }

        }
    }

    // the content field which contains the message text
    content: Item {
        id: queryContentWrapper

        property int upperBound: visualParent ? visualParent.height - titleField.height - buttonColFiller.height - 64
                                                : root.parent.height - titleField.height - buttonColFiller.height - 64
        property int __sizeHint: Math.min(Math.max(root.platformStyle.contentFieldMinSize, queryText.height), upperBound)

        height: __sizeHint + root.platformStyle.contentTopMargin
        width: root.width

        Item {
            id: queryContent
            width: parent.width

            y: root.platformStyle.contentTopMargin

            Flickable {
                id: queryFlickable
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                //anchors.bottom: parent.bottom
                height: queryContentWrapper.__sizeHint

                contentHeight: queryText.height
                flickableDirection: Flickable.VerticalFlick
                clip: true

                interactive:  queryText.height > queryContentWrapper.__sizeHint

                Text {
                    id: queryText
                    width: queryFlickable.width
                    horizontalAlignment: Text.AlignHCenter
                    font.family: root.platformStyle.messageFontFamily
                    font.pixelSize: root.platformStyle.messageFontPixelSize
                    color: root.platformStyle.messageTextColor
                    wrapMode: Text.WordWrap
                    text: root.message
                    onLinkActivated: root.linkActivated(link)
                }

            }


            ScrollDecorator {
                id: scrollDecorator
                flickableItem: queryFlickable
                anchors.rightMargin: - UI.SCROLLDECORATOR_LONG_MARGIN - 10 //ToDo: Don't use a hard-coded gap
            }

        }
    }


    buttons: Item {
        id: buttonColFiller
        width: parent.width
        height: childrenRect.height

        anchors.top: parent.top

        //ugly hack to assure, that we're always evaluating the correct height
        //otherwise the topMargin wouldn't be considered
        Item {id: dummy; anchors.fill:  parent}

        Column {
            id: buttonCol
            anchors.top: parent.top
            anchors.topMargin: root.platformStyle.buttonTopMargin
            spacing: root.platformStyle.buttonsColumnSpacing

            height: (acceptButton.text  == "" ? 0 : acceptButton.height)
                    + (rejectButton.text == "" ? 0 : rejectButton.height)
                    + anchors.buttonTopMargin  + spacing

            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: acceptButton
                text: ""
                onClicked: accept()
                visible: text != ""
                __dialogButton: true
                platformStyle: ButtonStyle {inverted: true}
            }
            Button {
                id: rejectButton
                text: ""
                onClicked: reject()
                visible: text != ""
                __dialogButton: true
                platformStyle: ButtonStyle {inverted: true}
            }
        }
    }

    StateGroup {
        id: statesWrapper

        state: "__query__hidden"

        // needed for button animation
        // without resetting the button row's coordinate system would be translated
        property int __buttonSaver: buttonColFiller.y

        states: [
            State {
                name: "__query__visible"
                when: root.__animationChief == "queryDialog" && (root.status == DialogStatus.Opening || root.status == DialogStatus.Open)
                PropertyChanges {
                    target: root
                    opacity: 1.0
                }
            },
            State {
                name: "__query__hidden"
                when: root.__animationChief == "queryDialog" && (root.status == DialogStatus.Closing || root.status == DialogStatus.Closed)
                PropertyChanges {
                    target: root
                    opacity: 0.0
                }
            }
        ]

        transitions: [
            Transition {
                from: "__query__visible"; to: "__query__hidden"
                SequentialAnimation {
                    ScriptAction {script: {
                            __fader().state = "hidden";
                            root.opacity = 1.0;
                            queryContent.opacity = 1.0
                            titleField.opacity = 1.0

                            statesWrapper.__buttonSaver = buttonColFiller.y
                            root.status = DialogStatus.Closing;

                        }
                    }

                    NumberAnimation { target: root; properties: "opacity"; from: 0.0; to: 1.0; duration: 0 }
                    NumberAnimation { target: queryContent; properties: "opacity"; from: 0.0; to: 1.0; duration: 0 }
                    NumberAnimation { target: titleField; properties: "opacity"; from: 0.0; to: 1.0; duration: 0 }

                    // The closing transition fades out the dialog's content from 100% to 0%,
                    // scales down the content to 80% anchored in the center over 175msec, quintic ease in,
                    // then, after a 175ms delay the background fades to alpha 0% (350ms, quintic ease out).
                    // (background fading is done in Fader.qml)

                    ParallelAnimation {
                        NumberAnimation {target: queryContent; properties: "opacity"; from: 1.0; to: 0.0; duration: 175}
                        NumberAnimation {target: titleField; properties: "opacity"; from: 1.0; to: 0.0; duration: 175}
                        NumberAnimation {target: titleScale; properties: "xScale,yScale"; from: 1.0 ; to: 0.8; duration: 175; easing.type: Easing.InQuint}
                        NumberAnimation {target: queryContent; property: "scale"; from: 1.0 ; to: 0.8; duration: 175; easing.type: Easing.InQuint}
                        NumberAnimation {target: buttonColFiller; properties: "opacity"; from: 1.0; to: 0.0; duration: 175}
                        NumberAnimation {target: buttonColFiller; property: "scale"; from: 1.0 ; to: 0.8; duration: 175; easing.type: Easing.InQuint}
                    }

                    ScriptAction {script: {

                            // reset button
                            buttonColFiller.y = statesWrapper.__buttonSaver

                            // make sure, root isn't visible:
                            root.opacity = 0.0;
                            status = DialogStatus.Closed;

                        }
                    }

                }
            },
            Transition {
                from: "__query__hidden"; to: "__query__visible"
                SequentialAnimation {
                    ScriptAction {script: {
                            __fader().state = "visible";

                            statesWrapper.__buttonSaver = buttonColFiller.y

                            root.status = DialogStatus.Opening;
                            // UPPERCASE-UGLY, but necessary to avoid flicker
                            root.opacity = 1.0
                            titleField.opacity = 0.0
                            queryContent.opacity = 0.0
                            buttonColFiller.opacity = 0.0
                        }
                    }

                    // The opening transition fades in from 0% to 100% and at the same time
                    // scales in the content from 80% to 100%, 350msec, anchored in the center
                    // cubic ease out). --> Done inside the fader

                    ParallelAnimation {
                        NumberAnimation {target: queryContent; properties: "opacity"; from: 0.0; to: 1.0; duration: 350; easing.type: Easing.OutCubic; }
                        NumberAnimation {target: titleField; properties: "opacity"; from: 0.0; to: 1.0; duration: 350; easing.type: Easing.OutCubic; }
                        NumberAnimation {target: titleScale; properties: "xScale,yScale"; from: 0.8 ; to: 1.0; duration: 350; easing.type: Easing.OutCubic; }
                        NumberAnimation {target: queryContent; property: "scale"; from: 0.8 ; to: 1.0; duration: 350; easing.type: Easing.OutCubic; }
                        NumberAnimation {target: buttonColFiller; property: "scale"; from: 0.8 ; to: 1.0; duration: 350; easing.type: Easing.OutCubic; }
                        NumberAnimation {target: buttonColFiller; properties: "opacity"; from: 0.0; to: 1.0; duration: 350; easing.type: Easing.OutCubic; }
                    }

                    ScriptAction {script: {
                            // reset button
                            buttonColFiller.y = statesWrapper.__buttonSaver
                            root.status = DialogStatus.Open;
                        }
                    }
                }
            }
        ]
    }
}
