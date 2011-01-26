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

#include "sdeclarativeframe.h"
#include <QtTest/QtTest>
#include <QImage>
#include <QPainter>

bool fuzzyCompareImages(const QImage &image1, const QImage &image2, int tolerance)
{
    if (image1.bytesPerLine() != image2.bytesPerLine()
        || image1.width() != image2.width()
        || image1.height() != image2.height()) {
        return false;
    }

    // E.g. (,10) and (10,0) sizes are treated similar.
    if ((!image1.width() || !image1.height())
        && (!image2.width() || !image2.height())) {
        return true;
    }

    bool same = true;

    for (int i = 0; i < image1.height(); i++) {
        const uchar *line1 = image1.scanLine(i);
        const uchar *line2 = image2.scanLine(i);

        int bytes = image1.bytesPerLine();
        for (int j = 0; j < bytes ; j++) {
            int delta = line1[j] - line2[j];
            if (qAbs(delta) > tolerance) {
                same = false;
                break;
            }
        }
        if (!same) {
            break;
        }
    }

    return same;
}


void doPaint(QPainter *painter, SDeclarativeFrame *frame)
{
    painter->eraseRect(painter->viewport());
    frame->paint(painter, 0, 0);
}

class TestDeclarativeFrame : public SDeclarativeFrame
{
    Q_OBJECT
public:
    TestDeclarativeFrame() : SDeclarativeFrame(),
                             frameNameChanged(false)
    {
        connect(this, SIGNAL(frameNameChanged(QString)), this, SLOT(handleFrameNameChanged()));
    }
    void setSize(int width, int height)
    {
        SDeclarativeFrame::setImplicitWidth(width);
        SDeclarativeFrame::setImplicitHeight(height);
    }

    bool frameNameChanged;
private slots:
    void handleFrameNameChanged()
    {
        frameNameChanged = true;
    }
};

class tst_SDeclarativeFrame : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testFrame_constructor();
    void testFrame_frameName();
    void testFrame_setFrameName();
    void testFrame_frameType();
    void testFrame_setFrameType();
    void testFrame_paint();
    void testFrame_signals();
};

void tst_SDeclarativeFrame::init()
{
}

void tst_SDeclarativeFrame::cleanup()
{
}

void tst_SDeclarativeFrame::initTestCase()
{
}

void tst_SDeclarativeFrame::cleanupTestCase()
{
}


void tst_SDeclarativeFrame::testFrame_constructor()
{
    SDeclarativeFrame frame;
}

void tst_SDeclarativeFrame::testFrame_frameName()
{
    SDeclarativeFrame frame;
    QVERIFY(frame.frameName().isEmpty());
}

void tst_SDeclarativeFrame::testFrame_setFrameName()
{
    SDeclarativeFrame frame;

    frame.setFrameName("qtg_fr_btn_normal");
    QCOMPARE(frame.frameName(), QString("qtg_fr_btn_normal"));

    frame.setFrameName("qtg_fr_btn_highlight");
    QCOMPARE(frame.frameName(), QString("qtg_fr_btn_highlight"));
}

void tst_SDeclarativeFrame::testFrame_frameType()
{
    SDeclarativeFrame frame;
    QCOMPARE(frame.frameType(), SDeclarativeFrame::Undefined);
    frame.setFrameType(SDeclarativeFrame::NinePieces);
    QCOMPARE(frame.frameType(), SDeclarativeFrame::NinePieces);
}

void tst_SDeclarativeFrame::testFrame_setFrameType()
{
    SDeclarativeFrame frame;

    frame.setFrameType(SDeclarativeFrame::OnePiece);
    QCOMPARE(frame.frameType(), SDeclarativeFrame::OnePiece);
    frame.setFrameType(SDeclarativeFrame::ThreePiecesHorizontal);
    QCOMPARE(frame.frameType(), SDeclarativeFrame::ThreePiecesHorizontal);
    frame.setFrameType(SDeclarativeFrame::ThreePiecesVertical);
    QCOMPARE(frame.frameType(), SDeclarativeFrame::ThreePiecesVertical);
    frame.setFrameType(SDeclarativeFrame::NinePieces);
    QCOMPARE(frame.frameType(), SDeclarativeFrame::NinePieces);
}

void tst_SDeclarativeFrame::testFrame_paint()
{
    QImage image1(100, 100, QImage::Format_ARGB32);
    QPainter painter1(&image1);
    QImage image2(100, 100, QImage::Format_ARGB32);
    QPainter painter2(&image2);

    TestDeclarativeFrame frame;
    frame.setFrameName("qtg_fr_btn_normal");
    frame.setSize(100, 100);
    frame.setFrameType(SDeclarativeFrame::NinePieces);

    // Paint to both painters to verify compareImages function works
    doPaint(&painter1, &frame);
    doPaint(&painter2, &frame);

    // Same frames, should be same result
    QVERIFY(fuzzyCompareImages(image1, image2,0));

    // Keep same size but change frame name
    frame.setFrameName("qtg_fr_btn_highlight");

    doPaint(&painter2, &frame);
    QVERIFY(!fuzzyCompareImages(image1, image2,0));

    // Change size smaller
    frame.setFrameName("qtg_fr_btn_normal");
    frame.setSize(95, 95);

    doPaint(&painter2, &frame);
    // Same icons but different size, should be different result
    QVERIFY(!fuzzyCompareImages(image1, image2, 0));

    // Back to original
    frame.setSize(100, 100);
    doPaint(&painter2, &frame);
    QVERIFY(fuzzyCompareImages(image1, image2, 0));

    // Change frame type
    frame.setFrameType(SDeclarativeFrame::ThreePiecesHorizontal);
    doPaint(&painter2, &frame);
    QVERIFY(!fuzzyCompareImages(image1, image2, 0));

    // Change frame type
    frame.setFrameType(SDeclarativeFrame::ThreePiecesVertical);
    doPaint(&painter1, &frame);
    QVERIFY(!fuzzyCompareImages(image1, image2, 0));

    // Test that undefined frame type does not draw anything
    frame.setFrameType(SDeclarativeFrame::Undefined);
    frame.setFrameName("qtg_fr_btn_normal");
    doPaint(&painter1, &frame);
    frame.setFrameName("qtg_fr_btn_highlight");
    doPaint(&painter2, &frame);
    // Type is undefined so results should be same even though frame names were different
    QVERIFY(fuzzyCompareImages(image1, image2, 0));

    // Test that empty size frame does not draw anything
    frame.setSize(0, 0);
    frame.setFrameType(SDeclarativeFrame::NinePieces);
    frame.setFrameName("qtg_fr_btn_normal");
    doPaint(&painter1, &frame);
    frame.setFrameName("qtg_fr_btn_highlight");
    doPaint(&painter2, &frame);
    // Size is empty so results should be same even though frame names were different
    QVERIFY(fuzzyCompareImages(image1, image2, 0));
}

void tst_SDeclarativeFrame::testFrame_signals()
{
    TestDeclarativeFrame frame;
    frame.frameNameChanged = false;

    frame.setFrameName("qtg_fr_btn_highlight"); // Frame graphics from theme
    // Should have triggered signal
    QVERIFY(frame.frameNameChanged);

    // Try setting properties to same values, that should not trigger signals
    frame.frameNameChanged = false;

    frame.setFrameName("qtg_fr_btn_highlight");
    QVERIFY(!frame.frameNameChanged);

    // Try changing values to something different, that should trigger signals again
    frame.frameNameChanged = false;

    frame.setFrameName("qtg_fr_btn_normal");
    QVERIFY(frame.frameNameChanged);
}
QTEST_MAIN(tst_SDeclarativeFrame)

#include "tst_declarativeframe.moc"
