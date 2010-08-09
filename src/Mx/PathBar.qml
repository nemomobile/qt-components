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

import Qt 4.7
import Qt.labs.components 1.0
import "PathBar.js" as Helper

Item {
    id: component

    property int level: 0  // ### readonly
    property bool editable: false
    property bool clearOnChange: false
    property Entry entry: internalEntry
    property Component buttonDelegate: Button { }

    function push(name) {
        return Helper.push(name, row);
    }

    function pop() {
        return Helper.pop();
    }

    function clear() {
        Helper.clear();
    }

    function label(level) {
        return Helper.label();
    }

    function setLabel(level, label) {
        Helper.setLabel(level, label);
    }

    width: row.width + entry.width
    height: Math.max(internalEntry.height, row.height)
    onLevelChanged: {
        if (component.clearOnChange)
            entry.text = "";
    }

    Entry {
        id: internalEntry
        anchors.left: row.right
        anchors.leftMargin: 3 // ###
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        // ### negative margin to provide overlap?

        // ### why visible doesn't work?
        // visible: component.editable
        opacity: component.editable ? 1 : 0
    }

    Row {
        id: row
        anchors.verticalCenter: parent.verticalCenter

        // ### negative margin to provide overlap
        spacing: 2
    }
}
