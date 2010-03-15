/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdatetimemodel.h"
#include "qdatetimemodel_p.h"
#include <math.h>
#include <qdebug.h>
#include <qlocale.h>

QDateTimeModel::QDateTimeModel(QObject *parent)
    : QObject(new QDateTimeModelPrivate, parent)
{
    Q_D(QDateTimeModel);
    d->init(QDateTime(QDateTimeModel_DATE_INITIAL, QDateTimeModel_TIME_MIN));
}

/*!
  \internal
*/

QDateTimeModel::QDateTimeModel(const QVariant &variant, QVariant::Type parserType, QObject *parent)
    : QObject(*new QDateTimeModelPrivate, parent)
{
    Q_D(QDateTimeModel);
    d->parserType = parserType;
    d->init(variant);
}

/*!
  \property QDateTimeModel::dateTime
  \brief the QDateTime that is set in the QDateTimeModel

  When setting this property the timespec of the QDateTimeModel remains the same
  and the timespec of the new QDateTime is ignored.

  By default, this property contains a date that refers to January 1,
  2000 and a time of 00:00:00 and 0 milliseconds.

  \sa date, time
*/

QDateTime QDateTimeModel::dateTime() const
{
    Q_D(const QDateTimeModel);
    return d->value.toDateTime();
}

void QDateTimeModel::setDateTime(const QDateTime &datetime)
{
    Q_D(QDateTimeModel);
    if (datetime.isValid()) {
        d->clearCache();
        if (!(d->sections & DateSections_Mask))
            setDateRange(datetime.date(), datetime.date());
        d->setValue(QDateTime(datetime.date(), datetime.time(), d->spec), EmitIfChanged);
    }
}

/*!
  \property QDateTimeModel::minimumDateTime

  \brief the minimum datetime of the date time control

  When setting this property the \l maximumDateTime() is adjusted if
  necessary to ensure that the range remains valid. If the datetime is
  not a valid QDateTime object, this function does nothing.

  The default minimumDateTime can be restored with
  clearMinimumDateTime()

  By default, this property contains a date that refers to September 14,
  1752 and a time of 00:00:00 and 0 milliseconds.

  \sa maximumDateTime(), minimumTime(), maximumTime(), minimumDate(),
  maximumDate(), setDateTimeRange(), setDateRange(), setTimeRange(),
  clearMaximumDateTime(), clearMinimumDate(),
  clearMaximumDate(), clearMinimumTime(), clearMaximumTime()
*/

QDateTime QDateTimeModel::minimumDateTime() const
{
    Q_D(const QDateTimeModel);
    return d->minimum.toDateTime();
}

void QDateTimeModel::clearMinimumDateTime()
{
    setMinimumDateTime(QDateTime(QDateTimeModel_DATE_MIN, QDateTimeModel_TIME_MIN));
}

void QDateTimeModel::setMinimumDateTime(const QDateTime &dateTime)
{
    Q_D(QDateTimeModel);
    if (dateTime.isValid() && dateTime.date() >= QDateTimeModel_DATE_MIN) {
        const QDateTime minumum = dateTime.toTimeSpec(d->spec);
        const QDateTime maximum = d->maximum.toDateTime();
        d->setRange(minimum, (maximum > minimum ? maximum : minimum));
    }
}

/*!
  \property QDateTimeEdit::maximumDateTime

  \brief the maximum datetime of the date time control

  When setting this property the \l minimumDateTime() is adjusted if
  necessary to ensure that the range remains valid. If the datetime is
  not a valid QDateTime object, this function does nothing.

  The default maximumDateTime can be restored with
  clearMaximumDateTime().

  By default, this property contains a date that refers to 31 December,
  7999 and a time of 23:59:59 and 999 milliseconds.

  \sa minimumDateTime(), minimumTime(), maximumTime(), minimumDate(),
  maximumDate(), setDateTimeRange(), setDateRange(), setTimeRange(),
  clearMinimumDateTime(), clearMinimumDate(),
  clearMaximumDate(), clearMinimumTime(), clearMaximumTime()
*/

QDateTime QDateTimeModel::maximumDateTime() const
{
    Q_D(const QDateTimeModel);
    return d->maximum.toDateTime();
}

void QDateTimeModel::clearMaximumDateTime()
{
    setMaximumDateTime(QDateTimeModel_DATETIME_MAX);
}

void QDateTimeModel::setMaximumDateTime(const QDateTime &dateTime)
{
    Q_D(QDateTimeModel);
    if (dateTime.isValid() && dateTime.date() <= QDateTimeModel_DATE_MAX) {
        const QDateTime maximum = dateTime.toTimeSpec(d->spec);
        const QDateTime minimum = d->minimum.toDateTime();
        d->setRange((minimum < maximum ? minimum : maximum), maximum);
    }
}


/*!
  Convenience function to set minimum and maximum date time with one
  function call.

  If either \a min or \a max are not valid, this function does
  nothing.

  \sa setMinimumDate(), maximumDate(), setMaximumDate(),
  clearMinimumDate(), setMinimumTime(), maximumTime(),
  setMaximumTime(), clearMinimumTime(), QDateTime::isValid()
*/

void QDateTimeModel::setDateTimeRange(const QDateTime &min, const QDateTime &max)
{
    Q_D(QDateTimeModel);
    const QDateTime minimum = min.toTimeSpec(d->spec);
    QDateTime maximum = max.toTimeSpec(d->spec);
    if (min > max)
        maximum = minimum;
    d->setRange(minimum, maximum);
}

/*!
  \property QDateTimeModel::displayFormat

  \brief the format used to display the time/date of the date time edit

  This format is the same as the one used described in QDateTime::toString()
  and QDateTime::fromString()

  Example format strings(assuming that the date is 2nd of July 1969):

  \table
  \header \i Format \i Result
  \row \i dd.MM.yyyy    \i 02.07.1969
  \row \i MMM d yy \i Jul 2 69
  \row \i MMMM d yy \i July 2 69
  \endtable

  Note that if you specify a two digit year, it will be interpreted
  to be in the century in which the date time edit was initialized.
  The default century is the 21 (2000-2099).

  If you specify an invalid format the format will not be set.

  \sa QDateTime::toString(), displayedSections()
*/

QString QDateTimeModel::displayFormat() const
{
    Q_D(const QDateTimeModel);
    return isRightToLeft() ? d->unreversedFormat : d->displayFormat;
}

template<typename C> static inline C reverse(const C &l)
{
    C ret;
    for (int i=l.size() - 1; i>=0; --i)
        ret.append(l.at(i));
    return ret;
}

void QDateTimeModel::setDisplayFormat(const QString &format)
{
    Q_D(QDateTimeModel);
    if (d->parseFormat(format)) {
        d->unreversedFormat.clear();
        if (isRightToLeft()) {
            d->unreversedFormat = format;
            d->displayFormat.clear();
            for (int i=d->sectionNodes.size() - 1; i>=0; --i) {
                d->displayFormat += d->separators.at(i + 1);
                d->displayFormat += d->sectionFormat(i);
            }
            d->displayFormat += d->separators.at(0);
            d->separators = reverse(d->separators);
            d->sectionNodes = reverse(d->sectionNodes);
        }

        d->formatExplicitlySet = true;
        d->sections = d->convertSections(d->display);
        d->clearCache();

        d->currentSectionIndex = qMin(d->currentSectionIndex, d->sectionNodes.size() - 1);
        const bool timeShown = (d->sections & TimeSections_Mask);
        const bool dateShown = (d->sections & DateSections_Mask);
        Q_ASSERT(dateShown || timeShown);
        if (timeShown && !dateShown) {
            setDateRange(d->value.toDate(), d->value.toDate());
        } else if (dateShown && !timeShown) {
            setTimeRange(QDateTimeModel_TIME_MIN, QDateTimeModel_TIME_MAX);
            d->value = QDateTime(d->value.toDate(), QTime(), d->spec);
        }
    }
}

/*!
    \property QDateTimeModel::timeSpec
    \brief the current timespec used by the date time edit.
*/

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

/*!
  \reimp
*/

QValidator::State QDateTimeModel::validate(QString &text, int &pos) const
{
    Q_D(const QDateTimeModel);
    QValidator::State state;
    d->validateAndInterpret(text, pos, state);
    return state;
}

/*!
  \reimp
*/

void QDateTimeEdit::fixup(QString &input) const
{
    Q_D(const QDateTimeEdit);
    QValidator::State state;
    int copy = d->edit->cursorPosition();

    d->validateAndInterpret(input, copy, state, true);
}

// --- QDateTimeModelPrivate ---

/*!
  \internal
  Constructs a QDateTimeModelPrivate object
*/


QDateTimeModelPrivate::QDateTimeModelPrivate()
    : QDateTimeParser(QVariant::DateTime, QDateTimeParser::DateTimeControl)
{
    formatExplicitlySet = false;
    cacheGuard = false;
    fixday = true;
    type = QVariant::DateTime;
    cachedDay = -1;

    minimum = QDateTimeModel_COMPAT_DATETIME_MIN;
    maximum = QDateTimeModel_DATETIME_MAX;

    readLocaleSettings();
}

void QDateTimeModelPrivate::updateTimeSpec()
{
    minimum = minimum.toDateTime().toTimeSpec(spec);
    maximum = maximum.toDateTime().toTimeSpec(spec);
    value = value.toDateTime().toTimeSpec(spec);
}

/*!
  \internal

  updates the cached values
*/

void QDateTimeModelPrivate::updateCache(const QVariant &val, const QString &str) const
{
    if (val != cachedValue || str != cachedText || cacheGuard) {
        cacheGuard = true;
        QString copy = str;
        int unused = edit->cursorPosition();
        QValidator::State unusedState;
        validateAndInterpret(copy, unused, unusedState);
        cacheGuard = false;
    }
}

/*!
  \internal

  parses and validates \a input
*/

QDateTime QDateTimeModelPrivate::validateAndInterpret(QString &input, int &position,
                                                        QValidator::State &state, bool fixup) const
{
    if (input.isEmpty()) {
        if (sectionNodes.size() == 1 || !specialValueText.isEmpty()) {
            state = QValidator::Intermediate;
        } else {
            state = QValidator::Invalid;
        }
        return getZeroVariant().toDateTime();
    } else if (cachedText == input && !fixup) {
        state = cachedState;
        return cachedValue.toDateTime();
    } else if (!specialValueText.isEmpty()) {
        bool changeCase = false;
        const int max = qMin(specialValueText.size(), input.size());
        int i;
        for (i=0; i<max; ++i) {
            const QChar ic = input.at(i);
            const QChar sc = specialValueText.at(i);
            if (ic != sc) {
                if (sc.toLower() == ic.toLower()) {
                    changeCase = true;
                } else {
                    break;
                }
            }
        }
        if (i == max) {
            state = specialValueText.size() == input.size() ? QValidator::Acceptable : QValidator::Intermediate;
            if (changeCase) {
                input = specialValueText.left(max);
            }
            return minimum.toDateTime();
        }
    }
    StateNode tmp = parse(input, position, value.toDateTime(), fixup);
    input = tmp.input;
    state = QValidator::State(int(tmp.state));
    if (state == QValidator::Acceptable) {
        if (tmp.conflicts && conflictGuard != tmp.value) {
            conflictGuard = tmp.value;
            clearCache();
            input = textFromValue(tmp.value);
            updateCache(tmp.value, input);
            conflictGuard.clear();
        } else {
            cachedText = input;
            cachedState = state;
            cachedValue = tmp.value;
        }
    } else {
        clearCache();
    }
    return (tmp.value.isNull() ? getZeroVariant().toDateTime() : tmp.value);
}

/*!
  \internal

  Internal function called by QDateTimeModel::stepBy(). Also takes a
  Section for which section to step on and a bool \a test for
  whether or not to modify the internal cachedDay variable. This is
  necessary because the function is called from the const function
  QDateTimeModel::stepEnabled() as well as QDateTimeModel::stepBy().
*/

QDateTime QDateTimeModelPrivate::stepBy(int sectionIndex, int steps, bool test) const
{
    Q_Q(const QDateTimeModel);
    QDateTime v = value.toDateTime();
    QString str = displayText();
    int pos = edit->cursorPosition();
    const SectionNode sn = sectionNode(sectionIndex);

    int val;
    // to make sure it behaves reasonably when typing something and then stepping in non-tracking mode
    if (!test && pendingEmit) {
        if (q->validate(str, pos) != QValidator::Acceptable) {
            v = value.toDateTime();
        } else {
            v = q->dateTimeFromText(str);
        }
        val = getDigit(v, sectionIndex);
    } else {
        val = getDigit(v, sectionIndex);
    }

    val += steps;

    const int min = absoluteMin(sectionIndex);
    const int max = absoluteMax(sectionIndex, value.toDateTime());

    if (val < min) {
        val = (wrapping ? max - (min - val) + 1 : min);
    } else if (val > max) {
        val = (wrapping ? min + val - max - 1 : max);
    }


    const int oldDay = v.date().day();

    setDigit(v, sectionIndex, val);
    // if this sets year or month it will make
    // sure that days are lowered if needed.

    const QDateTime minimumDateTime = minimum.toDateTime();
    const QDateTime maximumDateTime = maximum.toDateTime();
    // changing one section should only modify that section, if possible
    if (sn.type != AmPmSection && (v < minimumDateTime || v > maximumDateTime)) {
        const int localmin = getDigit(minimumDateTime, sectionIndex);
        const int localmax = getDigit(maximumDateTime, sectionIndex);

        if (wrapping) {
            // just because we hit the roof in one direction, it
            // doesn't mean that we hit the floor in the other
            if (steps > 0) {
                setDigit(v, sectionIndex, min);
                if (!(sn.type & (DaySection|DayOfWeekSection)) && sections & DateSectionMask) {
                    const int daysInMonth = v.date().daysInMonth();
                    if (v.date().day() < oldDay && v.date().day() < daysInMonth) {
                        const int adds = qMin(oldDay, daysInMonth);
                        v = v.addDays(adds - v.date().day());
                    }
                }

                if (v < minimumDateTime) {
                    setDigit(v, sectionIndex, localmin);
                    if (v < minimumDateTime)
                        setDigit(v, sectionIndex, localmin + 1);
                }
            } else {
                setDigit(v, sectionIndex, max);
                if (!(sn.type & (DaySection|DayOfWeekSection)) && sections & DateSectionMask) {
                    const int daysInMonth = v.date().daysInMonth();
                    if (v.date().day() < oldDay && v.date().day() < daysInMonth) {
                        const int adds = qMin(oldDay, daysInMonth);
                        v = v.addDays(adds - v.date().day());
                    }
                }

                if (v > maximumDateTime) {
                    setDigit(v, sectionIndex, localmax);
                    if (v > maximumDateTime)
                        setDigit(v, sectionIndex, localmax - 1);
                }
            }
        } else {
            setDigit(v, sectionIndex, (steps > 0 ? localmax : localmin));
        }
    }
    if (!test && oldDay != v.date().day() && !(sn.type & (DaySection|DayOfWeekSection))) {
        // this should not happen when called from stepEnabled
        cachedDay = qMax<int>(oldDay, cachedDay);
    }

    if (v < minimumDateTime) {
        if (wrapping) {
            QDateTime t = v;
            setDigit(t, sectionIndex, steps < 0 ? max : min);
            bool mincmp = (t >= minimumDateTime);
            bool maxcmp = (t <= maximumDateTime);
            if (!mincmp || !maxcmp) {
                setDigit(t, sectionIndex, getDigit(steps < 0
                                                   ? maximumDateTime
                                                   : minimumDateTime, sectionIndex));
                mincmp = (t >= minimumDateTime);
                maxcmp = (t <= maximumDateTime);
            }
            if (mincmp && maxcmp) {
                v = t;
            }
        } else {
            v = value.toDateTime();
        }
    } else if (v > maximumDateTime) {
        if (wrapping) {
            QDateTime t = v;
            setDigit(t, sectionIndex, steps > 0 ? min : max);
            bool mincmp = (t >= minimumDateTime);
            bool maxcmp = (t <= maximumDateTime);
            if (!mincmp || !maxcmp) {
                setDigit(t, sectionIndex, getDigit(steps > 0 ?
                                                   minimumDateTime :
                                                   maximumDateTime, sectionIndex));
                mincmp = (t >= minimumDateTime);
                maxcmp = (t <= maximumDateTime);
            }
            if (mincmp && maxcmp) {
                v = t;
            }
        } else {
            v = value.toDateTime();
        }
    }

    const QDateTime ret = bound(v, value, steps).toDateTime().toTimeSpec(spec);
    return ret;
}

/*!
  \internal
*/

void QDateTimeModelPrivate::emitSignals(EmitPolicy ep, const QVariant &old)
{
    Q_Q(QDateTimeModel);
    if (ep == NeverEmit) {
        return;
    }
    pendingEmit = false;

    const bool dodate = value.toDate().isValid() && (sections & DateSectionMask);
    const bool datechanged = (ep == AlwaysEmit || old.toDate() != value.toDate());
    const bool dotime = value.toTime().isValid() && (sections & TimeSectionMask);
    const bool timechanged = (ep == AlwaysEmit || old.toTime() != value.toTime());

    updateCache(value, displayText());

    if (datechanged || timechanged)
        emit q->dateTimeChanged(value.toDateTime());
    if (dodate && datechanged)
        emit q->dateChanged(value.toDate());
    if (dotime && timechanged)
        emit q->timeChanged(value.toTime());

}

/*!
  \internal

  Try to get the format from the local settings
*/
void QDateTimeModelPrivate::readLocaleSettings()
{
    const QLocale loc;
    defaultTimeFormat = loc.timeFormat(QLocale::ShortFormat);
    defaultDateFormat = loc.dateFormat(QLocale::ShortFormat);
    defaultDateTimeFormat = loc.dateTimeFormat(QLocale::ShortFormat);
}

QString QDateTimeModelPrivate::getAmPmText(AmPm ap, Case cs) const
{
    if (ap == AmText) {
        return (cs == UpperCase ? QDateTimeEdit::tr("AM") : QDateTimeEdit::tr("am"));
    } else {
        return (cs == UpperCase ? QDateTimeEdit::tr("PM") : QDateTimeEdit::tr("pm"));
    }
}

void QDateTimeModelPrivate::interpret(EmitPolicy ep)
{
    Q_Q(QDateTimeModel);
    QString tmp = displayText(); // ###
    int pos = edit->cursorPosition(); // ###
    const QValidator::State state = q->validate(tmp, pos);
    if (state != QValidator::Acceptable
        && correctionMode == QAbstractSpinBox::CorrectToPreviousValue // ###
        && (state == QValidator::Invalid || !(fieldInfo(currentSectionIndex) & AllowPartial))) {
        setValue(value, ep);
        updateTimeSpec();
    } else {
        QAbstractSpinBoxPrivate::interpret(ep);
    }
}

void QDateTimeModelPrivate::init(const QVariant &variant)
{
    Q_Q(QDateTimeModel);
    switch (variant.type()) {
    case QVariant::Date:
        value = QDateTime(variant.toDate(), QDateTimeModel_TIME_MIN);
        q->setDisplayFormat(defaultDateFormat);
        if (sectionNodes.isEmpty()) // ### safeguard for broken locale
            q->setDisplayFormat(QLatin1String("dd/MM/yyyy"));
        break;
    case QVariant::DateTime:
        value = variant;
        q->setDisplayFormat(defaultDateTimeFormat);
        if (sectionNodes.isEmpty()) // ### safeguard for broken locale
            q->setDisplayFormat(QLatin1String("dd/MM/yyyy hh:mm:ss"));
        break;
    case QVariant::Time:
        value = QDateTime(QDateTimeModel_DATE_INITIAL, variant.toTime());
        q->setDisplayFormat(defaultTimeFormat);
        if (sectionNodes.isEmpty()) // ### safeguard for broken locale
            q->setDisplayFormat(QLatin1String("hh:mm:ss"));
        break;
    default:
        Q_ASSERT_X(0, "QDateTimeModelPrivate::init", "Internal error");
        break;
    }
    updateTimeSpec();
}

QVariant QDateTimeModelPrivate::getZeroVariant() const
{
    Q_ASSERT(type == QVariant::DateTime);
    return QDateTime(QDateTimeModel_DATE_INITIAL, QTime(), spec);
}

#include "moc_qdatetimemodel.cpp"
