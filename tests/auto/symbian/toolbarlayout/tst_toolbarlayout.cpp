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

#include "tst_quickcomponentstest.h"
#include <QTest>
#include <QGraphicsObject>
#include <QDeclarativeItem>

class tst_toolbarlayout : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void layouting1();
    void layouting2();
    void layouting3();
    void layouting4();
    void layouting5();
    void layouting6();
    void layouting7();

private:
    QScopedPointer<QObject> componentObject;
};

void tst_toolbarlayout::initTestCase()
{
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_toolbarlayout.qml", &errors));
    QVERIFY2(componentObject, qPrintable(errors));

    QGraphicsObject *toolBarLayout1 = componentObject->findChild<QGraphicsObject*>("toolBarLayout1");
    QVERIFY(toolBarLayout1);
}

void tst_toolbarlayout::layouting1()
{
    QGraphicsObject *toolBarLayout1 = componentObject->findChild<QGraphicsObject*>("toolBarLayout1");
    QGraphicsObject *toolButton1_0 = componentObject->findChild<QGraphicsObject*>("toolButton1_0");
    QGraphicsObject *toolButton1_N = componentObject->findChild<QGraphicsObject*>("toolButton1_N");

    QVERIFY(toolButton1_0);
    QVERIFY(toolButton1_N);

    qreal w = 1000.0;

    toolBarLayout1->setProperty("width", QVariant(w));
    QMetaObject::invokeMethod(toolBarLayout1, "childrenChanged");

    qreal x0 = toolButton1_0->x();
    qreal xN = toolButton1_N->x();

    qreal w0 = toolButton1_0->property("width").toReal();
    qreal wN = toolButton1_N->property("width").toReal();

    // check hardwired positions (depends on the margins)
    QGraphicsObject* rootItem = qobject_cast<QGraphicsObject*>(componentObject.data());
    QVERIFY(rootItem);
    if(rootItem->property("portrait").toBool()) {
        QCOMPARE(x0, 0.0);
        QCOMPARE(xN, 940.0);
    }
    else {
        QCOMPARE(x0, 24.0);
        QCOMPARE(xN, 926.0);
    }


    // check that the right and left item occupy the same space
    QCOMPARE(x0 + w0, w - xN);
    QCOMPARE(x0, w - (xN + wN));
}

void tst_toolbarlayout::layouting2()
{
    QGraphicsObject *toolBarLayout2 = componentObject->findChild<QGraphicsObject*>("toolBarLayout2");
    QGraphicsObject *toolButton2_0 = componentObject->findChild<QGraphicsObject*>("toolButton2_0");
    QGraphicsObject *toolButton2_a = componentObject->findChild<QGraphicsObject*>("toolButton2_a");
    QGraphicsObject *toolButton2_b = componentObject->findChild<QGraphicsObject*>("toolButton2_b");
    QGraphicsObject *toolButton2_c = componentObject->findChild<QGraphicsObject*>("toolButton2_c");
    QGraphicsObject *toolButton2_N = componentObject->findChild<QGraphicsObject*>("toolButton2_N");

    QVERIFY(toolButton2_0);
    QVERIFY(toolButton2_a);
    QVERIFY(toolButton2_b);
    QVERIFY(toolButton2_c);
    QVERIFY(toolButton2_N);

    qreal w = 1000.0;

    toolBarLayout2->setProperty("width", QVariant(w));
    QMetaObject::invokeMethod(toolBarLayout2, "childrenChanged");

    qreal x0 = toolButton2_0->x();
    qreal x1 = toolButton2_a->x();
    qreal x2 = toolButton2_b->x();
    qreal x3 = toolButton2_c->x();
    qreal xN = toolButton2_N->x();

    qreal w0 = toolButton2_0->property("width").toReal();
    qreal w1 = toolButton2_a->property("width").toReal();
    qreal w2 = toolButton2_b->property("width").toReal();
    qreal w3 = toolButton2_c->property("width").toReal();
    qreal wN = toolButton2_N->property("width").toReal();

    // all icon toolButtons should be the same width
    QCOMPARE(w0, w1);
    QCOMPARE(w0, w2);
    QCOMPARE(w0, w3);
    QCOMPARE(w0, wN);

    // check that the center item is centered
    qreal offset = qFloor(w / 2.0) - qFloor(w2 / 2.0);
    QCOMPARE(x2, offset);

    // check that the other items are equidistant from the center item
    // this should be true even if w is odd
    qreal gap21 = x2 - (x1 + w1);
    qreal gap32 = x3 - (x2 + w2);
    QCOMPARE(gap21, gap32);

    // check that the other items are equidistant from the edge items.
    // if w is odd, there will be an extra pixel gap on the right
    qreal gap10 = x1 - (x0 + w1);
    qreal gapN3 = xN - (x3 + w3);
    qreal roundingError = qFloor((w / 2.0) - qFloor(w / 2.0) + 0.5);
    QCOMPARE(gap10, gapN3 - roundingError);
}

void tst_toolbarlayout::layouting3()
{
    QGraphicsObject *toolBarLayout3 = componentObject->findChild<QGraphicsObject*>("toolBarLayout3");
    QGraphicsObject *toolButton3_0 = componentObject->findChild<QGraphicsObject*>("toolButton3_0");
    QGraphicsObject *toolButton3_a = componentObject->findChild<QGraphicsObject*>("toolButton3_a");
    QGraphicsObject *toolButton3_b = componentObject->findChild<QGraphicsObject*>("toolButton3_b");
    QGraphicsObject *toolButton3_N = componentObject->findChild<QGraphicsObject*>("toolButton3_N");

    QVERIFY(toolButton3_0);
    QVERIFY(toolButton3_a);
    QVERIFY(toolButton3_b);
    QVERIFY(toolButton3_N);

    qreal w = 1000.0;

    toolBarLayout3->setProperty("width", QVariant(w));
    QMetaObject::invokeMethod(toolBarLayout3, "childrenChanged");

    qreal x0 = toolButton3_0->x();
    qreal x1 = toolButton3_a->x();
    qreal x2 = toolButton3_b->x();
    qreal xN = toolButton3_N->x();

    qreal w0 = toolButton3_0->property("width").toReal();
    qreal w1 = toolButton3_a->property("width").toReal();
    qreal w2 = toolButton3_b->property("width").toReal();
    qreal wN = toolButton3_N->property("width").toReal();

    // the text toolButtons should be the same width, but different width to the icon toolButtons
    QCOMPARE(w0, wN);
    QCOMPARE(w1, w2);

    QVERIFY(w0 < w1);
    QVERIFY(wN < w2);

    // check that the text items are equidistant from the center
    // if w is odd, there will be an extra pixel gap on the right
    qreal roundingError = qFloor((w / 2.0) - qFloor(w / 2.0) + 0.5);
    qreal offset1 = (w / 2) - (x1 + w1);
    qreal offset2 = x2 - (w / 2);
    QCOMPARE(offset1, offset2 + roundingError);

    // check that the text items are equidistant from the edge items.
    // if w is odd, there will be an extra pixel gap on the right
    qreal gap10 = x1 - (x0 + w0);
    qreal gapN2 = xN - (x2 + w2);
    QCOMPARE(gap10, gapN2 - roundingError);
}

void tst_toolbarlayout::layouting4()
{
    QGraphicsObject *toolBarLayout4 = componentObject->findChild<QGraphicsObject*>("toolBarLayout4");
    QGraphicsObject *buttonRow4_1 = componentObject->findChild<QGraphicsObject*>("buttonRow4");

    QVERIFY(buttonRow4_1);

    qreal w = 1000.0;

    toolBarLayout4->setProperty("width", QVariant(w));
    QMetaObject::invokeMethod(toolBarLayout4, "childrenChanged");

    qreal x1 = buttonRow4_1->x();
    qreal w1 = buttonRow4_1->property("width").toReal();

    // check that the button row is equidistant from the edges.
    QCOMPARE(x1, w - (x1 + w1));
}

void tst_toolbarlayout::layouting5()
{
    QGraphicsObject *toolBarLayout5 = componentObject->findChild<QGraphicsObject*>("toolBarLayout5");
    QGraphicsObject *buttonRow5_1 = componentObject->findChild<QGraphicsObject*>("buttonRow5");

    QVERIFY(buttonRow5_1);

    qreal w = 1000.0;

    toolBarLayout5->setProperty("width", QVariant(w));
    QMetaObject::invokeMethod(toolBarLayout5, "childrenChanged");

    qreal x1 = buttonRow5_1->x();
    qreal w1 = buttonRow5_1->property("width").toReal();

    // check that the button row is equidistant from the edges, even though there is no right
    // hand icon button. This is because there are only two text items in the button row.
    QCOMPARE(x1, w - (x1 + w1));
}

void tst_toolbarlayout::layouting6()
{
    QGraphicsObject *toolBarLayout6 = componentObject->findChild<QGraphicsObject*>("toolBarLayout6");
    QGraphicsObject *buttonRow6_1 = componentObject->findChild<QGraphicsObject*>("buttonRow6");

    QVERIFY(buttonRow6_1);

    qreal w = 1000.0;

    toolBarLayout6->setProperty("width", QVariant(w));
    QMetaObject::invokeMethod(toolBarLayout6, "childrenChanged");

    qreal x1 = buttonRow6_1->x();
    qreal w1 = buttonRow6_1->property("width").toReal();

    // check that the button row is NOT equidistant from the edges, because there are three
    // text items in the button row, and there is just one icon item.
    // if w is odd, there will be an extra pixel gap on the right
    qreal roundingError = qFloor((w / 2.0) - qFloor(w / 2.0) + 0.5);
    QVERIFY(x1 > w - (x1 + w1) - roundingError);
}

void tst_toolbarlayout::layouting7()
{
    QGraphicsObject *toolBarLayout7 = componentObject->findChild<QGraphicsObject*>("toolBarLayout7");
    QGraphicsObject *buttonRow7_1 = componentObject->findChild<QGraphicsObject*>("buttonRow7");

    QVERIFY(buttonRow7_1);

    qreal w = 1000.0;

    toolBarLayout7->setProperty("width", QVariant(w));
    QMetaObject::invokeMethod(toolBarLayout7, "childrenChanged");

    qreal x1 = buttonRow7_1->x();
    qreal w1 = buttonRow7_1->property("width").toReal();

    // check that the button row is equidistant from the edges, because there are three
    // text items in the button row, and there are no icon items.
    // if w is odd, there will be an extra pixel gap on the right
    QCOMPARE(x1, w - (x1 + w1));
}

QTEST_MAIN(tst_toolbarlayout)
#include "tst_toolbarlayout.moc"
