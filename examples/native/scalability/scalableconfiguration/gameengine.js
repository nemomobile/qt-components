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
