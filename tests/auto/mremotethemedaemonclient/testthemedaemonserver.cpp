/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

        if (!m_clientRegistered && (packet.type() == Packet::RequestRegistrationPacket)) {
            m_clientRegistered = true;
        }

        if (!m_clientRegistered) {
            // A packet has been requested before the client has send an initial registration packet
            stream << Packet(Packet::ErrorPacket, packet.sequenceNumber(),
                      new StringPacketData("A registration packet is required before requesting anything else"));
            return;
        }

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

        case Packet::RequestRegistrationPacket:
            break;

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
