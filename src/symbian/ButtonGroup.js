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

var self
var clickHandlers = []
var visibleButtons = []
var resizing = false
var checkingOverallExclusivity = false
var checkedBtn

function create(that) {
    destroy()
    self = that

    // If the item is not visible at this stage, we store the value of the property
    // checkedButton to ensure a proper initialization. The value is restored in
    // initCheckedButton().
    if (!self.visible)
        checkedBtn = self.checkedButton

    buildItems()
    self.childrenChanged.connect(buildItems)
    self.widthChanged.connect(resizeButtons)
    self.exclusiveChanged.connect(checkOverallExclusivity)
    self.checkedButtonChanged.connect(checkOverallExclusivity)
    self.visibleChanged.connect(initCheckedButton)
}

function destroy() {
    if (self !== undefined) {
        self.childrenChanged.disconnect(buildItems)
        self.widthChanged.disconnect(resizeButtons)
        self.exclusiveChanged.disconnect(checkOverallExclusivity)
        self.checkedButtonChanged.disconnect(checkOverallExclusivity)
        self.visibleChanged.disconnect(initCheckedButton)
        releaseItemConnections()
        self = undefined
    }
}

function initCheckedButton() {
    // When the item becomes visible, restore the value of checkedButton property
    // that was stored in the create function.
    if (self.visible && checkedBtn !== null) {
        self.checkedButton = checkedBtn
        checkedBtn = null
    }
}

function buildItems() {
    releaseItemConnections()
    visibleButtons = []
    for (var i = 0; i < self.children.length; i++) {
        var item = self.children[i]

        // set up item connections
        clickHandlers[i] = checkExclusive(item)
        item.clicked.connect(clickHandlers[i])
        item.visibleChanged.connect(buildItems)

        // update visibleButtons array
        if (item.visible)
            visibleButtons.push(item)
    }
    checkOverallExclusivity()
    resizeButtons()
}

function releaseItemConnections() {
    for (var i = 0; i < self.children.length; i++) {
        self.children[i].clicked.disconnect(clickHandlers[i])
        self.children[i].visibleChanged.disconnect(buildItems)
    }
    clickHandlers = []
}

function checkOverallExclusivity() {
    if (!checkingOverallExclusivity && self.exclusive) {
        // prevent re-entrant calls
        checkingOverallExclusivity = true
        if (visibleButtons.length > 0) {
            if ((self.checkedButton === null || !self.checkedButton.visible))
                self.checkedButton = visibleButtons[0]
            self.checkedButton.checked = true
        }
        else {
            self.checkedButton = null
        }

        for (var i = 0; i < self.children.length; i++) {
            var item = self.children[i]
            // e.g CheckBox can be added to ButtonGroup but doesn't have "checkable" property
            if (self.exclusive && item.hasOwnProperty("checkable"))
                item.checkable = true
            if (item !== self.checkedButton)
                item.checked = false
        }
        checkingOverallExclusivity = false
    }
}

function checkExclusive(item) {
    var button = item
    return function() {
        if (self.exclusive) {
            for (var i = 0, ref; (ref = visibleButtons[i]); i++)
                ref.checked = button === ref
        }
        self.checkedButton = button
    }
}

function resizeButtons() {
    if (!resizing && visibleButtons.length && self.privateDirection == Qt.Horizontal) {
        // if ButtonRow has implicit size, a loop may occur where resizing individual
        // Button affects ButtonRow size, which triggers again resizing...
        // therefore we prevent re-entrant resizing attempts
        resizing = true
        var buttonWidth = self.width / visibleButtons.length
        for (var i = 0; i < self.children.length; i++) {
            self.children[i].width = self.children[i].visible ? buttonWidth : 0
        }
        resizing = false
    }
}

// Binding would not work properly if visibleButtons would just be returned,
// it would miss visibility changes. In the long run ButtonGroup.js could be
// refactored.
function visibleItems(item) {
    var visibleChildren = []
    for (var i = 0; i < item.children.length; i++) {
        if (item.children[i].visible)
            visibleChildren.push(item.children[i])
    }
    return visibleChildren
}
