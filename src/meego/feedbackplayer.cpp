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

#include "feedbackplayer.h"

#include <QDir>
#include <QString>

namespace {
    // Version information of protocol used with feedback daemon
    const QString FeedbackProtocolVersion("FeedbackProtocolVersion#1");

    const int fastReconnectionTime = 1000;
    const int maximumFastReconnectionCount = 10;
}

// Should have the same value as libmeetouch MFeedback::Press etc
const QString FeedbackPlayer::Press = "priority2_static_press";
const QString FeedbackPlayer::Release = "priority2_static_release";

static const char gSocketServer[] = "/tmp/mfeedbackd/player.sock";

FeedbackPlayer::FeedbackPlayer(QObject *parent)
    : QObject(parent),
      previousSuccessfullConnection(QElapsedTimer())
{
    socketStream.setDevice(&socket);
    connect(&socket, SIGNAL(connected()), SLOT(onConnected()));
    connect(&socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            SLOT(onSocketError(QLocalSocket::LocalSocketError)));

    reconnectionAttempts = 0;
    fastReconnectionCount = 0;
    previousSuccessfullConnection.invalidate();

    // TODO: Load from a config file?
    reconnectionIntervalsList << 10;
    reconnectionIntervalsList << 100;
    reconnectionIntervalsList << 300;
    reconnectionIntervalsList << 600;
    reconnectionIntervalsList << 1000;
    reconnectionIntervalsList << 1000;
    reconnectionIntervalsList << 2000;
    reconnectionIntervalsList << 2000;
    reconnectionIntervalsList << 5000;
    reconnectionIntervalsList << 5000;
    reconnectionIntervalsList << 10000;
    reconnectionIntervalsList << 10000;
    reconnectionIntervalsList << 30000;
    reconnectionIntervalsList << 30000;
    reconnectionIntervalsList << 300000;
    reconnectionIntervalsList << 300000; // 5*60*1000 (five minutes)
}

FeedbackPlayer::~FeedbackPlayer()
{
    // socket emits disconnected() in its destructor. If we don't
    // disconnect from this signal we will get called after being
    // destroyed which will cause a SIGSEGV
    socket.disconnect(this);
}

bool FeedbackPlayer::init(const QString &applicationName)
{
    this->applicationName = applicationName;

    // Initial connection to server is immediate
    connectIdle();

    return true;
}

void FeedbackPlayer::sendPlaybackRequest(const QString &name)
{
    if (socket.state() == QLocalSocket::ConnectedState &&
            name.isEmpty() == false) {
        socketStream << name;
        socket.flush();
    }
}

void FeedbackPlayer::onConnected()
{
    reconnectionAttempts = 0;
    socketStream << FeedbackProtocolVersion;
    socketStream << applicationName;

    previousSuccessfullConnection.start();
}

void FeedbackPlayer::connectIdle()
{
    socket.connectToServer(gSocketServer);
}

void FeedbackPlayer::onSocketError(QLocalSocket::LocalSocketError socketError)
{
    Q_UNUSED(socketError);

    if (reconnectionAttempts == 0) {
        if (previousSuccessfullConnection.isValid() &&
            previousSuccessfullConnection.elapsed() < fastReconnectionTime) {
            // Increment fast reconnection count if time since previous succesfull
            // reconnection is less than fastReconnectionTime.
            fastReconnectionCount++;
        } else {
            fastReconnectionCount = 0;
        }
    }

    if (reconnectionAttempts < reconnectionIntervalsList.size() &&
        fastReconnectionCount <= maximumFastReconnectionCount) {
        // Try to reconnect to mfeedbackd
        QTimer::singleShot(reconnectionIntervalsList[reconnectionAttempts],
                           this, SLOT(connectIdle()));
        reconnectionAttempts++;
    } else {
        // Give up
        qWarning("FeedbackPlayer: Couldn't establish connection with mfeedbackd.");
    }
}
