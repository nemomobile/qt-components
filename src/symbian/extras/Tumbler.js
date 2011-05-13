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

var __columns = [];
var __autoColumnWidth = 0;
var __suppressLayoutUpdates = false;

function initialize() {
    // check the width requested by fixed width columns
    var requestedWidth = 0;
    var requestedCount = 0;
    var invisibleCount = 0;
    var firstColumn = false;
    for (var i = 0; i < columns.length; i++) {
        if (columns[i].visible) {
            if (columns[i].width > 0 && !columns[i].privateIsAutoWidth) {
                requestedWidth += columns[i].width;
                requestedCount++;
            }
        } else {
            invisibleCount++;
        }
    }
    // allocate the rest to auto width columns
    if ((columns.length - requestedCount - invisibleCount) > 0) {
        __autoColumnWidth = Math.floor((root.width - requestedWidth) / (columns.length - requestedCount - invisibleCount));
    }

    for (var i = 0; i < columns.length; i++) {
        var comp = Qt.createComponent("TumblerTemplate.qml");
        var newObj = comp.createObject(tumblerRow);
        if (!columns[i].width || columns[i].privateIsAutoWidth) {
            columns[i].width = __autoColumnWidth;
            columns[i].privateIsAutoWidth = true;
        }
        // TODO: Do not use Tumbler label as it will be deprecated.
        if (columns[i].label) {
            // enable label for the tumbler
            internal.hasLabel = true;
        }
        if (columns[i].visible && !firstColumn) {
            firstColumn = true;
            newObj.firstColumn = true;
        }
        newObj.height = root.height;
        newObj.index = i;
        newObj.tumblerColumn = columns[i];
        newObj.widthChanged.connect(layout);
        newObj.visibleChanged.connect(layout);
        __columns.push(newObj);
    }
    privateTemplates = __columns;
}

function clear() {
    var count = __columns.length;
    for (var i = 0; i < count; i++) {
        var tmp = __columns.pop();
        tmp.destroy();
    }
}

function forceUpdate() {
    for (var i = 0; i < columns.length; i++) {
        columns[i].selectedIndex = __columns[i].view.currentIndex;
    }
}

function layout() {
    if (__suppressLayoutUpdates) {
        // guard against onWidthChanged triggering again during this process
        return;
    }
    var requestedWidth = 0;
    var requestedCount = 0;
    var invisibleCount = 0;
    var firstColumn = false;
    for (var i = 0; i < columns.length; i++) {
        if (columns[i].visible) {
            var w = columns[i].width;
            var a = columns[i].privateIsAutoWidth;
            if (!a || (a && w != __autoColumnWidth)) {
                requestedWidth += columns[i].width;
                requestedCount++;
                columns[i].privateIsAutoWidth = false;
            } else {
                columns[i].privateIsAutoWidth = true;
            }
        } else {
            invisibleCount++;
        }
    }

    if ((columns.length - requestedCount - invisibleCount) > 0) {
        __autoColumnWidth = Math.floor((root.width - requestedWidth) / (columns.length - requestedCount - invisibleCount));
    }

    // guard against onWidthChanged triggering again during this process
    __suppressLayoutUpdates = true;
    for (var i = 0; i < columns.length; i++) {
        if (columns[i].privateIsAutoWidth) {
            columns[i].width = __autoColumnWidth;
        }
        if (columns[i].visible && !firstColumn) {
            firstColumn = true;
            __columns[i].firstColumn = true;
        } else if (__columns[i]) {
            __columns[i].firstColumn = false;
        }
    }
    __suppressLayoutUpdates = false;
}
