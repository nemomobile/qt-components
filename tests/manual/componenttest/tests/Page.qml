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
import com.nokia.symbian 1.0

Item {
    id: root

    property Item rootPageStack: pageStack

    function inPortrait() {
        return screen.height > screen.width
    }

    function resolveOrientationLockValue(lockPortrait, lockLandscape) {
        if (lockPortrait && lockLandscape)
            return PageOrientation.LockPrevious
        else if (lockPortrait && !lockLandscape)
            return PageOrientation.LockPortrait
        else if (!lockPortrait && lockLandscape)
            return PageOrientation.LockLandscape
        else
            return PageOrientation.Automatic
    }

    PageStack {
        id: pageStack
    }

    Component.onCompleted: {
        pageStack.push(pageComponent)
    }

    Component {
        id: pageComponent

        Page {
            id: testPage

            property string title

            Component.onCompleted: {
                title = "Page " + (rootPageStack.depth + 1).toString()
            }

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Row {
                    spacing: 5
                    Text { color: "white"; font.pointSize: 10; text: "depth: [ " + rootPageStack.depth + " ]" }
                    Text { color: "white"; font.pointSize: 10; text: "| page: [ " + testPage.title + " ]" }
                }

                Text {
                    color: "white"
                    font.pointSize: 12
                    text: "Orientation Settings"
                    font.bold: true
                }

                Row {
                    spacing: 3
                    width: parent.width

                    Button {
                        id: landlockButton
                        text: "Land lock"
                        checkable: true
                        width: parent.width / 3
                        onCheckedChanged: {
                            testPage.orientationLock = resolveOrientationLockValue(portlockButton.checked, checked)
                        }
                    }

                    Button {
                        id: portlockButton
                        text: "Port lock"
                        checkable: true
                        width: parent.width / 3
                        onCheckedChanged: {
                            testPage.orientationLock = resolveOrientationLockValue(checked, landlockButton.checked)
                        }
                    }

                    Button {
                        text: "Unlock"
                        width: parent.width / 3
                        onClicked: {
                            landlockButton.checked = false
                            portlockButton.checked = false
                        }
                    }
                }

                Row {
                    spacing: 3
                    width: parent.width

                    Button {
                        text: "Back"
                        width: parent.width / 3
                        onClicked: {
                            rootPageStack.pop()
                        }
                    }

                    Button {
                        text: "Clear stack"
                        width: parent.width / 3
                        onClicked: {
                            rootPageStack.clear()
                            rootPageStack.push(pageComponent)
                        }
                    }

                    Button {
                        text: "New page"
                        width: parent.width / 3
                        onClicked: {
                            rootPageStack.push(pageComponent);
                        }
                    }
                }

                Row {
                    spacing: 3
                    width: parent.width
                    Rectangle { color: "blue"; height: 40; width: parent.width; opacity: rootPageStack.busy ? 1 : 0.1 }
                }
            }
        }
    }
}

