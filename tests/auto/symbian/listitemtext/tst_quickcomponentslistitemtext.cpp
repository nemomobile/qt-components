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

class tst_quickcomponentslistitemtext : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void validateProperties();

private:
    QObject *m_componentObject;
};

void tst_quickcomponentslistitemtext::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentslistitemtext.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));
}

void tst_quickcomponentslistitemtext::validateProperties()
{
    // mode
    QVariant mode = m_componentObject->property("mode");
    QVERIFY(mode.isValid());
    QVERIFY(mode.canConvert(QVariant::String));
    QCOMPARE(mode.toString(), QString("normal"));
    m_componentObject->setProperty("mode", "pressed");
    mode = m_componentObject->property("mode");
    QCOMPARE(mode.toString(), QString("pressed"));

    // role
    QVariant role = m_componentObject->property("role");
    QVERIFY(role.isValid());
    QVERIFY(role.canConvert(QVariant::String));
    QCOMPARE(role.toString(), QString("Title"));
    m_componentObject->setProperty("role", "SubTitle");
    role = m_componentObject->property("role");
    QCOMPARE(role.toString(), QString("SubTitle"));
    m_componentObject->setProperty("role", "Heading");
    role = m_componentObject->property("role");
    QCOMPARE(role.toString(), QString("Heading"));

    // text
    QVariant text = m_componentObject->property("text");
    QVERIFY(text.isValid());
    QVERIFY(text.canConvert(QVariant::String));
    QVERIFY(text.toString().isEmpty());
    m_componentObject->setProperty("text", "My text");
    text = m_componentObject->property("text");
    QCOMPARE(text.toString(), QString("My text"));

    // platformInverted
    QVariant inverted = m_componentObject->property("platformInverted");
    QVERIFY(inverted.isValid());
    QVERIFY(inverted.canConvert(QVariant::Bool));
    QCOMPARE(inverted.toBool(), false);
    m_componentObject->setProperty("platformInverted", true);
    inverted = m_componentObject->property("platformInverted");
    QCOMPARE(inverted.toBool(), true);
}

QTEST_MAIN(tst_quickcomponentslistitemtext)
#include "tst_quickcomponentslistitemtext.moc"
