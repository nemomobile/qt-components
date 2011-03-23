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
import com.nokia.symbian 1.0
import com.nokia.extras 1.0

Item {
    id: root

    Column {
        x: 25
        spacing: 20

        Rectangle {
            id: rect
            width: 200
            height: 50
            color: "#f0f1f2"

            RatingIndicator {
                id: indicator
                anchors.centerIn: parent

                ratingValue: 3
                maximumValue: 5
                count: 3
            }
        }

        Button {
            id: addButton
            text: "Increase rating"
            onClicked: { indicator.ratingValue++ }
        }
        Button {
            id: removeButton
            text: "Decrease rating"
            onClicked: { indicator.ratingValue-- }
        }
        Button {
            id: addCount
            text: "Increase votes counted"
            onClicked: { indicator.count++ }
        }
        Button {
            id: removeCount
            text: "Decrease votes counted"
            onClicked: { indicator.count-- }
        }
        Button {
            id: negativeButton
            text: "Invert colors"
            onClicked: {
                if(rect.color == "#f0f1f2") {
                    indicator.inverted = true
                    rect.color = "#000000"
                }
                else {
                    indicator.inverted = false
                    rect.color = "#f0f1f2"
                }
            }
        }
    }
}
