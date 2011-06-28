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
    void testFont();
    void platformReleased();

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
    QVERIFY(testButton->property("platformAutoRepeat").isValid());
    QCOMPARE(testButton->property("platformAutoRepeat").toBool(), false);
    QVERIFY(testButton->property("font").isValid());
    QVERIFY(testButton->property("platformInverted").isValid());
    QCOMPARE(testButton->property("platformInverted").toBool(), false);
}

void tst_button::properties()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("button2");
    QVERIFY(testButton);

    testButton->setProperty("platformAutoRepeat", QVariant(true));
    QCOMPARE(testButton->property("platformAutoRepeat").toBool(), true);

    testButton->setProperty("platformAutoRepeat", QVariant(false));
    QCOMPARE(testButton->property("platformAutoRepeat").toBool(), false);

    testButton->setProperty("platformInverted", QVariant(true));
    QCOMPARE(testButton->property("platformInverted").toBool(), true);

    testButton->setProperty("platformInverted", QVariant(false));
    QCOMPARE(testButton->property("platformInverted").toBool(), false);
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
    button->setProperty("iconSource", "image://theme/qtg_graf_radiobutton_normal_selected");
    const qreal longTextSomeIconImplicitWidth = button->implicitWidth();
    const qreal longTextSomeIconImplicitHeight = button->implicitHeight();
    QVERIFY(longTextSomeIconImplicitWidth >= longTextNoIconImplicitWidth);
    QVERIFY(longTextSomeIconImplicitHeight > longTextNoIconImplicitHeight);
}

void tst_button::testFont()
{
    QDeclarativeItem *button = componentObject->findChild<QDeclarativeItem*>("testButton");
    QVERIFY(button);
    const QFont defaultFont = button->property("font").value<QFont>();


    // override family
    QDeclarativeItem *buttonFont1 = componentObject->findChild<QDeclarativeItem*>("buttonFont1");
    QVERIFY(buttonFont1);
    const QFont font1 = buttonFont1->property("font").value<QFont>();
#if QT_VERSION < 0x040702
    // http://bugreports.qt.nokia.com/browse/QTBUG-13719
    QEXPECT_FAIL("", "Will fix in the 4.7.2 release", Continue);
#endif // QT_VERSION
    QCOMPARE(font1.family(), QString("Arial"));
    QCOMPARE(font1.pixelSize(), defaultFont.pixelSize());

    // override pixelSize
    QDeclarativeItem *buttonFont2 = componentObject->findChild<QDeclarativeItem*>("buttonFont2");
    QVERIFY(buttonFont2);
    const QFont font2 = buttonFont2->property("font").value<QFont>();
    QCOMPARE(font2.family(), defaultFont.family());
#if QT_VERSION < 0x040702
    // http://bugreports.qt.nokia.com/browse/QTBUG-13719
    QEXPECT_FAIL("", "Will fix in the 4.7.2 release", Continue);
#endif // QT_VERSION
    QCOMPARE(font2.pixelSize(), 55);

}

void tst_button::platformReleased()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("button3");
    QVERIFY(testButton);

    QSignalSpy releasedSpy(testButton, SIGNAL(platformReleased()));
    QVERIFY(releasedSpy.isValid());

    QObject *internal = testButton->findChild<QObject*>("internal");
    QVERIFY(internal);

    QVERIFY(QMetaObject::invokeMethod(internal, "release"));
    QCOMPARE(releasedSpy.count(), 1);
}

QTEST_MAIN(tst_button)

#include "tst_button.moc"
