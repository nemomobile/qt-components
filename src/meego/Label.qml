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
import "EditBubble.js" as Popup
import "UIConstants.js" as UI

Text {
    id: root

    // Common public API
    property bool platformSelectable: false
    // Styling for the Button
    property Style platformStyle: LabelStyle{}

    //Deprecated, TODO Remove this on w13
    property alias style: root.platformStyle
    property color __textColor;

    property bool platformEnableEditBubble: true

    font.family: platformStyle.fontFamily
    font.pixelSize: platformStyle.fontPixelSize
    color: platformStyle.textColor

    wrapMode: Text.Wrap

    MouseArea {
        id: mouseFilter
        anchors.fill: parent

        Component {
            id: textSelectionComponent

            TextEdit {
                id: selectionTextEdit

                property bool canPaste: false
                readOnly: true
                selectByMouse: true

                clip : root.clip
                color : __textColor
                font.bold : root.font.bold
                font.capitalization : root.font.capitalization
                font.family : root.font.family
                font.italic : root.font.italic
                font.letterSpacing : root.font.letterSpacing
                font.pixelSize : root.font.pixelSize
                font.pointSize : root.font.pointSize
                font.strikeout : root.font.strikeout
                font.underline : root.font.underline
                font.weight : root.font.weight
                font.wordSpacing : root.font.wordSpacing
                horizontalAlignment : root.horizontalAlignment
                smooth : root.smooth
                text : root.text
                textFormat : root.textFormat
                verticalAlignment : root.verticalAlignment
                wrapMode : root.wrapMode

                Component.onCompleted: {
                    if ( root.elide == Text.ElideNone ) {
                        width = root.width;
                        height = root.height;
                    }
                    __textColor = root.color;
                    root.color = Qt.rgba(0, 0, 0, 0);
                    selectAll();
                    if (platformEnableEditBubble) {
                         Popup.open(selectionTextEdit,selectionTextEdit.positionToRectangle(selectionTextEdit.cursorPosition));
                    }
                }
                Component.onDestruction: {
                    root.color = __textColor;

                    if (Popup.isOpened(selectionTextEdit)) {
                        Popup.close(selectionTextEdit);
                    }
                }

                onSelectedTextChanged: {
                    if (Popup.isOpened(selectionTextEdit)) {
                        Popup.close(selectionTextEdit);
                    }
                }

                MouseFilter {
                    id: mouseSelectionFilter
                    anchors.fill: parent

                    onFinished: {
                        if (platformEnableEditBubble) {
                            Popup.open(selectionTextEdit,selectionTextEdit.positionToRectangle(selectionTextEdit.cursorPosition));
                        }
                    }
                }
                InverseMouseArea {
                    anchors.fill: parent
                    enabled: textSelectionLoader.sourceComponent != undefined

                    onPressedOutside: { // Pressed instead of Clicked to prevent selection overlap
                        if (Popup.isOpened(selectionTextEdit) && ((mouseX > Popup.geometry().left && mouseX < Popup.geometry().right) &&
                                                       (mouseY > Popup.geometry().top && mouseY < Popup.geometry().bottom))) {
                            return;
                        }
                        textSelectionLoader.sourceComponent = undefined;
                    }
                    onClickedOutside: { // Handles Copy click
                        if (Popup.isOpened(selectionTextEdit) && ((mouseX > Popup.geometry().left && mouseX < Popup.geometry().right) &&
                                                       (mouseY > Popup.geometry().top && mouseY < Popup.geometry().bottom))) {
                            textSelectionLoader.sourceComponent = undefined;
                            return;
                        }
                    }
                }
            }
        }
        Loader {
          id: textSelectionLoader
        }

        onPressAndHold:{
            if (root.platformSelectable == false) return;

            textSelectionLoader.sourceComponent = textSelectionComponent;
        }
    }
}
