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

import QtQuick 1.0
import com.meego 1.0

Page {
    tools: commonTools

    ListModel {
        id: m

        ListElement {
            name: "Push Button"
            source: "PushButton.qml"
        }

        ListElement {
            name: "Icon Button"
            source: "IconButtons.qml"
        }

        ListElement {
            name: "Switch"
            source: "PushButton.qml"
        }

        ListElement {
            name: "Checkbox"
            source: "PushButton.qml"
        }

        ListElement {
            name: "Button Group"
            source: "ButtonGroup.qml"
        }
    }

    ListView {
        id: buttonsView
        anchors.fill: parent
        model: m

        delegate: BasicListItem {
            title: name
            onClicked: {
                pageStack.push(Qt.createComponent(source));
            }
        }
    }

    ScrollDecorator {
        flickable: buttonsView
    }
}
