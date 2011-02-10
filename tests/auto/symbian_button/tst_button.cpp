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

#include <QTest>
#include <QApplication>
#include <QDeclarativeView>
#include <QGraphicsObject>
#include <QDeclarativeItem>
#include "tst_quickcomponentstest.h"

class tst_button : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void defaultPropertyValues();
    void properties();
    void testImplicitSize();
    void released();
    void pressAndHold();

private:
    QObject *componentObject;
};

void tst_button::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_button.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_button::defaultPropertyValues()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("button2");
    QVERIFY(testButton);
    QVERIFY(testButton->property("autoRepeat").isValid());
    QCOMPARE(testButton->property("autoRepeat").toBool(), false);
    QVERIFY(testButton->property("longPress").isValid());
    QCOMPARE(testButton->property("longPress").toBool(), false);
}

void tst_button::properties()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("button2");
    QVERIFY(testButton);

    testButton->setProperty("autoRepeat", QVariant(true));
    QCOMPARE(testButton->property("autoRepeat").toBool(), true);

    testButton->setProperty("autoRepeat", QVariant(false));
    QCOMPARE(testButton->property("autoRepeat").toBool(), false);

    testButton->setProperty("longPress", QVariant(true));
    QCOMPARE(testButton->property("longPress").toBool(), true);

    testButton->setProperty("longPress", QVariant(false));
    QCOMPARE(testButton->property("longPress").toBool(), false);
}

void tst_button::testImplicitSize()
{
    // this test assumes that icon and text are horizontally aligned
    // if this assumption changes, please update this test.
    QDeclarativeItem *button = componentObject->findChild<QDeclarativeItem*>("testButton");
    QVERIFY(button);

    // no text, no icon
    button->setProperty("text", "");
    button->setProperty("iconSource", "");
    const qreal noTextNoIconImplicitWidth = button->implicitWidth();
    const qreal noTextNoIconImplicitHeight = button->implicitHeight();
    QVERIFY(noTextNoIconImplicitWidth > 5); // needs to be something for the tool
    QVERIFY(noTextNoIconImplicitHeight > 5); // needs to be something for the tool

    // short text, no icon
    button->setProperty("text", "short");
    button->setProperty("iconSource", "");
    const qreal shortTextNoIconImplicitWidth = button->implicitWidth();
    const qreal shortTextNoIconImplicitHeight = button->implicitHeight();
    QVERIFY(shortTextNoIconImplicitWidth > noTextNoIconImplicitWidth);
    QVERIFY(shortTextNoIconImplicitHeight >= noTextNoIconImplicitHeight);

    // long text, no icon
    button->setProperty("text", "longer text than just 'short'");
    button->setProperty("iconSource", "");
    const qreal longTextNoIconImplicitWidth = button->implicitWidth();
    const qreal longTextNoIconImplicitHeight = button->implicitHeight();
    QVERIFY(longTextNoIconImplicitWidth > shortTextNoIconImplicitWidth);
    QVERIFY(longTextNoIconImplicitHeight >= shortTextNoIconImplicitHeight);

    // long text, some icon
    button->setProperty("text", "longer text than just 'short'");
    button->setProperty("iconSource", "image://theme/qtg_small_selected");
    const qreal longTextSomeIconImplicitWidth = button->implicitWidth();
    const qreal longTextSomeIconImplicitHeight = button->implicitHeight();
    QVERIFY(longTextSomeIconImplicitWidth > longTextNoIconImplicitWidth);
    QVERIFY(longTextSomeIconImplicitHeight >= longTextNoIconImplicitHeight);
}

void tst_button::released()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("button3");
    QVERIFY(testButton);
    QSignalSpy releasedSpy(testButton, SIGNAL(released()));
    int releasedCount = 0;
    QMetaObject::invokeMethod(testButton, "release", Qt::DirectConnection);
    QCOMPARE(releasedSpy.count(), ++releasedCount);
    QVERIFY(releasedSpy.isValid());
}

void tst_button::pressAndHold()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("button3");
    QVERIFY(testButton);
    QSignalSpy pressAndHoldSpy(testButton, SIGNAL(pressAndHold()));

    testButton->setProperty("longPress", QVariant(true));
    QCOMPARE(testButton->property("longPress").toBool(), true);

    int pressAndHoldCount = 0;
    QMetaObject::invokeMethod(testButton, "press", Qt::DirectConnection);
    QMetaObject::invokeMethod(testButton, "hold", Qt::DirectConnection);
    QCOMPARE(pressAndHoldSpy.count(), ++pressAndHoldCount);
    QVERIFY(pressAndHoldSpy.isValid());
}

QTEST_MAIN(tst_button)
#include "tst_button.moc"
