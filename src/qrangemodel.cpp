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
    pressValue = -1;
    offset_accumulated = 0;
    tracking = true;
    blocktracking = false;
    pressed = false;
    wrapping = false;
    repeatAction = QRangeModel::SliderNoAction;
#ifdef QT_KEYPAD_NAVIGATION
    isAutoRepeating = false;
    repeatMultiplier = 1;
#endif
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
        setSliderPosition(d->position); // re-bound
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
        setValue(d->value); // re-bound
    }
}

bool QRangeModel::hasNullRange() const
{
    Q_D(const QRangeModel);
    return d->maximum == d->minimum;
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

int QRangeModel::bound(int val) const
{
    Q_D(const QRangeModel);
    return qBound(d->minimum, val, d->maximum);
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

void QRangeModel::setSliderDown(bool down)
{
    Q_D(QRangeModel);
    bool doEmit = d->pressed != down;

    d->pressed = down;

    if (doEmit) {
        if (down)
            emit sliderPressed();
        else
            emit sliderReleased();
    }

    if (!down && d->position != d->value)
        triggerAction(SliderMove);
}

bool QRangeModel::isSliderDown() const
{
    Q_D(const QRangeModel);
    return d->pressed;
}

void QRangeModel::setSliderPosition(int position)
{
    Q_D(QRangeModel);
    position = bound(position);
    if (position == d->position)
        return;
    d->position = position;
    if (d->pressed)
        emit sliderMoved(position);
    if (d->tracking && !d->blocktracking)
        triggerAction(SliderMove);
}

int QRangeModel::sliderPosition() const
{
    Q_D(const QRangeModel);
    return d->position;
}

void QRangeModel::setPosition(int pos)
{
    Q_D(QRangeModel);
    pos = qBound(d->minpos, pos, d->maxpos);
    if (pos == d->pos)
        return;
    d->pos = pos;
    emit positionChanged(d->pos);

    qreal scale = qreal(d->maximum - d->minimum) / qreal(d->maxpos - d->minpos);
    setSliderPosition(scale * d->pos + d->minimum);
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
    value = bound(value);
    if (d->value == value && d->position == value)
        return;
    d->value = value;
    if (d->position != value) {
        d->position = value; // ###
        if (d->pressed)
            emit sliderMoved((d->position = value));
    }
    emit valueChanged(value);
}

void QRangeModel::setPressValue(int value)
{
    Q_D(QRangeModel);
    d->pressValue = value;
}

int QRangeModel::pressValue() const
{
    Q_D(const QRangeModel);
    return d->pressValue;
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

void QRangeModel::setStopMouseOverSlider(bool b)
{
    Q_D(QRangeModel);
    d->stopMouseOverSlider = b;
}

bool QRangeModel::stopMouseOverSlider() const
{
    Q_D(const QRangeModel);
    return d->stopMouseOverSlider;
}

void QRangeModel::triggerAction(SliderAction action)
{
    Q_D(QRangeModel);
    d->blocktracking = true;
    switch (action) {
    case SliderSingleStepAdd:
        setSliderPosition(d->overflowSafeAdd(d->effectiveSingleStep()));
        break;
    case SliderSingleStepSub:
        setSliderPosition(d->overflowSafeAdd(-d->effectiveSingleStep()));
        break;
    case SliderPageStepAdd:
        setSliderPosition(d->overflowSafeAdd(d->pageStep));
        break;
    case SliderPageStepSub:
        setSliderPosition(d->overflowSafeAdd(-d->pageStep));
        break;
    case SliderToMinimum:
        setSliderPosition(d->minimum);
        break;
    case SliderToMaximum:
        setSliderPosition(d->maximum);
        break;
    case SliderMove:
    case SliderNoAction:
        break;
    };
    emit actionTriggered(action);
    d->blocktracking = false;
    setValue(d->position);
}

void QRangeModel::setRepeatAction(SliderAction action, int thresholdTime, int repeatTime)
{
    Q_D(QRangeModel);
    if ((d->repeatAction = action) == SliderNoAction) {
        d->repeatActionTimer.stop();
    } else {
        d->repeatActionTime = repeatTime;
        d->repeatActionTimer.start(thresholdTime, this);
    }
}

QRangeModel::SliderAction QRangeModel::repeatAction() const
{
    return d_func()->repeatAction;
}

void QRangeModel::startRepeatActionTimer(int ms)
{
    d_func()->repeatActionTimer.start(ms, this);
}

bool QRangeModel::repeatActionTimerActive() const
{
    return d_func()->repeatActionTimer.isActive();
}

void QRangeModel::timerEvent(QTimerEvent *e)
{
    Q_D(QRangeModel);
    if (e->timerId() == d->repeatActionTimer.timerId()) {
        if (d->repeatActionTime) { // was threshold time, use repeat time next time
            d->repeatActionTimer.start(d->repeatActionTime, this);
            d->repeatActionTime = 0;
        }
        if (d->repeatAction == SliderPageStepAdd)
            d->setAdjustedSliderPosition(d->overflowSafeAdd(d->pageStep));
        else if (d->repeatAction == SliderPageStepSub)
            d->setAdjustedSliderPosition(d->overflowSafeAdd(-d->pageStep));
        else
            triggerAction(d->repeatAction);
    }
}

QT_END_NAMESPACE
