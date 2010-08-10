/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import Qt 4.7
import Qt.labs.components 1.0

// See MyButton.qml for details on how use Checkable and CheckableGroup to
// create a button that is checkable and can be part of a mutually exclusive
// group. This example will show this button in the different ways it can be
// used.

Item {
    width: 600
    height: 300

    // In this example, we explicitly create a CheckableGroup to use, however
    // different component sets can choose to hide this complexity, in a
    // container Item that will automatically set the group property.
    CheckableGroup {
        id: group
    }

    // Three buttons part of a mutually exclusive group.
    Row {
        id: buttons
        x: 30
        y: 30
        spacing: 5
        property int count: 3

        MyButton {
            text: "Button 1"
            exclusiveGroup: group
        }

        MyButton {
            text: "Button 2"
            exclusiveGroup: group
            checked: true
        }

        MyButton {
            text: "Button 3"
            exclusiveGroup: group
        }
    }

    // This two-state toggle is not part of a group, and is used to
    // control the visibility of "advanced" features.
    MyButton {
        id: advanced
        anchors {
            top: buttons.bottom
            topMargin: 40
            left: buttons.left
        }
        text: "More..."
        checkable: true
    }

    Text {
        visible: advanced.checked
        anchors {
            verticalCenter: advanced.verticalCenter
            left: advanced.right
            leftMargin: 20
        }
        text: "The selected one is '" + group.selectedValue + "'"
    }

    // Here the button is used as a regular clickable button. For this kind of
    // button, the interesting thing is to watch the 'clicked' signal.
    MyButton {
        visible: advanced.checked
        text: "Add another!"

        anchors {
            left: advanced.left
            top: advanced.bottom
            topMargin: 20
        }

        // When clicked, this will add another button to the exclusive group.
        onClicked: {
            buttons.count += 1;
            var b = Qt.createQmlObject("MyButton { checkable: true }", buttons);
            b.text = "Button " + buttons.count;
            b.exclusiveGroup = group;
        }
    }
}
