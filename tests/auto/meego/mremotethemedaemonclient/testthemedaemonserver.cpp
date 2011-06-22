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

#include "testthemedaemonserver.h"

#include <themedaemon/mthemedaemonprotocol.h>

#include <QDataStream>
#include <QLocalSocket>
#include <QPixmap>

using namespace M::MThemeDaemonProtocol;

TestThemeDaemonServer::TestThemeDaemonServer(const QString &serverAddress,
                                             QHash<PixmapIdentifier, const QPixmap*> pixmapCache,
                                             QObject *parent) :
    QObject(parent),
    m_protocolVersionExchanged(false),
    m_clientRegistered(false),
    m_server(0),
    m_pixmapCache(pixmapCache)
{
    QLocalServer::removeServer(serverAddress);

    m_server = new QLocalServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(slotClientConnected()));
    if (!m_server->listen(serverAddress)) {
        qWarning() << "Not able to start server for" << serverAddress;
    }
}

TestThemeDaemonServer::~TestThemeDaemonServer()
{
}

void TestThemeDaemonServer::slotClientConnected()
{
    while (m_server->hasPendingConnections()) {
        QLocalSocket *socket = m_server->nextPendingConnection();
        connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(slotClientDataAvailable()));
    }
}

void TestThemeDaemonServer::slotClientDataAvailable()
{
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    if (!socket) {
        return;
    }

    while (socket->bytesAvailable()) {
        while (socket->bytesAvailable() < sizeof(quint32)) {
            socket->waitForReadyRead();
        }

        QDataStream stream(socket);
        stream.setVersion(QDataStream::Qt_4_6);

        Packet packet;
        stream >> packet;

#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
        if (!m_protocolVersionExchanged && (packet.type() == Packet::ProtocolVersionPacket)) {
            m_protocolVersionExchanged = true;
        }

        if (!m_protocolVersionExchanged) {
            // A packet has been requested before the client has send an protocol version packet
            stream << Packet(Packet::ErrorPacket, packet.sequenceNumber(),
                      new StringPacketData("A protocol version packet is required before requesting anything else"));
            return;
        }

        if (packet.type() != Packet::ProtocolVersionPacket) {
#endif
            if (!m_clientRegistered && (packet.type() == Packet::RequestRegistrationPacket)) {
                m_clientRegistered = true;
            }

            if (!m_clientRegistered) {
                // A packet has been requested before the client has send an initial registration packet
                stream << Packet(Packet::ErrorPacket, packet.sequenceNumber(),
                          new StringPacketData("A registration packet is required as second packet after the protocol version packet"));
                return;
            }
#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
        }
#endif

        switch (packet.type()) {
        case Packet::RequestPixmapPacket: {
            // The client request a pixmap with a given image-id and size
            const RequestedPixmapPacketData *requestedPixmap =
                static_cast<const RequestedPixmapPacketData *>(packet.data());
            const PixmapIdentifier id = requestedPixmap->id;

            if (id.imageId == QLatin1String("test_pixmap_3")) {
                // See comment in tst_MRemoteThemeDaemonClient::testMostUsedPixmaps(): We use this image-ID
                // as trigger to send a list of most used pixmaps.
                sendMostUsedPixmaps(stream, packet.sequenceNumber());
            }

            const QPixmap *pixmap = m_pixmapCache.value(id, 0);
            if (pixmap) {
                // The requested pixmap has been found, forward it to the client

                // TODO: As soon as the handling of the meego graphicssystem sharing is clarified, the line:
                // QApplication::setGraphicsSystem("native");
                // from main.cpp should be removed and a correct assignment should be done here.
                PixmapHandle pixmapHandle;
                pixmapHandle.xHandle = pixmap->handle();

                stream << Packet(Packet::PixmapUpdatedPacket,
                                 packet.sequenceNumber(),
                                 new PixmapHandlePacketData(id, pixmapHandle));
            } else {
                // The requested pixmap has not been found, forward an empty pixmap handle to the client
                stream << Packet(Packet::PixmapUpdatedPacket,
                                 packet.sequenceNumber(),
                                 new PixmapHandlePacketData());
            }

            break;
        }

        case Packet::ReleasePixmapPacket: {
            const PixmapIdentifier *pixmapId =
                    static_cast<const PixmapIdentifier*>(packet.data());
            if (!m_pixmapCache.contains(*pixmapId)) {
                qWarning() << "Released pixmap that has not been requested first";
            }
            break;
        }

#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
        case Packet::ProtocolVersionPacket:
            stream << Packet(Packet::ProtocolVersionPacket,
                             packet.sequenceNumber(),
                             new NumberPacketData(M::MThemeDaemonProtocol::protocolVersion));
            break;

        case Packet::RequestRegistrationPacket:
            stream << Packet(Packet::ThemeChangedPacket,
                             packet.sequenceNumber(),
                             new ThemeChangeInfoPacketData(QStringList(), QStringList()));
            break;
#else
        case Packet::RequestRegistrationPacket:
            break;
#endif

        default:
            qWarning() << "Packet type" << packet.type() << "not handled!";
            break;
        }
    }
}

void TestThemeDaemonServer::sendMostUsedPixmaps(QDataStream &stream, quint64 sequenceNumber)
{
    // For testing purposes as added handles the whole pixmap cache is send. For the removed
    // identifiers the first entry will get removed.
    ++sequenceNumber;

    QList<PixmapHandlePacketData> addedHandles;
    QHash<PixmapIdentifier, const QPixmap*>::const_iterator it = m_pixmapCache.constBegin();
    while (it != m_pixmapCache.constEnd()) {
        PixmapHandle handle;
        handle.xHandle = it.value()->handle();
        PixmapHandlePacketData pixmapHandle(it.key(), handle);
        addedHandles.append(pixmapHandle);
        ++it;
    }

    QList<PixmapIdentifier> removedIdentifiers;
    it = m_pixmapCache.constBegin();
    if (it != m_pixmapCache.constEnd()) {
        removedIdentifiers.append(it.key());
    }

    stream << Packet(Packet::MostUsedPixmapsPacket,
                     sequenceNumber,
                     new MostUsedPixmapsPacketData(addedHandles, removedIdentifiers));
}
