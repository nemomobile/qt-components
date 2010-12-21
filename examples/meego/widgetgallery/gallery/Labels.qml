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

import Qt 4.7
import com.meego 1.0

Template {
    tools: commonTools

    flickableContentHeight: 440

    infoText: "Simple Label is used mainly by other components, and it does not support any formatting or interaction.\n\nRich Label supports multiple lines, HTML formatting and highlighting for links.\n\n All Labels can be aligned left, right or center and will be automatically truncated if the text does not fit inside the given area."

    Label {
        y: 40
        anchors.left: parent.left
        anchors.right: parent.right

        text: "Simple Label"
    }

    Label {
        y: 109
        anchors.left: parent.left
        anchors.right: parent.right

        text: "Rich Label <b>bold</b>, <i>italic</i>, http://www.nokia.com, +358401234567, fors.fagerstrom@email.com"
    }
}
