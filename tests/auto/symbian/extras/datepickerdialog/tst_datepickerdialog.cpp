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
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativeview.h>

#include "tst_quickcomponentstest.h"

class tst_datepickerdialog : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void year();
    void month();
    void day();
    void minimumYear();
    void maximumYear();
    void acceptButtonText();
    void rejectButtonText();

private:
    QObject *componentObject;
};


void tst_datepickerdialog::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_datepickerdialog.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_datepickerdialog::year()
{
    QVERIFY(componentObject->property("year").isValid());
    QCOMPARE(componentObject->property("year").toInt(), 0);
    componentObject->setProperty("year", 2011);
    QCOMPARE(componentObject->property("year").toInt(), 2011);
    componentObject->setProperty("minimumYear", 2011);
    componentObject->setProperty("maximumYear", 2031);

    // Out of bound
    componentObject->setProperty("year", -2);
    QCOMPARE(componentObject->property("year").toInt(), componentObject->property("year").toInt());
}

void tst_datepickerdialog::month()
{
    QVERIFY(componentObject->property("month").isValid());
    QCOMPARE(componentObject->property("month").toInt(), 1);
    componentObject->setProperty("month", 5);
    QCOMPARE(componentObject->property("month").toInt(), 5);

    // Out of bound
    componentObject->setProperty("month", -2);
    QCOMPARE(componentObject->property("month").toInt(), 1);
    componentObject->setProperty("month", 15);
    QCOMPARE(componentObject->property("month").toInt(), 12);
}

void tst_datepickerdialog::day()
{
    QVERIFY(componentObject->property("day").isValid());
    QCOMPARE(componentObject->property("day").toInt(), 1);
    componentObject->setProperty("day", 5);
    QCOMPARE(componentObject->property("day").toInt(), 5);

    // Out of bound
    componentObject->setProperty("day", -2);
    QCOMPARE(componentObject->property("day").toInt(), 1);
    componentObject->setProperty("day", 40);
    QCOMPARE(componentObject->property("day").toInt(), 31);

    // Leap year
    componentObject->setProperty("month", 2);
    componentObject->setProperty("year", 2012);
    componentObject->setProperty("day", 31);
    QCOMPARE(componentObject->property("day").toInt(), 29);
}

void tst_datepickerdialog::minimumYear()
{
    QVERIFY(componentObject->property("minimumYear").isValid());
    componentObject->setProperty("minimumYear", 1990);
    QCOMPARE(componentObject->property("minimumYear").toInt(), 1990);

    // Out of bound
    componentObject->setProperty("year", 2012);
    QCOMPARE(componentObject->property("year").toInt(), 2012);
    componentObject->setProperty("year", 1980);
    componentObject->setProperty("status", 2);
    QCOMPARE(componentObject->property("year").toInt(), componentObject->property("minimumYear").toInt());
    componentObject->setProperty("status", 3);
}

void tst_datepickerdialog::maximumYear()
{
    QVERIFY(componentObject->property("maximumYear").isValid());
    componentObject->setProperty("maximumYear", 2050);
    QCOMPARE(componentObject->property("maximumYear").toInt(), 2050);

    // Out of bound
    componentObject->setProperty("year", 2012);
    QCOMPARE(componentObject->property("year").toInt(), 2012);
    componentObject->setProperty("year", 2060);
    componentObject->setProperty("status", 2);
    QCOMPARE(componentObject->property("year").toInt(), componentObject->property("maximumYear").toInt());
}

void tst_datepickerdialog::acceptButtonText()
{
    QVERIFY(componentObject->property("acceptButtonText").isValid());
    QCOMPARE(componentObject->property("acceptButtonText").toString(),QString(""));
    componentObject->setProperty("acceptButtonText","Ok");
    QCOMPARE(componentObject->property("acceptButtonText").toString(),QString("Ok"));
}

void tst_datepickerdialog::rejectButtonText()
{
    QVERIFY(componentObject->property("rejectButtonText").isValid());
    QCOMPARE(componentObject->property("rejectButtonText").toString(),QString(""));
    componentObject->setProperty("rejectButtonText","Cancel");
    QCOMPARE(componentObject->property("rejectButtonText").toString(),QString("Cancel"));
}

QTEST_MAIN(tst_datepickerdialog)

#include "tst_datepickerdialog.moc"
