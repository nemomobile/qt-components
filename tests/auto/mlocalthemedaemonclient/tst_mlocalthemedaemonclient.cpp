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
 
