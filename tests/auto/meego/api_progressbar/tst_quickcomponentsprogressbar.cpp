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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <qdeclarativecontext.h>
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
