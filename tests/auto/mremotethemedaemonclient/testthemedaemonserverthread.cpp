/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "testthemedaemonserverthread.h"

#include "testthemedaemonserver.h"

#include <QDataStream>
#include <QLocalSocket>
#include <QPixmap>

TestThemeDaemonServerThread::TestThemeDaemonServerThread(const QString &serverAddress,
                                                         QObject *parent) :
    QThread(parent),
    m_serverAddress(serverAddress),
    m_pixmapCache()
{
    // Creating of QPixmaps may only get done in the context of the GUI thread. The
    // pixmaps are forwarded in TestThemeDaemonServerThread::run() to the
    // themedaemon server.
    createPixmap("test_pixmap_1");
    createPixmap("test_pixmap_2");
    createPixmap("test_pixmap_2", QSize(100, 100));
    createPixmap("test_pixmap_3");
    createPixmap("most_used_pixmap_1");
    createPixmap("most_used_pixmap_2");
    createPixmap("most_used_pixmap_3");
}

TestThemeDaemonServerThread::~TestThemeDaemonServerThread()
{
    qDeleteAll(m_pixmapCache);
}

int TestThemeDaemonServerThread::cachedMostUsedPixmapsCount() const
{
    // The test-themedaemon-server will send all cached pixmaps as "most used pixmaps"
    // to the client except of the first entry.
    return m_pixmapCache.count() - 1;
}

void TestThemeDaemonServerThread::run()
{
    TestThemeDaemonServer server(m_serverAddress, m_pixmapCache);
    exec();
}

void TestThemeDaemonServerThread::createPixmap(const QString &imageId, const QSize &size)
{
    // Invalid sizes are stored as empty sizes in the cache. The themedaemon server returns
    // the original size of a pixmap in this case.
    const QSize storedSize = size.isValid() ? size : QSize(0, 0);
    M::MThemeDaemonProtocol::PixmapIdentifier id(imageId, storedSize);

    QPixmap *pixmap = storedSize.isEmpty() ? new QPixmap(48, 48) : new QPixmap(size);
    m_pixmapCache.insert(id, pixmap);
}
