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

#ifndef FEEDBACKPLAYER_H
#define FEEDBACKPLAYER_H

#include <QLocalSocket>
#include <QMap>
#include <QTimer>
#include <QElapsedTimer>
#include <QDataStream>
#include <QObject>

class FeedbackPlayer : public QObject
{
    Q_OBJECT
public:
    FeedbackPlayer(QObject *parent);
    virtual ~FeedbackPlayer();

    bool init(const QString &applicationName);

    void sendPlaybackRequest(const QString &name);

private slots:
    void onConnected();
    void onSocketError(QLocalSocket::LocalSocketError socketError);
    void connectIdle();

public:
    QLocalSocket socket;
    QDataStream socketStream;
    QString applicationName;

    // Number of reconnection attempts. This value is zeroed when a connection is
    // successfully established.
    int reconnectionAttempts;

    // Contains the intervals, in milliseconds, to wait before attempting
    // to reconnect to feedback-manager daemon.
    // list[0] is the time to wait before trying to connect and reconnect for the first time.
    // If the first attempt fails then it waits list[1] milliseconds before attempting
    // to reconnect for the second time and so on.
    // It gives up trying to reconnect when failedReconnections == list.size()
    QList<int> reconnectionIntervalsList;

    // Contains time (in milliseconds) from the previous successfull connection
    // to the feedback daemon.
    QElapsedTimer previousSuccessfullConnection;

    // Contains number of successive successfull connections to feedback daemon
    // that have happened in less than fastReconnectionTime.
    int fastReconnectionCount;

    // String to be sent to the haptics daemon when press is played (see libmeegotouch MFeedback::Press)
    static const QString Press;
    static const QString Release;
};

#endif
