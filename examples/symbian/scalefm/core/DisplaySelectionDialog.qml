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

SelectionDialog {
    id: root
    titleText: "Select display"
    model: displayModel
    ListModel {
        id: displayModel
        ListElement { name: "3.5\" NHD Portrait"; width: 360; height: 640; dpi: 210 }
        ListElement { name: "3.5\" NHD Landscape"; width: 640; height: 360; dpi: 210 }
        ListElement { name: "2.5\" VGA Portrait"; width: 480; height: 640; dpi: 325 }
        ListElement { name: "2.5\" VGA Landscape"; width: 640; height: 480; dpi: 325 }
        ListElement { name: "3.2\" HVGA Portrait"; width: 320; height: 480; dpi: 180 }
        ListElement { name: "3.2\" HVGA Landscape"; width: 480; height: 320; dpi: 180 }
        ListElement { name: "4.0\" NHD Portrait"; width: 360; height: 640; dpi: 185 }
        ListElement { name: "4.0\" NHD Landscape"; width: 640; height: 360; dpi: 185 }
        ListElement { name: "4.0\" WVGA Portrait"; width: 480; height: 864; dpi: 245 }
        ListElement { name: "4.0\" WVGA Landscape"; width: 864; height: 480; dpi: 245 }
        ListElement { name: "7.0\" WSVGA Portrait"; width: 600; height: 1024; dpi: 170 }
        ListElement { name: "7.0\" WSVGA Landscape"; width: 1024; height: 600; dpi: 170 }
        ListElement { name: "11.0\" XGA Portrait"; width: 768; height: 1024; dpi: 130 }
        ListElement { name: "11.0\" XGA Landscape"; width: 1024; height: 768; dpi: 130 }
    }
    onAccepted: {
        screen.privateSetDisplay(
            displayModel.get(selectedIndex).width,
            displayModel.get(selectedIndex).height,
            displayModel.get(selectedIndex).dpi)
    }
}
