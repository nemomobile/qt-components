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
#include "model/qdatetimemodel.h"

// ### FIX THIS INCLUDE ###
#include <QtCore/private/qdatetime_p.h>

class tst_QDateTimeModel : public QObject
{
    Q_OBJECT

public:
    tst_QDateTimeModel();
    virtual ~tst_QDateTimeModel();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void getSetCheck();

protected:
    QDateTimeModel *model;
};

tst_QDateTimeModel::tst_QDateTimeModel()
{
}

tst_QDateTimeModel::~tst_QDateTimeModel()
{
}

void tst_QDateTimeModel::initTestCase()
{
}

void tst_QDateTimeModel::cleanupTestCase()
{
}

void tst_QDateTimeModel::init()
{
    model = new QDateTimeModel();
}

void tst_QDateTimeModel::cleanup()
{
    delete model;
}

void tst_QDateTimeModel::getSetCheck()
{
    // initial state
    QCOMPARE(model->date(), QDate());
    QCOMPARE(model->time(), QTime());
    QCOMPARE(model->dateTime(), QDateTime());
    QCOMPARE(model->weekNumber(), 0);
    QCOMPARE(model->dayOfWeek(), 1);
    QCOMPARE(model->dayOfYear(), 1);
    QCOMPARE(model->daysInMonth(), 31);
    QCOMPARE(model->daysInYear(), 366);
    QCOMPARE(model->toUTC(), QDateTime());
    QCOMPARE(model->firstDayOfWeek(), QDate());
    QCOMPARE(model->longDayName(), QString("Monday"));
    QCOMPARE(model->longMonthName(), QString("January"));
    QCOMPARE(model->shortDayName(), QString("Mon"));
    QCOMPARE(model->shortMonthName(), QString("Jan"));
    QCOMPARE(model->maximumDateTime(), QDATETIMEEDIT_DATETIME_MAX);
    QCOMPARE(model->minimumDateTime(), QDATETIMEEDIT_COMPAT_DATETIME_MIN);
    QCOMPARE(model->timeSpec(), Qt::LocalTime);

    // date
    QCOMPARE(model->date(), QDate());
    model->setDate(QDate(1999, 12, 30));
    QCOMPARE(model->date(), QDate(1999, 12, 30));

    // time
    QCOMPARE(model->time(), QTime());
    model->setTime(QTime(23, 59, 59, 999));
    QCOMPARE(model->time(), QTime(23, 59, 59, 999));

    // dateTime
    QCOMPARE(model->dateTime(), QDateTime(QDate(1999, 12, 30), QTime(23, 59, 59, 999)));
}

QTEST_MAIN(tst_QDateTimeModel)
#include "tst_qdatetimemodel.moc"
