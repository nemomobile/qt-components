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

QT_BEGIN_NAMESPACE
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
    position = 0;
    pos = 0;
    minpos = 0;
    maxpos = 0;
    tracking = true;
    blocktracking = false;
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
    qreal oldMin = d->minpos;
    qreal oldMax = d->maxpos;
    d->minpos = min;
    d->maxpos = max;
    if (oldMin != d->minpos || oldMax != d->maxpos) {
        emit positionRangeChanged(d->minpos, d->maxpos);
        setPosition(d->positionFromValue()); // re-bound
    }
}

void QRangeModel::setRange(qreal min, qreal max)
{
    Q_D(QRangeModel);
    qreal oldMin = d->minimum;
    qreal oldMax = d->maximum;
    d->minimum = min;
    d->maximum = qMax(min, max);
    if (oldMin != d->minimum || oldMax != d->maximum) {
        emit rangeChanged(d->minimum, d->maximum);
        setPosition(d->positionFromValue()); // re-bound
    }
}

void QRangeModelPrivate::setSteps(qreal single, qreal page)
{
    singleStep = qAbs(single);
    pageStep = qAbs(page);
}

void QRangeModel::setMinimum(qreal min)
{
    Q_D(QRangeModel);
    setRange(min, qMax(d->maximum, min));
}

qreal QRangeModel::minimum() const
{
    Q_D(const QRangeModel);
    return d->minimum;
}

void QRangeModel::setMaximum(qreal max)
{
    Q_D(QRangeModel);
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
    if (step != d->singleStep)
        d->setSteps(step, d->pageStep);
}

qreal QRangeModel::singleStep() const
{
    Q_D(const QRangeModel);
    return d->singleStep;
}

void QRangeModel::setPageStep(qreal step)
{
    Q_D(QRangeModel);
    if (step != d->pageStep)
        d->setSteps(d->singleStep, step);
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

bool QRangeModel::hasTracking() const
{
    Q_D(const QRangeModel);
    return d->tracking;
}

void QRangeModel::setSliderPosition(qreal position)
{
    setPosition(position);
}

qreal QRangeModel::sliderPosition() const
{
    return position();
}

void QRangeModel::setPosition(qreal pos)
{
    Q_D(QRangeModel);

    if (d->minpos < d->maxpos) {
        pos = qBound(d->minpos, pos, d->maxpos);
    } else {
        pos = qBound(d->maxpos, pos, d->minpos);
    }

    if (pos == d->pos)
        return;
    d->pos = pos;
    emit positionChanged(d->pos);

    const qreal newValue = d->valueFromPosition();
    if (d->value == newValue)
        return;
    d->value = newValue;
    emit valueChanged(d->value);
}

qreal QRangeModel::position() const
{
    Q_D(const QRangeModel);
    return d->pos;
}

void QRangeModel::setMinimumPosition(qreal min)
{
    Q_D(QRangeModel);
    setPositionRange(min, d->maxpos);
}

qreal QRangeModel::minimumPosition() const
{
    Q_D(const QRangeModel);
    return d->maxpos;
}

void QRangeModel::setMaximumPosition(qreal max)
{
    Q_D(QRangeModel);
    setPositionRange(d->minpos, max);
}

qreal QRangeModel::maximumPosition() const
{
    Q_D(const QRangeModel);
    return d->maxpos;
}

qreal QRangeModel::value() const
{
    Q_D(const QRangeModel);
    return d->value;
}

void QRangeModel::setValue(qreal value)
{
    Q_D(QRangeModel);

    value = qBound(d->minimum, value, d->maximum);
    if (value == d->value)
        return;
    d->value = value;
    if (d->tracking && !d->blocktracking)
        emit valueChanged(value);

    const qreal newPosition = d->positionFromValue();
    if (d->pos == newPosition)
        return;
    d->pos = newPosition;
    emit positionChanged(d->pos);
}

void QRangeModel::singleStepAdd()
{
    Q_D(QRangeModel);
    setValue(d->value + d->singleStep);
}

void QRangeModel::singleStepSub()
{
    Q_D(QRangeModel);
    setValue(d->value - d->singleStep);
}

void QRangeModel::pageStepAdd()
{
    Q_D(QRangeModel);
    setValue(d->value + d->pageStep);
}

void QRangeModel::pageStepSub()
{
    Q_D(QRangeModel);
    setValue(d->value - d->pageStep);
}

void QRangeModel::toMinimum()
{
    Q_D(QRangeModel);
    setValue(d->minimum);
}

void QRangeModel::toMaximum()
{
    Q_D(QRangeModel);
    setValue(d->maximum);
}

QT_END_NAMESPACE
