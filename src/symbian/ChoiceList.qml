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

import QtQuick 1.0
import "." 1.0
import "AppManager.js" as AppManager


ImplicitSizeItem {
    id: root

    property alias model: modelCreator.model
    property int currentIndex: 0
    property alias currentValue: valueText.text

    signal closed()

    implicitWidth: style.current.get("listIndicatorWidth") * 2
    implicitHeight: style.current.get("listIndicatorHeight")

    Keys.onPressed: {
        if ((event.key == Qt.Key_Select || event.key == Qt.Key_Return) && internal.popupObject == null)
            internal.showPopup()
    }

    // Deprecated since 29-03-2011
    Component.onCompleted: console.log("warning: ChoiceList is deprecated. Use SelectionListItem and SelectionDialog instead!")

    QtObject {
        id: internal

        property int animationDuration: 100
        property int animationDurationLandscape: 120
        property int visibleItems: 0
        property variant popupObject: null
        property Item applicationRoot: AppManager.rootObject()

        function mapRootToApplicationRoot() {
            return mapToItem(applicationRoot, choiceList.x, choiceList.y)
        }

        function getPopupAlignment() {
            var ret = Qt.AlignVCenter
            if (screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted) {
                var pos = mapRootToApplicationRoot()
                var downside = style.current.get("appRectHeight") - (pos.y + choiceList.height)
                var upside = pos.y
                if (upside > downside)
                    ret = Qt.AlignTop
                else
                    ret = Qt.AlignBottom
            }
            return ret
        }

        function animationTime() {
            var timeout = animationDurationLandscape
            if (screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted)
                timeout = visibleItems * animationDuration
            return timeout
        }

        function popupPos(listHeight) {
            var rootPos = mapRootToApplicationRoot()
            var alignment = getPopupAlignment()
            if (alignment == Qt.AlignBottom)
                rootPos.y += choiceList.height
            else if (alignment == Qt.AlignTop)
                rootPos.y -= listHeight
            else if (alignment == Qt.AlignVCenter)
                rootPos.y = style.current.get("appRectHeight") / 2 - listHeight / 2
            return rootPos
        }

        function showPopup() {
            console.log("warning: ChoiceList is deprecated. Use SelectionListItem and SelectionDialog instead!")
            popupObject = popup.createObject(applicationRoot)
            if (popupObject != null) {
                if (screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted)
                    popupObject.state = "Visible"
                else
                    popupObject.state = "VisibleLandscape"
            }
        }

        function hidePopup() {
            console.log("warning: ChoiceList is deprecated. Use SelectionListItem and SelectionDialog instead!")
            if (screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted)
                popupObject.state = "Hidden"
            else
                popupObject.state = "HiddenLandscape"

            style.mode = "normal"
            popupObject.destroy(animationTime())
            root.closed()
        }
    }

    Style {
        id: style
        styleClass: "ChoiceList"
        mode: root.enabled ? "normal" : "disabled"
    }

    Repeater {
        id: modelCreator
        Item {
            Component.onCompleted: optionsModel.append({"optionText": modelData})
        }
    }

    ListModel {
        id: optionsModel
    }

    Item {
        id: choiceList

        anchors.fill: parent

        Item {
            id: valueArea

            anchors { top: parent.top; bottom: parent.bottom; left: parent.left }
            width: parent.width - listIndicator.width > 0 ? parent.width - listIndicator.width : listIndicator.width

            BorderImage {
                source: style.current.get("background")
                border { left: 10; top: 10; right: 10; bottom: 10 }
                smooth: true
                anchors.fill: parent
            }

            Text {
                id: valueText

                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: style.current.get("marginLeft")
                    rightMargin: style.current.get("marginMiddle")
                    verticalCenter: parent.verticalCenter
                }
                height: style.current.get("textHeight")
                text: (optionsModel.count != 0 && root.currentIndex >= 0 && root.currentIndex < optionsModel.count)
                    ? optionsModel.get(root.currentIndex).optionText : ""
                font: style.current.get("font")
                color: style.current.get("textColor")
            }
        }

        Item {
            id: listIndicator

            anchors { top: parent.top; bottom: parent.bottom; left: valueArea.right }
            width: style.current.get("listIndicatorWidth");

            Image {
                source: style.current.get("listIndicatorImage")
                smooth: true
                anchors.fill: parent
            }
        }

        MouseArea {
            property bool exited: false

            anchors.fill: parent
            enabled: root.enabled

            onPressed: {
                style.mode = "pressed"
                exited = false
            }
            onReleased: {
                if (containsMouse && !exited) {
                    if (internal.popupObject == null)
                        internal.showPopup()
                    else
                        internal.hidePopup()
                }
            }
            onExited: {
                exited = true
                if (internal.popupObject == null)
                    style.mode = "normal"
                else if (internal.popupObject.state != "Hidden" && internal.popupObject.state != "HiddenLandscape")
                    style.mode = "latched"
            }
        }
    }

    Component {
        id: popup

        Flickable {
            id: popupFlickable

            property variant position: internal.popupPos(popupFlickable.height)

            function getContentY() {
                var ret = 0
                var alignment = internal.getPopupAlignment()
                if (alignment == Qt.AlignBottom)
                    ret = popupFlickable.height
                else if (alignment == Qt.AlignTop)
                    ret = -popupFlickable.height
                else if (alignment == Qt.AlignVCenter)
                    ret = 0
                return ret
            }

            function getPopupHeight() {
                var itemsShown = optionsModel.count
                var popupHeight = optionsModel.count * style.current.get("listItemHeight") + style.current.get("listMargin") * 2
                var alignment = internal.getPopupAlignment()
                var availableHeight = 0
                var pos = internal.mapRootToApplicationRoot()

                if (alignment == Qt.AlignBottom) //below
                    availableHeight = style.current.get("appRectHeight") - (pos.y + choiceList.height)
                else if (alignment == Qt.AlignTop) //above
                    availableHeight = pos.y
                else if (alignment == Qt.AlignVCenter) //over
                    availableHeight = style.current.get("appRectHeight")

                while (availableHeight < popupHeight) {
                    popupHeight -= style.current.get("listItemHeight")
                    itemsShown--
                }

                internal.visibleItems = itemsShown
                return popupHeight
            }

            width: choiceList.width; height: getPopupHeight()
            contentWidth: width; contentHeight: height
            interactive: false; clip: true
            state: screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted ? "Hidden" : "HiddenLandscape"
            x: position.x; y: position.y

            Timer {
                id: orientationChangeTimer
                interval: 1
                onTriggered: {
                    popupFlickable.height = popupFlickable.getPopupHeight()
                    var pos = internal.popupPos(popupFlickable.height);
                    popupFlickable.y = pos.y;
                    popupFlickable.x = pos.x;
                }
            }

            Connections {
                target: screen
                // the screen.layoutRect is not correctly updated without this timer
                onCurrentOrientationChanged: orientationChangeTimer.start()
            }

            BorderImage {
                source: style.current.get("listBackground")
                border { left: 20; top: 20; right: 20; bottom: 20 }
                smooth: true
                anchors.fill: parent
            }

            ListView {
                id: popupList

                anchors {
                    fill: parent
                    margins: style.current.get("listMargin")
                }
                height: parent.height - style.current.get("listMargin") * 2
                delegate: listDelegate
                model: optionsModel
                clip: true
                boundsBehavior: "StopAtBounds"
                focus: true
                highlight: null

                Component.onCompleted: positionViewAtIndex(root.currentIndex, ListView.End)

                Keys.onPressed: {
                    if (event.key == Qt.Key_Select || event.key == Qt.Key_Return) {
                        if (popupList.highlight != null) {
                            root.currentIndex = popupList.currentIndex
                            popupList.highlight = null
                        }
                        internal.hidePopup()
                    } else if (event.key == Qt.Key_Down || event.key == Qt.Key_Up) {
                        if (popupList.highlight == null) {
                            if (event.key == Qt.Key_Down)
                                popupList.currentIndex = -1
                            else
                                popupList.currentIndex = 0
                            popupList.highlight = popupListHighlight
                        }
                    }
                }
            }

            ScrollBar {
                flickableItem: popupList
                anchors {
                    top: popupList.top
                    right: popupList.right
                }
                width: style.current.get("scrollbarWidth")
                visible: internal.visibleItems < optionsModel.count ? true : false
            }

            Style {
                id: highlightStyle
                styleClass: "ChoiceList"
                mode: "highlight"
            }

            Component {
                id: popupListHighlight

                BorderImage {
                    source: highlightStyle.current.get("listItemBackground")
                    border { left: 20; top: 20; right: 20; bottom: 20 }
                }
            }

            Component {
                id: listDelegate

                Item {
                    width: popupList.width; height: style.current.get("listItemHeight")

                    Style {
                        id: itemStyle
                        styleClass: "ChoiceList"
                        mode: "normal"
                    }

                    BorderImage {
                        source: itemStyle.current.get("listItemBackground")
                        border { left: 20; top: 20; right: 20; bottom: 20 }
                        anchors.fill: parent
                    }

                    Item {
                        id: selectionIndicator
                        anchors {
                            left: parent.left
                            top: parent.top
                            bottom: parent.bottom
                            topMargin: itemStyle.current.get("marginTop")
                            bottomMargin: itemStyle.current.get("marginBottom")
                            leftMargin: itemStyle.current.get("marginLeft")
                        }
                        width: itemStyle.current.get("selectionIndicatorWidth")

                        Image {
                            source: style.current.get("selectionIndicatorImage")
                            smooth: true
                            visible: index == root.currentIndex
                            anchors.fill: parent
                        }
                    }

                    Text {
                        anchors {
                            left: selectionIndicator.right
                            top: parent.top
                            bottom: parent.bottom
                            topMargin: itemStyle.current.get("marginTop")
                            bottomMargin: itemStyle.current.get("marginBottom")
                            leftMargin: itemStyle.current.get("marginMiddle")
                            rightMargin: itemStyle.current.get("marginRight")
                        }
                        font: itemStyle.current.get("listItemFont")
                        color: itemStyle.current.get("listItemTextColor")
                        text: optionText
                    }

                    MouseArea {
                        anchors.fill: parent

                        onPressed: {
                            popupList.highlight = null
                            itemStyle.mode = "pressed"
                        }
                        onReleased: {
                            if (containsMouse) {
                                root.currentIndex = index
                                itemStyle.mode = "normal"
                                internal.hidePopup()
                            }
                        }
                        onCanceled: {
                            itemStyle.mode = "normal"
                        }
                        onExited: {
                            itemStyle.mode = "normal"
                        }
                    }
                }
            }

            states: [
                State {
                    name: "Hidden"
                    PropertyChanges { target: popupFlickable; contentY: getContentY(); opacity: 1 }
                },
                State {
                    name: "Visible"
                    PropertyChanges { target: popupFlickable; contentY: 0; opacity: 1 }
                },
                State {
                    name: "HiddenLandscape"
                    PropertyChanges { target: popupFlickable; contentY: 0; opacity: 0 }
                },
                State {
                    name: "VisibleLandscape"
                    PropertyChanges { target: popupFlickable; contentY: 0; opacity: 1 }
                }
            ]

            transitions: Transition {
                NumberAnimation { target: popupFlickable; properties: "contentY,opacity"; duration: internal.animationTime() }
            }
        }
    }
}
