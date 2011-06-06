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

    // Out of bound
    componentObject->setProperty("year", -2);
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
    QCOMPARE(componentObject->property("month").toInt(), 0);
}

void tst_datepickerdialog::month()
{
    QVERIFY(componentObject->property("month").isValid());
    QCOMPARE(componentObject->property("month").toInt(), 1);
    componentObject->setProperty("month", 5);
    QCOMPARE(componentObject->property("month").toInt(), 5);

    // Out of bound
    componentObject->setProperty("month", -2);
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
    QCOMPARE(componentObject->property("month").toInt(), 1);
    componentObject->setProperty("month", 15);
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
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
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
    QCOMPARE(componentObject->property("day").toInt(), 1);
    componentObject->setProperty("day", 40);
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
    QCOMPARE(componentObject->property("day").toInt(), 5);

    // Leap year
    componentObject->setProperty("month", 2);
    componentObject->setProperty("year", 2012);
    componentObject->setProperty("day", 31);
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
    QCOMPARE(componentObject->property("day").toInt(), 29);
}

void tst_datepickerdialog::minimumYear()
{
    QVERIFY(componentObject->property("minimumYear").isValid());
    componentObject->setProperty("minimumYear", 1990);
    QCOMPARE(componentObject->property("minimumYear").toInt(), 1990);

    // Out of bound
    componentObject->setProperty("minimumYear", 1990);
    componentObject->setProperty("year", 1980);
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
    QCOMPARE(componentObject->property("year").toInt(), 1990);
}

void tst_datepickerdialog::maximumYear()
{
    QVERIFY(componentObject->property("maximumYear").isValid());
    componentObject->setProperty("maximumYear", 2050);
    QCOMPARE(componentObject->property("maximumYear").toInt(), 2050);

    // Out of bound
    componentObject->setProperty("maximumYear", 2020);
    componentObject->setProperty("year", 2030);
    QEXPECT_FAIL("", "Will fix in Qt Components 1.1 release, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-764", Continue);
    QCOMPARE(componentObject->property("year").toInt(), 2020);
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
