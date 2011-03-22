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
#include <QSignalSpy>
#include "tst_quickcomponentstest.h"

class tst_radiobutton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void defaultPropertyValues();
    void properties();
    void testImplicitSize();
    void clicked();

private:
    QObject *componentObject;
};

void tst_radiobutton::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_radiobutton.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_radiobutton::cleanupTestCase()
{
}

void tst_radiobutton::init()
{
}

void tst_radiobutton::cleanup()
{
}

void tst_radiobutton::defaultPropertyValues()
{
    QVERIFY(componentObject->property("checked").isValid());
    QCOMPARE(componentObject->property("checked").toBool(), false);

    QVERIFY(componentObject->property("text").isValid());
    QVERIFY(componentObject->property("text").toString().isNull());

    QVERIFY(componentObject->property("platformExclusiveGroup").isValid());
    QVERIFY(componentObject->property("platformExclusiveGroup").toString().isNull());
}


void tst_radiobutton::properties()
{
    QVERIFY(componentObject->setProperty("checked", false));
    QCOMPARE(componentObject->property("checked").toBool(), false);
    QVERIFY(componentObject->setProperty("checked", true));
    QCOMPARE(componentObject->property("checked").toBool(), true);

    QVERIFY(componentObject->setProperty("text", "radiobutton_text"));
    QCOMPARE(componentObject->property("text").toString(), QString("radiobutton_text"));
}

void tst_radiobutton::testImplicitSize()
{
    QDeclarativeItem *radioButton = qobject_cast<QDeclarativeItem*>(componentObject);
    QVERIFY(radioButton);

    // no text
    radioButton->setProperty("text", "");
    const qreal noTextImplicitWidth = radioButton->implicitWidth();
    const qreal noTextImplicitHeight = radioButton->implicitHeight();
    QVERIFY(noTextImplicitWidth > 5); // needs to be something for the tool
    QVERIFY(noTextImplicitHeight > 5); // needs to be something for the tool

    // short text
    radioButton->setProperty("text", "short");
    const qreal shortTextImplicitWidth = radioButton->implicitWidth();
    const qreal shortTextImplicitHeight = radioButton->implicitHeight();
    QVERIFY(shortTextImplicitWidth > noTextImplicitWidth);
    QVERIFY(shortTextImplicitHeight >= noTextImplicitHeight);

    // long text
    radioButton->setProperty("text", "longer text than just 'short'");
    const qreal longTextImplicitWidth = radioButton->implicitWidth();
    const qreal longTextImplicitHeight = radioButton->implicitHeight();
    QVERIFY(longTextImplicitWidth > shortTextImplicitWidth);
    QVERIFY(longTextImplicitHeight >= shortTextImplicitHeight);

    // long text, change checkable
    radioButton->setProperty("text", "longer text than just 'short'");
    radioButton->setProperty("checkable", true);
    radioButton->setProperty("checked", false);
    QCOMPARE(longTextImplicitWidth, radioButton->implicitWidth());
    QCOMPARE(longTextImplicitHeight, radioButton->implicitHeight());
    radioButton->setProperty("checked", true);
    QCOMPARE(longTextImplicitWidth, radioButton->implicitWidth());
    QCOMPARE(longTextImplicitHeight, radioButton->implicitHeight());
    radioButton->setProperty("checkable", false);
    QCOMPARE(longTextImplicitWidth, radioButton->implicitWidth());
    QCOMPARE(longTextImplicitHeight, radioButton->implicitHeight());
}

void tst_radiobutton::clicked()
{
    QSignalSpy spy(componentObject, SIGNAL(clicked()));
    QMetaObject::invokeMethod(componentObject, "clicked", Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

QTEST_MAIN(tst_radiobutton)
#include "tst_radiobutton.moc"
