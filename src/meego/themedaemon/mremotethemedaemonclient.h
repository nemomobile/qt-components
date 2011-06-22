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

#ifndef MREMOTETHEMEDAEMONCLIENT_H
#define MREMOTETHEMEDAEMONCLIENT_H

#include <themedaemon/mabstractthemedaemonclient.h>
#include <themedaemon/mthemedaemonprotocol.h>
#include <QHash>
#include <QLocalSocket>
#include <QPixmap>
#include <QString>

/**
 * \brief Allows to request pixmaps from the remote themedaemon server.
 */
class MRemoteThemeDaemonClient : public MAbstractThemeDaemonClient
{
    Q_OBJECT

public:
    MRemoteThemeDaemonClient(const QString &serverAddress = QString(), QObject *parent = 0);
    virtual ~MRemoteThemeDaemonClient();

    /**
     * \see MAbstractThemeDaemonClient::requestPixmap()
     */
    virtual QPixmap requestPixmap(const QString &id, const QSize &requestedSize);

    /**
     * \return True if the client is connected to the themedaemon server. The connection
     *         is established in the constructor.
     */
    bool isConnected() const;

private Q_SLOTS:
    void connectionDataAvailable();

private:
    /**
     * Waits for the packet with the sequence number \a sequenceNumber.
     * If other packets are received in the meantime, they get handled
     * in MRemoteThemeDaemonClient::processOnePacket().
     */
    M::MThemeDaemonProtocol::Packet waitForPacket(quint64 sequenceNumber);

    /**
     * Processes the packet \a packet. Usually this means that a pixmap
     * has been received and will get applied to the local cache
     * m_pixmapCache.
     */
    void processOnePacket(const M::MThemeDaemonProtocol::Packet &packet);

    /**
     * Reads one packet from m_stream and returns the result.
     */
    M::MThemeDaemonProtocol::Packet readOnePacket();

    /**
     * Adds the handles of most used pixmaps to m_mostUsedPixmaps.
     */
    void addMostUsedPixmaps(const QList<M::MThemeDaemonProtocol::PixmapHandlePacketData> &handles);

    /**
     * Removes the pixmaps indicated by \a identifiers from m_mostUsedPixmaps.
     */
    void removeMostUsedPixmaps(const QList<M::MThemeDaemonProtocol::PixmapIdentifier> &identifiers);

    /**
     * Tries to connect m_socket to the server specified by \a serverAddress.
     * \param timeout Timeout in milliseconds until waiting for a connection
     *                gets aborted. A value smaller or equal 0 means that no
     *                timeout is given.
     */
    bool connectToServer(const QString &serverAddress, int timeout);

    /**
     * Registers the application to the themedaemon server.
     */
    void registerApplication(const QString &applicationName);

    /**
     * \return Priority of the application when requesting a pixmap from
     *         the themedaemon server.
     */
    qint32 priority() const;

    /**
     * Initializes the priority m_priority dependent from the application name:
     * Libmeegotouch provides a setting with application specific priorities.
     */
    void initializePriority(const QString &applicationName);

#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
    /**
     * Verify whether the protocol version used by the client is compatible with
     * the protocol version used by the themedaemon.
     */
    void negotiateProtocolVersion();

    /**
     * Print a warning message in case if \p packet is an error-packet and
     * exit. If \p packet is no error-packet a warning message is print and
     * an exit done afterwards.
     */
    void handleUnexpectedPacket(const M::MThemeDaemonProtocol::Packet &packet);
#endif

    /**
     * \return The pixmap for the given identifier if the identifier is part of
     *         m_mostUsedPixmaps. A null-pixmap is returned otherwise.
     */
    QPixmap pixmapFromMostUsed(const M::MThemeDaemonProtocol::PixmapIdentifier& pixmapId);

    static QPixmap createPixmapFromHandle(const M::MThemeDaemonProtocol::PixmapHandle &pixmapHandle);

    quint64 m_sequenceCounter;
    qint32 m_priority;
    QLocalSocket m_socket;
    QDataStream m_stream;

    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, QPixmap*> m_pixmapCache;

    /**
     * Contains the list of pixmaps that is send by the server to indicate which pixmaps
     * are the most used and hence already might have a handle to the pixmap data.
     */
    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, M::MThemeDaemonProtocol::PixmapHandle> m_mostUsedPixmaps;

    friend class tst_MRemoteThemeDaemonClient; // Unit tests
};

#endif

