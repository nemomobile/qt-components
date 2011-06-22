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

#ifndef MTHEMEDAEMONPROTOCOL_H
#define MTHEMEDAEMONPROTOCOL_H

#include <QDataStream>
#include <QImage>
#include <QSharedPointer>
#include <QSize>
#include <QStringList>
#include <QList>

namespace M
{
    namespace MThemeDaemonProtocol
    {
#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
        const qint32 protocolVersion = 1;
#endif

        extern const QString ServerAddress;

        struct PacketData {
            virtual ~PacketData() = 0;
        };

        class Packet
        {
        public:
            enum PacketType {
                // Initial packet type
                Unknown                             = 0,
#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
                // Packet which must be sent by the client to the daemon after
                // sending the protocol version and before doing any other
                // communication.
                RequestRegistrationPacket           = 1,

                // Package sent to the themedaemon before doing any other
                // communication. It contains a simple integer representing
                // the ideally used protocol version.
                // Themedaemon answers with the protocol version it will
                // use if any further communication happens. It may be a different
                // protocol version. If the client wants to continue it must use
                // the protocol version sent by the themedaemon from now on.
                ProtocolVersionPacket               = 2,
#else
                // Packet which must be sent by the client to the daemon before asking
                // anything else. The type of the data is StringPacketData
                // and should contain the name of the client.
                RequestRegistrationPacket           = 1,
#endif

                // The client tells the daemon that it will use a specific
                // package. It does not expect an answer as it already
                // knows the handle.
                PixmapUsedPacket                     = 7,

                // The client requests a pixmap with the data type of the packet
                // PixmapIdentifier. The Client can request one pixmap only once. If the same pixmap
                // is requested twice the daemon will reply with ErrorPacket.
                RequestPixmapPacket                 = 8,

                // The client can requests a pixmap release
                // for a pixmap that the client doesn't need anymore.
                // The data type of the packet is PixmapIdentifier.
                // If client tries to release a pixmap that is not loaded
                // by the daemon, the daemon will reply with ErrorPacket.
                ReleasePixmapPacket                 = 9,

                // The daemon will send this packet when a requested pixmap
                // has been loaded, or when the theme has changed
                // and the pixmap data must be replaced.
                // The data type of this packet is PixmapHandlePacket.
                PixmapUpdatedPacket                 = 10,

                // The client requests the daemon to add a new directory from which
                // to search pixmaps for the client. The data type of this packet is
                // StringPacketData, which contains an absolute path to the directory.
                RequestNewPixmapDirectoryPacket     = 16,

                // If the client wishes to remove all manually added search directories
                // from the daemon, it can be done with this packet.
                // The data type of the packet is undetermined, so 0 should be passed.
                RequestClearPixmapDirectoriesPacket = 17,

                // The daemon will send this packet when the theme has changed.
                // The data type of the packet is StringListPacketData, and contains
                // the new inheritance chain of the theme
                // e.g:
                // /usr/share/themes/devel/
                // /usr/share/themes/base/
                ThemeChangedPacket                  = 33,

                // The client will send this packet as a reply to ThemeChangedPacket
                // when the client has applied the theme change.
                ThemeChangeAppliedPacket            = 34,

                // The daemon will send this packet to all clients when the theme change
                // has completed.
                ThemeChangeCompletedPacket          = 35,

                // The daemon sends the handles of the most used pixmaps to the
                // application.
                MostUsedPixmapsPacket               = 36,

                // The client acknowledges the MostUsedPixmapsPacket from the server.
                AckMostUsedPixmapsPacket            = 37,

                // The client requests the state of the daemon. The data type
                // is undetermined, so 0 should be passed. The daemon will reply with
                // ThemeDaemonStatusPacket and the data type
                // is ClientList.
                QueryThemeDaemonStatusPacket        = 129,
                ThemeDaemonStatusPacket             = 130,

                // The daemon will send this packet when client has
                // made something illegal
                ErrorPacket                         = 255
            };

            Packet() : m_seq(0), m_data(), m_type(Unknown) {}
            Packet(PacketType type, quint64 seq, PacketData *data = 0);
            ~Packet();

            PacketType type() const { return m_type; }
            void setType(PacketType type) { m_type = type; }

            quint64 sequenceNumber() const { return m_seq; }
            void setSequenceNumber(quint64 seq) { m_seq = seq; }

            const PacketData *data() const { return m_data.data(); }
            void setData(PacketData *data);

        private:
            quint64 m_seq;
            QSharedPointer<PacketData> m_data;
            PacketType m_type;
        };

        struct PixmapHandle {
            PixmapHandle();

            bool isValid() const;

            Qt::HANDLE xHandle;
            Qt::HANDLE eglHandle;
#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
            QByteArray shmHandle;
#else
            QString shmHandle;
#endif
            QSize size;
            QImage::Format format;
            int numBytes;

            bool directMap;
        };

        struct PixmapIdentifier : PacketData {
            PixmapIdentifier() {}
            PixmapIdentifier(const QString &imageId, const QSize &size) :
                imageId(imageId), size(size) {}
            virtual ~PixmapIdentifier();

            QString imageId;
            QSize size;
            bool operator==(const PixmapIdentifier &other) const
                { return (imageId == other.imageId && size == other.size); }
            bool operator!=(const PixmapIdentifier &other) const
                { return (imageId != other.imageId || size != other.size); }
        };
        uint qHash(const PixmapIdentifier &id);

        struct NumberPacketData : PacketData {
            NumberPacketData(qint32 value) : value(value) {}
            virtual ~NumberPacketData();
            qint32 value;
        };

        struct StringPacketData : PacketData {
            StringPacketData(const QString &string) : string(string) {}
            virtual ~StringPacketData();
            QString string;
        };

        struct StringBoolPacketData : PacketData {
            StringBoolPacketData(const QString &string, bool b) :
                string(string),
                b(b)
            {}
            virtual ~StringBoolPacketData();

            QString string;
            bool b;
        };

        struct PixmapHandlePacketData : PacketData {
            PixmapHandlePacketData() : pixmapHandle()
            {}

            PixmapHandlePacketData(const PixmapIdentifier &identifier, PixmapHandle pixmapHandle) :
                identifier(identifier),
                pixmapHandle(pixmapHandle)
            {}

            PixmapHandlePacketData(const PixmapHandlePacketData &handle) :
                PacketData(),
                identifier(handle.identifier),
                pixmapHandle(handle.pixmapHandle)
            {}

            virtual ~PixmapHandlePacketData();

            PixmapIdentifier identifier;
            PixmapHandle pixmapHandle;
        };

        struct ClientInfo {
            QString name;
            QList<PixmapIdentifier> pixmaps;
            QList<PixmapIdentifier> requestedPixmaps;
            QList<PixmapIdentifier> releasedPixmaps;
        };

        struct ClientList : PacketData {
            ClientList(const QList<ClientInfo>& clients) :
                clients(clients)
            {}
            virtual ~ClientList();

            QList<ClientInfo> clients;
        };

        struct ThemeChangeInfoPacketData : PacketData {
            ThemeChangeInfoPacketData(const QStringList &themeInheritance, const QStringList &themeLibraryNames) :
                themeInheritance(themeInheritance), 
                themeLibraryNames(themeLibraryNames) 
            {}
            virtual ~ThemeChangeInfoPacketData();

            QStringList themeInheritance;
            QStringList themeLibraryNames;
        };

        struct MostUsedPixmapsPacketData : PacketData {
            MostUsedPixmapsPacketData() {}
            MostUsedPixmapsPacketData(const QList<PixmapHandlePacketData>& addedHandles, const QList<PixmapIdentifier>& removedIdentifiers) :
                addedHandles(addedHandles),
                removedIdentifiers(removedIdentifiers)
            {}

            virtual ~MostUsedPixmapsPacketData();

            QList<PixmapHandlePacketData> addedHandles;
            QList<PixmapIdentifier> removedIdentifiers;
        };

        struct RequestedPixmapPacketData : PacketData {
            RequestedPixmapPacketData(const PixmapIdentifier& id, qint32 priority) :
                id(id),
                priority(priority)
            {}

            virtual ~RequestedPixmapPacketData();

            PixmapIdentifier id;
            qint32 priority;
        };
    }
}

#ifdef HAVE_THEMEDAEMON_PROTOCOL_V1
    QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::Packet &packet);
    void writePacketData(QDataStream &stream, const M::MThemeDaemonProtocol::Packet &packet);
    QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::Packet &packet);
    void readPacketData(QDataStream &stream, M::MThemeDaemonProtocol::Packet &packet);
#else
    QString readQString(QDataStream &stream);
    QStringList readQStringList(QDataStream &stream);

    QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::Packet &packet);
    QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::Packet &packet);
#endif

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapHandlePacketData &handle);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapHandlePacketData &handle);

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapIdentifier &id);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapIdentifier &id);

#endif
 
