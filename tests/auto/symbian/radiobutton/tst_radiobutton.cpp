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

    QVERIFY(componentObject->property("platformInverted").isValid());
    QCOMPARE(componentObject->property("platformInverted").toBool(), false);
}


void tst_radiobutton::properties()
{
    QVERIFY(componentObject->setProperty("checked", false));
    QCOMPARE(componentObject->property("checked").toBool(), false);
    QVERIFY(componentObject->setProperty("checked", true));
    QCOMPARE(componentObject->property("checked").toBool(), true);

    QVERIFY(componentObject->setProperty("text", "radiobutton_text"));
    QCOMPARE(componentObject->property("text").toString(), QString("radiobutton_text"));

    QVERIFY(componentObject->setProperty("platformInverted", true));
    QCOMPARE(componentObject->property("platformInverted").toBool(), true);
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
