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

#include "mremotethemedaemonclient.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QPainter>
#include <QSettings>
#include <QTime>
#include <QUuid>

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
#include <QtMeeGoGraphicsSystemHelper>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#endif

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

using namespace M::MThemeDaemonProtocol;

MRemoteThemeDaemonClient::MRemoteThemeDaemonClient(const QString &serverAddress, QObject *parent) :
    MAbstractThemeDaemonClient(parent),
    m_sequenceCounter(0),
    m_priority(100),
    m_socket(),
    m_stream(),
    m_pixmapCache(),
    m_mostUsedPixmaps()
{
    m_stream.setVersion(QDataStream::Qt_4_6);

    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));

    const QString address = serverAddress.isEmpty() ? M::MThemeDaemonProtocol::ServerAddress : serverAddress;
    if (connectToServer(address, 2000)) {
        m_stream.setDevice(&m_socket);
#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
        negotiateProtocolVersion();
#endif

        QString applicationName = QCoreApplication::instance()->applicationName();
        if (applicationName.isEmpty()) {
            applicationName = QUuid::createUuid().toString();
        }
        registerApplication(applicationName);
        initializePriority(applicationName);
    } else {
        qWarning() << "RemoteThemeDaemonClient: Failed to connect to theme server (that's OK if you're on a PC)";
    }
}

MRemoteThemeDaemonClient::~MRemoteThemeDaemonClient()
{
    // Tell the themedaemon server to release all requested pixmaps
    QHash<PixmapIdentifier, QPixmap*>::const_iterator it = m_pixmapCache.constBegin();
    while (it != m_pixmapCache.constEnd()) {
        ++m_sequenceCounter;
        m_stream << Packet(Packet::ReleasePixmapPacket,
                           m_sequenceCounter,
                           new PixmapIdentifier(it.key()));
        ++it;
    }

    m_socket.disconnectFromServer();
    qDeleteAll(m_pixmapCache);
}

QPixmap MRemoteThemeDaemonClient::requestPixmap(const QString &id, const QSize &requestedSize)
{
    QSize size = requestedSize;
    if (size.width() < 1) {
        size.rwidth() = 0;
    }
    if (size.height() < 1) {
        size.rheight() = 0;
    }

    const PixmapIdentifier pixmapId(id, size);
    QPixmap *pixmap = m_pixmapCache.value(pixmapId, 0);
    if (pixmap) {
        // The pixmap is already cached
        return *pixmap;
    }

    // The pixmap has not been cached yet: Create a pixmap placeholder
    // and apply the pixmap-data
    pixmap = new QPixmap();
    m_pixmapCache.insert(pixmapId, pixmap);

    if (m_mostUsedPixmaps.contains(pixmapId)) {
        // The requested is marked as "most used" and hence can be created
        // without requesting it from the themedaemon server
        *pixmap = pixmapFromMostUsed(pixmapId);
        if (!pixmap->isNull()) {
            // The pixmap is cached now in m_pixmapCache
            m_mostUsedPixmaps.remove(pixmapId);
        }
    } else {
        // Request the pixmap from the themedaemon server
        ++m_sequenceCounter;
        m_stream << Packet(Packet::RequestPixmapPacket,
                           m_sequenceCounter,
                           new RequestedPixmapPacketData(pixmapId, priority()));
        const Packet reply = waitForPacket(m_sequenceCounter);
        processOnePacket(reply);
    }

    if (pixmap->isNull()) {
        // The requested pixmap is not available
        delete pixmap;
        pixmap = 0;
        m_pixmapCache.remove(pixmapId);
        return QPixmap();
    }

    return *pixmap;
}

bool MRemoteThemeDaemonClient::isConnected() const
{
    return m_socket.state() == QLocalSocket::ConnectedState;
}

void MRemoteThemeDaemonClient::connectionDataAvailable()
{
    // When reading a packet block all signals to not start
    // reading a second one
    const bool blocked = m_socket.blockSignals(true);
    while (m_socket.bytesAvailable()) {
        processOnePacket(readOnePacket());
    }

    m_socket.blockSignals(blocked);
}

Packet MRemoteThemeDaemonClient::waitForPacket(quint64 sequenceNumber)
{
    m_socket.flush();

    QObject::disconnect(&m_socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));

    while (m_socket.waitForReadyRead(3000)) {
        while (m_socket.bytesAvailable()) {
            const Packet packet = readOnePacket();
            if (packet.sequenceNumber() == sequenceNumber) {
                QObject::connect(&m_socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));
                connectionDataAvailable();
                return packet;
            }
            // If it was not the packet we're waiting for, lets process it
            processOnePacket(packet);
        }
    }

    QObject::connect(&m_socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));
    return Packet();

}

void MRemoteThemeDaemonClient::processOnePacket(const Packet &packet)
{
    switch (packet.type()) {
    case Packet::PixmapUpdatedPacket: {
        const PixmapHandlePacketData *handle =
            static_cast<const PixmapHandlePacketData *>(packet.data());

        if (m_pixmapCache.contains(handle->identifier)) {
            QPixmap *pixmap = m_pixmapCache.value(handle->identifier);
            *pixmap = createPixmapFromHandle(handle->pixmapHandle);
        }
        break;
    }

    case Packet::MostUsedPixmapsPacket: {
        const MostUsedPixmapsPacketData *mostUsedPacket =
            static_cast<const MostUsedPixmapsPacketData*>(packet.data());

        addMostUsedPixmaps(mostUsedPacket->addedHandles);
        if (!mostUsedPacket->removedIdentifiers.empty()) {
            removeMostUsedPixmaps(mostUsedPacket->removedIdentifiers);
            m_stream << Packet(Packet::AckMostUsedPixmapsPacket, packet.sequenceNumber());
        }
        break;
    }

    case Packet::ErrorPacket:
        qWarning() << "Packet::ErrorPacket:" << static_cast<const StringPacketData*>(packet.data())->string;
        break;

    case Packet::ThemeChangedPacket:
    case Packet::ThemeChangeCompletedPacket:
    default:
        break;
    }
}

Packet MRemoteThemeDaemonClient::readOnePacket()
{
    Packet packet;
    m_stream >> packet;
    return packet;
}

void MRemoteThemeDaemonClient::addMostUsedPixmaps(const QList<PixmapHandlePacketData> &handles)
{
    foreach (const PixmapHandlePacketData &handle, handles) {
        if (!m_mostUsedPixmaps.contains(handle.identifier)) {
            m_mostUsedPixmaps[handle.identifier] = handle.pixmapHandle;
        }
    }
}

void MRemoteThemeDaemonClient::removeMostUsedPixmaps(const QList<PixmapIdentifier> &identifiers)
{
    foreach (const PixmapIdentifier &identifier, identifiers) {
        m_mostUsedPixmaps.remove(identifier);
    }
}

bool MRemoteThemeDaemonClient::connectToServer(const QString &serverAddress, int timeout)
{
    QTime time;
    time.start();

    while (1) {
        // Try to connect
        m_socket.connectToServer(serverAddress);
        if (m_socket.state() == QLocalSocket::ConnectedState) {
            return true;
        }

        // Check for timeout
        if ((timeout > 0) && (time.elapsed() >= timeout)) {
            return false;
        }

        // Wait for a while
#ifndef Q_OS_WIN
        sleep(1);
#else
        Sleep(1000);
#endif
    }
}

void MRemoteThemeDaemonClient::registerApplication(const QString &applicationName)
{
    ++m_sequenceCounter;
    m_stream << Packet(Packet::RequestRegistrationPacket,
                       m_sequenceCounter,
                       new StringPacketData(applicationName));
#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
    M::MThemeDaemonProtocol::Packet reply = waitForPacket(m_sequenceCounter);
    if (reply.type() == M::MThemeDaemonProtocol::Packet::ThemeChangedPacket) {
        // TODO: The packet Packet::ThemeChangedPacket provides the
        // theme inheritance chain and the theme libraries names. Currently we ignore this.
    } else {
       handleUnexpectedPacket(reply);
    }
#else
    waitForPacket(m_sequenceCounter);
    // TODO: The themedaemon replies a Packet::ThemeChangedPacket, which provides the
    // theme inheritance chain and the theme libraries names. Currently we ignore this.
#endif
}

qint32 MRemoteThemeDaemonClient::priority() const
{
    return m_priority;
}

void MRemoteThemeDaemonClient::initializePriority(const QString &applicationName)
{
    // TODO: The libmeego specific applications have additional criteria for
    // the priority:
    // - Prestarted applications get a priority of -10
    // - If the window is on the display a priority of 100 is used
    // - If a window is in background a priority of 0 is used if there is
    //   no application specific priority

    QSettings settings("/etc/meegotouch/themedaemonpriorities.conf", QSettings::IniFormat);
    if (settings.status() == QSettings::NoError) {
        m_priority = settings.value("ForegroundApplication/priority", m_priority).toInt();
        settings.beginGroup("SpecificApplicationPriorities");
        if (!applicationName.isEmpty() && settings.contains(applicationName)) {
            m_priority = settings.value(applicationName).toInt();
        }
    }
}

#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
    void MRemoteThemeDaemonClient::negotiateProtocolVersion()
    {
        m_stream << Packet(Packet::ProtocolVersionPacket,
                           m_sequenceCounter,
                           new M::MThemeDaemonProtocol::NumberPacketData(M::MThemeDaemonProtocol::protocolVersion));
        Packet reply = waitForPacket(m_sequenceCounter);
        if (reply.type() == Packet::ProtocolVersionPacket) {
            const M::MThemeDaemonProtocol::NumberPacketData* protocolVersion =
                    static_cast<const M::MThemeDaemonProtocol::NumberPacketData*>(reply.data());
            if (protocolVersion->value != M::MThemeDaemonProtocol::protocolVersion) {
                qCritical("Running themedaemon and this client do not support the same protocol version.\n"
                          "Maybe you need to restart the themedaemon server or to upgrade your installation.\n"
                          "Exiting.");
                exit(EXIT_FAILURE);
            }
        } else {
            handleUnexpectedPacket(reply);
        }
    }

    void MRemoteThemeDaemonClient::handleUnexpectedPacket(const M::MThemeDaemonProtocol::Packet &packet)
    {
        if (packet.type() == Packet::ErrorPacket) {
            const M::MThemeDaemonProtocol::StringPacketData *errorString =
                    static_cast<const M::MThemeDaemonProtocol::StringPacketData*>(packet.data());
            qCritical() << "Themedaemon replied with error packet:\n" << errorString->string << "\nExiting.";
            exit(EXIT_FAILURE);
        } else {
            qCritical() << "Received unexpected packet from themedaemon. Exiting.";
            exit(EXIT_FAILURE);
        }
    }
#endif

QPixmap MRemoteThemeDaemonClient::pixmapFromMostUsed(const M::MThemeDaemonProtocol::PixmapIdentifier& pixmapId)
{
    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, M::MThemeDaemonProtocol::PixmapHandle>::iterator it = m_mostUsedPixmaps.find(pixmapId);
    if (it != m_mostUsedPixmaps.end()) {
        ++m_sequenceCounter;
        m_stream << Packet(Packet::PixmapUsedPacket,
                           m_sequenceCounter,
                           new M::MThemeDaemonProtocol::PixmapIdentifier(pixmapId.imageId, pixmapId.size));
        return createPixmapFromHandle(it.value());
    }

    return QPixmap();
}

QPixmap MRemoteThemeDaemonClient::createPixmapFromHandle(const PixmapHandle& pixmapHandle)
{
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    const bool isMeeGoRunning =  QMeeGoGraphicsSystemHelper::isRunningRuntime() &&
                                 (QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("meego") ||
                                  QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("raster"));
    if (isMeeGoRunning && pixmapHandle.eglHandle) {
        int fd = -1;
#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
        if (pixmapHandle.directMap)
            fd = open(pixmapHandle.shmHandle.constData(), O_RDONLY);
        else
            fd = shm_open(pixmapHandle.shmHandle.constData(), O_RDONLY, 0444);

        if (fd == -1) {
            qFatal("Failed to open shared memory: %s, %s", strerror(errno), pixmapHandle.shmHandle.constData());
        }
#else
        if (pixmapHandle.directMap)
            fd = open(qPrintable(pixmapHandle.shmHandle), O_RDONLY);
        else
            fd = shm_open(qPrintable(pixmapHandle.shmHandle), O_RDONLY, 0444);

        if (fd == -1) {
            qFatal("Failed to open shared memory: %s, %s", strerror(errno), qPrintable(pixmapHandle.shmHandle));
        }
#endif

        void *addr = mmap(0, pixmapHandle.numBytes, PROT_READ, MAP_SHARED, fd, 0);
        close(fd);
        if (addr == (void*)-1) {
            qFatal("mmap failed: %s", strerror(errno));
        }

        QImage image((const uchar*)addr, pixmapHandle.size.width(), pixmapHandle.size.height(), pixmapHandle.format);

        return QMeeGoGraphicsSystemHelper::pixmapFromEGLSharedImage(pixmapHandle.eglHandle, image);
    }
#endif

#ifdef Q_WS_X11
    if (!pixmapHandle.xHandle) {
        if (pixmapHandle.eglHandle) {
            qWarning("Valid eglHandle received but not running with meego compatible graphicssystem.");
            return QPixmap();
        } else {
            qWarning("No valid handle to create pixmap from received.");
            return QPixmap();
        }
    }
    return QPixmap::fromX11Pixmap(pixmapHandle.xHandle, QPixmap::ImplicitlyShared);
#else
    QPixmap *pixmapPointer = reinterpret_cast<QPixmap*>(pixmapHandle.xHandle);
    return *pixmapPointer;
#endif
}
