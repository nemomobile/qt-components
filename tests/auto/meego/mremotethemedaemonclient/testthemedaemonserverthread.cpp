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
