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
import "SectionScroller.js" as Sections
import "." 1.1

Item {
    id: root

    property Flickable flickableItem: null
    property int orientation: Qt.Vertical
    property bool interactive: true
    property int policy: Symbian.ScrollBarWhenScrolling
    property bool privateSectionScroller: false
    property bool platformInverted: false

    //implicit values for qml designer when no Flickable is present
    implicitHeight: privateStyle.scrollBarThickness * (orientation == Qt.Vertical ? 3 : 1)
    implicitWidth: privateStyle.scrollBarThickness * (orientation == Qt.Horizontal ? 3 : 1)
    height: {
        if (flickableItem && orientation == Qt.Vertical)
            return Math.floor(Math.round(flickableItem.height) - anchors.topMargin - anchors.bottomMargin)
        return undefined
    }
    width: {
        if (flickableItem && orientation == Qt.Horizontal)
            return Math.floor(Math.round(flickableItem.width) - anchors.leftMargin - anchors.rightMargin)
        return undefined
    }
    opacity: internal.rootOpacity

    onFlickableItemChanged: internal.initSectionScroller();
    onPrivateSectionScrollerChanged: internal.initSectionScroller();

    //For showing explicitly a ScrollBar if policy is Symbian.ScrollBarWhenScrolling
    function flash(type) {
        if (policy == Symbian.ScrollBarWhenScrolling && internal.scrollBarNeeded) {
            flashEffect.type = (type == undefined) ? Symbian.FadeOut : type
            flashEffect.restart()
        }
    }

    Connections {
        target: screen
        onCurrentOrientationChanged: flash()
    }

    Connections {
        target: root.privateSectionScroller ? flickableItem : null
        onModelChanged: internal.initSectionScroller()
    }

    Loader {
        id: listItemInteraction
        sourceComponent: flickableItem && flickableItem.hasOwnProperty("currentIndex") && flickableItem.activeFocus ? listItemInteractionComponent : undefined
    }

    Component {
        id: listItemInteractionComponent
        Item {
            Connections {
                target: symbian
                onListInteractionModeChanged: flash()
                onPrivateListItemKeyNavigation: flash()
            }
        }
    }

    QtObject {
        id: internal
        property int hideTimeout: 500
        property bool scrollBarNeeded: root.visible && hasScrollableContent()
        //Sets currentSection to empty string when flickableItem.currentSection is null
        property string currentSection: flickableItem ? flickableItem.currentSection || "" : ""
        //To be able to pressed on trackMouseArea, opacity needs to be greater than 0
        property real rootOpacity: root.privateSectionScroller ? 0.01 : 0

        // The following properties are used for calculating handle position and size:
        // Minimum allowed handle length
        property real minHandleLength: 3 * Math.floor(privateStyle.scrollBarThickness)
        // Scroll bar "container" length
        property real totalLength: !flickableItem ? NaN : (orientation == Qt.Vertical ? root.height : root.width)
        property real relativeHandlePosition: !flickableItem ? NaN : (orientation == Qt.Vertical
                                                                      ? flickableItem.visibleArea.yPosition
                                                                      : flickableItem.visibleArea.xPosition)
        property real lengthRatio: !flickableItem ? NaN : (orientation == Qt.Vertical
                                                           ? flickableItem.visibleArea.heightRatio
                                                           : flickableItem.visibleArea.widthRatio)
        // Scroll bar handle length under normal circumstances
        property real staticHandleLength: Math.max(minHandleLength, lengthRatio * totalLength)
        // Adjustment factor needed to calculate correct position, which is needed because
        // relativeHandlePosition and lengthRatio are assuming default handle length. Dividend is
        // the maximum handle position taking into account restricted minimum handle length, whereas
        // divisor is the maximum handle position if default handle size would be used.
        property real handlePositionAdjustment: (totalLength - staticHandleLength) / (totalLength * (1 - lengthRatio))
        property real handlePosition
        handlePosition: {
            // handle's position, which is adjusted to take into account non-default handle
            // length and restricted never to exceed flickable boundaries
            var pos = totalLength * relativeHandlePosition * handlePositionAdjustment
            return Math.min(Math.max(0, pos), totalLength - minHandleLength)
        }
        property real dynamicHandleLength
        dynamicHandleLength: {
            // dynamic handle length, which may differ from static due to bounds behavior, i.e.
            // handle gets shorter when content exceeds flickable boundaries
            var length = 0
            if (relativeHandlePosition >= 1 - lengthRatio) // overflow
                length = 1 - relativeHandlePosition
            else if (relativeHandlePosition < 0) // underflow
                length = lengthRatio + relativeHandlePosition
            else
                length = lengthRatio
            return Math.max(minHandleLength, length * totalLength)
        }

        // Show flash effect in case if no flicking (flickableItem.moving) has occured
        // but there is a need to indicate scrollable content.
        onScrollBarNeededChanged: {
            if (stateGroup.state == "") {
                if (scrollBarNeeded)
                    flash()
                else
                    idleEffect.restart()
            }
        }

        /**
         * Checks whether ScrollBar is needed or not
         * based on Flickable visibleArea height and width ratios
         */
        function hasScrollableContent() {
            if (!flickableItem)
                return false
            var ratio = orientation == Qt.Vertical ? flickableItem.visibleArea.heightRatio : flickableItem.visibleArea.widthRatio
            return ratio < 1.0 && ratio > 0
        }

        /**
         * Does movement of flickableItem
         * when ScrollBar Handle is being dragged
         */
        function moveToHandlePosition() {
            if (orientation == Qt.Vertical)
                flickableItem.contentY = handle.y / (height-staticHandleLength) * (flickableItem.contentHeight-flickableItem.height)
            else
                flickableItem.contentX = handle.x / (width-staticHandleLength) * (flickableItem.contentWidth-flickableItem.width)
        }

        function adjustContentPosition(y) {
            if (y < 0 || y > trackMouseArea.height)
                return;

            var sect = Sections.closestSection(flickableItem, y / trackMouseArea.height);
            var idx = Sections.indexOf(flickableItem, sect);
            currentSection = sect;
            flickableItem.positionViewAtIndex(idx, ListView.Beginning);
        }

        function initSectionScroller() {
            if (root.privateSectionScroller && flickableItem && flickableItem.model)
                Sections.initSectionData(flickableItem);
        }
    }

    Loader {
        id: sectionScrollBackground
        anchors.right: trackMouseArea.right
        width: flickableItem ? flickableItem.width : 0
        height: platformStyle.fontSizeMedium * 5
        sourceComponent: root.privateSectionScroller ? sectionScrollComponent : null
    }

    Component {
        id: sectionScrollComponent
        BorderImage {
            id: indexFeedbackBackground
            objectName: "indexFeedbackBackground"
            source: privateStyle.imagePath("qtg_fr_popup_transparent", root.platformInverted)
            border { left: platformStyle.borderSizeMedium; top: platformStyle.borderSizeMedium; right: platformStyle.borderSizeMedium; bottom: platformStyle.borderSizeMedium }
            visible: trackMouseArea.pressed
            anchors.fill: parent
            Text {
                id: indexFeedbackText
                objectName: "indexFeedbackText"
                color: root.platformInverted ? platformStyle.colorNormalDarkInverted // intentionally dark inverted
                                             : platformStyle.colorNormalLight
                anchors {
                    left: parent.left;
                    leftMargin: platformStyle.paddingLarge;
                    right: parent.right;
                    rightMargin: platformStyle.paddingLarge;
                    verticalCenter: parent.verticalCenter
                }
                font {
                    family: platformStyle.fontFamilyRegular;
                    pixelSize: indexFeedbackText.text.length == 1 ? platformStyle.fontSizeMedium * 4 : platformStyle.fontSizeMedium * 2;
                    capitalization: indexFeedbackText.text.length == 1 ? Font.AllUppercase : Font.MixedCase
                }
                text: internal.currentSection
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
            }
            states: [
                State {
                    when: (handle.y + (handle.height / 2)) - (sectionScrollBackground.height / 2) < 0
                    AnchorChanges {
                        target: sectionScrollBackground
                        anchors { verticalCenter: undefined; top: handle.top; bottom: undefined }
                    }
                },
                State {
                    when: (handle.y + (handle.height / 2)) + (sectionScrollBackground.height / 2) >= trackMouseArea.height
                    AnchorChanges {
                        target: sectionScrollBackground
                        anchors { verticalCenter: undefined; top: undefined; bottom: handle.bottom }
                    }
                },
                State {
                    when: (handle.y + (handle.height / 2)) - (sectionScrollBackground.height / 2) >= 0
                    AnchorChanges {
                        target: sectionScrollBackground
                        anchors { verticalCenter: handle.verticalCenter; top: undefined; bottom: undefined }
                    }
                }
            ]
        }
    }

    // MouseArea for section scrolling
    MouseArea {
        id: trackMouseArea
        objectName: "trackMouseArea"
        enabled: root.privateSectionScroller
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        width: privateStyle.scrollBarThickness * 3
        drag {
            target: sectionScrollBackground
            // axis is set XandY to prevent flickable from stealing the mouse event
            // SectionScroller is anchored to the right side of the mouse area so the user
            // won't be able to drag it along the X axis
            axis: Drag.XandYAxis
            minimumY: flickableItem ? flickableItem.y : 0
            maximumY: flickableItem ? (trackMouseArea.height - sectionScrollBackground.height) : 0
        }

        onPositionChanged: {
            if (root.privateSectionScroller)
                internal.adjustContentPosition(trackMouseArea.mouseY);
        }

        onPressedChanged: {
            if (root.privateSectionScroller && trackMouseArea.pressed)
                internal.adjustContentPosition(trackMouseArea.mouseY);
        }
    }

    BorderImage {
        id: handle
        objectName: "handle"
        source: privateStyle.imagePath(handleFileName(), root.platformInverted)
        x: orientation == Qt.Horizontal ? internal.handlePosition : NaN
        y: orientation == Qt.Vertical ? internal.handlePosition : NaN
        height: orientation == Qt.Vertical ? internal.dynamicHandleLength : root.height
        width: orientation == Qt.Horizontal ? internal.dynamicHandleLength : root.width
        border.right: orientation == Qt.Horizontal ? 7 : 0
        border.left: orientation == Qt.Horizontal ? 7 : 0
        border.top: orientation == Qt.Vertical ? 7 : 0
        border.bottom: orientation == Qt.Vertical ? 7 : 0

        function handleFileName() {
            var fileName = (orientation == Qt.Vertical ? "qtg_fr_scrollbar_v_handle_" :
                            "qtg_fr_scrollbar_h_handle_")
            if (!interactive)
                fileName += "indicative"
            else if (handleMouseArea.pressed)
                fileName += "pressed"
            else
                fileName += "normal"
            return fileName
        }
    }

    MouseArea {
        id: handleMouseArea
        objectName: "handleMouseArea"
        property real maxDragY: flickableItem ? flickableItem.height - handle.height - root.anchors.topMargin - root.anchors.bottomMargin : NaN
        property real maxDragX: flickableItem ? flickableItem.width - handle.width - root.anchors.leftMargin - root.anchors.rightMargin : NaN
        enabled: interactive && !root.privateSectionScroller
        width: orientation == Qt.Vertical ? 3 * privateStyle.scrollBarThickness : handle.width
        height: orientation == Qt.Horizontal ? 3 * privateStyle.scrollBarThickness : handle.height
        anchors {
            verticalCenter: flickableItem ? handle.verticalCenter : undefined
            horizontalCenter: flickableItem ? handle.horizontalCenter : undefined
        }
        drag {
            target: handle
            axis: orientation == Qt.Vertical ? Drag.YAxis : Drag.XAxis
            minimumY: 0
            maximumY: maxDragY
            minimumX: 0
            maximumX: maxDragX
        }
        onPositionChanged: internal.moveToHandlePosition()
    }

    PropertyAnimation {
        id: indicateEffect

        function play() {
            flashEffect.stop()
            idleEffect.stop()
            restart()
        }

        target: root
        property: "opacity"
        to: 1
        duration: 0
    }
    SequentialAnimation {
        id: idleEffect

        function play() {
            indicateEffect.stop()
            if (internal.scrollBarNeeded && root.policy == Symbian.ScrollBarWhenScrolling)
                restart()
        }

        PauseAnimation { duration: root.interactive ? 1500 : 0 }
        PropertyAnimation {
            target: root
            property: "opacity"
            to: internal.rootOpacity
            duration: internal.hideTimeout
        }
    }
    SequentialAnimation {
        id: flashEffect
        property int type: Symbian.FadeOut

        PropertyAnimation {
            target: root
            property: "opacity"
            to: 1
            duration: (flashEffect.type == Symbian.FadeInFadeOut) ? internal.hideTimeout : 0
        }
        PropertyAnimation {
            target: root
            property: "opacity"
            to: internal.rootOpacity
            duration: internal.hideTimeout
        }
    }
    StateGroup {
        id: stateGroup
        states: [
            State {
                name: "Move"
                when: handleMouseArea.pressed
            },
            State {
                name: "Indicate"
                when: (internal.scrollBarNeeded && flickableItem.moving) ||
                      (trackMouseArea.pressed && root.privateSectionScroller)
            },
            State {
                name: ""
            }
        ]
        transitions: [
            Transition {
                to: "Move"
                ScriptAction { script: privateStyle.play(Symbian.BasicSlider) }
                ScriptAction { script: indicateEffect.play() }
            },
            Transition {
                from: "Move"
                to: "Indicate"
                ScriptAction { script: privateStyle.play(Symbian.BasicSlider) }
                ScriptAction { script: indicateEffect.play() }
            },
            Transition {
                to: "Indicate"
                ScriptAction { script: indicateEffect.play() }
            },
            Transition {
                from: "Move"
                to: ""
                ScriptAction { script: privateStyle.play(Symbian.BasicSlider) }
                ScriptAction { script: idleEffect.play() }
            },
            Transition {
                to: ""
                ScriptAction { script: idleEffect.play() }
            }
        ]
    }
}
