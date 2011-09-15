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
import com.nokia.extras 1.1

Page {
    id: sectionScrollerPage
    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    Item {
        anchors.fill: parent

        ListModel {
            id: testModel

            ListElement { title: "A Cat 1"; alphabet: "A" }
            ListElement { title: "A Cat 2"; alphabet: "A" }
            ListElement { title: "A Cat 3"; alphabet: "A" }
            ListElement { title: "A Cat 1"; alphabet: "A" }
            ListElement { title: "A Cat 2"; alphabet: "A" }
            ListElement { title: "A Cat 3"; alphabet: "A" }
            ListElement { title: "Boo 1"; alphabet: "B" }
            ListElement { title: "Boo 2"; alphabet: "B" }
            ListElement { title: "Boo 3"; alphabet: "B" }
            ListElement { title: "Cat 1"; alphabet: "C" }
            ListElement { title: "Cat 2"; alphabet: "C" }
            ListElement { title: "Cat 3"; alphabet: "C" }
            ListElement { title: "Cat 4"; alphabet: "C" }
            ListElement { title: "Cat 5"; alphabet: "C" }
            ListElement { title: "Cat 6"; alphabet: "C" }
            ListElement { title: "Dog 1"; alphabet: "D" }
            ListElement { title: "Dog 2"; alphabet: "D" }
            ListElement { title: "Dog 3"; alphabet: "D" }
            ListElement { title: "Dog 4"; alphabet: "D" }
            ListElement { title: "Dog 5"; alphabet: "D" }
            ListElement { title: "Dog 6"; alphabet: "D" }
            ListElement { title: "Dog 7"; alphabet: "D" }
            ListElement { title: "Dog 8"; alphabet: "D" }
            ListElement { title: "Dog 9"; alphabet: "D" }
            ListElement { title: "Elephant 1"; alphabet: "E" }
            ListElement { title: "Elephant 2"; alphabet: "E" }
            ListElement { title: "Elephant 3"; alphabet: "E" }
            ListElement { title: "Elephant 4"; alphabet: "E" }
            ListElement { title: "Elephant 5"; alphabet: "E" }
            ListElement { title: "Elephant 6"; alphabet: "E" }
            ListElement { title: "FElephant 1"; alphabet: "F" }
            ListElement { title: "FElephant 2"; alphabet: "F" }
            ListElement { title: "FElephant 3"; alphabet: "F" }
            ListElement { title: "FElephant 4"; alphabet: "F" }
            ListElement { title: "FElephant 5"; alphabet: "F" }
            ListElement { title: "FElephant 6"; alphabet: "F" }
            ListElement { title: "Guinea pig"; alphabet: "G" }
            ListElement { title: "Goose"; alphabet: "G" }
            ListElement { title: "Giraffe"; alphabet: "G" }
            ListElement { title: "Guinea pig"; alphabet: "G" }
            ListElement { title: "Goose"; alphabet: "G" }
            ListElement { title: "Giraffe"; alphabet: "G" }
            ListElement { title: "Guinea pig"; alphabet: "G" }
            ListElement { title: "Goose"; alphabet: "G" }
            ListElement { title: "Giraffe"; alphabet: "G" }
            ListElement { title: "Horse"; alphabet: "H" }
            ListElement { title: "Horse"; alphabet: "H" }
            ListElement { title: "Horse"; alphabet: "H" }
            ListElement { title: "Parrot"; alphabet: "P" }
            ListElement { title: "Parrot"; alphabet: "P" }
            ListElement { title: "Parrot"; alphabet: "P" }
            ListElement { title: "Parrot"; alphabet: "P" }
        }

        ListView {
            id: list

            anchors.fill: parent
            delegate:  ListDelegate {}

            model: testModel
            section.property: "alphabet"
            section.criteria: ViewSection.FullString
            section.delegate: Item {
                // "GroupHeader" component?
                width: parent.width
                height: 40
                Text {
                    id: headerLabel
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 8
                    anchors.bottomMargin: 2
                    text: section
                    font.bold: true
                    font.pointSize: 18
                    color: theme.inverted ? "#4D4D4D" : "#3C3C3C";
                }
                Image {
                    anchors.right: headerLabel.left
                    anchors.left: parent.left
                    anchors.verticalCenter: headerLabel.verticalCenter
                    anchors.rightMargin: 24
                    source: "image://theme/meegotouch-groupheader" + (theme.inverted ? "-inverted" : "") + "-background"
                }
            }
        }

        SectionScroller {
            listView: list
        }
        ScrollDecorator {
            flickableItem: list
        }
    }
}
