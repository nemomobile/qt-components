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

#include "mremotethemedaemonclient.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QPainter>
#include <QSettings>
#include <QTime>

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

using namespace M::MThemeDaemonProtocol;

MRemoteThemeDaemonClient::MRemoteThemeDaemonClient(const QString &serverAddress, QObject *parent) :
    MAbstractThemeDaemonClient(parent),
    m_sequenceCounter(0),
    m_priority(100),
    m_socket(),
    m_stream(),
    m_pixmapCache()
{
    m_stream.setVersion(QDataStream::Qt_4_6);

    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(connectionDataAvailable()));

    const QString address = serverAddress.isEmpty() ? M::MThemeDaemonProtocol::ServerAddress : serverAddress;
    if (connectToServer(address, 2000)) {
        m_stream.setDevice(&m_socket);

        const QFileInfo fileInfo(QCoreApplication::instance()->applicationName());
        const QString applicationName = fileInfo.fileName();
        registerApplication(applicationName);
        initializePriority(applicationName);
    } else {
        qWarning() << "MRemoteThemeDaemonClient: Failed to connect to theme daemon (IPC)";
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

    // The pixmap has not been cached yet. Create a pixmap placeholder and request
    // the pixmap from the themedaemon server.
    pixmap = new QPixmap();
    m_pixmapCache.insert(pixmapId, pixmap);

    ++m_sequenceCounter;
    m_stream << Packet(Packet::RequestPixmapPacket,
                       m_sequenceCounter,
                       new RequestedPixmapPacketData(pixmapId, priority()));
    const Packet reply = waitForPacket(m_sequenceCounter);
    processOnePacket(reply);

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
        if (!m_pixmapCache.contains(handle.identifier)) {
            QPixmap *pixmap = new QPixmap();
            *pixmap = createPixmapFromHandle(handle.pixmapHandle);
            m_pixmapCache.insert(handle.identifier, pixmap);
        }
    }
}

void MRemoteThemeDaemonClient::removeMostUsedPixmaps(const QList<PixmapIdentifier> &identifiers)
{
    foreach (const PixmapIdentifier &identifier, identifiers) {
        if (m_pixmapCache.contains(identifier)) {
            QPixmap *pixmap = m_pixmapCache.value(identifier);
            delete pixmap;
            m_pixmapCache.remove(identifier);
        }
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
    waitForPacket(m_sequenceCounter);
    // TODO: The themedaemon replies a Packet::ThemeChangedPacket, which provides the
    // theme inheritance chain and the theme libraries names. Currently we ignore this.
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

QPixmap MRemoteThemeDaemonClient::createPixmapFromHandle(const PixmapHandle& pixmapHandle)
{
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    const bool isMeeGoRunning =  QMeeGoGraphicsSystemHelper::isRunningRuntime() &&
                                 (QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("meego") ||
                                  QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("raster"));
    if (isMeeGoRunning && pixmapHandle.eglHandle) {
        const int fd = shm_open(qPrintable(pixmapHandle.shmHandle), O_RDONLY, 0444);
        if (fd == -1) {
            qFatal("Failed to open shared memory: %s, %s", strerror(errno), qPrintable(pixmapHandle.shmHandle));
        }

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
