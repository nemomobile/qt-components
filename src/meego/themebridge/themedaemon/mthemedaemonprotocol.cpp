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

#include "mthemedaemonprotocol.h"
#include <QHash>
#include <QtEndian>

using namespace M::MThemeDaemonProtocol;

const QString M::MThemeDaemonProtocol::ServerAddress = "m.mthemedaemon";
static const int SOCKET_DELAY_MS = 15000;

PacketData::~PacketData()
{}

PixmapHandle::PixmapHandle() :
    xHandle(0),
    eglHandle(0),
    shmHandle(QString())
{
}

bool PixmapHandle::isValid() const
{
    return (!size.isEmpty() && eglHandle && !shmHandle.isEmpty()) ^ (unsigned long)xHandle;
}

PixmapIdentifier::~PixmapIdentifier()
{}

NumberPacketData::~NumberPacketData()
{}

StringPacketData::~StringPacketData()
{}

StringBoolPacketData::~StringBoolPacketData()
{}

PixmapHandlePacketData::~PixmapHandlePacketData()
{}

ClientList::~ClientList()
{}

ThemeChangeInfoPacketData::~ThemeChangeInfoPacketData()
{}

MostUsedPixmapsPacketData::~MostUsedPixmapsPacketData()
{}

RequestedPixmapPacketData::~RequestedPixmapPacketData()
{}

Packet::Packet(PacketType type, quint64 seq, PacketData *data) :
    m_seq  (seq),
    m_data (data),
    m_type (type)
{}

Packet::~Packet()
{}

void Packet::setData(PacketData *data)
{
    m_data = QSharedPointer<PacketData>(data);
}

QDataStream &operator<<(QDataStream &stream, const Packet &packet)
{
    Q_ASSERT(packet.type() != Packet::Unknown);

    stream << quint32(packet.type());
    stream << packet.sequenceNumber();

    switch (packet.type()) {

    // NULL as data
    case Packet::RequestClearPixmapDirectoriesPacket:
    case Packet::QueryThemeDaemonStatusPacket:
        break;

    // string as data
    case Packet::ErrorPacket:
    case Packet::RequestRegistrationPacket: {
        stream << static_cast<const StringPacketData *>(packet.data())->string;
    } break;

    // two string lists as data
    case Packet::ThemeChangedPacket: {
        const ThemeChangeInfoPacketData* info = static_cast<const ThemeChangeInfoPacketData*>(packet.data());
        stream << info->themeInheritance << info->themeLibraryNames;
    } break;

    case Packet::ThemeChangeAppliedPacket: {
        stream << static_cast<const NumberPacketData *>(packet.data())->value;
    } break;

    // stringbool as data
    case Packet::RequestNewPixmapDirectoryPacket: {
        const StringBoolPacketData *sb = static_cast<const StringBoolPacketData *>(packet.data());
        stream << sb->string << sb->b;
    } break;

    // pixmap identifier as data
    case Packet::PixmapUsedPacket:
    case Packet::ReleasePixmapPacket: {
        const PixmapIdentifier *id = static_cast<const PixmapIdentifier *>(packet.data());
        stream << *id;
    } break;

    case Packet::RequestPixmapPacket: {
        const RequestedPixmapPacketData *pixmap = static_cast<const RequestedPixmapPacketData *>(packet.data());
        stream << pixmap->priority;
        stream << pixmap->id;
    } break;

    // pixmap handle as data
    case Packet::PixmapUpdatedPacket: {
        const PixmapHandlePacketData *h = static_cast<const PixmapHandlePacketData *>(packet.data());
        stream << *h;
    } break;

    case Packet::MostUsedPixmapsPacket: {
        const MostUsedPixmapsPacketData *mostUsedPixmaps = static_cast<const MostUsedPixmapsPacketData *>(packet.data());

        quint32 addedHandlesCount = mostUsedPixmaps->addedHandles.count();
        stream << addedHandlesCount;
        for (quint32 i = 0; i < addedHandlesCount; ++i) {
            stream << mostUsedPixmaps->addedHandles.at(i);
        }

        quint32 removedIdentifiersCount = mostUsedPixmaps->removedIdentifiers.count();
        stream << removedIdentifiersCount;
        for (quint32 i = 0; i < removedIdentifiersCount; ++i) {
            stream << mostUsedPixmaps->removedIdentifiers.at(i);
        }
    } break;

    // client list as data
    case Packet::ThemeDaemonStatusPacket: {
        const ClientList *cl = static_cast<const ClientList *>(packet.data());
        quint32 clientCount = cl->clients.count();
        stream << clientCount;
        for (uint i = 0; i < clientCount; ++i)
        {
            const ClientInfo &info = cl->clients.at(i);
            stream << info.name;
            quint32 pixmapCount = info.pixmaps.count();
            stream << pixmapCount;
            for (quint32 j = 0; j < pixmapCount; ++j) {
                stream << info.pixmaps.at(j);
            }
            quint32 requestedPixmapCount = info.requestedPixmaps.count();
            stream << requestedPixmapCount;
            for (quint32 j = 0; j < requestedPixmapCount; ++j) {
                stream << info.requestedPixmaps.at(j);
            }
            quint32 releasedPixmapCount = info.releasedPixmaps.count();
            stream << releasedPixmapCount;
            for (quint32 j = 0; j < releasedPixmapCount; ++j) {
                stream << info.releasedPixmaps.at(j);
            }
        }
    } break;

    default:
        // print out warning
        break;
    }

    return stream;
}

static bool waitForAvailableBytes(QDataStream &stream, quint32 count)
{
    while (stream.device()->bytesAvailable() < count) {
        if (!stream.device()->waitForReadyRead(SOCKET_DELAY_MS)) {
            return false;
        }
    }
    return true;
}

QString readQString(QDataStream &stream)
{
    if (!waitForAvailableBytes(stream, sizeof(quint32))) {
        return QString();
    }
    char b[sizeof(quint32)];
    stream.device()->peek(b, sizeof(quint32));
    quint32 stringSize = qFromBigEndian<quint32>(reinterpret_cast<uchar*>(b));
    if (stringSize == 0xFFFFFFFF) {
        stringSize = 0;
    }
    if (!waitForAvailableBytes(stream, stringSize + sizeof(quint32))) {
        return QString();
    }
    QString string;
    stream >> string;
    return string;
}

QStringList readQStringList(QDataStream &stream)
{
    quint32 nrOfStrings;
    if (!waitForAvailableBytes(stream, sizeof(quint32))) {
        return QStringList();
    }
    stream >> nrOfStrings;

    QStringList stringList;
    for (quint32 i = 0; i < nrOfStrings; ++i) {
        stringList.append(readQString(stream));
    }
    return stringList;
}

QDataStream &operator>>(QDataStream &stream, Packet &packet)
{
    Q_ASSERT(!packet.data());

    quint32 type = 0;
    quint64 seq  = 0;
    if (!waitForAvailableBytes(stream, sizeof(quint32) + sizeof(quint64))) {
        return stream;
    }
    stream >> type >> seq;
    packet.setType(Packet::PacketType(type));
    packet.setSequenceNumber(seq);

    switch (packet.type()) {

    // NULL as data
    case Packet::RequestClearPixmapDirectoriesPacket:
    case Packet::QueryThemeDaemonStatusPacket:
        break;

    // string as data
    case Packet::ErrorPacket:
    case Packet::RequestRegistrationPacket: {
        QString string = readQString(stream);
        packet.setData(new StringPacketData(string));
    } break;

    // two string lists as data
    case Packet::ThemeChangedPacket: {
        QStringList themeInheritance, themeLibraryNames;
        themeInheritance = readQStringList(stream);
        themeLibraryNames = readQStringList(stream);
        packet.setData(new ThemeChangeInfoPacketData(themeInheritance, themeLibraryNames));
    } break;

    case Packet::ThemeChangeAppliedPacket: {
        qint32  priority;
        waitForAvailableBytes(stream, sizeof(qint32));
        stream >> priority;
        packet.setData(new NumberPacketData(priority));
    } break;

    // stringbool as data
    case Packet::RequestNewPixmapDirectoryPacket: {
        QString string = readQString(stream);
        bool b = false;
        waitForAvailableBytes(stream, sizeof(bool));
        stream >> b;
        packet.setData(new StringBoolPacketData(string, b));
    } break;

    // pixmap identifier as data
    case Packet::PixmapUsedPacket:
    case Packet::ReleasePixmapPacket: {
        PixmapIdentifier id;
        stream >> id;
        packet.setData(new PixmapIdentifier(id));
    } break;

    case Packet::RequestPixmapPacket: {
        qint32 priority;
        waitForAvailableBytes(stream, sizeof(qint32));
        stream >> priority;
        PixmapIdentifier id;
        stream >> id;
        packet.setData(new RequestedPixmapPacketData(id, priority));
    } break;

    // pixmap handle as data
    case Packet::PixmapUpdatedPacket: {
        PixmapHandlePacketData h;
        stream >> h;
        packet.setData(new PixmapHandlePacketData(h));
    } break;

    case Packet::MostUsedPixmapsPacket: {
        waitForAvailableBytes(stream, sizeof(qint32));
        qint32 addedHandlesCount;
        stream >> addedHandlesCount;
        QList<PixmapHandlePacketData> addedHandles;
        for (int i = 0; i < addedHandlesCount; ++i) {
            PixmapHandlePacketData h;
            stream >> h;
            addedHandles.append(h);
        }

        waitForAvailableBytes(stream, sizeof(qint32));
        qint32 removedIdentifiersCount;
        stream >> removedIdentifiersCount;
        QList<PixmapIdentifier> removedIdentifiers;
        for (int i = 0; i < removedIdentifiersCount; ++i) {
            PixmapIdentifier id;
            stream >> id;
            removedIdentifiers.append(id);
        }

        packet.setData(new MostUsedPixmapsPacketData(addedHandles, removedIdentifiers));
    } break;


    // client list as data
    case Packet::ThemeDaemonStatusPacket: {
        QList<ClientInfo> clients;
        quint32 clientCount = 0;
        stream >> clientCount;
        while (clientCount) {
            ClientInfo info;
            info.name = readQString(stream);
            quint32 pixmapCount = 0;
            waitForAvailableBytes(stream, 2*sizeof(quint32));
            stream >> pixmapCount;
            while (pixmapCount) {
                PixmapIdentifier id;
                stream >> id;
                info.pixmaps.append(id);
                --pixmapCount;
            }
            quint32 requestedPixmapCount = 0;
            stream >> requestedPixmapCount;
            while (requestedPixmapCount) {
                PixmapIdentifier id;
                stream >> id;
                info.requestedPixmaps.append(id);
                --requestedPixmapCount;
            }
            quint32 releasedPixmapCount = 0;
            stream >> releasedPixmapCount;
            while (releasedPixmapCount) {
                PixmapIdentifier id;
                stream >> id;
                info.releasedPixmaps.append(id);
                --releasedPixmapCount;
            }

            clients.append(info);
            --clientCount;
        }
        packet.setData(new ClientList(clients));
    } break;

    default:
        // print out warning
        break;
    }

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapHandlePacketData &handle)
{
    stream << handle.identifier;
    stream << quint64(quintptr(handle.pixmapHandle.xHandle));
    stream << quint64(quintptr(handle.pixmapHandle.eglHandle));
    stream << handle.pixmapHandle.shmHandle;
    stream << handle.pixmapHandle.size;
    stream << (quint64)handle.pixmapHandle.format;
    stream << handle.pixmapHandle.numBytes;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapHandlePacketData &handle)
{
    stream >> handle.identifier;

    waitForAvailableBytes(stream, 2 * sizeof(quint64));
    quint64 h;
    stream >> h;
    handle.pixmapHandle.xHandle = (Qt::HANDLE) quintptr(h);
    stream >> h;
    handle.pixmapHandle.eglHandle = (Qt::HANDLE) quintptr(h);
    handle.pixmapHandle.shmHandle = readQString(stream);
    waitForAvailableBytes(stream, 2*sizeof(qint32));
    stream >> handle.pixmapHandle.size;
    waitForAvailableBytes(stream, sizeof(quint64));
    stream >> h;
    handle.pixmapHandle.format = QImage::Format(h);
    waitForAvailableBytes(stream, sizeof(int));
    stream >> handle.pixmapHandle.numBytes;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapIdentifier &id)
{
    stream << id.imageId;
    stream << id.size;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapIdentifier &id)
{
    QString imageId = readQString(stream);;
    QSize size;
    waitForAvailableBytes(stream, 2*sizeof(qint32));
    stream >> size;
    id.imageId = imageId;
    id.size = size;
    return stream;
}

uint M::MThemeDaemonProtocol::qHash(const PixmapIdentifier &id)
{
    using ::qHash;

    const uint idHash     = qHash(id.imageId);
    const uint widthHash  = qHash(id.size.width());
    const uint heightHash = qHash(id.size.height());

    // Twiddle the bits a little, taking a cue from Qt's own qHash() overloads
    return idHash ^ (widthHash << 8) ^ (widthHash >> 24) ^ (heightHash << 24) ^ (heightHash >> 8);
}
