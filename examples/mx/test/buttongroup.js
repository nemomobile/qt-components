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

// Helper code for the button-group.qml example

var buttonList = [];
var count = 0;

function addButton(parent) {
    var button = Qt.createQmlObject("import Qt.labs.Mx 1.0; Button { id: me; checkable: true; text: 'Button " + count + "' }", parent);
    count += 1;
    buttonList.push(button);
}

function removeLastButton() {
    if (buttonList.length > 0) {
        var last = buttonList.pop();
        last.destroy();
    }
}

function removeButton(button) {
    for (var i = 0; i < buttonList.length; i++) {
        if (buttonList[i] === button) {
            buttonList.splice(i, 1);
            break;
        }
    }

    button.destroy();
}


