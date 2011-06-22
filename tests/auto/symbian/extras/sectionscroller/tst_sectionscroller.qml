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
    id: testTarget
    width: 640; height: 480

    ListModel {
        id: listModel

        ListElement { name: "A Cat 1"; alphabet: "A" }
        ListElement { name: "A Cat 2"; alphabet: "A" }
        ListElement { name: "A Cat 3"; alphabet: "A" }
        ListElement { name: "A Cat 1"; alphabet: "A" }
        ListElement { name: "A Cat 2"; alphabet: "A" }
        ListElement { name: "A Cat 3"; alphabet: "A" }
        ListElement { name: "Boo 1"; alphabet: "B" }
        ListElement { name: "Boo 2"; alphabet: "B" }
        ListElement { name: "Boo 3"; alphabet: "B" }
        ListElement { name: "Cat 1"; alphabet: "C" }
        ListElement { name: "Cat 2"; alphabet: "C" }
        ListElement { name: "Cat 3"; alphabet: "C" }
        ListElement { name: "Cat 4"; alphabet: "C" }
        ListElement { name: "Cat 5"; alphabet: "C" }
        ListElement { name: "Cat 6"; alphabet: "C" }
        ListElement { name: "Dog 1"; alphabet: "D" }
        ListElement { name: "Dog 2"; alphabet: "D" }
        ListElement { name: "Dog 3"; alphabet: "D" }
        ListElement { name: "Dog 4"; alphabet: "D" }
        ListElement { name: "Dog 5"; alphabet: "D" }
        ListElement { name: "Dog 6"; alphabet: "D" }
        ListElement { name: "Dog 7"; alphabet: "D" }
        ListElement { name: "Dog 8"; alphabet: "D" }
        ListElement { name: "Dog 9"; alphabet: "D" }
        ListElement { name: "Elephant 1"; alphabet: "E" }
        ListElement { name: "Elephant 2"; alphabet: "E" }
        ListElement { name: "Elephant 3"; alphabet: "E" }
        ListElement { name: "Elephant 4"; alphabet: "E" }
        ListElement { name: "Elephant 5"; alphabet: "E" }
        ListElement { name: "Elephant 6"; alphabet: "E" }
        ListElement { name: "FElephant 1"; alphabet: "F" }
        ListElement { name: "FElephant 2"; alphabet: "F" }
        ListElement { name: "FElephant 3"; alphabet: "F" }
        ListElement { name: "FElephant 4"; alphabet: "F" }
        ListElement { name: "FElephant 5"; alphabet: "F" }
        ListElement { name: "FElephant 6"; alphabet: "F" }
        ListElement { name: "Guinea pig"; alphabet: "G" }
        ListElement { name: "Goose"; alphabet: "G" }
        ListElement { name: "Giraffe"; alphabet: "G" }
        ListElement { name: "Guinea pig"; alphabet: "G" }
        ListElement { name: "Goose"; alphabet: "G" }
        ListElement { name: "Giraffe"; alphabet: "G" }
        ListElement { name: "Guinea pig"; alphabet: "G" }
        ListElement { name: "Goose"; alphabet: "G" }
        ListElement { name: "Giraffe"; alphabet: "G" }
        ListElement { name: "Horse"; alphabet: "H" }
        ListElement { name: "Horse"; alphabet: "H" }
        ListElement { name: "Horse"; alphabet: "H" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Parrot"; alphabet: "P" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }
        ListElement { name: "Multi-character"; alphabet: "2001, Apr 24" }

    }

    ListView {
        id: listView

        anchors.fill: parent
        delegate:  Rectangle {
            property string section: name[0]

            width: parent.width
            height: childrenRect.height + (2 * platformStyle.paddingLarge)
            border { color: platformStyle.colorNormalDark; width: 1 }
            color: index % 2 == 0 ? platformStyle.colorNormalLight : platformStyle.colorNormalMid
            Text {
                x: platformStyle.paddingLarge
                y: platformStyle.paddingLarge
                text: name + " (index " + index + ")"
            }
        }

        model: listModel
        clip: true
        section { property: "alphabet"; criteria: ViewSection.FullString }
        section.delegate: Rectangle {
            width: listView.width
            height: childrenRect.height + (2 * platformStyle.paddingSmall)
            color: platformStyle.colorDisabledMid
            Text {
                objectName: "Label" + section
                x: platformStyle.paddingSmall
                y: platformStyle.paddingSmall
                color: platformStyle.colorNormalLight
                text: section
                font { bold: true; pointSize: platformStyle.fontSizeLarge }
            }
        }
    }

    SectionScroller {
        id: sectionScroller
        objectName: "sectionScroller"
        listView: listView
    }
}
