/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qdatetimemodel.h"
#include "qdatetimemodel_p.h"

#include <math.h>
#include <QtCore/qdebug.h>
#include <QtCore/qlocale.h>

// ### FIX THIS INCLUDE ###
#include <QtCore/private/qdatetime_p.h>

/*!
  \internal
  Constructs a QDateTimeModelPrivate object
*/


QDateTimeModelPrivate::QDateTimeModelPrivate(QDateTimeModel *qq)
    : QDateTimeParser(QVariant::DateTime, QDateTimeParser::DateTimeEdit),
      q_ptr(qq)
{
    minimum = QDATETIMEEDIT_COMPAT_DATETIME_MIN;
    maximum = QDATETIMEEDIT_DATETIME_MAX;
}

void QDateTimeModelPrivate::setRange(const QDateTime &min, const QDateTime &max)
{
    minimum = min.toTimeSpec(spec);
    maximum = max.toTimeSpec(spec);
}

void QDateTimeModelPrivate::updateTimeSpec()
{
    minimum = minimum.toTimeSpec(spec);
    maximum = maximum.toTimeSpec(spec);
    value = value.toTimeSpec(spec);
}

QVariant QDateTimeModelPrivate::getZeroVariant() const
{
    return QDateTime(QDATETIMEEDIT_DATE_INITIAL, QTime(), spec);
}

// QDateTimeModel

QDateTimeModel::QDateTimeModel(QObject *parent)
    : QObject(parent), d_ptr(new QDateTimeModelPrivate(this))
{
}

QDateTimeModel::QDateTimeModel(QDateTimeModelPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

QDateTimeModel::~QDateTimeModel()
{
    delete d_ptr;
}

QDateTime QDateTimeModel::dateTime() const
{
    Q_D(const QDateTimeModel);
    return d->value;
}

void QDateTimeModel::setDateTime(const QDateTime &datetime)
{
    Q_D(QDateTimeModel);
    if (datetime.isValid() && datetime > d->minimum && datetime < d->maximum) {
        d->value.setDate(datetime.date());
        d->value.setTime(datetime.time());
        emit dateTimeChanged(d->value);
    }
}

QDate QDateTimeModel::date() const
{
    Q_D(const QDateTimeModel);
    return d->value.date();
}

void QDateTimeModel::setDate(const QDate &date)
{
    Q_D(QDateTimeModel);
    if (date.isValid() && date > d->minimum.date() && date < d->maximum.date()) {
        d->value.setDate(date);
        emit dateTimeChanged(d->value);
    }
}

QTime QDateTimeModel::time() const
{
    Q_D(const QDateTimeModel);
    return d->value.time();
}

void QDateTimeModel::setTime(const QTime &time)
{
    Q_D(QDateTimeModel);
    if (time.isValid() && time > d->minimum.time() && time < d->maximum.time()) {
        d->value.setTime(time);
        emit dateTimeChanged(d->value);
    }
}

int QDateTimeModel::weekNumber() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().weekNumber();
}

void QDateTimeModel::setWeekNumber(int week)
{
    Q_D(QDateTimeModel);
    int diff = week - d->value.date().weekNumber();
    addDays(7 * diff);
}

int QDateTimeModel::dayOfWeek() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().dayOfWeek();
}

int QDateTimeModel::dayOfYear() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().dayOfYear();
}

int QDateTimeModel::daysInMonth() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().daysInMonth();
}

int QDateTimeModel::daysInYear() const
{
    Q_D(const QDateTimeModel);
    return d->value.date().daysInYear();
}

QDateTime QDateTimeModel::toUTC() const
{
    Q_D(const QDateTimeModel);
    return d->value.toUTC();
}

QDate QDateTimeModel::firstDayOfWeek()
{
    Q_D(QDateTimeModel);
    QDate date(d->value.date());
    int dest = date.dayOfWeek() - 1;
    if (dest > 0) {
        return date.addDays(-1 * dest);
    }
    return date;
}

QString QDateTimeModel::longDayName() const
{
    return QDate::longDayName(dayOfWeek());
}

QString QDateTimeModel::longMonthName() const
{
    Q_D(const QDateTimeModel);
    return QDate::longMonthName(d->value.date().month());
}

QString QDateTimeModel::shortDayName() const
{
    return QDate::shortDayName(dayOfWeek());
}

QString QDateTimeModel::shortMonthName() const
{
    Q_D(const QDateTimeModel);
    return QDate::shortMonthName(d->value.date().month());
}

void QDateTimeModel::clearMinimumDateTime()
{
    setMinimumDateTime(QDateTime(QDATETIMEEDIT_DATE_MIN));
}

void QDateTimeModel::setMinimumDateTime(const QDateTime &dateTime)
{
    Q_D(QDateTimeModel);
    if (dateTime.isValid() && dateTime.date() >= QDATETIMEEDIT_DATE_MIN) {
        const QDateTime minimum = dateTime.toTimeSpec(d->spec);
        const QDateTime maximum = d->maximum;
        d->setRange(minimum, (maximum > minimum ? maximum : minimum));
    }
}

QDateTime QDateTimeModel::maximumDateTime() const
{
    Q_D(const QDateTimeModel);
    return d->maximum;
}

void QDateTimeModel::clearMaximumDateTime()
{
    setMaximumDateTime(QDATETIMEEDIT_DATETIME_MAX);
}

void QDateTimeModel::setMaximumDateTime(const QDateTime &dateTime)
{
    Q_D(QDateTimeModel);
    if (dateTime.isValid() && dateTime.date() <= QDATETIMEEDIT_DATE_MAX) {
        const QDateTime maximum = dateTime.toTimeSpec(d->spec);
        const QDateTime minimum = d->minimum;
        d->setRange((minimum < maximum ? minimum : maximum), maximum);
    }
}

void QDateTimeModel::setDateTimeRange(const QDateTime &min, const QDateTime &max)
{
    Q_D(QDateTimeModel);
    const QDateTime minimum = min.toTimeSpec(d->spec);
    QDateTime maximum = max.toTimeSpec(d->spec);
    if (min > max) {
        maximum = minimum;
    }
    d->setRange(minimum, maximum);
}

Qt::TimeSpec QDateTimeModel::timeSpec() const
{
    Q_D(const QDateTimeModel);
    return d->spec;
}

void QDateTimeModel::setTimeSpec(Qt::TimeSpec spec)
{
    Q_D(QDateTimeModel);
    if (spec != d->spec) {
        d->spec = spec;
        d->updateTimeSpec();
    }
}

QString QDateTimeModel::localeFormat() const
{
    const QLocale loc;
    return loc.dateTimeFormat(QLocale::ShortFormat);
}

QDateTime QDateTimeModel::currentTime() const
{
    return QDateTime::currentDateTime();
}

QDateTime QDateTimeModel::minimumDateTime() const
{
    Q_D(const QDateTimeModel);
    return d->minimum;
}

bool QDateTimeModel::addDays(int ndays)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addDays(ndays));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setDate(ndate.date());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addMonths(int nmonths)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addMonths(nmonths));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setDate(ndate.date());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addYears(int nyears)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addYears(nyears));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setDate(ndate.date());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addMSecs(qint64 ms)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addMSecs(ms));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setTime(ndate.time());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

bool QDateTimeModel::addSecs(int s)
{
    Q_D(QDateTimeModel);
    QDateTime ndate(d->value.addSecs(s));
    if (ndate > d->minimum && ndate < d->maximum) {
        d->value.setTime(ndate.time());
        emit dateTimeChanged(d->value);
        return true;
    }
    return false;
}

int QDateTimeModel::daysTo(const QDateTime &date) const
{
    Q_D(const QDateTimeModel);
    return d->value.daysTo(date);
}

int QDateTimeModel::secsTo(const QDateTime &time) const
{
    Q_D(const QDateTimeModel);
    return d->value.secsTo(time);
}
