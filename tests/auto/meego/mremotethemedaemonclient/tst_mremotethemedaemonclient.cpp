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
    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(),
             m_themeDaemonServerThread->cachedMostUsedPixmapsCount());
}

QTEST_MAIN(tst_MRemoteThemeDaemonClient)
#include "tst_mremotethemedaemonclient.moc"
 
