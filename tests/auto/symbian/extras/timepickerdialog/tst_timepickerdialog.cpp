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

class tst_timepickerdialog : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void hour();
    void minute();
    void second();
    void hourMode();
    void fields();
    void acceptButtonText();
    void rejectButtonText();

private:
    QObject *componentObject;
};


void tst_timepickerdialog::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_timepickerdialog.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_timepickerdialog::hour()
{
    QVERIFY(componentObject->property("hour").isValid());
    QCOMPARE(componentObject->property("hour").toInt(), 0);
    componentObject->setProperty("hour", 11);
    QCOMPARE(componentObject->property("hour").toInt(), 11);

    //Out of bound
    componentObject->setProperty("hour", -6);
    QCOMPARE(componentObject->property("hour").toInt(), 0);
    componentObject->setProperty("hour", 30);
    QCOMPARE(componentObject->property("hour").toInt(), 23);
}

void tst_timepickerdialog::minute()
{
    QVERIFY(componentObject->property("minute").isValid());
    QCOMPARE(componentObject->property("minute").toInt(), 0);
    componentObject->setProperty("minute", 30);
    QCOMPARE(componentObject->property("minute").toInt(), 30);

    //Out of bound
    componentObject->setProperty("minute", -6);
    QCOMPARE(componentObject->property("minute").toInt(), 0);
    componentObject->setProperty("minute", 70);
    QCOMPARE(componentObject->property("minute").toInt(), 59);
}

void tst_timepickerdialog::second()
{
    QVERIFY(componentObject->property("second").isValid());
    QCOMPARE(componentObject->property("second").toInt(), 0);
    componentObject->setProperty("second", 30);
    QCOMPARE(componentObject->property("second").toInt(), 30);

    //Out of bound
    componentObject->setProperty("second", -6);
    QCOMPARE(componentObject->property("second").toInt(), 0);
    componentObject->setProperty("second", 70);
    QCOMPARE(componentObject->property("second").toInt(), 59);
}

void tst_timepickerdialog::hourMode()
{
    QVERIFY(componentObject->property("hourMode").isValid());
    componentObject->setProperty("hourMode", 1 /*DateTime.TwelveHours*/);
    QCOMPARE(componentObject->property("hourMode").toInt(), 1 /*DateTime.TwelveHours*/);
    componentObject->setProperty("hourMode", 2 /*DateTime.TwentyFourHours*/);
    QCOMPARE(componentObject->property("hourMode").toInt(), 2 /*DateTime.TwentyFourHours*/);
}

void tst_timepickerdialog::fields()
{
    QVERIFY(componentObject->property("fields").isValid());
    QCOMPARE(componentObject->property("fields").toInt(), 3 /*DateTime.Hours | DateTime.Minutes*/);
}

void tst_timepickerdialog::acceptButtonText()
{
    QVERIFY(componentObject->property("acceptButtonText").isValid());
    QCOMPARE(componentObject->property("acceptButtonText").toString(),QString(""));
    componentObject->setProperty("acceptButtonText","Ok");
    QCOMPARE(componentObject->property("acceptButtonText").toString(),QString("Ok"));
}

void tst_timepickerdialog::rejectButtonText()
{
    QVERIFY(componentObject->property("rejectButtonText").isValid());
    QCOMPARE(componentObject->property("rejectButtonText").toString(),QString(""));
    componentObject->setProperty("rejectButtonText","Cancel");
    QCOMPARE(componentObject->property("rejectButtonText").toString(),QString("Cancel"));
}

QTEST_MAIN(tst_timepickerdialog)

#include "tst_timepickerdialog.moc"
