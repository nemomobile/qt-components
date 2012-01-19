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
#include <qdeclarativecomponent.h>
#include <qdeclarativeview.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsbutton : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void checked();
    void checkable();
    void clicked();
    void text();
    void iconSource();

private:
    QObject *componentObject;
};

void tst_quickcomponentsbutton::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsbutton.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsbutton::checkable()
{
    // make sure properties are present and set initial values
    QVERIFY( componentObject->setProperty("checked", false) );
    QVERIFY( componentObject->setProperty("checkable", false) );

    // test both properties have right initial value
    QCOMPARE( componentObject->property("checkable").toBool(), false );
    QCOMPARE( componentObject->property("checked").toBool(), false );

    // try to set the property while checkable is false
    QVERIFY( componentObject->setProperty("checked",true) );
    QEXPECT_FAIL("", "QML Doesn't have readonly properties", Continue);
    QCOMPARE( componentObject->property("checked").toBool(),false );

    // allow checked to change again, and verify it changes
    QVERIFY( componentObject->setProperty("checkable",true) );
    QVERIFY( componentObject->setProperty("checked",true) );
    QCOMPARE( componentObject->property("checked").toBool(),true );
}

void tst_quickcomponentsbutton::checked()
{
    // make sure properties are present and set initial values
    QVERIFY( componentObject->setProperty("checked",false) );
    QVERIFY( componentObject->setProperty("checkable",true) );

    // test switching the checked property
    QCOMPARE( componentObject->property("checked").toBool(),false );
    QVERIFY( componentObject->setProperty("checked",true) );
    QCOMPARE( componentObject->property("checked").toBool(), true );
}
void tst_quickcomponentsbutton::text()
{
    QCOMPARE(componentObject->property("text").toString(),QString("Testing Button"));
    componentObject->setProperty("text","Button Tested!");
    QCOMPARE(componentObject->property("text").toString(),QString("Button Tested!"));
}

void tst_quickcomponentsbutton::iconSource()
{
    QCOMPARE(componentObject->property("iconSource").toString(),QString("logo.png"));
}

void tst_quickcomponentsbutton::clicked()
{
    QSignalSpy spy(componentObject, SIGNAL(clicked()));
    QMetaObject::invokeMethod(componentObject,"clicked",Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

QTEST_MAIN(tst_quickcomponentsbutton)

#include "tst_quickcomponentsbutton.moc"
