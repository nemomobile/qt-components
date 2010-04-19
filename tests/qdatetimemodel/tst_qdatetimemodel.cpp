/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtTest/QtTest>
#include "qdatetimemodel.h"

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
    //QCOMPARE(model->maximumDateTime(), QDATETIMEEDIT_DATETIME_MAX);
    //QCOMPARE(model->minimumDateTime(), QDATETIMEEDIT_DATETIME_MIN);
    QCOMPARE(model->timeSpec(), Qt::LocalTime);

}

QTEST_MAIN(tst_QDateTimeModel)
#include "tst_qdatetimemodel.moc"
