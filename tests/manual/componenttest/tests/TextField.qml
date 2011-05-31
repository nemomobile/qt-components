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

Item {
    id: root
    anchors.fill: parent

    TabBar {
        id: tabBar
        anchors.top: parent.top
        TabButton { tab: selection; text: "Selection" }
        TabButton { tab: font; text: "Font" }
        TabButton { tab: other; text: "Other" }
        TabButton { tab: maxLength; text: "MaxLength" }
    }

    TabGroup {
        id: tabGroup
        anchors {
            left: parent.left
            right: parent.right
            top: tabBar.bottom
            bottom: parent.bottom
        }
        TextFieldSelection { id: selection }
        TextFieldFont { id: font }
        TextFieldOther { id: other }
        TextFieldMaxLength { id: maxLength }
        onCurrentTabChanged: parent.forceActiveFocus()
    }
}
