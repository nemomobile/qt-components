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
