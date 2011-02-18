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
