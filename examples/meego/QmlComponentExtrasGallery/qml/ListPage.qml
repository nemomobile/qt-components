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
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Page {
    id: listPage
    anchors.margins: UiConstants.DefaultMargin

    tools:
        ToolBarLayout {
            ToolIcon {
               iconId: "toolbar-view-menu"; onClicked: myMenu.open();
               anchors.right: parent==undefined ? undefined : parent.right
            }
        }

    function openFile(file) {
        var component = Qt.createComponent(file)

        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    ListModel {
        id: pagesModel
        ListElement {
            page: "CountBubble.qml"
            title: "CountBubble"
            subtitle: "Indicates current count"
        }
        ListElement {
            page: "NetPromoterScore.qml"
            title: "NetPromoterScore"
            subtitle: "Shows net promotion query page"
        }
        ListElement {
            page: "PageIndicator.qml"
            title: "PageIndicator"
            subtitle: "Indicates total & current pages"
        }
        ListElement {
            page: "RatingIndicator.qml"
            title: "RatingIndicator"
            subtitle: "Indicates ratings"
        }
        ListElement {
            page: "ListDelegates.qml"
            title: "List Delegates"
            subtitle: "Various List Delegates"
        }
        ListElement {
            page: "MoreIndicator.qml"
            title: "MoreIndicator"
            subtitle: "Indicates that there are more contents"
        }
        ListElement {
            page: "StyledButtons.qml"
            title: "StyledButtons"
            subtitle: "Buttons with different styles"
        }
        ListElement {
            page: "InfoBanner.qml"
            title: "InfoBanner"
            subtitle: "Shows information to user"
        }
        ListElement {
            page: "Tumbler.qml"
            title: "Tumbler"
            subtitle: "A date tumbler"
        }
        ListElement {
            page: "TumblerDialog.qml"
            title: "TumblerDialog"
            subtitle: "A date tumbler dialog"
        }
        ListElement {
            page: "DatePickerDialog.qml"
            title: "DatePickerDialog"
            subtitle: "A date picker dialog"
        }
        ListElement {
            page: "TimePickerDialog.qml"
            title: "TimePickerDialog"
            subtitle: "A time picker dialog"
        }
    }

    ListView {
        id: listview
        anchors.fill: parent
        model: pagesModel

        delegate: ListDelegate {
            Image {
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }

            subtitleColor: "#cc6633"
            onClicked: { openFile(model.page); }
        }
    }
    ScrollDecorator {
        flickableItem: listview
    }
}
