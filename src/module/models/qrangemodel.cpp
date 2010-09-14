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

#include <QEvent>
#include <QApplication>
#include <QGraphicsSceneEvent>
#include <QDebug>

#ifndef QT_NO_ACCESSIBILITY
#include <QAccessible>
#endif

#include "qrangemodel.h"
#include "qrangemodel_p.h"

QRangeModelPrivate::QRangeModelPrivate(QRangeModel *qq)
    : q_ptr(qq)
{
}

QRangeModelPrivate::~QRangeModelPrivate()
{
}

void QRangeModelPrivate::init()
{
    isSedated = false;
    valueOrPosIsValue = true;
    minimum = 0;
    maximum = 99;
    singleStep = 1;
    pageStep = 10;
    steps = 0;
    valueOrPos = 0;
    posatmin = 0;
    posatmax = 0;
    tracking = true;
    inverted = false;
}

QRangeModel::QRangeModel(QObject *parent)
    : QObject(parent), d_ptr(new QRangeModelPrivate(this))
{
    Q_D(QRangeModel);
    d->init();
}

QRangeModel::QRangeModel(QRangeModelPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(QRangeModel);
    d->init();
}

QRangeModel::~QRangeModel()
{
    delete d_ptr;
    d_ptr = 0;
}

void QRangeModel::sedate()
{
    Q_D(QRangeModel);
    if (d->isSedated)
        return;

    d->isSedated = true;
    d->signalsBlocked = blockSignals(true);
}

void QRangeModel::awake()
{
    Q_D(QRangeModel);
    if (!d->isSedated)
        return;

    d->isSedated = false;
    if (!d->signalsBlocked)
        blockSignals(false);
}

void QRangeModelPrivate::emitValueAndPositionIfChanged(const qreal oldValue, const qreal oldPosition)
{
    Q_Q(QRangeModel);

    // Effective value and position might have changed even in cases when e.g. d->value is
    // unchanged. This will be the case when operating with values outside range:
    const qreal newValue = q->value();
    const qreal newPosition = q->position();
    if (newValue != oldValue)
        emit q->valueChanged(newValue);
    if (newPosition != oldPosition)
        emit q->positionChanged(newPosition);
}

void QRangeModel::setPositionRange(qreal min, qreal max)
{
    Q_D(QRangeModel);

    if ((min == d->posatmin) && (max == d->posatmax))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();
    const qreal oldMinimum = d->posatmin;
    d->posatmin = min;
    d->posatmax = max;

    if (d->posatmin != oldMinimum && !d->valueOrPosIsValue) {
        // Since d->pos is relative (meaning it will always be >= 0), it
        // needs to change when changing minimum so that absolute value (minimum + pos)
        // remains unchanged:
        d->valueOrPos += oldMinimum - d->minimum;
    }

    emit positionRangeChanged(d->posatmin, d->posatmax);
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setRange(qreal min, qreal max)
{
    Q_D(QRangeModel);

    if ((min == d->minimum) && (max == d->maximum))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();
    const qreal oldMinimum = d->minimum;
    d->minimum = min;
    d->maximum = qMax(min, max);

    if (d->minimum != oldMinimum && d->valueOrPosIsValue) {
        // Since d->value is relative (meaning it will always be >= 0), it
        // needs to change when changing minimum so that absolute value (minimum + value)
        // remains unchanged:
        d->valueOrPos += oldMinimum - d->minimum;
    }

    emit rangeChanged(d->minimum, d->maximum);
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setMinimum(qreal min)
{
    Q_D(const QRangeModel);
    setRange(min, d->maximum);
}

qreal QRangeModel::minimum() const
{
    Q_D(const QRangeModel);
    return d->minimum;
}

void QRangeModel::setMaximum(qreal max)
{
    Q_D(const QRangeModel);
    // if the new maximum value is smaller than
    // minimum, update minimum too
    setRange(qMin(d->minimum, max), max);
}

qreal QRangeModel::maximum() const
{
    Q_D(const QRangeModel);
    return d->maximum;
}


void QRangeModel::setSingleStep(qreal step)
{
    Q_D(QRangeModel);
    const qreal newStep = qAbs(step);
    if (newStep != d->singleStep)
        d->singleStep = newStep;
}

qreal QRangeModel::singleStep() const
{
    Q_D(const QRangeModel);
    return d->singleStep;
}

void QRangeModel::setPageStep(qreal step)
{
    Q_D(QRangeModel);
    const qreal newPageStep = qAbs(step);
    if (step != d->pageStep)
        d->pageStep = newPageStep;
}

qreal QRangeModel::pageStep() const
{
    Q_D(const QRangeModel);
    return d->pageStep;
}

void QRangeModel::setSteps(qreal steps)
{
    Q_D(QRangeModel);
    if (steps == d->steps)
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();
    d->steps = steps;

    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

qreal QRangeModel::steps() const
{
    Q_D(const QRangeModel);
    return d->steps;
}

void QRangeModel::setTracking(bool enable)
{
    Q_D(QRangeModel);
    d->tracking = enable;
}

bool QRangeModel::isTracking() const
{
    Q_D(const QRangeModel);
    return d->tracking;
}

qreal QRangeModel::position() const
{
    Q_D(const QRangeModel);

    // This must return the absolute position based on the internal
    // one, that is relative. so we need to add the start position
    // and also bound it to d->posatmin and d->posatmax

    // It is also important to do value-within-range check this
    // late (as opposed to during setPosition()). The reason is
    // QML bindings; a position that is initially invalid because it lays
    // outside the range, might become valid later if the range changes.

    qreal pos = d->posatmin + (d->valueOrPosIsValue ? d->positionFromValue(d->valueOrPos) : d->valueOrPos);
    const qreal s = d->positionFromValue(d->steps);
    if (s > 0)
        pos = s * qRound(pos/s);
    if (d->posatmin > d->posatmax)
        return qBound(d->posatmax, pos, d->posatmin);
    return qBound(d->posatmin, pos, d->posatmax);
}

void QRangeModel::setPosition(qreal newPosition)
{
    Q_D(QRangeModel);
    if (d->valueOrPosIsValue) {
        if (qFuzzyCompare(newPosition, d->positionFromValue(d->valueOrPos)))
            return;
    } else {
        if (qFuzzyCompare(newPosition, d->valueOrPos))
            return;
    }

    // Flag that we need to translate from position
    // to value upon calls to value():
    d->valueOrPosIsValue = false;
    const qreal oldValue = value();
    const qreal oldPosition = position();

    // 'pos' came as an absolute position and we want to store it
    // as a relative position, so we should subtract the start value
    d->valueOrPos = newPosition - d->posatmin;
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setPositionAtMinimum(qreal min)
{
    Q_D(QRangeModel);
    setPositionRange(min, d->posatmax);
}

qreal QRangeModel::positionAtMinimum() const
{
    Q_D(const QRangeModel);
    return d->posatmin;
}

void QRangeModel::setPositionAtMaximum(qreal max)
{
    Q_D(QRangeModel);
    setPositionRange(d->posatmin, max);
}

qreal QRangeModel::positionAtMaximum() const
{
    Q_D(const QRangeModel);
    return d->posatmax;
}

qreal QRangeModel::value() const
{
    Q_D(const QRangeModel);

    // this must return the absolute value based on the internal
    // one, that is relative. so we need to add the start value
    // considering if the value is 'inverted' or not and also bound
    // it to d->minimum and d->maximum.

    // It is also important to do value-within-range check this
    // late (as opposed to during setPosition()). The reason is
    // QML bindings; a position that is initially invalid because it lays
    // outside the range, might become valid later if the range changes.

    qreal value = d->valueOrPosIsValue ? d->valueOrPos : d->valueFromPosition(d->valueOrPos);
    value = d->inverted ? d->maximum - value : value + d->minimum;
    if (d->steps > 0)
        value = d->steps * qRound(value/d->steps);
    return qBound(d->minimum, value, d->maximum);
}

void QRangeModel::setValue(qreal newValue)
{
    Q_D(QRangeModel);
    // Flag that we need to translate from value
    // to position upon calls to position():
    if (d->valueOrPosIsValue) {
        if (qFuzzyCompare(newValue, d->valueOrPos))
            return;
    } else {
        if (qFuzzyCompare(newValue, d->valueFromPosition(d->valueOrPos)))
            return;
    }

    d->valueOrPosIsValue = true;
    const qreal oldValue = value();
    const qreal oldPosition = position();

    // 'value' came as an absolute value and we want to store it
    // as a relative value, so we should subtract/add the start
    // value, considering if the value is 'inverted' or not
    d->valueOrPos = (d->inverted) ? d->maximum - newValue : newValue - d->minimum;
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setInverted(bool inverted)
{
    Q_D(QRangeModel);
    d->inverted = inverted;
}

bool QRangeModel::inverted() const
{
    Q_D(const QRangeModel);
    return d->inverted;
}

void QRangeModel::singleStepAdd()
{
    Q_D(const QRangeModel);
    // we use the public (absolute) value in order to let
    // make life easier and let setValue() do all the dirty job
    // for us - we just have to add d->singleStep to the value
    const qreal oldValue = value();
    const qreal newValue = oldValue + d->singleStep;
    setValue(newValue);
}

void QRangeModel::singleStepSub()
{
    Q_D(const QRangeModel);
    // we use the public (absolute) value in order to let
    // make life easier and let setValue() do all the dirty job
    // for us - we just have to subtract d->singleStep from the value
    const qreal oldValue = value();
    const qreal newValue = oldValue - d->singleStep;
    setValue(newValue);
}

void QRangeModel::pageStepAdd()
{
    Q_D(const QRangeModel);
    // we use the public (absolute) value in order to let
    // make life easier and let setValue() do all the dirty job
    // for us - we just have to add d->pageStep to the value
    const qreal oldValue = value();
    const qreal newValue = oldValue + d->pageStep;
    setValue(newValue);
}

void QRangeModel::pageStepSub()
{
    Q_D(const QRangeModel);
    // we use the public (absolute) value in order to let
    // make life easier and let setValue() do all the dirty job
    // for us - we just have to subtract d->pageStep from the value
    const qreal oldValue = value();
    const qreal newValue = oldValue - d->pageStep;
    setValue(newValue);
}

void QRangeModel::toMinimum()
{
    Q_D(const QRangeModel);
    setValue(d->minimum);
}

void QRangeModel::toMaximum()
{
    Q_D(const QRangeModel);
    setValue(d->maximum);
}
