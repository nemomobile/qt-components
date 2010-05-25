/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
    minimum = 0;
    maximum = 99;
    singleStep = 1;
    pageStep = 10;
    value = 0;
    pos = 0;
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

void QRangeModel::setPositionRange(qreal min, qreal max)
{
    Q_D(QRangeModel);

    // if both values didnt change we can safely return
    if ((min == d->posatmin) && (max == d->posatmax)) {
        return;
    }

    d->posatmin = min;
    d->posatmax = max;

    // update the position based on the new range
    bool emitPosChanged = false;
    const qreal newPosition = d->positionFromValue(d->value);
    if (newPosition != d->pos) {
        d->pos = newPosition;
        emitPosChanged = true;
    }

    emit positionRangeChanged(d->posatmin, d->posatmax);

    // if the position was updated, emit a signal with
    // the new absolute position
    if (emitPosChanged)
        emit positionChanged(position());
}

void QRangeModel::setRange(qreal min, qreal max)
{
    Q_D(QRangeModel);

    // if both values didnt change we can safely return
    if ((min == d->minimum) && (max == d->maximum)) {
        return;
    }

    // bound the maximum value to be at least equal to the minimum
    d->minimum = min;
    d->maximum = qMax(min, max);

    // update the position based on the new value range
    bool emitPosChanged = false;
    const qreal newPosition = d->positionFromValue(d->value);
    if (newPosition != d->pos) {
        d->pos = newPosition;
        emitPosChanged = true;
    }

    emit rangeChanged(d->minimum, d->maximum);

    // if the position was updated, emit a signal with the new position
    if (emitPosChanged)
        emit positionChanged(position());
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

    // this must return the absolute position based on the internal
    // one, that is relative. so we need to add the start position
    // and also bound it to d->posatmin and d->posatmax
    const qreal pos = d->posatmin + d->pos;
    if (d->posatmin > d->posatmax)
        return qBound(d->posatmax, pos, d->posatmin);
    return qBound(d->posatmin, pos, d->posatmax);
}

void QRangeModel::setPosition(qreal pos)
{
    Q_D(QRangeModel);

    // 'pos' came as an absolute position and we want to store it
    // as a relative position, so we should subtract the start value
    const qreal newPos = pos - d->posatmin;

    // if the new relative position is the same, we can safely return
    if (newPos == d->pos)
        return;

    // saves the relative position
    d->pos = newPos;

    // update the value based on the new position
    bool emitValueChanged = false;
    const qreal newValue = d->valueFromPosition(d->pos);
    if (newValue != d->value) {
        d->value = newValue;
        emitValueChanged = true;
    }

    emit positionChanged(position());

    // if we have a new value then we emit the signal with
    // the new absolute value
    if (emitValueChanged)
        emit valueChanged(value());
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
    // it to d->minimum and d->maximum
    qreal value;
    if (d->inverted) {
        value = d->maximum - d->value;
    } else {
        value = d->value + d->minimum;
    }
    return qBound(d->minimum, value, d->maximum);
}

void QRangeModel::setValue(qreal value)
{
    Q_D(QRangeModel);

    // 'value' came as an absolute value and we want to store it
    // as a relative value, so we should subtract/add the start
    // value, considering if the value is 'inverted' or not
    qreal newValue;
    if (d->inverted) {
        newValue = d->maximum - value;
    } else {
        newValue = value - d->minimum;
    }

    // if the new relative value is the same, we can safely return
    if (newValue == d->value)
        return;

    // saves the relative value
    d->value = newValue;

    // update the position based on the new value
    bool emitPosChanged = false;
    const qreal newPosition = d->positionFromValue(d->value);
    if (newPosition != d->pos) {
        d->pos = newPosition;
        emitPosChanged = true;
    }

    if (d->tracking)
        emit valueChanged(this->value());

    // if we have a new position then we emit the signal with
    // the new absolute position
    if (emitPosChanged)
        emit positionChanged(position());
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
