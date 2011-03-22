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
import "." 1.0

ImplicitSizeItem {
    id: listHeading
    property variant style: style
    property alias padding: paddingRectangle

    Style {
        id: style
        styleClass: "ListHeading"
    }

    implicitWidth: style.current.preferredWidth
    implicitHeight: style.current.preferredHeight

    BorderImage {
        source: style.current.get("background")
        border { left: 28; top: 0; right: 28; bottom: 0 }
        smooth: true
        anchors.fill: parent
    }

    Item {
        // non-visible item to create a padding boundary that content items can bind to
        id: paddingRectangle
        anchors {
            fill: parent
            leftMargin: style.current.get("marginLeft")
            rightMargin: style.current.get("marginRight")
            topMargin: style.current.get("marginTop")
            bottomMargin: style.current.get("marginBottom")
        }
        width: style.current.get("marginLeft") + style.current.get("marginRight")
        height: style.current.get("marginTop") + style.current.get("marginBottom")
    }
}
