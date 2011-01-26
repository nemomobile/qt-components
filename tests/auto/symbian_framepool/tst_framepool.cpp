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

#include "sframepool.h"
#include <QtTest/QtTest>
#include <QUrl>
#include <QImage>
#include <QPainter>

class tst_SFramePool : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testFramePool_get();
    void testFramePool_release();
};

void tst_SFramePool::init()
{
}

void tst_SFramePool::cleanup()
{
}

void tst_SFramePool::initTestCase()
{
}

void tst_SFramePool::cleanupTestCase()
{
}

void tst_SFramePool::testFramePool_get()
{
    QCOMPARE(SFramePool::totalCount(), 0);
    QPixmap pm = SFramePool::get(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::NinePieces, QSize(30, 10));

    // Verify that images from same pixmap are compared to be same
    QImage i1 = pm.toImage();
    QImage i2 = pm.toImage();
    QCOMPARE(i1,  i2);

    QCOMPARE(SFramePool::totalCount(), 1);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::NinePieces, QSize(30, 10)), 1);

    QVERIFY(!pm.isNull());
    // Change frame type
    QPixmap pm2 = SFramePool::get(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::ThreePiecesHorizontal, QSize(30, 10));

    QCOMPARE(SFramePool::totalCount(), 2);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::ThreePiecesHorizontal, QSize(30, 10)), 1);

    QVERIFY(!pm2.isNull());
    i2 = pm2.toImage();
    QVERIFY(i1 != i2);

    // Yet another frame type
    QPixmap pm3 = SFramePool::get(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::ThreePiecesVertical, QSize(30, 10));

    QCOMPARE(SFramePool::totalCount(), 3);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::ThreePiecesVertical, QSize(30, 10)), 1);

    QVERIFY(!pm3.isNull());
    QImage i3 = pm3.toImage();
    QVERIFY(i1 != i3);
    QVERIFY(i2 != i3);

    // Change size
    pm2 = SFramePool::get(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::NinePieces, QSize(31, 10));

    QCOMPARE(SFramePool::totalCount(), 4);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_normal.svg", SDeclarativeFrame::NinePieces, QSize(31, 10)), 1);

    QVERIFY(!pm2.isNull());
    i2 = pm2.toImage();
    QVERIFY(i1 != i2);

    // Change frame name
    pm2 = SFramePool::get(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10));

    QCOMPARE(SFramePool::totalCount(), 5);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10)), 1);

    QVERIFY(!pm2.isNull());
    i2 = pm2.toImage();
    QVERIFY(i1 != i2);

    // Ask same frame again
    pm2 = SFramePool::get(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10));
    QCOMPARE(SFramePool::totalCount(), 5);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10)), 2);

    // Try with empty filename, should not add item in pool
    pm2 = SFramePool::get(QString(), SDeclarativeFrame::NinePieces, QSize(30, 10));
    QCOMPARE(SFramePool::totalCount(), 5);
    QCOMPARE(SFramePool::count(QString(), SDeclarativeFrame::NinePieces, QSize(30, 10)), 0);

    // Try with empty size, should not add item in pool
    pm2 = SFramePool::get(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize());
    pm2 = SFramePool::get(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(0, 0));
    QCOMPARE(SFramePool::totalCount(), 5);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize()), 0);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(0, 0)), 0);

    // Try with undefined frame type, should not add item in pool
    pm2 = SFramePool::get(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::Undefined, QSize(30, 10));
    QCOMPARE(SFramePool::totalCount(), 5);
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::Undefined, QSize(30, 10)), 0);
}

void tst_SFramePool::testFramePool_release()
{
    // Inserted from previous test case
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10)), 2);
    SFramePool::release(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10));
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10)) , 1);
    SFramePool::release(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10));
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10)) , 0);
    // Try release once more after ref count has become zero
    SFramePool::release(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10));
    QCOMPARE(SFramePool::count(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(30, 10)) , 0);

    int count = SFramePool::totalCount();
    // Try release something which does not exist
    SFramePool::release(":/qtg_fr_btn_pressedd.svg", SDeclarativeFrame::NinePieces, QSize(30, 10));
    // Should not affect count
    QCOMPARE(count, SFramePool::totalCount());

    // Get one more
    SFramePool::get(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(50, 10));
    count = SFramePool::totalCount();
    // Release with wrong frame type, should not do anything
    SFramePool::release(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::ThreePiecesHorizontal, QSize(50, 10));
    QCOMPARE(count, SFramePool::totalCount());
    // Release with wrong size, should not do anything
    SFramePool::release(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(51, 10));
    QCOMPARE(count, SFramePool::totalCount());
    // Release with correct parameters
    SFramePool::release(":/qtg_fr_btn_pressed.svg", SDeclarativeFrame::NinePieces, QSize(50, 10));
    QCOMPARE(count, SFramePool::totalCount() + 1);
}

QTEST_MAIN(tst_SFramePool)

#include "tst_framepool.moc"
