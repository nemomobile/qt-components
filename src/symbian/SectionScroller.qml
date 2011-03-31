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
import "SectionScroller.js" as Sections

ImplicitSizeItem {
    id: root

    property ListView listView
    property bool platformSingleRow: false

    onListViewChanged: {
        if (listView && listView.model) {
            internal.initDirtyObserver();
        } else if (listView) {
            listView.modelChanged.connect(function() {
                if (listView.model) {
                    internal.initDirtyObserver();
                }
            })
        }
    }

    Rectangle {
        id: container

        property bool dragging: false

        color: "transparent"
        width: platformSingleRow ? listView.width : 3 * privateStyle.scrollBarThickness
        height: listView.height
        x: listView.x + listView.width - width

        MouseArea {
            id: dragArea
            objectName: "dragArea"
            anchors { top: parent.top; bottom: parent.bottom; right: parent.right }
            width: internal.dragAreaWidth
            drag.axis: Drag.YAxis
            drag.minimumY: listView.y
            drag.maximumY: listView.y + listView.height - toolTip.height

            onPressed: {
                mouseDownTimer.restart()
            }

            onReleased: {
                container.dragging = false;
                mouseDownTimer.stop()
            }

            onPositionChanged: {
                internal.adjustContentPosition(dragArea.mouseY);
            }

            Timer {
                id: mouseDownTimer
                interval: 150

                onTriggered: {
                    container.dragging = true;
                    internal.adjustContentPosition(dragArea.mouseY);
                }
            }
        }

        BorderImage {
            id: singleRowBackground
            objectName: "singleRowBackground"
            width: internal.backgroundWidth()
            height: 24 * platformStyle.paddingMedium
            source: privateStyle.imagePath("qtg_fr_popup_transparent")
            border { left: 10; top: 10; right: 10; bottom: 10 }
            anchors.centerIn: parent
            visible: platformSingleRow && parent.dragging

            Text {
                id: singleRowText
                objectName: "singleRowText"
                color: "white"
                anchors.centerIn: parent
                font { family: platformStyle.fontFamilyRegular; pixelSize: 8 * platformStyle.fontSizeMedium }
                text: internal.currentArea
            }
        }

        ImplicitSizeItem {
            id: toolTip
            objectName: "toolTip"
            visible: !platformSingleRow
            opacity: container.dragging ? 1 : 0
            anchors.right: parent.right
            anchors.rightMargin: 50 // TODO: not aligned with the layout spec
            anchors.verticalCenter: parent.verticalCenter
            width: childrenRect.width
            height: childrenRect.height

            BorderImage {
                id: background
                width: childrenRect.width
                height: childrenRect.height
                anchors.left: parent.left
                source: privateStyle.imagePath("qtg_fr_popup_transparent")
                border { left: 10; top: 10; right: 10; bottom: 10 }

                Column {
                    width: Math.max(previousSectionLabel.text.length, currentSectionLabel.text.length, nextSectionLabel.text.length) == 1 ? 100 : internal.backgroundWidth()
                    height: childrenRect.height

                    SectionScrollerLabel {
                        id: previousSectionLabel
                        objectName: "previousSectionLabel"
                        text: internal.prevSection
                        highlighted: internal.currentArea === text
                        up: !internal.down
                        anchors.horizontalCenter: internal.prevSection.length == 1 ? parent.horizontalCenter : undefined
                    }

                    Image {
                        objectName: "divider1"
                        source: privateStyle.imagePath("qtg_fr_popup_divider")
                        sourceSize.width: 40    // TODO: not aligned with the layout spec
                        sourceSize.height: 40   // TODO: not aligned with the layout spec
                        width: parent.width - 2 * platformStyle.paddingLarge
                        height: Math.round(platformStyle.graphicSizeTiny / 2) / 10
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    SectionScrollerLabel {
                        id: currentSectionLabel
                        objectName: "currentSectionLabel"
                        text: internal.currentSection
                        highlighted: internal.currentArea === text
                        up: !internal.down
                        anchors.horizontalCenter: internal.currentSection.length == 1 ? parent.horizontalCenter : undefined
                    }

                    Image {
                        objectName: "divider2"
                        source: privateStyle.imagePath("qtg_fr_popup_divider")
                        sourceSize.width: 40    // TODO: not aligned with the layout spec
                        sourceSize.height: 40   // TODO: not aligned with the layout spec
                        width: parent.width - 2 * platformStyle.paddingLarge
                        height: Math.round(platformStyle.graphicSizeTiny / 2) / 10
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    SectionScrollerLabel {
                        id: nextSectionLabel
                        objectName: "nextSectionLabel"
                        text: internal.nextSection
                        highlighted: internal.currentArea === text
                        up: !internal.down
                        anchors.horizontalCenter: internal.nextSection.length == 1 ? parent.horizontalCenter : undefined
                    }
                }
            }

            states: [
                State {
                    name: "Visible"
                    when: container.dragging
                },

                State {
                    extend: "Visible"
                    name: "AtTop"
                    when: internal.currentPosition === "first"
                    PropertyChanges {
                        target: previousSectionLabel
                        text: internal.currentSection
                    }
                    PropertyChanges {
                        target: currentSectionLabel
                        text: internal.nextSection
                    }
                    PropertyChanges {
                        target: nextSectionLabel
                        text: Sections.nextSection(internal.nextSection)
                    }
                },

                State {
                    extend: "Visible"
                    name: "AtBottom"
                    when: internal.currentPosition === "last"
                    PropertyChanges {
                        target: previousSectionLabel
                        text: Sections.previousSection(internal.prevSection)
                    }
                    PropertyChanges {
                        target: currentSectionLabel
                        text: internal.prevSection
                    }
                    PropertyChanges {
                        target: nextSectionLabel
                        text: internal.currentSection
                    }
                }
            ]

            Behavior on opacity {
                NumberAnimation { duration: 100 }
            }
        }
    }

    Timer {
        id: dirtyTimer
        interval: 100
        running: false
        onTriggered: {
            Sections.initSectionData(listView);
            internal.modelDirty = false;
        }
    }

    QtObject {
        id: internal

        property string prevSection: ""
        property string currentSection: listView.currentSection
        property string nextSection: ""
        property string currentArea: ""
        property string currentPosition: "first"
        property int oldY: 0
        property bool modelDirty: false
        property bool down: true
        property int dragAreaWidth: 35  // TODO: not aligned with the layout spec

        function backgroundWidth() {
            return Math.min(screen.width, screen.height) - 8 * platformStyle.paddingLarge
        }

        function initDirtyObserver() {
            Sections.initSectionData(listView);
            function dirtyObserver() {
                if (!internal.modelDirty) {
                    internal.modelDirty = true;
                    dirtyTimer.running = true;
                }
            }

            if (listView.model.countChanged)
                listView.model.countChanged.connect(dirtyObserver);

            if (listView.model.itemsChanged)
                listView.model.itemsChanged.connect(dirtyObserver);

            if (listView.model.itemsInserted)
                listView.model.itemsInserted.connect(dirtyObserver);

            if (listView.model.itemsMoved)
                listView.model.itemsMoved.connect(dirtyObserver);

            if (listView.model.itemsRemoved)
                listView.model.itemsRemoved.connect(dirtyObserver);
        }

        function adjustContentPosition(y) {
            if (y < 0 || y > dragArea.height) return;

            internal.down = y > internal.oldY;
            var sect = Sections.closestSection(y / dragArea.height, internal.down);
            internal.oldY = y;
            if (internal.currentArea != sect) {
                internal.currentArea = sect;
                internal.currentPosition = Sections.sectionPositionString(internal.currentArea);
                var relativeSection = Sections.relativeSections(internal.currentArea);
                internal.prevSection = relativeSection[0];
                internal.currentSection = relativeSection[1];
                internal.nextSection = relativeSection[2];
                var idx = Sections.indexOf(sect);
                listView.positionViewAtIndex(idx, ListView.Beginning);
            }
        }
    }
}
