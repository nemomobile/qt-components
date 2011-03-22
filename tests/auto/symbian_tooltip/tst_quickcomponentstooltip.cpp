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

QTEST_MAIN(tst_quickcomponentstooltip)

#include "tst_quickcomponentstooltip.moc"

