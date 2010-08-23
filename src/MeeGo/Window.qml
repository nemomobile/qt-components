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

Rectangle {
    id: window

    state: screen.orientationString;

    property bool statusbarVisible: true
    property bool titlebarVisible: true;
    property bool fullscreen: false;

    property double __pageX: 0;
    property double __pageY: decoration.y + decoration.height;
    property alias __pageWidth: window.width;
    property double __pageHeight: height - __pageY + autoScroll;
    property double __scrollOffset: 0;
    property double autoScroll: __scrollOffset;

    property variant __currentPage: null
    property int __currentPageIdx: -1

//    Behavior on autoScroll {
//        NumberAnimation { easing.type: Easing.InOutQuad; duration: 200; }
//    }

    Snapshot {
        id: snapshot;
        anchors.centerIn: parent;
    }

    states:  [
        State {
            name: "Landscape"
            PropertyChanges {
                target: window;
                rotation: 0
                width: screen.width;
                height: screen.height;
                x: 0;
                y: 0;
            }
        },
        State {
            name: "LandscapeInverted"
            PropertyChanges {
                target: window;
                rotation: 180
                width: screen.width;
                height: screen.height;
                x: 0;
                y: 0;
            }
        },
        State {
            name: "Portrait"
            PropertyChanges {
                target: window;
                rotation: 270
                width: screen.height;
                height: screen.width;
                x: (screen.width - screen.height)/2;
                y: -(screen.width - screen.height)/2;
            }
        },
        State {
            name: "PortraitInverted"
            PropertyChanges {
                target: window;
                rotation: 90
                width: screen.height;
                height: screen.width;
                x: (screen.width - screen.height)/2;
                y: -(screen.width - screen.height)/2;
            }
        }
    ]

    transitions: Transition {
        SequentialAnimation {
            ScriptAction {
                script: {
                    snapshot.take();
                    snapshot.opacity = 1;
                    snapshot.rotation = -window.rotation;
                    window.opacity = 0;
                    __scrollOffset = 0;
                }
            }
            PropertyAction { target: window; properties: "x,y,width,height" }
            ParallelAnimation {
                NumberAnimation { target: window; property: "opacity"; to: 1; duration: 300 }
                NumberAnimation { target: snapshot; property: "opacity"; to: 0; duration: 300 }
                RotationAnimation { target: window; property: "rotation"; direction: RotationAnimation.Shortest; easing.type: Easing.InOutQuad; duration: 300 }
            }
            ScriptAction { script: { snapshot.free(); window.scrollPageIfRequired(); } }
        }
    }

    // this function receives a Page Component as argument, sets
    // it as the current page and initiates the transition animation.
    // during a running page change animation it will be ignored and returns 'false'.
    function nextPage(pageComponent) {
        if (pageAnimation.running)
            return false

        __currentPage = pageComponent.createObject(pages)
        __currentPageIdx++

        if (__currentPageIdx < 1)
            return true

        pages.x -= __pageWidth
        titlebar.showBackButton = true
        return true
    }

    // this function sets the previous in the navigation history as the
    // current Page, initiates the back animation and deletes the old current after
    // the animation finishes.
    // it cannot run again before finishing the animation, so the navigation is
    // one page at a time. This function returns 'false' when it ignores the prevPage request.
    function prevPage() {
        if (__currentPageIdx < 1 || pageAnimation.running)
            return false

        __currentPage.destroy(pageAnimation.duration + 250)
        __currentPage = pages.children[--__currentPageIdx]

        pages.x += __pageWidth

        if (__currentPageIdx == 0)
            titlebar.showBackButton = false
        return true
    }

    // add page before the title and status bar
    Row {
        id: pages
        Behavior on x { NumberAnimation { id: pageAnimation } }
    }

    Column {
        id: decoration;
        y: autoScroll;
        width: parent.width;
        height: titlebar.y + titlebar.height;

        StatusBar {
            id: statusbar;

            width: parent.width;
            orientation: screen.orientation;

            states: State {
                    name: 'hidden';
                    when:  (window.statusbarVisible == false);
                    PropertyChanges { target: statusbar; y: -statusbar.height; }
            }

            transitions: Transition {
                NumberAnimation { target: statusbar; property: "y"; duration: 300; easing.type: Easing.InOutQuad }
            }
        }
        TitleBar {
            id: titlebar;
            y: statusbar.y + statusbar.height;
            onMinimize: { screen.minimized = true; }
            onQuit:  { Qt.quit(); }
            onBackClicked: { prevPage() }

            states: State {
                    name: 'hidden';
                    when:  (window.titlebarVisible == false);
                    PropertyChanges { target: titlebar; y: statusbar.y + statusbar.height - titlebar.height; }
            }
            transitions: Transition {
                NumberAnimation { target: titlebar; property: "y"; duration: 300; easing.type: Easing.InOutQuad }
            }
        }

        states: State {
                name: 'hidden';
                when:  (window.fullscreen == true);
                PropertyChanges { target: decoration; y: autoscroll - decoration.height; }
        }

        transitions: Transition {
            NumberAnimation { target: decoration; property: "y"; duration: 300; easing.type: Easing.InOutQuad }
        }

    }

    Connections {
        target: screen;
        onMicroFocusChanged: { scrollPageIfRequired(); }
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

}
