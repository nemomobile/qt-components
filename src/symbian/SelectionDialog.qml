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

CommonDialog {
    id: root

    // Common API
    property alias model: listView.model
    property int selectedIndex: -1
    property Component delegate: defaultDelegate

    privateCloseIcon: true

    Component {
        id: defaultDelegate

        MenuItem {
            text: modelData
            onClicked: {
                selectedIndex = index
                root.accept()
            }
        }
    }

    content: Item {
        id: contentItem

        function preferredHeight() {
            // Need to create artifical binding to listView.delegate because of QTBUG-19037
            var dummy = listView.delegate
            var maxHeight = root.platformContentMaximumHeight
            maxHeight -= maxHeight % privateStyle.menuItemHeight
            return Math.min(maxHeight, listView.count * privateStyle.menuItemHeight)
        }

        height: preferredHeight()
        width: root.platformContentMaximumWidth

        ListView {
            id: listView

            currentIndex : -1
            anchors.fill: parent
            delegate: root.delegate
            clip: true
        }

        ScrollBar {
            id: scrollBar

            flickableItem: listView
            interactive: false
            visible: listView.contentHeight > contentItem.height
            anchors { top: listView.top; right: listView.right }
        }
    }

    onClickedOutside: {
        privateStyle.play(Symbian.PopupClose)
        reject()
    }

    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            if (selectedIndex <= 0)
                listView.positionViewAtIndex(0, ListView.Beginning)
            else
                listView.positionViewAtIndex(selectedIndex, ListView.Center)
        }
        else if (status == DialogStatus.Open) {
            listView.focus = true
            if (listView.contentHeight > contentItem.height)
                scrollBar.flash(Symbian.FadeInFadeOut)
        }
    }
}
