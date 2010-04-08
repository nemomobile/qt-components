/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
import Qt 4.7

TopLevelItem {
    id: tooltip;
    property string text;
    property bool shown: false;

    width: model.width + 22;
    height: model.height + 16;

    transformOrigin: Item.Top;
    scale: 0;
    visible: false;

    // Visible items bellow should anchor / be sized in relation to
    // 'parent'. They'll be reparented to a proper positioned and
    // resized toplevel item.
    keepInside: true;

    BorderImage {
        id: background;
        anchors.fill: parent;
        source: "images/tooltip-background.png";
        border.top: 4;
        border.left: 11;
        border.bottom: 12;
        border.right: 11;
    }

    // ### This Label is used to get the "preferred size" information, that
    // will be considered when calculating the toplevel item geometry. This
    // could be replaced by having this information available in a regular Text
    // item. Similar issue of trying to know the "real image size" inside an
    // Image item.
    Label {
        id: model;
        text: tooltip.text;
        visible: false;
    }

    Label {
        anchors.centerIn: parent;
        anchors.verticalCenterOffset: -4;
        width: parent.width - 22;
        horizontalAlignment: Text.AlignHCenter;

        text: tooltip.text;
        color: "#ffffff";
        elide: Text.ElideRight
    }

    states: [
        State {
            name: "shown";
            when: tooltip.shown && (tooltip.text !== "");
            PropertyChanges { target: tooltip; scale: 1; visible: true }
        }
    ]

    transitions: [
        Transition {
            from: "";
            to: "shown";
            SequentialAnimation {
                PropertyAction {
                    target: tooltip;
                    property: "visible";
                }
                NumberAnimation {
                    duration: 500;
                    target: tooltip;
                    easing.type: "OutElastic";
                    easing.period: 0.25;
                    property: "scale";
                }
            }
        },
        Transition {
            from: "shown";
            to: "";
            SequentialAnimation {
                NumberAnimation {
                    duration: 150;
                    target: tooltip;
                    easing.type: "InSine";
                    property: "scale";
                }
                PropertyAction {
                    target: tooltip;
                    property: "visible";
                }
            }
        }
    ]
}
