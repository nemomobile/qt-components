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

var score = 0;
var backlog = [];

var BLUE = "blue";
var GREEN = "green";
var RED = "red";
var ORANGE = "orange";
var colors = [BLUE,RED,GREEN,ORANGE];

function reset() {
    score = 0;
    backlog = [];
}

function check(color) {
    if (backlog.splice(0,1) == color) {
        score++;
        var scoreString = score.toString();
        for (var i = 4 - scoreString.length; i > 0; --i)
            scoreString = "0" + scoreString;
        scoreText.text = scoreString;
    } else {
        gameTimer.stop();
        root.gameOver();
    }
}

function blink() {
    if (backlog.length > 10) {
        gameTimer.stop();
        gameOver();
    }
    var random = Math.floor(Math.random() * 4);
    backlog.push(colors[random]);
    switch (colors[random]) {
    case ORANGE:
        orangeButton.blink(gameTimer.interval);
        break;
    case GREEN:
        greenButton.blink(gameTimer.interval);
        break;
    case RED:
        redButton.blink(gameTimer.interval);
        break;
    case BLUE:
        blueButton.blink(gameTimer.interval);
        break;
    }
}
