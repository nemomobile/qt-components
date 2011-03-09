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

ImplicitSizeItem {
    id: root

    property alias text: label.current
    property alias up: label.up
    property bool highlighted: false

    width: wrapper.width
    height: wrapper.height

    Style {
        id: style
        styleClass: "SectionScroller"
    }

    ImplicitSizeItem {
        id: wrapper

        clip: true
        height: label.height

        Text {
            id: label

            property string current: ""
            property bool up: true
            property int shift: 30

            height: style.current.get("textHeight")
            width: parent.width - style.current.get("textMargin")
            x: style.current.get("indexLeftMargin")
            color: root.highlighted ? style.current.get("highlightedTextColor") : style.current.get("textColor")
            font: style.current.get("smallFont")
            verticalAlignment: Text.AlignVCenter
            onCurrentChanged: {
                text = current;
            }
            onTextChanged: {
                var w = paintedWidth
                parent.width = w + style.current.get("indexLeftMargin") + style.current.get("indexRightMargin")
            }

            Behavior on current {
                SequentialAnimation {
                    NumberAnimation { target: label; property: "y"; to: label.up ? label.shift : -label.shift; duration: 50 }
                    PropertyAction { target: label; property: "y"; value: label.up ? -label.shift : label.shift }
                    NumberAnimation { target: label; property: "y"; to: 0; duration: 50 }
                }
            }
        }
    }
}
