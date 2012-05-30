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
    id: testItem

    width: 400
    height: 600

    property bool portrait: screen.width < screen.height

    ToolBar {
        tools: ToolBarLayout {
            objectName: "toolBarLayout1"
            ToolButton { objectName: "toolButton1_0"; iconSource: "toolbar-back" }
            ToolButton { objectName: "toolButton1_N"; iconSource: "toolbar-back" }
        }
    }

    ToolBar {
        tools: ToolBarLayout {
            objectName: "toolBarLayout2"
            ToolButton { objectName: "toolButton2_0"; iconSource: "toolbar-back" }
            ToolButton { objectName: "toolButton2_a"; iconSource: "toolbar-home" }
            ToolButton { objectName: "toolButton2_b"; iconSource: "toolbar-home" }
            ToolButton { objectName: "toolButton2_c"; iconSource: "toolbar-home" }
            ToolButton { objectName: "toolButton2_N"; iconSource: "toolbar-back" }
        }
    }

    ToolBar {
        tools: ToolBarLayout {
            objectName: "toolBarLayout3"
            ToolButton { objectName: "toolButton3_0"; iconSource: "toolbar-back" }
            ToolButton { objectName: "toolButton3_a"; text: "a" }
            ToolButton { objectName: "toolButton3_b"; text: "b" }
            ToolButton { objectName: "toolButton3_N"; iconSource: "toolbar-back" }
        }
    }

    ToolBar {
        tools: ToolBarLayout {
            objectName: "toolBarLayout4"
            ToolButton { objectName: "toolButton4_0"; iconSource: "toolbar-back" }
            ButtonRow {
                objectName: "buttonRow4"
                ToolButton { objectName: "toolButton4_a"; text: "a" }
                ToolButton { objectName: "toolButton4_b"; text: "b" }
            }
            ToolButton { objectName: "toolButton4_N"; iconSource: "toolbar-back" }
        }
    }

    ToolBar {
        tools: ToolBarLayout {
            objectName: "toolBarLayout5"
            ToolButton { objectName: "toolButton5_0"; iconSource: "toolbar-back" }
            ButtonRow {
                objectName: "buttonRow5"
                ToolButton { objectName: "toolButton5_a"; text: "a" }
                ToolButton { objectName: "toolButton5_b"; text: "b" }
            }
        }
    }

    ToolBar {
        tools: ToolBarLayout {
            objectName: "toolBarLayout6"
            ToolButton { objectName: "toolButton6_0"; iconSource: "toolbar-back" }
            ButtonRow {
                objectName: "buttonRow6"
                ToolButton { objectName: "toolButton6_a"; text: "a" }
                ToolButton { objectName: "toolButton6_b"; text: "b" }
                ToolButton { objectName: "toolButton6_c"; text: "c" }
            }
        }
    }

    ToolBar {
        tools: ToolBarLayout {
            objectName: "toolBarLayout7"
            ButtonRow {
                objectName: "buttonRow7"
                ToolButton { objectName: "toolButton7_a"; text: "a" }
                ToolButton { objectName: "toolButton7_b"; text: "b" }
                ToolButton { objectName: "toolButton7_c"; text: "c" }
            }
        }
    }
}
