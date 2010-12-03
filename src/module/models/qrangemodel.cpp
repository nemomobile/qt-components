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
    minimum = 0;
    maximum = 99;
    steps = 0;
    value = 0;
    pos = 0;
    posatmin = 0;
    posatmax = 0;
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
    d->posatmin = min;
    d->posatmax = max;

    // When a new positionRange is defined, the position property must be updated based on the value property.
    // For instance, imagine that you have a valueRange of [0,100] and a position range of [20,100],
    // if a user set the value to 50, the position would be 60. If this positionRange is updated to [0,100], then
    // the new position, based on the value (50), will be 50.
    // If the newPosition is different than the old one, it must be updated, in order to emit
    // the positionChanged signal.
    d->pos = d->positionFromValue(d->value);

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
    const qreal oldMaximum = d->maximum;

    d->minimum = min;
    d->maximum = qMax(min, max);

    // Internal value property must be updated when the range is changed, since it can become valid with the
    // new range. For instance, imagine a range of [50,100] and a user sets the value to 10 (public value), even 
    // this value is not inside the valid range, it must be stored, since it can become valid later. Thus, it
    // will be internally stored as a relative value of -40 (publicValue - minimum). Later, if the user sets a new
    // range of [10,100], the value that was invalid previously, now is valid in the new range, so its relative value
    // to be stored is 0 (oldRelativeValue + (oldMinimum - newMinimum)). Additionally the position must be updated
    // too.
    d->value += oldMinimum - d->minimum;

    // Update internal position if it was changed. It can occurs if internal value changes, due to range update
    d->pos = d->positionFromValue(d->value);

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

qreal QRangeModel::positionForValue(qreal value) const
{
    Q_D(const QRangeModel);

    const qreal relativeValue = value - d->minimum;
    const qreal relativePosition = d->positionFromValue(relativeValue);
    const qreal absolutePosition = relativePosition + d->effectivePosAtMin();

    // ### TODO: Observe "steps" property.
    if (d->effectivePosAtMin() > d->effectivePosAtMax())
        return qBound(d->effectivePosAtMax(), absolutePosition, d->effectivePosAtMin());
    return qBound(d->effectivePosAtMin(), absolutePosition, d->effectivePosAtMax());
}

qreal QRangeModel::position() const
{
    Q_D(const QRangeModel);

    // This must return the absolute position based on the internal
    // one, that is relative. so we need to add the start position
    // and also bound it to effective posatmin and posatmax

    // It is also important to do value-within-range check this
    // late (as opposed to during setPosition()). The reason is
    // QML bindings; a position that is initially invalid because it lays
    // outside the range, might become valid later if the range changes.

    qreal pos = d->effectivePosAtMin() + d->pos;
    const qreal s = d->positionFromValue(d->steps);
    if (s > 0)
        pos = s * qRound(pos/s);
    if (d->effectivePosAtMin() > d->effectivePosAtMax())
        return qBound(d->effectivePosAtMax(), pos, d->effectivePosAtMin());
    return qBound(d->effectivePosAtMin(), pos, d->effectivePosAtMax());
}

void QRangeModel::setPosition(qreal newPosition)
{
    Q_D(QRangeModel);

    const qreal oldPosition = position();

    // We can't call position() here to get the oldInternalPosition, instead we must get the internal position and
    // add the posatmin. For instance, if we have a positionRange of [50,100] and a user has set position to
    // 10, we must store 10 because it can become a valid position if the positionRange is changed in the
    // future. position() method always return a position bounded to [posatmin,posatmax], so it would return 50,
    // instead of 10. So, if a user wants to set position to 50, the internal position would not be updated
    // with this newPosition, since position() would return 50 for the oldInternalPosition and the qFuzzyCompare
    // between newPosition and oldInternalPosition would return true.
    const qreal oldInternalPosition = d->pos + d->effectivePosAtMin();

    if (qFuzzyCompare(newPosition, oldInternalPosition))
        return;
    // Flag that we need to translate from position
    // to value upon calls to value():
    const qreal oldValue = value();

    // Update relative position with the new one (get absolute position "newPosition" and subtract
    // the posatmin property). Value property must be updated too.
    d->pos = newPosition - d->effectivePosAtMin();
    d->value = d->valueFromPosition(d->pos);
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

qreal QRangeModel::valueForPosition(qreal position) const
{
    Q_D(const QRangeModel);

    const qreal relativePosition = position - d->effectivePosAtMin();
    const qreal relativeValue = d->valueFromPosition(relativePosition);
    const qreal absoluteValue = relativeValue + d->minimum;

    // ### TODO: Observe "steps" property.
    return qBound(d->minimum, absoluteValue, d->maximum);
}

qreal QRangeModel::value() const
{
    Q_D(const QRangeModel);

    // It is important to do value-within-range check this
    // late (as opposed to during setPosition()). The reason is
    // QML bindings; a position that is initially invalid because it lays
    // outside the range, might become valid later if the range changes.
    qreal value = d->value + d->minimum;
    if (d->steps > 0)
        value = d->steps * qRound(value/d->steps);
    return qBound(d->minimum, value, d->maximum);
}

void QRangeModel::setValue(qreal newValue)
{
    Q_D(QRangeModel);

    const qreal oldValue = value();

    // We can't call value() here to get the oldInternalValue, instead we must get the internal value and
    // add the minimum. For instance, if we have a valueRange of [50,100] and a user has set value to
    // 10, we must store 10 because it can become a valid value if the valueRange is changed in the
    // future. value() method always return a value bounded to [min,max], so it would return 50, instead of
    // 10. So, if a user wants to set value to 50, the internal value would not be updated with this new
    // value, since value() would return 50 for the oldInternalValue and the qFuzzyCompare between newValue and
    // oldInternalValue would return true.
    const qreal oldInternalValue = d->value + d->minimum;

    if (qFuzzyCompare(newValue, oldInternalValue))
        return;

    const qreal oldPosition = position();

    // Update relative value and position
    d->value = newValue - d->minimum;
    d->pos = d->positionFromValue(d->value);
    d->emitValueAndPositionIfChanged(oldValue, oldPosition);
}

void QRangeModel::setInverted(bool inverted)
{
    Q_D(QRangeModel);
    if (inverted == d->inverted)
        return;

    d->inverted = inverted;

    // After updating the internal value, the position property can change.
    setPosition(d->positionFromValue(d->value) + d->effectivePosAtMin());
}

bool QRangeModel::inverted() const
{
    Q_D(const QRangeModel);
    return d->inverted;
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
