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
#include <QDeclarativeItem>
#include <QDeclarativeComponent>
#include <QDebug>
#include "tst_quickcomponentstest.h"

class tst_switch : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testClickedSignal();
    void testCheckedProperty();
    void testPressedProperty();
    void testPlatformInvertedProperty();
    void testImplicitSize();

private:
    QObject *componentObject;
};

void tst_switch::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_switch.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_switch::testClickedSignal()
{
    QSignalSpy spy(componentObject, SIGNAL(clicked()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(componentObject, "clicked");
    QCOMPARE(spy.count(),1);
}

void tst_switch::testCheckedProperty()
{
    QVariant checked = componentObject->property("checked");
    QVERIFY(checked.isValid());
    QVERIFY(checked.canConvert(QVariant::Bool));
    QCOMPARE(checked.toBool(), false);
    componentObject->setProperty("checked", true);
    checked = componentObject->property("checked");
    QCOMPARE(checked.toBool(), true);
}

void tst_switch::testPressedProperty()
{
    QVariant pressed = componentObject->property("pressed");
    QVERIFY(pressed.isValid());
    QVERIFY(pressed.canConvert(QVariant::Bool));
    QCOMPARE(pressed.toBool(), false);
    componentObject->setProperty("pressed", true);
    pressed = componentObject->property("pressed");
    QCOMPARE(pressed.toBool(), true);
}

void tst_switch::testPlatformInvertedProperty()
{
    QVariant inverted = componentObject->property("platformInverted");
    QVERIFY(inverted.isValid());
    QVERIFY(inverted.canConvert(QVariant::Bool));
    QCOMPARE(inverted.toBool(), false);
    componentObject->setProperty("platformInverted", true);
    inverted = componentObject->property("platformInverted");
    QCOMPARE(inverted.toBool(), true);
}

void tst_switch::testImplicitSize()
{
    QDeclarativeItem *switchButton = qobject_cast<QDeclarativeItem*>(componentObject);
    QVERIFY(switchButton);

    const qreal implicitWidth = switchButton->implicitWidth();
    const qreal implicitHeight = switchButton->implicitHeight();

    switchButton->setProperty("checked", false);
    QCOMPARE(implicitWidth, switchButton->implicitWidth());
    QCOMPARE(implicitHeight, switchButton->implicitHeight());
    switchButton->setProperty("checked", true);
    QCOMPARE(implicitWidth, switchButton->implicitWidth());
    QCOMPARE(implicitHeight, switchButton->implicitHeight());
}

QTEST_MAIN(tst_switch)
#include "tst_switch.moc"
