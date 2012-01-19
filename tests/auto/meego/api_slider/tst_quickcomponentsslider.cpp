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

class tst_quickcomponentsslider : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void value();
    void stepSize();
    void minimumValue();
    void maximumValue();
    void orientation();
    void valueIndicatorVisible();
    void valueIndicatorText();
    void pressed();

private:
    QObject *componentObject;
};


void tst_quickcomponentsslider::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsslider.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsslider::value()
{
    // minimum setup to set a value
    componentObject->setProperty("minimumValue", 0.0);
    componentObject->setProperty("maximumValue", 100.0);

    // test if setting/getting value works properly
    QVERIFY(componentObject->setProperty("value", 10.0));
    QCOMPARE(componentObject->property("value").toReal(), 10.0);
}

void tst_quickcomponentsslider::stepSize()
{
    // test if setting/getting stepSize works properly
    QVERIFY(componentObject->setProperty("stepSize", 10.0));
    QCOMPARE(componentObject->property("stepSize").toReal(), 10.0);

    // test if stepSize if working properly (sometimes it may not use
    // the range model and that's why it's a valid test)
    componentObject->setProperty("value", 19.0);
    QCOMPARE(componentObject->property("value").toReal(), 20.0);
}

void tst_quickcomponentsslider::minimumValue()
{
    // test if setting/getting minimumValue works properly
    QVERIFY(componentObject->setProperty("minimumValue", 0.0));
    QCOMPARE(componentObject->property("minimumValue").toReal(), 0.0);

    // small setup to test the set of values below minimum
    componentObject->setProperty("maximumValue", 100.0);
    componentObject->setProperty("value", 50.0);

    // try to set value below minimum (0)
    componentObject->setProperty( "value", -15.0 );

    // the value should be equal to the minimum
    QVERIFY(componentObject->property("value").toReal() ==
            componentObject->property("minimumValue").toReal());
}

void tst_quickcomponentsslider::maximumValue()
{
    // test if setting/getting maximumValue works properly
    QVERIFY(componentObject->setProperty("maximumValue", 200.0));
    QCOMPARE(componentObject->property("maximumValue").toReal(), 200.0);

    // small setup to test the set of values above maximum
    componentObject->setProperty("minimumValue", 0.0);
    componentObject->setProperty("value", 50.0 );

    // try to set a value above the maximum
    componentObject->setProperty("value", 210.0 );

    // the value should be equal to the maximum
    QVERIFY(componentObject->property("value").toReal() ==
            componentObject->property("maximumValue").toReal());
}

void tst_quickcomponentsslider::orientation()
{
    // test if setting/getting the orientation works properly
    QVERIFY(componentObject->setProperty("orientation", Qt::Vertical));
    QCOMPARE(componentObject->property("orientation").toInt(),
             (int)Qt::Vertical);

    QVERIFY(componentObject->setProperty("orientation", Qt::Horizontal));
    QCOMPARE(componentObject->property("orientation").toInt(),
             (int)Qt::Horizontal);

    // try to set a higher value
    componentObject->setProperty("orientation", 3);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("orientation").toInt()
            <= (int)Qt::Vertical);

    // try to set a lower value
    componentObject->setProperty("orientation", 0);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("orientation").toInt()
            >= (int)Qt::Horizontal);

    // try to set random value
    componentObject->setProperty("orientation", qrand());
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("orientation").toInt()
            <= (int)Qt::Horizontal
            && componentObject->property("orientation").toInt()
            >= (int)Qt::Vertical);
}

void tst_quickcomponentsslider::pressed()
{
    // as the implementation can change, we can't test the
    // emission of this signal without relying in specific
    // internal members of the slider
}

void tst_quickcomponentsslider::valueIndicatorVisible()
{
    // test if setting/getting stepSize works properly
    QVERIFY(componentObject->setProperty("valueIndicatorVisible", true));
    QCOMPARE(componentObject->property("valueIndicatorVisible").toBool(),
             true);
}

void tst_quickcomponentsslider::valueIndicatorText()
{
    // test if setting/getting stepSize works properly
    QVERIFY(componentObject->setProperty("valueIndicatorText", "Label"));
    QCOMPARE(componentObject->property("valueIndicatorText").toString(),
             QString("Label"));
}

QTEST_MAIN(tst_quickcomponentsslider)

#include "tst_quickcomponentsslider.moc"
