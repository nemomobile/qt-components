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

import Qt 4.7
import com.meego 1.0

Template {
    id: page

    tools: commonTools

    anchors.fill: parent
    flickableContentHeight: 520

    Text {
        id: firstText
        anchors {
            top: parent.top
            left: parent.left
        }

        text: "Horizontal group"
        font.pixelSize: 24
    }

    ButtonRow {
        id: buttonRow1
        anchors {
            top: firstText.bottom
            left: parent.left
            topMargin: 10
        }
        groupType: "None"

        Button {
            id: leftButton
            text: "Yes"
            width: middleButton_2.width
            height: 64
        }

        Button {
            id: middleButton
            text: "No"
            width: middleButton_2.width
            height: leftButton.height
        }

        Button {
            id: rightButton
            text: "Maybe"
            width: middleButton_2.width
            height: leftButton.height
        }
    }

    Text {
        id: secondText
        anchors {
            top: buttonRow1.bottom
            left: parent.left
            topMargin: 20
        }
        text: "Horizontal group, exclusive"
        font.pixelSize: 24
    }

    ButtonRow {
        id: buttonRow2
        anchors {
            top: secondText.bottom
            left: parent.left
            topMargin: 10
        }

        Button {
            id: leftButton_2
            text: "Cost"
            width: middleButton_2.width
            height: leftButton.height
        }

        Button {
            id: middleButton_2
            text: "Quality"
            width: 138
            height: leftButton.height
        }

        Button {
            id: rightButton_2
            text: "Schedule"
            width: middleButton_2.width
            height: leftButton.height
        }
    }

    Text {
        id: thirdText
        anchors {
            top: buttonRow2.bottom
            left: parent.left
            topMargin: 20
        }
        text: "Vertical group, checkable"
        font.pixelSize: 24
    }

    ButtonColumn {
        id: buttonColumn1
        anchors {
            top: thirdText.bottom
            left: parent.left
            topMargin: 10
        }
        groupType: "Many"

        Button {
            id: topButton
            text: "Milk"
            width: 414
            height: leftButton.height
        }

        Button {
            id: middleButton_3
            text: "Sugar"
            width: topButton.width
            height: leftButton.height
        }

        Button {
            id: bottomButton
            text: "Biscotti"
            width: topButton.width
            height: leftButton.height
        }
    }

    infoText: "Buttons can be grouped together based on visual or logic requirements.\n\nThe basic group is formed by placing buttons together into a layout. Depending on the theme, the buttons are typically drawn as connected with each other.\n\nLogical exclusivity can be added to a group, which ensures only one of the buttons can be selected at a time. If a group is not exclusive, multiple options from the group can be selected."
}
