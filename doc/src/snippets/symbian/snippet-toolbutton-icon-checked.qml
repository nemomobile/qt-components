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
//! [0]
import QtQuick 1.1
import com.nokia.symbian 1.1

Window {

    Text {
        text: "This illustrates the various states of the icon ToolButton, and how platformExclusiveGroup works. Try clicking the buttons to see the effect.  \n\nThe icon ToolButton defaults to flat. Each ToolButton is checkable, and the flat checked appearance is applied when checked."
        anchors {
            left: parent.left;
            right: parent.right
            top: parent.top
            margins: platformStyle.paddingMedium
        }
        wrapMode: Text.WordWrap;
        color: platformStyle.colorNormalLight
        font.family: platformStyle.fontFamilyRegular
        font.pixelSize: platformStyle.fontSizeMedium
    }

    //! [toolbar1a]
    ToolBar {
        anchors.bottom: parent.bottom
        tools: ToolBarLayout {

            ToolButton {
                visible: false
                iconSource: "toolbar-back"
            }
            ToolButton {
                id: stop
                iconSource: "toolbar-mediacontrol-stop"
                checkable: true
                checked: true
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-play"
                checkable: true
                checked: false
            }
            ToolButton {
                visible: false
                iconSource: "toolbar-menu"
            }

        }
    }
    //! [toolbar1a]
}
//! [0]
