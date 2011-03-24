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
import "." 1.0

Item {
    id: root

    // The status of the page. One of the following:
    //      PageStatus.Inactive - the page is not visible
    //      PageStatus.Activating - the page is transitioning into becoming the active page
    //      PageStatus.Active - the page is the current active page
    //      PageStatus.Deactivating - the page is transitioning into becoming inactive
    property int status: PageStatus.Inactive

    property PageStack pageStack

    // Defines orientation lock for a page
    property int orientationLock: PageOrientation.Automatic

    property Item tools: null

    visible: false

    width: visible && parent ? parent.width : internal.previousWidth
    height: visible && parent ? parent.height : internal.previousHeight

    onWidthChanged: internal.previousWidth = visible ? width : internal.previousWidth
    onHeightChanged: internal.previousHeight = visible ? height : internal.previousHeight

    onStatusChanged: {
        if (status == PageStatus.Activating)
            internal.orientationLockCheck();
    }

    onOrientationLockChanged: {
        if (status == PageStatus.Activating || status == PageStatus.Active)
            internal.orientationLockCheck();
    }

    QtObject {
        id: internal
        property int previousWidth: 0
        property int previousHeight: 0

        function isScreenInPortrait() {
            return screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted;
        }

        function isScreenInLandscape() {
            return screen.currentOrientation == Screen.Landscape || screen.currentOrientation == Screen.LandscapeInverted;
        }

        function orientationLockCheck() {
            switch (orientationLock) {
            case PageOrientation.Automatic:
                screen.allowedOrientations = Screen.Default
                break
            case PageOrientation.LockPortrait:
                screen.allowedOrientations = Screen.Portrait
                break
            case PageOrientation.LockLandscape:
                screen.allowedOrientations = Screen.Landscape
                break
            case PageOrientation.LockPrevious:
                screen.allowedOrientations = screen.currentOrientation
                break
            case PageOrientation.Manual:
            default:
                // Do nothing
                // In manual mode it is expected that orientation is set
                // explicitly to "screen.allowedOrientations" by the user.
                break
            }
        }
    }
}
