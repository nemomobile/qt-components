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
     * Adds the handles of most used pixmaps to the pixmap cache m_pixmapCache.
     */
    void addMostUsedPixmaps(const QList<M::MThemeDaemonProtocol::PixmapHandlePacketData> &handles);

    /**
     * Removes the pixmaps indicated by \a identifiers from the pixmap cache m_pixmapCache.
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

    static QPixmap createPixmapFromHandle(const M::MThemeDaemonProtocol::PixmapHandle &pixmapHandle);

    quint64 m_sequenceCounter;
    qint32 m_priority;
    QLocalSocket m_socket;
    QDataStream m_stream;

    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, QPixmap*> m_pixmapCache;

    friend class tst_MRemoteThemeDaemonClient; // Unit tests
};

#endif

