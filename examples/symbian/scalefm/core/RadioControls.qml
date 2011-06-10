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

import QtQuick 1.0
import Qt.labs.components.native 1.0

Item {
    id: radioControls
    property int count : 3
    property bool stretch : (width / height > 1.4)

    function itemHeight(n) {
        return stretch ? height : Math.round((height - (appStyle.spacingSize * (n - 1))) / n)
    }
    function itemWidth(n) {
        return stretch ? Math.round((width - (appStyle.spacingSize * (n - 1))) / n) : width
    }

    Grid {
        id: grid
        columns: stretch ? radioControls.count : 1
        rows: stretch ? 1 : radioControls.count
        spacing:  appStyle.spacingSize

        Button {
            id: buttonStations
            text: "Stations"
            iconSource: "qrc:/graphics/qtg_mono_radio_stations.svg"
            width: itemWidth(radioControls.count)
            height: itemHeight(radioControls.count)
            visible: radioControls.count >= 3
            onClicked: if (!window.pageStack.busy) window.pageStack.push(stationView)
        }
        Button {
            id: buttonFavorites
            checkable: true
            checked: window.forceFallbackStyle
            text: "Fallback"
            iconSource: "qrc:/graphics/qtg_mono_favourites.svg"
            width: itemWidth(radioControls.count)
            height: itemHeight(radioControls.count)
            onClicked: window.forceFallbackStyle = !window.forceFallbackStyle
        }
        Button {
            id: buttonLoudspeaker
            checkable: true
            checked: container.magnifyGlass
            text: "Magnify"
            iconSource: "qrc:/graphics/qtg_mono_speaker.svg"
            width: itemWidth(radioControls.count)
            height: itemHeight(radioControls.count)
            onClicked: container.magnifyGlass = !container.magnifyGlass
        }
    }
}
