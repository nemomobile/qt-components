/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qrangemodel.h"
#include "qrangemodel_p.h"
#include "qevent.h"
#include "QtGui/qgraphicssceneevent.h"
#include "qapplication.h"
#include "qdebug.h"
#ifndef QT_NO_ACCESSIBILITY
#include "qaccessible.h"
#endif

QT_BEGIN_NAMESPACE

QRangeModelPrivate::QRangeModelPrivate()
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
    wrapping = false;
}

QRangeModel::QRangeModel(QObject *parent)
    : QObject(*new QRangeModelPrivate(), parent)
{
    Q_D(QRangeModel);
    d->init();
}

QRangeModel::QRangeModel(QRangeModelPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(QRangeModel);
    d->init();
}

QRangeModel::~QRangeModel()
{
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

void QRangeModel::setPositionRange(int min, int max)
{
    Q_D(QRangeModel);
    int oldMin = d->minpos;
    int oldMax = d->maxpos;
    d->minpos = min;
    d->maxpos = max;
    if (oldMin != d->minpos || oldMax != d->maxpos) {
        emit positionRangeChanged(d->minpos, d->maxpos);
        setPosition(d->positionFromValue()); // re-bound
    }
}

void QRangeModel::setRange(int min, int max)
{
    Q_D(QRangeModel);
    int oldMin = d->minimum;
    int oldMax = d->maximum;
    d->minimum = min;
    d->maximum = qMax(min, max);
    if (oldMin != d->minimum || oldMax != d->maximum) {
        emit rangeChanged(d->minimum, d->maximum);
        setPosition(d->positionFromValue()); // re-bound
    }
}

void QRangeModelPrivate::setSteps(int single, int page)
{
    singleStep = qAbs(single);
    pageStep = qAbs(page);
}

void QRangeModel::setMinimum(int min)
{
    Q_D(QRangeModel);
    setRange(min, qMax(d->maximum, min));
}

int QRangeModel::minimum() const
{
    Q_D(const QRangeModel);
    return d->minimum;
}

void QRangeModel::setMaximum(int max)
{
    Q_D(QRangeModel);
    setRange(qMin(d->minimum, max), max);
}

int QRangeModel::maximum() const
{
    Q_D(const QRangeModel);
    return d->maximum;
}


void QRangeModel::setSingleStep(int step)
{
    Q_D(QRangeModel);
    if (step != d->singleStep)
        d->setSteps(step, d->pageStep);
}

int QRangeModel::singleStep() const
{
    Q_D(const QRangeModel);
    return d->singleStep;
}

void QRangeModel::setPageStep(int step)
{
    Q_D(QRangeModel);
    if (step != d->pageStep)
        d->setSteps(d->singleStep, step);
}

int QRangeModel::pageStep() const
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

void QRangeModel::setSliderPosition(int position)
{
    setPosition(position);
}

int QRangeModel::sliderPosition() const
{
    return position();
}

void QRangeModel::setPosition(int pos)
{
    Q_D(QRangeModel);

    pos = qBound(d->minpos, pos, d->maxpos);
    if (pos == d->pos)
        return;
    d->pos = pos;
    emit positionChanged(d->pos);

    const int newValue = d->valueFromPosition();
    if (d->value == newValue)
        return;
    d->value = newValue;
    emit valueChanged(d->value);
}

int QRangeModel::position() const
{
    Q_D(const QRangeModel);
    return d->pos;
}

void QRangeModel::setMinimumPosition(int min)
{
    Q_D(QRangeModel);
    setPositionRange(min, d->maxpos);
}

int QRangeModel::minimumPosition() const
{
    Q_D(const QRangeModel);
    return d->maxpos;
}

void QRangeModel::setMaximumPosition(int max)
{
    Q_D(QRangeModel);
    setPositionRange(d->minpos, max);
}

int QRangeModel::maximumPosition() const
{
    Q_D(const QRangeModel);
    return d->maxpos;
}

int QRangeModel::value() const
{
    Q_D(const QRangeModel);
    return d->value;
}

void QRangeModel::setValue(int value)
{
    Q_D(QRangeModel);

    value = qBound(d->minimum, value, d->maximum);
    if (value == d->value)
        return;
    d->value = value;
    if (d->tracking && !d->blocktracking)
        emit valueChanged(value);

    const int newPosition = d->positionFromValue();
    if (d->pos == newPosition)
        return;
    d->pos = newPosition;
    emit positionChanged(d->pos);
}

void QRangeModel::setWrapping(bool b)
{
    Q_D(QRangeModel);
    d->wrapping = b;
}

bool QRangeModel::wrapping() const
{
    Q_D(const QRangeModel);
    return d->wrapping;
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
