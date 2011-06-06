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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QDeclarativeView>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsprogressbar : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void value();
    void minimumValue();
    void maximumValue();
    void indeterminate();

private:
    QStringList standard;
    QString qmlSource;

    QObject *componentObject;
};


void tst_quickcomponentsprogressbar::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsprogressbar.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsprogressbar::value()
{
    componentObject->setProperty( "minimumValue", 0.0 );
    componentObject->setProperty( "maximumValue", 1.0 );

    // first, test if the same value set can be retrieved later
    QVERIFY( componentObject->setProperty( "value", 0.5 ) );
    QCOMPARE( componentObject->property("value").toReal(), 0.5 );

    // when a value lower than minimumValue is set, we expect value to be equal to minimumValue
    QVERIFY( componentObject->setProperty( "value", -0.5 ) );
    QCOMPARE( componentObject->property("value").toReal(), componentObject->property ("minimumValue").toReal ());

    // when a value higher than maximumValue is set, we expect value to be equal to maximumValue
    QVERIFY( componentObject->setProperty( "value", 2.0 ) );
    QCOMPARE( componentObject->property("value").toReal(), componentObject->property ("maximumValue").toReal ());
}

void tst_quickcomponentsprogressbar::minimumValue()
{
    // check presence of property and set initial value different than the default minimumValue
    componentObject->setProperty( "maximumValue", 1.0 );
    QVERIFY( componentObject->setProperty( "minimumValue", -1.0 ) );
    QCOMPARE( componentObject->property("minimumValue").toReal(), -1.0 );

    // try to set value below the minimum value set previously
    componentObject->setProperty( "value", -2.0 );
    // the minimum limit must be respected, so setting a value bellow minimumValue should set value to minimumValue
    QVERIFY( componentObject->property("value") == componentObject->property ("minimumValue"));
}

void tst_quickcomponentsprogressbar::maximumValue()
{
    // check presence of property and set initial value
    componentObject->setProperty( "minimumValue", 0.0 );
    QVERIFY( componentObject->setProperty( "maximumValue", 10.0 ) );
    QCOMPARE( componentObject->property("maximumValue").toReal(), 10.0 );

    // tro ty set value above maximum, maximum value set previously
    componentObject->setProperty( "value", 50.0 );
    // the maximum limit must be respected, so setting a value above maximumValue should set value to maximumValue
    QVERIFY( componentObject->property("value") == componentObject->property ("maximumValue"));
}

void tst_quickcomponentsprogressbar::indeterminate()
{
    // check presence of property and set initial value
    QVERIFY( componentObject->setProperty( "indeterminate", true ) );
    QCOMPARE( componentObject->property("indeterminate").toBool(), true );

    // now check if we can set to false again
    QVERIFY( componentObject->setProperty( "indeterminate", false ) );
    QCOMPARE( componentObject->property("indeterminate").toBool(), false );
}

QTEST_MAIN(tst_quickcomponentsprogressbar)

#include "tst_quickcomponentsprogressbar.moc"
