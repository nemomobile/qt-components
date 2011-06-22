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

/// This is an example of a button component with 'checkable' abilities, it can
/// be a two-state toggle or part of a exclusive group. The contents of this
/// example is in an unusual order to emphasize the role of each component in
/// the tree.

Item {
    id: root

    width: 100
    height: 30

    // Our button has a background, which the color depends on whether it is
    // 'pressed' or 'checked'.
    Rectangle {
        id: background
        anchors.fill: root

        // Due to QML property binding, if either 'pressed' or 'checked' changes
        // then the color is recalculated.
        color: {
            if (pressed)
                return "crimson";
            if (checked)
                return "gold";
            return "tomato";
        }

    }

    // This inner rectangle complements the background, the lighter color and
    // smaller size makes the other rectangle looks like a border.
    Rectangle {
        color: { Qt.lighter(background.color); }
        anchors.fill: root
        anchors.margins: 2
    }


    // The first new property our button exposes is the 'text'. Since it is
    // only used to set the caption of our button with an Text QML item, we can
    // just alias the property.
    //
    // For users of this button, there shouldn't be a difference between using a
    // common property and an alias property, this is an implementation detail.
    property alias text: label.text

    Text {
        id: label
        anchors.fill: root
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }


    // Users of our button would like to know whether it is pressed, and also
    // have the convenience of being signalled when it was clicked. So we do
    // that by having a new property and signal.
    property alias pressed: area.pressed
    signal clicked

    // To implement it, we use a MouseArea QML item, which is the regular way
    // of tracking mouse events.
    MouseArea {
        id: area
        anchors.fill: root

        // When the mouse area is clicked, we want to emit the 'clicked' signal
        // of our button (root) and also tell the Checkable component that it
        // needs to toggle itself (more details in the next comment).
        onClicked: {
            ch.toggle();
            root.clicked();
        }
    }


    // Since this button can be used as a toggle button, we also have a
    // property to indicate this condition, and whether it is checked or
    // not. In both cases we "delegate" those by aliasing them to the Checkable
    // component.
    property alias checkable: ch.enabled
    property alias checked: ch.checked

    // The Checkable component acts as a helper to deal with the
    // checked/unchecked state and also with mutually exclusive groups of
    // checkable items (CheckableGroups).
    //
    // It has one slot (used earlier in the onClicked handler) to indicate that
    // the button needs to be toggled. Whether or not this will change the
    // state of other buttons in the group is something that Checkable will
    // take care of.
    Checkable {
        id: ch

        // CheckableGroups have a property to indicate which value is checked
        // (only one can be since it is mutually exclusive). This property sets
        // the value when this button is checked.
        value: root.text

        // By default the button will act as a regular button. Remeber the
        // button 'checkable' is aliased to this property, so when the user
        // marks "checkable: true" it will enable it.
        enabled: false
    }

    // This makes possible to users of the button set the exclusivity group
    // that the button will be part of. Again this information internally is
    // just delegated to the Checkable.
    property alias exclusiveGroup: ch.exclusiveGroup
}
