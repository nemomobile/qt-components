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
