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

import QtQuick 2.0
import com.nokia.meego 2.0

Page {
    id: labelsPage
    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    Flickable {
        id: labelFlick
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick

        anchors.fill: parent
        Column {
            id: col
            SelectableLabel { text: "Plain label"; }
            Label { text: "<a href=\"http://www.nokia.com\">Invert</a> label via link"; 
                    onLinkActivated: platformStyle.inverted = !platformStyle.inverted; }
            SelectableLabel { text: "Bold label"; font.bold: true; }
            SelectableLabel { text: "Italic label"; font.italic: true; }
            SelectableLabel { text: "Large label"; font.pixelSize: 100;  }

            SelectableLabel {
                id: coloredLabel
                text: "Large label with MouseArea"
                width: parent.width
                font.pixelSize: 48
                color: Qt.rgba(1.0, 0.5, 0.5, 1.0)
                wrapMode: Text.WordWrap

                MouseArea {
                    id: ma
                    anchors.fill:  parent
                    onClicked: coloredLabel.color ==  Qt.rgba(1.0, 0.5, 0.5, 1.0) ?
                                   coloredLabel.color =  Qt.rgba(0.5, 1.0, 0.5, 1.0)
                                 : coloredLabel.color =  Qt.rgba(1.0, 0.5, 0.5, 1.0)
                }

            }

            SelectableLabel { text: "Red label"; color: "red"; }
            SelectableLabel { text: "Elided labels are too long"; font.italic: true; width: 200; elide: Text.ElideRight; }
            Label { text: "Unselectable plain label <br>" }
            SelectableLabel {
                text: "<b>Wrapping label with a lot of text:</b> The quick brown fox jumps over the lazy dog. \
                The quick brown fox jumps over the lazy dog. <br>The quick brown fox jumps over the lazy dog. \
                The quick brown fox jumps over the lazy dog."
                font.pixelSize: 30
                wrapMode: Text.Wrap
                width: labelsPage.width
            }

        }
    }
}
