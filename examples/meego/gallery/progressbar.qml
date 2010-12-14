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

Item {
    width: 400
    height: 400

    ProgressBar {
        id: progressBar
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: toggleUnknown.verticalCenter
        width: 350
        indeterminate: toggleUnknown.checked
        value: progress.value
    }

    Button {
        id: toggleUnknown
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: progressBar.right
        anchors.leftMargin: 40

        width: 200
        checkable: true
        text: "Unknown"
    }

    Slider {
        id: progress
        anchors.top: progressBar.bottom
        anchors.topMargin: 40
        anchors.left: progressBar.left
        anchors.right: parent.right
    }
}
