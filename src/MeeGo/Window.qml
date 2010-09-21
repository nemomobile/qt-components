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

import "pagemanager.js" as PageManager
import "ActionManager.js" as ActionManager

Item {
    id: window

    state: screen.orientationString;

    property bool statusbarVisible: true
    property bool titlebarVisible: true
    property bool fullscreen: false

    property double __scrollOffset: 0
    property double autoScroll: __scrollOffset

    property variant currentPage: null

    property list<Action> actions

    function showQuery(title, message, callback) {
        decoration.showQuery(title, message, callback)
    }

    Behavior on autoScroll {
        NumberAnimation { easing.type: Easing.InOutQuad; duration: 200 }
    }

    Snapshot {
        id: snapshot
        anchors.centerIn: parent
    }

    states:  [
        State {
            name: "Landscape"
            PropertyChanges {
                target: window
                rotation: 0
                width: screen.width
                height: screen.height
                x: 0
                y: 0
            }
        },
        State {
            name: "LandscapeInverted"
            PropertyChanges {
                target: window
                rotation: 180
                width: screen.width
                height: screen.height
                x: 0
                y: 0
            }
        },
        State {
            name: "Portrait"
            PropertyChanges {
                target: window
                rotation: 270
                width: screen.height
                height: screen.width
                x: (screen.width - screen.height) / 2
                y: -(screen.width - screen.height) / 2
            }
        },
        State {
            name: "PortraitInverted"
            PropertyChanges {
                target: window
                rotation: 90
                width: screen.height
                height: screen.width
                x: (screen.width - screen.height) / 2
                y: -(screen.width - screen.height) / 2
            }
        }
    ]

    transitions: Transition {
        SequentialAnimation {
            ScriptAction {
                script: {
                    snapshot.take()
                    snapshot.opacity = 1
                    snapshot.rotation = -window.rotation
                    window.opacity = 0
                    __scrollOffset = 0
                }
            }
            PropertyAction { target: window; properties: "x,y,width,height" }
            ParallelAnimation {
                NumberAnimation { target: window; property: "opacity"; to: 1; duration: 300 }
                NumberAnimation { target: snapshot; property: "opacity"; to: 0; duration: 300 }
                RotationAnimation { target: window; property: "rotation"; direction: RotationAnimation.Shortest; easing.type: Easing.InOutQuad; duration: 300 }
            }
            ScriptAction { script: { snapshot.free(); window.scrollPageIfRequired() } }
        }
    }

    Item {
        id: pages
        anchors.fill: parent
        anchors.topMargin: autoScroll
    }

    Component {
        id: pageContainerComponent
        PageContainer {
            topDecorationHeight: decoration.topDecorationHeight
            bottomDecorationHeight: decoration.bottomDecorationHeight
        }
    }

    // this function receives a Page Component as argument, sets
    // it as the current page and initiates the transition animation.
    // during a running page change animation it will be ignored and returns 'false'.
    function nextPage(pageComponent) {
        var pageContainerObj = pageContainerComponent.createObject(pages)
        if (pageContainerObj == null)
            return false
        var page = PageManager.nextPage(pageComponent, pageContainerObj)
        if (page == null) {
            pageContainerObj.destroy()
            return false
        }

        window.currentPage = page
        ActionManager.setActionsForPage(window.currentPage.actions);
        return true
    }

    // this function sets the previous in the navigation history as the
    // current Page, initiates the back animation and deletes the old current after
    // the animation finishes.
    // it cannot run again before finishing the animation, so the navigation is
    // one page at a time. This function returns 'false' when it ignores the prevPage request.
    function prevPage() {
        var page = PageManager.prevPage()
        if (page != null) {
            window.currentPage = page
            ActionManager.setActionsForPage(window.currentPage.actions);
            return true
        }
        return false
    }

    WindowDecoration {
        id: decoration
        anchors.fill: parent
        anchors.topMargin: autoScroll

        orientation: screen.orientation
        statusbarVisible: !window.fullscreen && window.statusbarVisible
        titlebarVisible: !window.fullscreen && window.titlebarVisible
        title: window.currentPage ? window.currentPage.title : ""
        toolbar: toolbarContents

        onMinimize: screen.minimized = true
        onQuit: Qt.quit()
        onBackClicked: window.prevPage()
    }

    Connections {
        target: screen
        onMicroFocusChanged: { scrollPageIfRequired() }
    }

    Connections {
        target: screen
        onSoftwareInputPanelVisibleChanged: { scrollPageIfRequired() }
    }

    function scrollPageIfRequired() {
        if (screen.softwareInputPanelVisible) {
            var mf = screen.microFocus;
            var mfy = mf.y + mf.height/2;
            var mfx = mf.x + mf.width/2;

            var sipRect = screen.softwareInputPanelRect;
            var max;

            switch (screen.orientation) {
            case Screen.Portrait:
                __scrollOffset += sipRect.x/2 - mfx;
                max = sipRect.width;
                break;
            case Screen.PortraitInverted:
                __scrollOffset += mfx - (screen.width + sipRect.width)/2;
                max = sipRect.width;
                break;
            case Screen.Landscape:
                __scrollOffset += sipRect.y/2 - mfy;
                max = sipRect.height;
                break;
            case Screen.LandscapeInverted:
                __scrollOffset +=  mfy - (screen.height + sipRect.height)/2;
                max = sipRect.height;
                break;
            }
            if (__scrollOffset < -max)
                __scrollOffset = -max;
            if (__scrollOffset > 0)
                __scrollOffset = 0;
        } else {
            __scrollOffset = 0;
        }
    }

    Row {
        id: toolbarContents

        property bool hasInteractiveActions: false

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom


        // ### Still doesn't match MTF layout for the case of custom items for actions
        spacing: {
            var i;
            var childrenWidth = 0;

            if (!children.length) {
                return 0;
            }

            for (i = 0; i < children.length; i++) {
                childrenWidth += children[i].width;
            }

            return (parent.width - childrenWidth) / (children.length + 1);
        }
    }

    Component.onCompleted: {
        ActionManager.setActionsForApp(window.actions);
    }
}
