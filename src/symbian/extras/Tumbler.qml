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

import QtQuick 1.0
import com.nokia.symbian 1.0
import "Tumbler.js" as Engine
import "Constants.js" as C

ImplicitSizeItem {
    id: root
    objectName: "root"

    property list<Item> columns
    signal changed(int index)

    property bool privateDelayInit: false
    property list<Item> privateTemplates

    implicitWidth: screen.width - 6 * platformStyle.paddingMedium
    implicitHeight: 4 * privateStyle.menuItemHeight

    function privateInitialize() {
        if (!internal.initialized) {
            Engine.initialize();
            internal.initialized = true;
        }
    }

    function privateForceUpdate() {
        Engine.forceUpdate();
    }

    clip: true
    Component.onCompleted: {
        if (!privateDelayInit && !internal.initialized) {
            initializeTimer.restart();
        }
    }
    onChanged: {
        if (internal.movementCount == 0)
            Engine.forceUpdate();
    }
    onColumnsChanged: {
        if (internal.initialized) {
            // when new columns are added, the system first removes all
            // the old columns
            internal.initialized = false;
            Engine.clear();
            internal.reInit = true;
        } else if (internal.reInit && columns.length > 0) {
            // timer is used because the new columns are added one by one
            // we only want to act after the last column is added
            internal.reInit = false;
            initializeTimer.restart();
        }
    }
    onWidthChanged: {
        Engine.layout();
    }

    QtObject {
        id: internal

        property int movementCount: 0
        property bool initialized: false
        property bool reInit: false
        property bool hasLabel: false // TODO: Do not use Tumbler label as it will be deprecated.

        property Timer timer: Timer {
            id: initializeTimer
            interval: 50
            onTriggered: {
                Engine.initialize();
                internal.initialized = true;
            }
        }
    }

    Row {
        id: tumblerRow
        anchors { fill: parent }
    }

    BorderImage {
        width: parent.width
        height: internal.hasLabel ?
                    parent.height - privateStyle.menuItemHeight : // decrease by bottom text height
                    parent.height
        source: privateStyle.imagePath("qtg_fr_tumbler")
        anchors.top: parent.top
        border { left: platformStyle.borderSizeMedium; top: platformStyle.borderSizeMedium; right: platformStyle.borderSizeMedium; bottom: platformStyle.borderSizeMedium }
    }
}
