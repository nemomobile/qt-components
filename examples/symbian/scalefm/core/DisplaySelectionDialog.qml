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

SelectionDialog {
    id: root
    titleText: "Select display"
    model: displayModel
    ListModel {
        id: displayModel
        ListElement { modelData: "3.5\" NHD Portrait"; width: 360; height: 640; dpi: 210 }
        ListElement { modelData: "3.5\" NHD Landscape"; width: 640; height: 360; dpi: 210 }
        ListElement { modelData: "2.5\" VGA Portrait"; width: 480; height: 640; dpi: 325 }
        ListElement { modelData: "2.5\" VGA Landscape"; width: 640; height: 480; dpi: 325 }
        ListElement { modelData: "3.2\" HVGA Portrait"; width: 320; height: 480; dpi: 180 }
        ListElement { modelData: "3.2\" HVGA Landscape"; width: 480; height: 320; dpi: 180 }
        ListElement { modelData: "4.0\" NHD Portrait"; width: 360; height: 640; dpi: 185 }
        ListElement { modelData: "4.0\" NHD Landscape"; width: 640; height: 360; dpi: 185 }
        ListElement { modelData: "4.0\" WVGA Portrait"; width: 480; height: 864; dpi: 245 }
        ListElement { modelData: "4.0\" WVGA Landscape"; width: 864; height: 480; dpi: 245 }
        ListElement { modelData: "7.0\" WSVGA Portrait"; width: 600; height: 1024; dpi: 170 }
        ListElement { modelData: "7.0\" WSVGA Landscape"; width: 1024; height: 600; dpi: 170 }
        ListElement { modelData: "11.0\" XGA Portrait"; width: 768; height: 1024; dpi: 130 }
        ListElement { modelData: "11.0\" XGA Landscape"; width: 1024; height: 768; dpi: 130 }
    }
    onAccepted: {
        screen.privateSetDisplay(
            displayModel.get(selectedIndex).width,
            displayModel.get(selectedIndex).height,
            displayModel.get(selectedIndex).dpi)
    }
}
