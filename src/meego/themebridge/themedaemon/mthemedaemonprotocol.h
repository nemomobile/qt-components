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

                // Packet which must be sent by the client to the daemon before asking
                // anything else. The type of the data is StringPacketData
                // and should contain the name of the client.
                RequestRegistrationPacket           = 1,

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
            QString shmHandle;
            QSize size;
            QImage::Format format;
            int numBytes;
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

QString readQString(QDataStream &stream);
QStringList readQStringList(QDataStream &stream);

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::Packet &packet);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::Packet &packet);

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapHandlePacketData &handle);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapHandlePacketData &handle);

QDataStream &operator<<(QDataStream &stream, const M::MThemeDaemonProtocol::PixmapIdentifier &id);
QDataStream &operator>>(QDataStream &stream, M::MThemeDaemonProtocol::PixmapIdentifier &id);

#endif
 
