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
import com.meego 1.0

Page {
    id: page

    tools: commonTools

    anchors.fill: parent
    Flickable {
        id: flick

        anchors.fill: parent
        contentHeight: content.height

        Item {
            id: content

            width: parent.width
            height: childrenRect.height

            Text {
                id: firstText
                text: "Predefined common types are available for Push Buttons in cases where they have no custom requirement whatsoever. These buttons can have icons next to their label and predefined background colors, according to their types."
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    leftMargin: 10
                    rightMargin: 10
                }
            }

            Text {
                id: secondText
                text: "<b><br/>&nbsp;1. Single button\n<br/></b>\nWhen using a button that literally has no other button next to it (neither in rows or columns) only declares a Button element with no predefined type."
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                anchors {
                    left: parent.left
                    right: parent.right
                    top: firstText.bottom
                    leftMargin: 10
                    rightMargin: 10
                }
            }

            Rectangle {
                id: firstLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: secondText.bottom
                width: parent.width
                height: 1
            }

            Button {
                id: onlyButton
                anchors.horizontalCenter: parent.horizontalCenter
                text: "only button"
                anchors.top: firstLine.bottom
                anchors.topMargin: 5
                height: 64
                width: 444
            }

            Rectangle {
                id: secondLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: onlyButton.bottom
                width: parent.width
                height: 1
            }

            Text {
                id: thirdText
                text: "<b><br/>&nbsp;2. Button with Icons\n<br/></b>\nWhen a button with an icon is desired, you can simply pass the icon url/path or the image provider addres of this image, for instance <i>image://theme/icon-m-telephony-call-end</i>"
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                anchors {
                    left: parent.left
                    right: parent.right
                    top: secondLine.bottom
                    leftMargin: 10
                    rightMargin: 10
                }
            }

            Rectangle {
                id: thirdLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: thirdText.bottom
                width: parent.width
                height: 1
            }

            Button {
                id: iconButton
                text: "Button with icon"
                anchors.top: thirdLine.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 7
                height: 64
                width: 424
                iconSource: "../images/testIcon.png"
            }

            Button {
                id: imageProviderButton
                text: "Button Image Provider"
                anchors.top: thirdLine.bottom
                anchors.left: iconButton.right
                anchors.topMargin: 5
                height: 64
                width: 424
                iconSource: "image://theme/icon-m-telephony-call-end"
            }

            Rectangle {
                id: fourthLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: iconButton.bottom
                width: parent.width
                height: 1
            }

            Text {
                id: fourthText
                text: "<b><br/>&nbsp;3. Other types of single buttons\n<br/></b>\nThere are two more types of single buttons, namely <i>\"Affirmative\"</i> and <i>\"Negative\"</i>. To use those buttons you have only to set the <i><b>buttonType</b></i> property to either <i>\"affirmative\"<i> or <i>\"negative\"</i>."
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                anchors {
                    left: parent.left
                    right: parent.right
                    top: fourthLine.bottom
                    leftMargin: 10
                    rightMargin: 10
                }
            }

            Rectangle {
                id: fifthLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: fourthText.bottom
                width: parent.width
                height: 1
            }

            Button {
                id: affirmativeButton
                text: "Affirmative Button"
                anchors.top: fifthLine.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 7
                height: 64
                width: 424
                buttonType: "affirmative"
            }

            Button {
                id: negativeButton
                text: "Negative Button"
                anchors.top: fifthLine.bottom
                anchors.left: affirmativeButton.right
                anchors.topMargin: 5
                height: 64
                width: 424
                buttonType: "negative"
            }

            Rectangle {
                id: sixthLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: affirmativeButton.bottom
                width: parent.width
                height: 1
            }

            Text {
                id: fifthText
                text: "<b><br/>&nbsp;4. Toggle button\n<br/></b>\nThere is also a button that can remains \"checked\" when it is pressed. To enable this feature set the <i>checkable</i> property to true."
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                anchors {
                    left: parent.left
                    right: parent.right
                    top: sixthLine.bottom
                    leftMargin: 10
                    rightMargin: 10
                }
            }

            Rectangle {
                id: seventhLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: fifthText.bottom
                width: parent.width
                height: 1
            }

            Button {
                id: toggleButton
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Toggle Button"
                anchors.top: seventhLine.bottom
                anchors.topMargin: 5
                height: 64
                width: 424
                checkable: true
            }

            Rectangle {
                id: eighthLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: toggleButton.bottom
                width: parent.width
                height: 1
            }

            Text {
                id: sixthText
                text: "<b><br/>&nbsp;5. Disabled button\n<br/></b>\nButtons can be \"disabled\" preventing interaction and using a disabled look and feel. To use this feature set the <i>enabled</i> property to false."
                font.pixelSize: 24
                width: parent.width
                wrapMode: Text.WordWrap
                anchors {
                    left: parent.left
                    right: parent.right
                    top: eighthLine.bottom
                    leftMargin: 10
                    rightMargin: 10
                }
            }

            Rectangle {
                id: ninthLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: sixthText.bottom
                width: parent.width
                height: 1
            }

            Button {
                id: disabledButton
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Disabled Button"
                anchors.top: ninthLine.bottom
                anchors.topMargin: 5
                height: 64
                width: 424
                enabled: false
            }

            Rectangle {
                id: tenthLine
                color: "black"
                anchors.topMargin: 5
                anchors.top: disabledButton.bottom
                width: parent.width
                height: 1
            }
         }
    }

    ScrollDecorator {
        flickable: flick
    }
}
