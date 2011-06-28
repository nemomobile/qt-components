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


#include <qtest.h>
#include "tst_quickcomponentstest.h"

class tst_quickcomponentslistitem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void validateProperties();
    void testClickedSignal();
    void testPressAndHoldSignal();

private:
    QObject *m_componentObject;
};

void tst_quickcomponentslistitem::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentslistitem.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));
}

void tst_quickcomponentslistitem::validateProperties()
{
    // mode
    QVariant mode = m_componentObject->property("mode");
    QVERIFY(mode.isValid());
    QVERIFY(mode.canConvert(QVariant::String));
    QCOMPARE(mode.toString(), QString("normal"));

    // paddingItem
    QVERIFY(m_componentObject->property("paddingItem").isValid());

    // enabled
    QVariant enabled = m_componentObject->property("enabled");
    QVERIFY(enabled.isValid());
    QVERIFY(enabled.canConvert(QVariant::Bool));
    QCOMPARE(enabled.toBool(), true);
    m_componentObject->setProperty("enabled", false);
    enabled = m_componentObject->property("enabled");
    QCOMPARE(enabled.toBool(), false);

    // subItemIndicator
    QVariant indicator = m_componentObject->property("subItemIndicator");
    QVERIFY(indicator.isValid());
    QVERIFY(indicator.canConvert(QVariant::Bool));
    QCOMPARE(indicator.toBool(), false);
    m_componentObject->setProperty("subItemIndicator", true);
    indicator = m_componentObject->property("subItemIndicator");
    QCOMPARE(indicator.toBool(), true);

    // platformInverted
    QVariant inverted = m_componentObject->property("platformInverted");
    QVERIFY(inverted.isValid());
    QVERIFY(inverted.canConvert(QVariant::Bool));
    QCOMPARE(inverted.toBool(), false);
    m_componentObject->setProperty("platformInverted", true);
    inverted = m_componentObject->property("platformInverted");
    QCOMPARE(inverted.toBool(), true);
}

void tst_quickcomponentslistitem::testClickedSignal()
{
    QSignalSpy spy(m_componentObject, SIGNAL(clicked()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(m_componentObject, "clicked", Qt::DirectConnection);
    QCOMPARE(spy.count(), 1);
}

void tst_quickcomponentslistitem::testPressAndHoldSignal()
{
    QSignalSpy spy(m_componentObject, SIGNAL(pressAndHold()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(m_componentObject, "pressAndHold", Qt::DirectConnection);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(tst_quickcomponentslistitem)

#include "tst_quickcomponentslistitem.moc"
