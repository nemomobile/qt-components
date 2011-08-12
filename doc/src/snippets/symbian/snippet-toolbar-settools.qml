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

//! [0]
import com.nokia.symbian 1.1

Window {

    Text {
        id: label1
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: platformStyle.paddingLarge
        }
        text: "Call setTools on a ToolBar to change the current ToolBarLayout. Different combinations of iconic ToolButton are illustrated."
        wrapMode: Text.WordWrap;
        color: "white"
        font {
            family: platformStyle.fontFamilyRegular
            pixelSize: platformStyle.fontSizeMedium
        }
    }

    Flow {
        anchors {
            left: parent.left
            right: parent.right
            top: label1.bottom
            bottom: sharedToolBar.top
            margins: platformStyle.paddingLarge
        }
        spacing: platformStyle.paddingLarge
        Button {
            text: "0+TB_I+0";
            onClicked: sharedToolBar.setTools(toolBarLayout1)
        }
        Button {
            text: "0+2TB_I+0";
            onClicked: sharedToolBar.setTools(toolBarLayout2)
        }
        Button {
            text: "0+3TB_I+0";
            onClicked: sharedToolBar.setTools(toolBarLayout3a)
        }
        Button {
            text: "B+BR_3TB+M";
            onClicked: sharedToolBar.setTools(toolBarLayout3b)
        }
        Button {
            text: "B+BR_4TB";
            onClicked: sharedToolBar.setTools(toolBarLayout4a)
        }
        Button {
            text: "BR_4TB+M";
            onClicked: sharedToolBar.setTools(toolBarLayout4b)
        }
        //! [button5a]
        Button {
            //! [button5a]
            text: "BR_5TB";
            //! [button5b]
            onClicked: sharedToolBar.setTools(toolBarLayout5)
        }
        //! [button5b]
    }

    // this shared ToolBar hosts the current ToolBarLayout instance
    //! [sharedtoolbar]
    ToolBar {
        id: sharedToolBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
    //! [sharedtoolbar]

    //! [toolbarlayout1]
    ToolBarLayout {
        id: toolBarLayout1

        ToolButton {
            visible: false
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-play"
        }
        ToolButton {
            visible: false
        }
    }
    //! [toolbarlayout1]

    //! [toolbarlayout2]
    ToolBarLayout {
        id: toolBarLayout2

        ToolButton {
            visible: false
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-stop"
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-play"
        }
        ToolButton {
            visible: false
        }
    }
    //! [toolbarlayout2]

    //! [toolbarlayout3a]
    ToolBarLayout {
        id: toolBarLayout3a

        ToolButton {
            visible: false
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-stop"
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-pause"
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-play"
        }
        ToolButton {
            visible: false
        }
    }
    //! [toolbarlayout3a]

    //! [toolbarlayout3b]
    ToolBarLayout {
        id: toolBarLayout3b
        ToolButton { iconSource: "toolbar-back" }

        ButtonRow {
            checkedButton: stop3b

            ToolButton {
                id: stop3b;
                iconSource: "toolbar-mediacontrol-stop"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-pause"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-play"
            }
        }

        ToolButton {
            iconSource: "toolbar-menu"
        }
    }
    //! [toolbarlayout3b]

    //! [toolbarlayout4a]
    ToolBarLayout {
        id: toolBarLayout4a
        ToolButton {
            iconSource: "toolbar-back"
        }

        ButtonRow {
            checkedButton: stop4a

            ToolButton {
                iconSource: "toolbar-mediacontrol-backwards"
            }
            ToolButton {
                id: stop4a
                iconSource: "toolbar-mediacontrol-stop"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-play"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-forward"
            }
        }
    }
    //! [toolbarlayout4a]

    //! [toolbarlayout4b]
    ToolBarLayout {
        id: toolBarLayout4b

        ButtonRow {
            checkedButton: stop4b

            ToolButton {
                iconSource: "toolbar-mediacontrol-backwards"
            }
            ToolButton {
                id: stop4b;
                iconSource: "toolbar-mediacontrol-stop"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-play"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-forward"
            }
        }

        ToolButton {
            iconSource: "toolbar-menu"
        }
    }
    //! [toolbarlayout4b]

    //! [toolbarlayout5a]
    ToolBarLayout {
        id: toolBarLayout5

        ButtonRow {
            checkedButton: stop5

            ToolButton {
                iconSource: "toolbar-mediacontrol-backwards"
            }
            //! [toolbarlayout5a]
            ToolButton {
                iconSource: "toolbar-mediacontrol-pause"
            }
            ToolButton {
                id: stop5;
                iconSource: "toolbar-mediacontrol-stop"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-play"
            }
            ToolButton {
                iconSource: "toolbar-mediacontrol-forward"
            }
            //! [toolbarlayout5b]
        }
    }
    //! [toolbarlayout5b]

    Component.onCompleted:sharedToolBar.setTools(toolBarLayout1)
}
//! [0]
