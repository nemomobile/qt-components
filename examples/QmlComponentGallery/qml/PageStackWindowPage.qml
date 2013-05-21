/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import com.nokia.meego 1.1

Page {
    id: pageStackWindowPage
    tools: pageStackWindowTools
    anchors.margins: UiConstants.DefaultMargin

    ToolBarLayout {
        id: pageStackWindowTools
        visible: false
        ToolIcon { iconId: "toolbar-back"; onClicked: { enableSwipe = true; screen.allowSwipe = enableSwipe; myMenu.close(); pageStack.pop(); } }
        ToolIcon { iconId: "toolbar-view-menu"; onClicked: { if (myMenu.status == DialogStatus.Closed) { myMenu.open(); enableSwipe = screen.allowSwipe; screen.allowSwipe = true; } else { myMenu.close(); } } }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: col
            spacing: 30
            width:  flickable.width

            Component.onCompleted: {
                var count = children.length;
                for (var i = 0; i < count; i++) {
                    var item = children[i];
                    item.anchors.horizontalCenter = item.parent.horizontalCenter;
                }
             }

            Button { text: "Toggle StatusBar"; checkable: true; checked: rootWindow.showStatusBar;  onClicked: { rootWindow.showStatusBar = !rootWindow.showStatusBar; } }

            Button { text: "Alternate background image"; checkable: true; checked: rootWindow.platformStyle==customStyle; onClicked: { if (rootWindow.platformStyle==defaultStyle) rootWindow.platformStyle=customStyle; else rootWindow.platformStyle=defaultStyle; } }

            Button { text: "Toggle Rounded corners"; checkable:true; checked: rootWindow.platformStyle.cornersVisible; onClicked: { rootWindow.platformStyle.cornersVisible = !rootWindow.platformStyle.cornersVisible; } }

            Button { text: "Toggle ToolBar"; checkable: true; checked: rootWindow.showToolBar; onClicked: { rootWindow.showToolBar = !rootWindow.showToolBar } }

            Button { text: "Toggle Swipe"; checkable: true; checked: enableSwipe; onClicked: { enableSwipe = !enableSwipe; screen.allowSwipe = enableSwipe } }
          }

    }
    ScrollDecorator {
        flickableItem: flickable
    }

}
