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
    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        model: ListModel { id: listModel }
        delegate: listDelegate
        Component.onCompleted: {
            initializeModel()
        }
        ScrollBar {
            flickableItem: listView
            anchors { top: listView.top; right: listView.right }
        }
    }
    Component {
        id: listDelegate
        Rectangle {
            color: m_inverse ? platformStyle.colorBackgroundInverted : platformStyle.colorBackground
            height: text1.height + 2 * platformStyle.paddingLarge
            width: listView.width
            Text {
                id: text1
                anchors.left: parent.left
                anchors.leftMargin: platformStyle.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                text: m_text
                color: m_color
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium }
            }
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: platformStyle.paddingMedium
                width: height
                color: m_color
            }
        }
    }

    function initializeModel() {
        listView.model.append( {
            "m_color": platformStyle.colorBackground,
            "m_text": "colorBackground",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalLight,
            "m_text": "colorNormalLight",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalMid,
            "m_text": "colorNormalMid",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalDark,
            "m_text": "colorNormalDark",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalLink,
            "m_text": "colorNormalLink",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorPressed,
            "m_text": "colorPressed",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorLatched,
            "m_text": "colorLatched",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorHighlighted,
            "m_text": "colorHighlighted",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledLight,
            "m_text": "colorDisabledLight",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledMid,
            "m_text": "colorDisabledMid",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledDark,
            "m_text": "colorDisabledDark",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorTextSelection,
            "m_text": "colorTextSelection",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorBackgroundInverted,
            "m_text": "colorBackgroundInverted",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalLightInverted,
            "m_text": "colorNormalLightInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalMidInverted,
            "m_text": "colorNormalMidInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalDarkInverted,
            "m_text": "colorNormalDarkInverted",
            "m_inverse": false
        } )
        listView.model.append( {
            "m_color": platformStyle.colorNormalLinkInverted,
            "m_text": "colorNormalLinkInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorPressedInverted,
            "m_text": "colorPressedInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorLatchedInverted,
            "m_text": "colorLatchedInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorHighlightedInverted,
            "m_text": "colorHighlightedInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledLightInverted,
            "m_text": "colorDisabledLightInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledMidInverted,
            "m_text": "colorDisabledMidInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorDisabledDarkInverted,
            "m_text": "colorDisabledDarkInverted",
            "m_inverse": true
        } )
        listView.model.append( {
            "m_color": platformStyle.colorTextSelectionInverted,
            "m_text": "colorTextSelectionInverted",
            "m_inverse": true
        } )
    }
}
