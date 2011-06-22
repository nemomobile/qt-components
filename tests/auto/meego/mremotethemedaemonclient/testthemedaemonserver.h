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

#ifndef MTESTTHEMEDAEMONSERVER_H
#define MTESTTHEMEDAEMONSERVER_H

#include <QHash>
#include <QObject>
#include <QLocalServer>

#include <themedaemon/mthemedaemonprotocol.h>

class QDataStream;
class QPixmap;

/**
 * \brief Test implementation of the themedaemon server from libmeegotouch.
 *
 * Implements the core behavior of the themedaemon server to be able to test the
 * themedaemon client implemented by MRemoteThemeDaemonClient.
 */
class TestThemeDaemonServer : public QObject
{
    Q_OBJECT

public:
    /**
     * \param serverAddress Address of the server where the client will be connected to.
     * \param pixmapCache   List of available pixmaps that may get requested by the client.
     * \param parent        Parent object.
     */
    TestThemeDaemonServer(const QString &serverAddress,
                          QHash<M::MThemeDaemonProtocol::PixmapIdentifier, const QPixmap*> pixmapCache,
                          QObject *parent = 0);

    virtual ~TestThemeDaemonServer();

private slots:
    void slotClientConnected();
    void slotClientDataAvailable();

private:
    void sendMostUsedPixmaps(QDataStream &stream, quint64 sequenceNumber);

    bool m_protocolVersionExchanged;
    bool m_clientRegistered;
    QString m_serverAddress;
    QLocalServer* m_server;
    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, const QPixmap*> m_pixmapCache;
};

#endif

