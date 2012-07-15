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

class tst_tabbarlayout : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void defaultPropertyValues();
    void layoutingDefault();
    void layoutingWidthChanged();
    void implicitSize();

private:
    QScopedPointer<QObject> componentObject;
};

void tst_tabbarlayout::initTestCase()
{
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_tabbarlayout.qml", &errors));
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_tabbarlayout::defaultPropertyValues()
{
    QGraphicsObject *testLayout = componentObject->findChild<QGraphicsObject*>("testLayout");
    QVERIFY(testLayout);
    QCOMPARE(testLayout->property("width").toReal(), qreal(0));
    QCOMPARE(testLayout->property("height").toReal(), qreal(0));
}

void tst_tabbarlayout::layoutingDefault()
{
    QGraphicsObject *tabLayout1 = componentObject->findChild<QGraphicsObject*>("tabLayout1");
    QVERIFY(tabLayout1);

    QGraphicsObject *button1 = componentObject->findChild<QGraphicsObject*>("button1");
    QVERIFY(button1);

    QGraphicsObject *button2 = componentObject->findChild<QGraphicsObject*>("button2");
    QVERIFY(button2);

    QGraphicsObject *button3 = componentObject->findChild<QGraphicsObject*>("button3");
    QVERIFY(button3);

    // set initial size
    tabLayout1->setProperty("width", 50);
    tabLayout1->setProperty("height", 50);
    // Tabbarlayout is updated asynchronously so process event loop.
    QTest::qWait(250);
    QCoreApplication::processEvents();

    // check button1
    QCOMPARE(button1->pos(), QPointF(0,0));
    QVERIFY(button1->property("width").toReal() > 0);
    QCOMPARE(button1->property("height").toReal(), tabLayout1->property("height").toReal());

    // check button2
    QVERIFY(button2->x() >= button1->x() + button1->property("width").toReal());
    QCOMPARE(button2->y(), qreal(0.0));
    QVERIFY(button2->property("width").toReal() > 0);
    QCOMPARE(button2->property("height").toReal(), tabLayout1->property("height").toReal());

    // check button3
    QVERIFY(button3->x() >= button2->x() + button2->property("width").toReal());
    QCOMPARE(button3->y(), qreal(0.0));
    QVERIFY(button3->property("width").toReal() > 0);
    QCOMPARE(button3->property("height").toReal(), tabLayout1->property("height").toReal());
    QCOMPARE(button3->x() + button3->property("width").toReal(), tabLayout1->property("width").toReal());
}

void tst_tabbarlayout::layoutingWidthChanged()
{
    QGraphicsObject *tabLayout1 = componentObject->findChild<QGraphicsObject*>("tabLayout1");
    QVERIFY(tabLayout1);

    tabLayout1->setProperty("width", QVariant(2000));
    layoutingDefault(); // do the same test with different parent width
}

void tst_tabbarlayout::implicitSize()
{
    QGraphicsObject *tabLayout2 = componentObject->findChild<QGraphicsObject*>("tabLayout2");
    QVERIFY(tabLayout2);

    QGraphicsObject *tabButton1 = componentObject->findChild<QGraphicsObject*>("tabButton1");
    QVERIFY(tabButton1);

    QGraphicsObject *tabButton2 = componentObject->findChild<QGraphicsObject*>("tabButton2");
    QVERIFY(tabButton2);

    // set implicitSize for the content items
    tabButton1->setProperty("implicitWidth", 20);
    tabButton1->setProperty("implicitHeight", 31);

    tabButton2->setProperty("implicitWidth", 35);
    tabButton2->setProperty("implicitHeight", 20);

    QMetaObject::invokeMethod(tabLayout2, "childrenChanged");

    QCOMPARE(tabLayout2->property("implicitWidth").toReal(), qreal(35*2));
    QCOMPARE(tabLayout2->property("implicitHeight").toReal(), qreal(31));
}


QTEST_MAIN(tst_tabbarlayout)
#include "tst_tabbarlayout.moc"
