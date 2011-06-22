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
import "gameengine.js" as GameEngine

Rectangle {
    id: root

    property Item configuration

    //![startGame]
    function startGame() {
        GameEngine.reset();
        scoreText.text = "0000";
        gameTimer.interval = root.configuration.interval;
        gameTimer.start();
    }
    //![startGame]

    function gameOver() {
        statusRect.text = "Game Over\n\nScore: "
                + scoreText.text
                + "\n\nTap to restart game!";
        statusRect.visible = true;
    }

    anchors.fill: parent
    color: "black"

    Rectangle {
        id: statusRect

        property string text: "Tap on the buttons\n in the sequence\n"
                              + " that they flash in.\n\nTap to start game!"

        anchors.centerIn: parent
        border.width: 3
        border.color: "red"
        color: "black"
        opacity: 0.8
        radius: root.configuration.radius
        width: statusText.width + 10
        height: statusText.height + 10
        z: 1

        //![statusText]
        Text {
            id: statusText

            text: statusRect.text
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            font: root.configuration.smallFont
            color: "red"
        }
        //![statusText]

        MouseArea {
            anchors.fill: parent
            onClicked: {
                statusRect.visible = false;
                root.startGame();
            }
        }
    }

    Text {
        id: scoreText

        color: "white"
        text: "0000"
        anchors { top: parent.top; bottom: grid.top; horizontalCenter: parent.horizontalCenter }
        verticalAlignment: Text.AlignVCenter
        font: root.configuration.largeFont
    }

    //![grid]
    Grid {
        id: grid

        property int cellWidth: (width - ((columns - 1) * spacing)) / columns

        anchors { left: parent.left; bottom: parent.bottom; right: parent.right }
        columns: root.configuration.columns
        rows: root.configuration.rows
        spacing: root.configuration.spacing
        //![grid]

        GameButton {
            id: orangeButton

            width: grid.cellWidth
            radius: root.configuration.radius
            bottomColor: GameEngine.ORANGE
            enabled: gameTimer.running
            onClicked: GameEngine.check(GameEngine.ORANGE)
        }

        //![greenButton]
        GameButton {
            id: greenButton

            width: grid.cellWidth
            radius: root.configuration.radius
            bottomColor: GameEngine.GREEN
            enabled: gameTimer.running
            onClicked: GameEngine.check(GameEngine.GREEN)
        }
        //![greenButton]

        GameButton {
            id: redButton

            width: grid.cellWidth
            radius: root.configuration.radius
            bottomColor: GameEngine.RED
            enabled: gameTimer.running
            onClicked: GameEngine.check(GameEngine.RED)
        }

        GameButton {
            id: blueButton

            width: grid.cellWidth
            radius: root.configuration.radius
            bottomColor: GameEngine.BLUE
            enabled: gameTimer.running
            onClicked: GameEngine.check(GameEngine.BLUE)
        }
    }

    //![gameTimer]
    Timer {
        id: gameTimer

        repeat:true
        onTriggered: {
            gameTimer.interval -= root.configuration.intervalDecrease;
            GameEngine.blink();
        }
    }
    //![gameTimer]
}
