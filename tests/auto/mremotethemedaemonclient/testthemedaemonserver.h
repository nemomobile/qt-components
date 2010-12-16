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

    bool m_clientRegistered;
    QString m_serverAddress;
    QLocalServer* m_server;
    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, const QPixmap*> m_pixmapCache;
};

#endif

