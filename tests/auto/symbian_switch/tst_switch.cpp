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
    void testTextProperty();
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

void tst_switch::testTextProperty()
{
    QVariant text = componentObject->property("text");
    QVERIFY(text.isValid());
    QVERIFY(text.canConvert(QVariant::String));
    QVERIFY(text.toString().isEmpty());
    componentObject->setProperty("text", "My Switch");
    text = componentObject->property("text");
    QCOMPARE(text.toString(), QString("My Switch"));
}

void tst_switch::testImplicitSize()
{
    QDeclarativeItem *switchButton = qobject_cast<QDeclarativeItem*>(componentObject);
    QVERIFY(switchButton);

    // no text
    switchButton->setProperty("text", "");
    const qreal noTextImplicitWidth = switchButton->implicitWidth();
    const qreal noTextImplicitHeight = switchButton->implicitHeight();
    QVERIFY(noTextImplicitWidth > 5); // needs to be something for the tool
    QVERIFY(noTextImplicitHeight > 5); // needs to be something for the tool

    // short text
    switchButton->setProperty("text", "short");
    const qreal shortTextImplicitWidth = switchButton->implicitWidth();
    const qreal shortTextImplicitHeight = switchButton->implicitHeight();
    QVERIFY(shortTextImplicitWidth > noTextImplicitWidth);
    QVERIFY(shortTextImplicitHeight >= noTextImplicitHeight);

    // long text
    switchButton->setProperty("text", "longer text than just 'short'");
    const qreal longTextImplicitWidth = switchButton->implicitWidth();
    const qreal longTextImplicitHeight = switchButton->implicitHeight();
    QVERIFY(longTextImplicitWidth > shortTextImplicitWidth);
    QVERIFY(longTextImplicitHeight >= shortTextImplicitHeight);

    // long text
    switchButton->setProperty("text", "longer text than just 'short'");
    switchButton->setProperty("checked", false);
    QCOMPARE(longTextImplicitWidth, switchButton->implicitWidth());
    QCOMPARE(longTextImplicitHeight, switchButton->implicitHeight());
    switchButton->setProperty("checked", true);
    QCOMPARE(longTextImplicitWidth, switchButton->implicitWidth());
    QCOMPARE(longTextImplicitHeight, switchButton->implicitHeight());
}

QTEST_MAIN(tst_switch)
#include "tst_switch.moc"
