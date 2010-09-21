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
import com.meego.themebridge 1.0
import "QueryDialog.js" as Private

Item {
    id: root

    property alias dialogTitle: queryPanel.dialogTitle
    property alias dialogText: queryPanel.dialogText
    signal queryAnswered(string button)

    function appear(title, message, callback) {
        root.visible = true
        Private.appear(queryPanel, title, message, callback);
    }

    function dismiss() {
        queryPanel.dismiss();
    }

    // Black rectangle to "fade-out" the screen. This is sensible to clicks so
    // it can dismiss the panel when clicked
    Rectangle {
        id: fadeRect
        anchors.fill: parent
        color: "black"
        opacity: 0

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (!panelContainsPoint(mouse.x, mouse.y))
                    queryPanel.dismiss();
            }

            function panelContainsPoint(x, y) {
                var left = queryPanel.x;
                var right = left + queryPanel.width;
                var top = queryPanel.y;
                var bottom = top + queryPanel.height;

                return ((x >= left) && (x <= right) && (y >= top) && (y <= bottom)) ? true : false;
            }
        }
    }

    // Actual query dialog
    QueryPanel {
        id: queryPanel
        anchors.top: root.bottom
        anchors.left: root.left
        anchors.right: root.right

        anchors.leftMargin: leftMargin
        anchors.rightMargin: rightMargin

        // Used in the "shown" modes. As a simplification I keep those set
        anchors.bottomMargin: bottomMargin
        anchors.verticalCenterOffset: (topMargin - bottomMargin) / 2

        maximumHeight: root.height - topMargin - bottomMargin

        property bool shown: false
        property string lastButtonClicked

        onButtonClicked: {
            lastButtonClicked = button;
            shown = false;
        }

        states: [
            State {
                name: "shownCentered"
                when: queryPanel.shown && queryPanel.centered
                AnchorChanges {
                    target: queryPanel
                    anchors.top: undefined
                    anchors.verticalCenter: root.verticalCenter
                }
                PropertyChanges {
                    target: fadeRect
                    opacity: 0.87
                }
            },
            State {
                name: "shownBottom"
                when: queryPanel.shown && !queryPanel.centered
                AnchorChanges {
                    target: queryPanel
                    anchors.top: undefined
                    anchors.bottom: root.bottom
                }
                PropertyChanges {
                    target: fadeRect
                    opacity: 0.87
                }
            }
        ]

        // XXX TODO: Get these timings from MWidgetFadeAnimationStyle and
        //           MWidgetSlideAnimationStyle. The problem is that it requires tuning
        //           of Style based on ascendent list, which is not in place yet.
        transitions: [
            // In transition
            Transition {
                from: ""
                SequentialAnimation {
                    PropertyAnimation {
                        duration: 400
                        property: "opacity"
                        easing.type: Easing.InOutCubic
                    }
                    AnchorAnimation {
                        duration: 300
                        easing.type: Easing.OutExpo
                    }
                }
            },
            // Out transition
            Transition {
                to: ""
                SequentialAnimation {
                    AnchorAnimation {
                        duration: 300
                        easing.type: Easing.InCubic
                    }
                    PropertyAnimation {
                        duration: 400
                        property: "opacity"
                        easing.type: Easing.OutCubic
                    }
                    ScriptAction {
                        script: {
                            Private.queryAnswered(queryPanel.lastButtonClicked);
                            root.queryAnswered(queryPanel.lastButtonClicked);
                            root.visible = false
                        }
                    }
                }
            }
        ]
    }
}
