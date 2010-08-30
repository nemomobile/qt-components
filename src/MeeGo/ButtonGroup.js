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

/// Helper code that is shared between ButtonRow.qml and ButtonColumn.qml.

var buttons = [];
var params = {
    checkable: false,
    exclusive: false,
    group: null,
    orientation: Qt.Horizontal
};


// "duck-typing" to identify the Buttons, find a better way to do it :-P
function isButton(item) {
    // ### QTBUG-12837, this should be: "__exclusiveGroup" in item
    for (var c in item) {
        if (c === "__exclusiveGroup") {
            return true;
        }
    }

    return false;
}

function updateButtons(children) {
    var length = children.length;
    var buttonCount = 0;

    buttons = [];

    for (var i = 0; i < length; i++) {
        var item = children[i];

        if (isButton(item)) {
            buttons.push(item);
            item.buttonType = "group";
            item.checkable = params.checkable;
            item.__exclusiveGroup = params.exclusive ? params.group : null;
        }
    }

    updateButtonTypes();
}

function updateButtonTypes() {
    var firstPos;
    var middlePos;
    var lastPos;
    if (params.orientation === Qt.Horizontal) {
        firstPos = Background.HorizontalLeftPosition;
        middlePos = Background.HorizontalCenterPosition;
        lastPos = Background.HorizontalRightPosition;
    } else {
        firstPos = Background.VerticalTopPosition;
        middlePos = Background.VerticalCenterPosition;
        lastPos = Background.VerticalBottomPosition;
    }

    // Fix the children group position
    var buttonLength = buttons.length;
    if (buttonLength == 1) {
        buttons[0].groupPosition = Background.DefaultPosition;
    } else if (buttonLength > 1) {
        buttons[0].groupPosition = firstPos;
        for (var i = 1; i < buttonLength - 1; i++) {
            buttons[i].groupPosition = middlePos;
        }
        buttons[buttonLength - 1].groupPosition = lastPos;
    }
}

function setParameters(c, e, g, o) {
    params.checkable = c;
    params.exclusive = e;
    params.group = g;
    params.orientation = o;

    // This avoids unnecessary signal tracking to remove all the
    // items from the group (which would happen if we iterate one
    // by one setting the group to null).
    if (!params.exclusive) {
        params.group.clear();
    }

    var buttonLength = buttons.length;
    for (var i = 0; i < buttonLength; i++) {
        var b = buttons[i];
        b.checkable = params.checkable;
        b.__exclusiveGroup = params.exclusive ? params.group : null;
    }
}
