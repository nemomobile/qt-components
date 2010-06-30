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

// This script assumes 'component' in the scope is a PathBar.

var path = [];

function push(name, parent) {
    var b = component.buttonDelegate.createObject(parent);
    b.text = name;
    b.z = -path.length; // First items stay on top
    b.clicked.connect(function() { popUntil(b) });
    path.push(b);

    component.level = path.length;
    return path.length;
}

function popUntil(button) {
    while (path[path.length - 1] !== button) {
        path.pop().destroy();
    }
    component.level = path.length;
}

function pop() {
    var b = path.pop();
    if (b) {
        b.destroy();
    }
    component.level = path.length;
    return path.length;
}

function clear() {
    while (path.length > 0) {
        path.pop().destroy();
    }
    component.level = 0;
}

function label(level) {
    if (level < 1 || level > path.length)
        return ""; // ### Warning?

    return path[level - 1].text;
}

function setLabel(level, label) {
    if (level < 1 || level > path.length)
        return; // ### Warning?

    path[level - 1].text = label;
}
