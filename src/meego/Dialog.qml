/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Item {
    id: root

    // API
    property alias title: titleBar.text
    property alias content: contentField.children
    property alias buttons: buttonRow.children

    property alias modal: dialogFader.modal

    property alias closeButtonVisible: titleBar.closeButtonVisible

    signal activated
    signal deactivated
    signal accepted
    signal rejected

    //private api
    Item {
        id: margin
        //ToDo: use fix values for margins to avoid themebridge & lmt style
        property int leftMargin: dialogStyle.current.get("dialogLeftMargin")
        property int rightMargin: dialogStyle.current.get("dialogRightMargin")
        property int topMargin: dialogStyle.current.get("dialogTopMargin")
        property int bottomMargin: dialogStyle.current.get("dialogBottomMargin")
        property bool centered: dialogStyle.current.get("dialogVerticalAlignment") == Qt.AlignCenter
    }

    width:  parent.width - margin.leftMargin - margin.rightMargin
    height: parent.height - margin.topMargin - margin.bottomMargin

    state: "hidden"

    Fader {
        id: dialogFader
    }

    function isVisible() {
        return state == "visible"
    }

    onAccepted: state = "hidden"
    onRejected: state = "hidden"

    Item {
        id: dialogItem

        //ToDo: add support for layoutDirection Qt::RightToLeft
        x: margin.leftMargin
        y: margin.topMargin
        height:  root.height
        width: root.width

        state: root.state
        opacity: state == "visible" ? 1.0 : 0.0

        // this item contains the whole dialog (title bar + content rectangle, button row)
        Item {
            id: backgroundRect
            anchors.fill: parent

            height: calculateContentHeight()

            // Calculate height that can fit all label content, the button row and
            // two spacings (between label and buttons, and below the buttons)
            function calculateContentHeight() {
                var height = 0;
                height += titleBar.height;
                height += 250; //bad: hard-coded value for content-field!!
                if (buttonRow.children[0]) {
                    height += buttonRow.children[0].height;
                }
                height += dialogStyle.current.get("verticalSpacing") * 2;
                return height;
            }


            DialogTitleBar {
                id: titleBar
                opacity: dialogItem.opacity
                onRequestClose: root.state = "hidden"
            }

            Item {
                id: contentRect

                anchors.top: titleBar.bottom
                anchors.bottom: buttonRow.top
                anchors.left: parent.left
                anchors.right: parent.right

                Background {
                    id: meegoStyleBackground
                    anchors.fill: contentRect
                    style: contentsStyle
                }

            }

            Flickable {
                id: contentFlicker
                contentHeight: contentField.height
                //contentWidth: contentField.width
                anchors.fill: contentRect

                clip: true
                interactive: true

                //responsible for loading the content
                //via content property
                Item {id: contentField}

            }

            Item {
                id: buttonRow
                opacity: dialogItem.opacity
                anchors.bottom: backgroundRect.bottom
                anchors.horizontalCenter: backgroundRect.horizontalCenter

                height: childrenRect.height
               // spacing: dialogStyle.current.get("buttonSpacing")

            }

        }

        states: [
            State {
                name: "visible"
                PropertyChanges {
                    target: dialogItem
                    y: margin.topMargin
                    // width:  root.width
                    // height: root.height
                    opacity: 1.0
                }
            },
            State {
                name: "hidden"
                PropertyChanges {
                    target: dialogItem
                    y: 0 - height
                    // width:  0
                    // height: 0
                    opacity: 0.0
                }
            }
        ]

        transitions: [
            Transition {
                from: "visible"; to: "hidden"
                NumberAnimation {properties: "x,y"; easing.type: Easing.InOutQuad; duration: 600}
                NumberAnimation {properties: "opacity"; duration: 900}
                ScriptAction {script: {
                        deactivated();
                        dialogFader.state = state;
                    }
                }
            },
            Transition {
                from: "hidden"; to: "visible"
                NumberAnimation {properties: "x,y"; easing.type: Easing.InOutQuad; duration: 600}
                NumberAnimation {properties: "opacity"; duration: 900}
                ScriptAction {script: {
                        activated();
                        dialogFader.state = state;
                    }
                }
            }
        ]

    }


    //ToDo: remove lmt dependency/avoid using themebridge
    Style {
        id: contentsStyle
        styleClass: "MPannableWidgetStyle"
        styleObjectName: "MDialogContentsViewport"
    }

    Style {
        id: dialogStyle
        styleClass: "MDialogStyle"
    }

}



