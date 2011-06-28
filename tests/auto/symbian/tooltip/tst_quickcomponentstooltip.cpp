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

class tst_quickcomponentstooltip : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void text();
    void font();
    void target();
    void implicitSize();
    void platformInverted();

private:
    QObject *m_componentObject;
};

void tst_quickcomponentstooltip::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstooltip.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));
}

void tst_quickcomponentstooltip::text()
{
    QVERIFY(m_componentObject->property("text").isValid());
    QVERIFY(m_componentObject->property("text").toString().isEmpty());
    QVERIFY(m_componentObject->setProperty("text", "Test Text Change"));
    QCOMPARE(m_componentObject->property("text").toString(), QString("Test Text Change"));
}

void tst_quickcomponentstooltip::font()
{
    QVERIFY(m_componentObject->property("font").isValid());
    QVERIFY(m_componentObject->setProperty("font", "Nokia Sans"));
    QVERIFY(m_componentObject->property("font").toString().contains("Nokia Sans"));
    QVERIFY(m_componentObject->setProperty("font", "Helvetica"));
    QVERIFY(m_componentObject->property("font").toString().contains("Helvetica"));
}

void tst_quickcomponentstooltip::target()
{
    // TODO: how to test property variant?
    // test target with proper values and test tha calculatePosition
    // changes items x.y accordingly
    QEXPECT_FAIL("", "Test not done", Continue);
}

void tst_quickcomponentstooltip::implicitSize()
{
    QVERIFY(m_componentObject->property("implicitWidth").isValid());
    QVERIFY(m_componentObject->property("implicitHeight").isValid());
    int implicitWidth = m_componentObject->property("implicitWidth").toInt();
    // Test that a change to the text resize the implicit width
    QVERIFY(m_componentObject->setProperty("text", m_componentObject->property("text").toString()+"ImplicitSizeTest"));
#ifdef Q_OS_SYMBIAN
    QVERIFY(m_componentObject->property("implicitWidth").toInt() > implicitWidth);
#else
    QEXPECT_FAIL("", "screen.width=0 on desktop", Continue);
    QVERIFY(m_componentObject->property("implicitWidth").toInt() > implicitWidth);
#endif
    //TODO: Add richtext test
    //TODO: Add fontsize test
}

void tst_quickcomponentstooltip::platformInverted()
{
    QVERIFY(m_componentObject->property("platformInverted").isValid());
    QCOMPARE(m_componentObject->property("platformInverted").toBool(), false);
    m_componentObject->setProperty("platformInverted", QVariant(true));
    QCOMPARE(m_componentObject->property("platformInverted").toBool(), true);
}

QTEST_MAIN(tst_quickcomponentstooltip)

#include "tst_quickcomponentstooltip.moc"

