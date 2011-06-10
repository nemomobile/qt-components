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

#include "siconpool.h"
#include <QtTest/QtTest>
#include <QUrl>
#include <QImage>
#include <QPainter>

class tst_SIconPool : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testIconPool_get();
    void testIconPool_release();
    void testIconPool_defaultSize();
};

void tst_SIconPool::init()
{
}

void tst_SIconPool::cleanup()
{
}

void tst_SIconPool::initTestCase()
{
}

void tst_SIconPool::cleanupTestCase()
{
}

void tst_SIconPool::testIconPool_get()
{
    QCOMPARE(SIconPool::totalCount(), 0);
    QPixmap pm = SIconPool::get(":/p1.png", QSize(30, 10), Qt::KeepAspectRatio);

    // Verify that images from same pixmap are compared to be same
    QImage i1 = pm.toImage();
    QImage i2 = pm.toImage();
    QCOMPARE(i1, i2);

    QCOMPARE(SIconPool::totalCount(), 1);
    QCOMPARE(SIconPool::count(":/p1.png", QSize(30, 10), Qt::KeepAspectRatio),  1);

    QVERIFY(!pm.isNull());
    // Change aspect ratio
    QPixmap pm2 = SIconPool::get(":/p1.png", QSize(30, 10), Qt::IgnoreAspectRatio);

    QCOMPARE(SIconPool::totalCount(), 2);
    QCOMPARE(SIconPool::count(":/p1.png", QSize(30, 10), Qt::IgnoreAspectRatio), 1);

    QVERIFY(!pm2.isNull());
    i2 = pm2.toImage();
    QVERIFY(i1 != i2);

    // Change size
    pm2 = SIconPool::get(":/p1.png", QSize(40, 20), Qt::KeepAspectRatio);

    QCOMPARE(SIconPool::totalCount(), 3);
    QCOMPARE(SIconPool::count(":/p1.png", QSize(40, 20), Qt::KeepAspectRatio), 1);

    QVERIFY(!pm2.isNull());
    i2 = pm2.toImage();
    QVERIFY(i1 != i2);

    // Change icon name
    pm2 = SIconPool::get(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio);

    QCOMPARE(SIconPool::totalCount(), 4);
    QCOMPARE(SIconPool::count(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio), 1);

    QVERIFY(!pm2.isNull());
    i2 = pm2.toImage();
    QVERIFY(i1 != i2);

    // Ask same image again
    pm2 = SIconPool::get(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio);
    QCOMPARE(SIconPool::totalCount(), 4);
    QCOMPARE(SIconPool::count(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio), 2);

    // Try with empty filename, should not add item in pool
    pm2 = SIconPool::get(QString(), QSize(30, 10), Qt::KeepAspectRatio);
    QCOMPARE(SIconPool::totalCount(), 4);
    QCOMPARE(SIconPool::count(QString(), QSize(30, 10), Qt::KeepAspectRatio), 0);

    // Try with empty size, should not add item in pool
    pm2 = SIconPool::get(QString(), QSize(), Qt::KeepAspectRatio);
    pm2 = SIconPool::get(QString(), QSize(0, 0), Qt::KeepAspectRatio);
    QCOMPARE(SIconPool::totalCount(), 4);
    QCOMPARE(SIconPool::count(QString(), QSize(), Qt::KeepAspectRatio), 0);
    QCOMPARE(SIconPool::count(QString(), QSize(0, 0), Qt::KeepAspectRatio), 0);
}

void tst_SIconPool::testIconPool_release()
{
    // Inserted from previous test case
    QCOMPARE(SIconPool::count(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio), 2);
    SIconPool::release(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio);
    QCOMPARE(SIconPool::count(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio), 1);
    SIconPool::release(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio);
    QCOMPARE(SIconPool::count(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio), 0);
    // Try release once more after ref count has become zero
    SIconPool::release(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio);
    QCOMPARE(SIconPool::count(":/p2.png", QSize(30, 10), Qt::KeepAspectRatio), 0);

    int count = SIconPool::totalCount();
    // Try release something which does not exist
    SIconPool::release("qwertyui", QSize(30, 10), Qt::KeepAspectRatio);
    // Should not affect count
    QCOMPARE(count,  SIconPool::totalCount());

    // Get one more
    SIconPool::get(":/s1.svg", QSize(30, 10), Qt::KeepAspectRatio);
    count = SIconPool::totalCount();
    // Release with wrong aspect ratio, should not do anything
    SIconPool::release(":/s1.svg", QSize(30, 10), Qt::IgnoreAspectRatio);
    QCOMPARE(count, SIconPool::totalCount());
    // Release with wrong size, should not do anything
    SIconPool::release(":/s1.svg", QSize(30, 11), Qt::KeepAspectRatio);
    QCOMPARE(count, SIconPool::totalCount());
    // Release with correct settings
    SIconPool::release(":/s1.svg", QSize(30, 10), Qt::KeepAspectRatio);
    QCOMPARE(count, SIconPool::totalCount() + 1);
}

void tst_SIconPool::testIconPool_defaultSize()
{
    QSize s1 = SIconPool::defaultSize(":/s1.svg");
    QSize s2 = SIconPool::defaultSize(":/s2.svg");
    QSize p1 = SIconPool::defaultSize(":/p1.png");
    QSize p2 = SIconPool::defaultSize(":/p2.png");

    QVERIFY(!s1.isEmpty());
    QVERIFY(!s2.isEmpty());
    QVERIFY(!p1.isEmpty());
    QVERIFY(!p2.isEmpty());

    QVERIFY(s1 != s2);
    QVERIFY(p1 != s1);
    QCOMPARE(p1, p2); // PNGs have same size

    QSize none = SIconPool::defaultSize("siusdfuh");
    QVERIFY(none.isEmpty());
}

QTEST_MAIN(tst_SIconPool)

#include "tst_iconpool.moc"
