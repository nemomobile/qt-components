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

#include <QtTest/QtTest>
#include "testthemedaemonserverthread.h"
#include <themedaemon/mremotethemedaemonclient.h>

#include <QPixmap>

class tst_MRemoteThemeDaemonClient : public QObject
{
    Q_OBJECT

public:
    tst_MRemoteThemeDaemonClient();
    virtual ~tst_MRemoteThemeDaemonClient();

private slots:
    void init();
    void cleanup();
    void requestPixmap();
    void requestUnknownPixmap();
    void requestPixmapTwice();
    void requestPixmapWithDifferentSizes();
    void requestPixmapIncreasesCache();
    void requestPixmapTwiceIncreasesCacheByOne();
    void requestUnknownPixmapDoesNotIncreaseCache();
    void requestDifferentSizesIncreasesCache();
    void testMostUsedPixmaps();

private:
    TestThemeDaemonServerThread *m_themeDaemonServerThread;
    MRemoteThemeDaemonClient *m_themeDaemonClient;
};

tst_MRemoteThemeDaemonClient::tst_MRemoteThemeDaemonClient()
{
}

tst_MRemoteThemeDaemonClient::~tst_MRemoteThemeDaemonClient()
{
}

void tst_MRemoteThemeDaemonClient::init()
{
    const QString serverAddress = "tst_MRemoteThemeDaemonClient";
    m_themeDaemonServerThread = new TestThemeDaemonServerThread(serverAddress);
    m_themeDaemonServerThread->start();
    m_themeDaemonClient = new MRemoteThemeDaemonClient(serverAddress);
}

void tst_MRemoteThemeDaemonClient::cleanup()
{
    delete m_themeDaemonClient;

    m_themeDaemonServerThread->exit();
    m_themeDaemonServerThread->wait();
    delete m_themeDaemonServerThread;
}

void tst_MRemoteThemeDaemonClient::requestPixmap()
{
    QPixmap pixmap = m_themeDaemonClient->requestPixmap("test_pixmap_1", QSize());
    QVERIFY(!pixmap.isNull());
}

void tst_MRemoteThemeDaemonClient::requestUnknownPixmap()
{
    QPixmap pixmap = m_themeDaemonClient->requestPixmap("unknown_pixmap", QSize());
    QVERIFY(pixmap.isNull());
}


void tst_MRemoteThemeDaemonClient::requestPixmapTwice()
{
    QPixmap pixmap1 = m_themeDaemonClient->requestPixmap("test_pixmap_2", QSize());
    QVERIFY(!pixmap1.isNull());

    QPixmap pixmap2 = m_themeDaemonClient->requestPixmap("test_pixmap_2", QSize());
    QVERIFY(!pixmap2.isNull());
}

void tst_MRemoteThemeDaemonClient::requestPixmapWithDifferentSizes()
{
    QPixmap pixmap1 = m_themeDaemonClient->requestPixmap("test_pixmap_2", QSize());
    QVERIFY(!pixmap1.isNull());

    QPixmap pixmap2 = m_themeDaemonClient->requestPixmap("test_pixmap_2", QSize(100, 100));
    QVERIFY(!pixmap2.isNull());
    QCOMPARE(pixmap2.size(), QSize(100, 100));

    // A size of 50 x 50 is not available for test_pixmap_2
    QPixmap pixmap3 = m_themeDaemonClient->requestPixmap("test_pixmap_2", QSize(50, 50));
    QVERIFY(pixmap3.isNull());
}

void tst_MRemoteThemeDaemonClient::requestPixmapIncreasesCache()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    QPixmap pixmap = m_themeDaemonClient->requestPixmap("test_pixmap_1", QSize());
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 1);
}

void tst_MRemoteThemeDaemonClient::requestPixmapTwiceIncreasesCacheByOne()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    QPixmap pixmap = m_themeDaemonClient->requestPixmap("test_pixmap_1", QSize());
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 1);

    pixmap = m_themeDaemonClient->requestPixmap("test_pixmap_1", QSize());
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 1);
}

void tst_MRemoteThemeDaemonClient::requestUnknownPixmapDoesNotIncreaseCache()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    QPixmap pixmap = m_themeDaemonClient->requestPixmap("unknown_pixmap", QSize());
    QVERIFY(pixmap.isNull());

    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());
}

void tst_MRemoteThemeDaemonClient::requestDifferentSizesIncreasesCache()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    QPixmap pixmap = m_themeDaemonClient->requestPixmap("test_pixmap_2", QSize());
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 1);

    pixmap = m_themeDaemonClient->requestPixmap("test_pixmap_2", QSize(100, 100));
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 2);
}

void tst_MRemoteThemeDaemonClient::testMostUsedPixmaps()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    // To simplify the testing the test-themedaemon-server has implemented in a way to
    // send a list of most used pixmaps if the pixmap "test_pixmap_3" has been requested.
    // The real theme daemon will send a list of most used pixmaps at an undefined point of time.
    QPixmap pixmap = m_themeDaemonClient->requestPixmap("test_pixmap_3", QSize());
    QVERIFY(!pixmap.isNull());
    QCOMPARE(m_themeDaemonClient->m_mostUsedPixmaps.count(),
             m_themeDaemonServerThread->cachedMostUsedPixmapsCount());
}

int main(int argc, char *argv[])
{
    // Until it is clarified how to handle the meego graphicssystem sharing,
    // the native graphicssystem is used. Adjustments in TestThemeDaemonServer::slotClientDataAvailable()
    // are required as soon as tests with the meego graphicssystem should be done.
    QApplication::setGraphicsSystem("native");

    QApplication app(argc, argv);
    tst_MRemoteThemeDaemonClient tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_mremotethemedaemonclient.moc"
 
