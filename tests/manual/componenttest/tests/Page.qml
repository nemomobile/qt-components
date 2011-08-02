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
import com.nokia.symbian 1.1
import "../TestUtils.js" as Utils
import "../components"

Item {
    id: root

    property Item rootPageStack: pageStack
    property bool platformInverted: false

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

        initialPage: pageComponent
    }

    Component {
        id: pageComponent

        Page {
            id: testPage

            property string title

            Component.onCompleted: {
                title = "Page " + (rootPageStack.depth + 1).toString()
                // manually set inversion mode for dynamically created item tree
                Utils.setItemTreeInversion(testPage, root.platformInverted)
            }

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Row {
                    spacing: 5
                    Label { text: "depth: [ " + rootPageStack.depth + " ]" }
                    Label { text: "| page: [ " + testPage.title + " ]" }
                }

                Label {
                    text: "Orientation Settings"
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

