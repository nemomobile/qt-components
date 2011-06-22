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
import Qt.labs.components 1.1

// See MyButton.qml for details on how use Checkable and CheckableGroup to
// create a button that is checkable and can be part of a mutually exclusive
// group. This example will show this button in the different ways it can be
// used.

Item {
    width: 600
    height: 300

    // In this example, we explicitly create a CheckableGroup to use, however
    // different component sets can choose to hide this complexity, in a
    // container Item that will automatically set the group property.
    CheckableGroup {
        id: group
    }

    // Three buttons part of a mutually exclusive group.
    Row {
        id: buttons
        x: 30
        y: 30
        spacing: 5
        property int count: 3

        MyButton {
            text: "Button 1"
            exclusiveGroup: group
        }

        MyButton {
            text: "Button 2"
            exclusiveGroup: group
            checked: true
        }

        MyButton {
            text: "Button 3"
            exclusiveGroup: group
        }
    }

    // This two-state toggle is not part of a group, and is used to
    // control the visibility of "advanced" features.
    MyButton {
        id: advanced
        anchors {
            top: buttons.bottom
            topMargin: 40
            left: buttons.left
        }
        text: "More..."
        checkable: true
    }

    Text {
        visible: advanced.checked
        anchors {
            verticalCenter: advanced.verticalCenter
            left: advanced.right
            leftMargin: 20
        }
        text: "The selected one is '" + group.selectedValue + "'"
    }

    // Here the button is used as a regular clickable button. For this kind of
    // button, the interesting thing is to watch the 'clicked' signal.
    MyButton {
        visible: advanced.checked
        text: "Add another!"

        anchors {
            left: advanced.left
            top: advanced.bottom
            topMargin: 20
        }

        // When clicked, this will add another button to the exclusive group.
        onClicked: {
            buttons.count += 1;
            var b = Qt.createQmlObject("MyButton { checkable: true }", buttons);
            b.text = "Button " + buttons.count;
            b.exclusiveGroup = group;
        }
    }
}
