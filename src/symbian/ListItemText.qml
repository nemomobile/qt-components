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

Text {
    id: root
    property string mode: "normal"
    property string role: "Title"

    // Also role "Heading" taken into account although not explicitely used in evaluations below
    font {
        family: platformStyle.fontFamilyRegular
        pixelSize: (role == "Title" || role == "SelectionTitle") ? platformStyle.fontSizeLarge : platformStyle.fontSizeSmall
        weight: (role == "SubTitle" || role == "SelectionSubTitle") ? Font.Light : Font.Normal
    }
    color: internal.normalColor
    elide: Text.ElideRight

    // Performance optimization:
    // Use value assignment when property changes instead of binding to js function
    onModeChanged: { color = internal.getColor() }

    QtObject {
        id: internal

        // Performance optmization:
        // Use tertiary operations even though it doesn't look that good
        property color normalColor: (root.role == "SelectionSubTitle" || root.role == "SubTitle")
                                        ? platformStyle.colorNormalMid : platformStyle.colorNormalLight

        function getColor() {
            if (root.mode == "pressed")
                return platformStyle.colorPressed
            else if (root.mode == "highlighted")
                 return platformStyle.colorHighlighted
            else if (root.mode == "disabled")
                return platformStyle.colorDisabledLight
            else 
                return normalColor
        }
    }
}
