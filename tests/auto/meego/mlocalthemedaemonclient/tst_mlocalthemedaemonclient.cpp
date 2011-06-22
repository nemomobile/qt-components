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
#include <themedaemon/mlocalthemedaemonclient.h>

#include <QDir>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTemporaryFile>

namespace {
    const char *IconDir = "icons";
}

class tst_MLocalThemeDaemonClient : public QObject
{
    Q_OBJECT

public:
    tst_MLocalThemeDaemonClient();
    virtual ~tst_MLocalThemeDaemonClient();

private slots:
    void initTestCase();
    void cleanupTestCase();
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

private:
    void saveImage(QTemporaryFile *file);
    QString imageId(int index) const;

    MLocalThemeDaemonClient *m_themeDaemonClient;

    QList<QTemporaryFile*> m_images;
};

tst_MLocalThemeDaemonClient::tst_MLocalThemeDaemonClient()
{
}

tst_MLocalThemeDaemonClient::~tst_MLocalThemeDaemonClient()
{
}

void tst_MLocalThemeDaemonClient::initTestCase()
{
    // Create a temporary directory "icons" that contains 5
    // PNG icons for testing
    QDir tempDir(QDir::tempPath());
    if (!tempDir.exists(IconDir)) {
        const bool ok = tempDir.mkdir(IconDir);
        QVERIFY(ok);
    }

    for (int i = 0; i < 5; ++i) {
        // QTemporaryFile replaces the placeholder "XXXXXX" by
        // a unique ID
        const QString name = QDir::tempPath() + QDir::separator()
                             + IconDir + QDir::separator()
                             + "image_" + QString::number(i) + "_XXXXXX.png";
        QTemporaryFile *file = new QTemporaryFile(name);
        QVERIFY(file);

        saveImage(file);

        m_images.append(file);
    }
}

void tst_MLocalThemeDaemonClient::cleanupTestCase()
{
    qDeleteAll(m_images);

    QDir tempDir(QDir::tempPath());
    tempDir.remove(IconDir);
}

void tst_MLocalThemeDaemonClient::init()
{
    m_themeDaemonClient = new MLocalThemeDaemonClient(QDir::tempPath());
}

void tst_MLocalThemeDaemonClient::cleanup()
{
    delete m_themeDaemonClient;
}

void tst_MLocalThemeDaemonClient::requestPixmap()
{
    QPixmap pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize());
    QVERIFY(!pixmap.isNull());
}

void tst_MLocalThemeDaemonClient::requestUnknownPixmap()
{
    QPixmap pixmap = m_themeDaemonClient->requestPixmap("unknown_pixmap", QSize());
    QVERIFY(pixmap.isNull());
}

void tst_MLocalThemeDaemonClient::requestPixmapTwice()
{
    QPixmap pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize());
    QVERIFY(!pixmap.isNull());

    pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize());
    QVERIFY(!pixmap.isNull());
}

void tst_MLocalThemeDaemonClient::requestPixmapWithDifferentSizes()
{
    QPixmap pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize());
    QVERIFY(!pixmap.isNull());

    pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize(100, 100));
    QVERIFY(!pixmap.isNull());
    QCOMPARE(pixmap.size(), QSize(100, 100));
}

void tst_MLocalThemeDaemonClient::requestPixmapIncreasesCache()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    for (int i = 0; i < m_images.count(); ++i) {
        QPixmap pixmap = m_themeDaemonClient->requestPixmap(imageId(i), QSize());
        QVERIFY(!pixmap.isNull());

        QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), i + 1);
    }
}

void tst_MLocalThemeDaemonClient::requestPixmapTwiceIncreasesCacheByOne()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    QPixmap pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize());
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 1);

    pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize());
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 1);
}

void tst_MLocalThemeDaemonClient::requestUnknownPixmapDoesNotIncreaseCache()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    QPixmap pixmap = m_themeDaemonClient->requestPixmap("unknown_pixmap", QSize());
    QVERIFY(pixmap.isNull());

    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());
}

void tst_MLocalThemeDaemonClient::requestDifferentSizesIncreasesCache()
{
    QVERIFY(m_themeDaemonClient->m_pixmapCache.isEmpty());

    QPixmap pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize());
    QVERIFY(!pixmap.isNull());

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 1);

    pixmap = m_themeDaemonClient->requestPixmap(imageId(0), QSize(100, 100));
    QVERIFY(!pixmap.isNull());
    QCOMPARE(pixmap.size(), QSize(100, 100));

    QCOMPARE(m_themeDaemonClient->m_pixmapCache.count(), 2);
}

void tst_MLocalThemeDaemonClient::saveImage(QTemporaryFile *file)
{
    bool ok = file->open();
    QVERIFY(ok);

    QImage image(48, 48, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(0, 0, image.width(), image.height(), Qt::green);

    ok = image.save(file, "png");
    QVERIFY(ok);
}

QString tst_MLocalThemeDaemonClient::imageId(int index) const
{
    QFileInfo fileInfo(*m_images[index]);
    return fileInfo.baseName();
}

QTEST_MAIN(tst_MLocalThemeDaemonClient)
#include "tst_mlocalthemedaemonclient.moc"
 
