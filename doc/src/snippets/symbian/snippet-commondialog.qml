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
import com.nokia.symbian 1.1

Item {
    id: root

    Button {
        id: toolbar
        text: "Open dialog"
        anchors.centerIn: parent
        onClicked: dialog.open()
    }

    //! [1]
    CommonDialog {
        id: dialog
        titleText: "Title text"
        titleIcon: "list4.svg"
        buttonTexts: ["OK", "Cancel"]
        content: Rectangle {
            width: 300
            height: 200
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#30ff0000" // this makes the margins visible on the ui
            Grid {
                id: grid

                property int itemWidth: (width - platformStyle.paddingSmall) / 2
                property int itemHeight: (height - platformStyle.paddingSmall) / 2

                anchors.fill: parent
                anchors.margins: platformStyle.paddingMedium
                spacing: platformStyle.paddingSmall
                columns: 2

                Rectangle { color: "red"; width: grid.itemWidth; height: grid.itemHeight }
                Rectangle { color: "blue"; width: grid.itemWidth; height: grid.itemHeight }
                Rectangle { color: "green"; width: grid.itemWidth; height: grid.itemHeight }
                Rectangle { color: "yellow"; width: grid.itemWidth; height: grid.itemHeight }
            }
        }

    }
    //! [1]
}
