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

Item {
    id: combobox;
    Common{id:mx}

    property int elementsToShow;
    property alias current: value.text;
    property alias model: elements.model;
    property alias currentIndex: elements.currentIndex;
    property alias tooltipText: tooltip.text;

    width: 161;
    height: 27;
    elementsToShow: 5;

    BorderImage {
        id: background;
        source: "images/combobox.png";
        anchors.fill: parent;
        border.left:5;
        border.right:5;
    }

    Text {
        id: value;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: background.left;
        anchors.right: marker.left;
        anchors.leftMargin: 12;
        font.pixelSize: mx.fontSize
        color: mx.fontColor
    }

    BorderImage {
        id: marker;
        source: "images/combobox-marker.png";
        border.left:5;
        border.right:5;
        anchors.verticalCenter: background.verticalCenter;
        anchors.right: background.right;
        anchors.rightMargin: 10;
    }

    MouseArea {
        id: markerArea;
        anchors.fill: background;
        hoverEnabled: tooltip.text;
        onClicked: {
            list.state == "" ? list.state = "shown" : list.state = "";
            elements.currentIndex = list.lastIndex;
        }
        onPressed: { tooltip.pressDismiss = true; }
        onExited: { tooltip.pressDismiss = false; }
    }

    TopLevelItem {
        id: list;
        property int lastIndex;

        opacity: 0;
        height: Math.min(background.height * elements.count,
                         background.height * combobox.elementsToShow);

        anchors.top: background.bottom;
        anchors.left: background.left;
        anchors.right: background.right;

        Component {
            id: delegate
            Item {
                id: wrapper
                width: background.width;
                height: background.height;

                Row {
                    x: 5;
                    y: 5;
                    spacing: 5;
                    Text {
                        font.pixelSize: mx.fontSize
                        color: mx.fontColor
                        text: content;
                    }
                    Image {
                        source: icon;
                        anchors.verticalCenter: parent.verticalCenter;
                        height: wrapper.height - 10;
                    }
                }

                function selectItem(index) {
                    combobox.current = elements.model.get(index).content;
                    list.lastIndex = index;
                    list.state = "";
                }

                MouseArea {
                    anchors.fill: parent;
                    hoverEnabled: true;

                    onEntered: {
                        elements.currentIndex = index;
                    }
                    onClicked: selectItem(index);
                }

                Keys.onPressed: {
                    if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
                        selectItem(index);
                    } else if (event.key == Qt.Key_Escape) {
                        list.state = "";
                    }
                }
            }
        }

        Component {
            id: highlight
            Rectangle {
                color: "#cccccc";
                x:1
                width:parent.width-2
            }
        }

        BorderImage {
            id: listBackground;
            source: "images/menu.png"
            border.left: 6; border.top: 6
            border.right: 6; border.bottom: 6
            anchors.fill: parent;
        }

        ListView {
            id: elements;
            anchors.fill: parent;

            clip:true;
            boundsBehavior: "StopAtBounds";
            keyNavigationWraps: true;

            delegate: delegate;
            highlight: highlight;
        }

        states: [
            State {
                name: "shown";
                PropertyChanges {
                    target: list;
                    opacity: 1;
                }
                PropertyChanges {
                    target: elements;
                    focus: true;
                }
            }
        ]

        transitions: [
            Transition {
                from: "shown";
                to: "";
                NumberAnimation {
                    target: list;
                    properties: "opacity";
                    easing.type: "OutCubic";
                    duration: 500;
                }
            }
        ]
    }

    TooltipLoader {
        id: tooltip;
        anchors.fill: parent;

        property bool pressDismiss: false;
        shown: !pressDismiss && markerArea.containsMouse && (list.state == "");
    }
}
