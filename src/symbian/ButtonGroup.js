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

var self;
var clickHandlers = [];
var visibleButtons = [];
var nonVisibleButtons = [];
var checkedButton;

function create(that) {
    self = that;
    self.childrenChanged.connect(childrenChanged);
    self.widthChanged.connect(resizeButtons);
    self.exclusiveChanged.connect(exclusiveChanged);
    build();
}

function destroy() {
    self.childrenChanged.disconnect(childrenChanged);
    self.widthChanged.disconnect(resizeButtons);
    self.exclusiveChanged.disconnect(exclusiveChanged);
    cleanup();
    self = undefined;
}

function exclusiveChanged() {
    if (self.exclusive) {
        for (var i = 0; i < visibleButtons.length; i++)
            visibleButtons[i].checked = false;
    }
    build();
}

function childrenChanged() {
    visibleButtons = [];
    nonVisibleButtons = [];

    for (var i = 0; i < self.children.length; i++) {
        var item = self.children[i];
        if (!item.visible)
            nonVisibleButtons.push(item);

        visibleButtons.push(item);
    }

    if (self.exclusive)
        setButtonsCheckable(true);

    build();
}

function setButtonsCheckable(checkable) {
    for (var i = 0; i < self.children.length; i++) {
        if (self.children[i].hasOwnProperty("checkable"))
            self.children[i].checkable = checkable;
    }
}

function build() {
    cleanup();
    visibleButtons = [];
    nonVisibleButtons = [];

    for (var i = 0; i < self.children.length; i++) {
        var item = self.children[i];

        if (!item.hasOwnProperty("checked"))
            continue;

        item.visibleChanged.connect(build);
        if (!item.visible) {
            nonVisibleButtons.push(item);
            continue;
        }
        visibleButtons.push(item);

        if (self.exclusive) {
            setButtonsCheckable(true);
            clickHandlers[i] = checkExclusive(item);
            item.clicked.connect(clickHandlers[i]);
        }
    }

    if (self.exclusive) {
        if (self.checkedButton) {
            self.checkedButton.checked = true;
        }
        else if (self.checkedButton == null && visibleButtons.length > 0) {
            self.checkedButton = visibleButtons[0];
            self.checkedButton.checked = true;
        }
    }

    resizeButtons();
}

function cleanup() {
    for (var i = 0; i < visibleButtons.length; i++) {
        var item = visibleButtons[i];

        if (clickHandlers[i])
            item.clicked.disconnect(clickHandlers[i]);
        item.visibleChanged.disconnect(build);
    }
    clickHandlers = [];

    for (var j = 0; j < nonVisibleButtons.length; j++) {
        var item = nonVisibleButtons[j];
        item.visibleChanged.disconnect(build);
    }
}

function checkExclusive(item) {
    var button = item;
    return function() {
        for (var i = 0, ref; (ref = visibleButtons[i]); i++)
            ref.checked = button === ref;
        self.checkedButton = button;
    }
}

function resizeButtons() {
    if (self.__direction != Qt.Horizontal)
        return;

    var buttonWidth = self.width / visibleButtons.length;
    for (var i = 0; i < visibleButtons.length; i++)
        visibleButtons[i].width = buttonWidth;
}
