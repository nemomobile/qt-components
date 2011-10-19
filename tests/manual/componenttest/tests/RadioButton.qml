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
import Qt.labs.components 1.1

Item {
    id: root

    CheckableGroup { id: group1 }
    CheckableGroup { id: group2 }
    CheckableGroup { id: group3 }

    Column {
        anchors.fill: parent

        Row {
            // Don't mirror ascending number order
            LayoutMirroring.enabled: false
            LayoutMirroring.childrenInherit: false

            RadioButton {
                id: button1
                text: "1"
                platformExclusiveGroup: group1
                focus: true
            }
            RadioButton {
                id: button2
                text: "2"
                platformExclusiveGroup: group1
                checked: true
            }
            RadioButton {
                id: button3
                text: "3"
                platformExclusiveGroup: group1
            }
        }

        Row {
            // Don't mirror ascending number order
            LayoutMirroring.enabled: false
            LayoutMirroring.childrenInherit: false

            RadioButton {
                id: button4
                text: "4"
                platformExclusiveGroup: group2
            }
            RadioButton {
                id: button5
                text: "5"
                platformExclusiveGroup: group2
                checked: true
            }
            RadioButton {
                id: button6
                text: "6"
                platformExclusiveGroup: group2
            }
        }

        Grid {
            // Don't mirror ascending number order
            LayoutMirroring.enabled: false
            LayoutMirroring.childrenInherit: false

            columns: 3

            Text {
                color: "red"
                text: "Disabled"
            }
            Text {
                color: "red"
                text: "No group set"
            }
            Text {
                color: "red"
                text: "Single group member"
            }

            // Disabled
            RadioButton {
                id: button7
                text: "7"
                enabled: false
            }
            // No group set
            RadioButton {
                id: button8
                text: "8"
            }
            // Single group member
            RadioButton {
                id: button9
                text: "9"
                platformExclusiveGroup: group3
            }
        }
        RadioButton {
            id: longTextButton
            width: parent.width
            text: "The long line of text is sad because he does not fit on a small screen"
        }
    }
}
