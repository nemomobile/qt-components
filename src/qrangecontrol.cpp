/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qrangecontrol.h"
#include "qrangecontrol_p.h"
#include "qevent.h"
#include "QtGui/qgraphicssceneevent.h"
#include "qapplication.h"
#include "qdebug.h"
#ifndef QT_NO_ACCESSIBILITY
#include "qaccessible.h"
#endif

QT_BEGIN_NAMESPACE

QRangeControlPrivate::QRangeControlPrivate(QAbstractSlider *slider)
    : slider(slider), graphicsObject(0)
{
}

QRangeControlPrivate::QRangeControlPrivate(QGraphicsObject *graphicsObject)
    : slider(0), graphicsObject(graphicsObject)
{
}

QRangeControlPrivate::~QRangeControlPrivate()
{
}

void QRangeControlPrivate::init()
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
    invertedAppearance = false;
    invertedControls = false;
    wrapping = false;
    orientation = Qt::Horizontal;
    repeatAction = QRangeControl::SliderNoAction;
#ifdef QT_KEYPAD_NAVIGATION
    isAutoRepeating = false;
    repeatMultiplier = 1;
#endif
}

QRangeControl::QRangeControl(QAbstractSlider *parent)
    : QObject(*new QRangeControlPrivate(parent), parent)
{
    Q_D(QRangeControl);
    d->init();
}

QRangeControl::QRangeControl(QGraphicsObject *parent)
    : QObject(*new QRangeControlPrivate(parent), parent)
{
    Q_D(QRangeControl);
    d->init();
}

QRangeControl::QRangeControl(QRangeControlPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(QRangeControl);
    d->init();
}

QRangeControl::~QRangeControl()
{
}

void QRangeControl::sedate()
{
    Q_D(QRangeControl);
    if (d->isSedated)
        return;

    d->isSedated = true;
    d->signalsBlocked = blockSignals(true);
}

void QRangeControl::awake()
{
    Q_D(QRangeControl);
    if (!d->isSedated)
        return;

    d->isSedated = false;
    if (!d->signalsBlocked)
        blockSignals(false);
}

void QRangeControl::setPositionRange(int min, int max)
{
    Q_D(QRangeControl);
    int oldMin = d->minpos;
    int oldMax = d->maxpos;
    d->minpos = min;
    d->maxpos = max;
    if (oldMin != d->minpos || oldMax != d->maxpos) {
        //sliderChange(SliderRangeChange);
        emit positionRangeChanged(d->minpos, d->maxpos);
        setSliderPosition(d->position); // re-bound
    }
}

void QRangeControl::setRange(int min, int max)
{
    Q_D(QRangeControl);
    int oldMin = d->minimum;
    int oldMax = d->maximum;
    d->minimum = min;
    d->maximum = qMax(min, max);
    if (oldMin != d->minimum || oldMax != d->maximum) {
        sliderChange(SliderRangeChange);
        emit rangeChanged(d->minimum, d->maximum);
        setValue(d->value); // re-bound
    }
}

bool QRangeControl::hasNullRange() const
{
    Q_D(const QRangeControl);
    return d->maximum == d->minimum;
}

void QRangeControlPrivate::setSteps(int single, int page)
{
    Q_Q(QRangeControl);
    singleStep = qAbs(single);
    pageStep = qAbs(page);
    q->sliderChange(QRangeControl::SliderStepsChange);
}

void QRangeControl::setOrientation(Qt::Orientation orientation)
{
    Q_D(QRangeControl);
    if (d->orientation == orientation)
        return;

    d->orientation = orientation;
}

Qt::Orientation QRangeControl::orientation() const
{
    Q_D(const QRangeControl);
    return d->orientation;
}

void QRangeControl::setMinimum(int min)
{
    Q_D(QRangeControl);
    setRange(min, qMax(d->maximum, min));
}

int QRangeControl::minimum() const
{
    Q_D(const QRangeControl);
    return d->minimum;
}

void QRangeControl::setMaximum(int max)
{
    Q_D(QRangeControl);
    setRange(qMin(d->minimum, max), max);
}

int QRangeControl::maximum() const
{
    Q_D(const QRangeControl);
    return d->maximum;
}

int QRangeControl::bound(int val) const
{
    Q_D(const QRangeControl);
    return qBound(d->minimum, val, d->maximum);
}

void QRangeControl::setSingleStep(int step)
{
    Q_D(QRangeControl);
    if (step != d->singleStep)
        d->setSteps(step, d->pageStep);
}

int QRangeControl::singleStep() const
{
    Q_D(const QRangeControl);
    return d->singleStep;
}

void QRangeControl::setPageStep(int step)
{
    Q_D(QRangeControl);
    if (step != d->pageStep)
        d->setSteps(d->singleStep, step);
}

int QRangeControl::pageStep() const
{
    Q_D(const QRangeControl);
    return d->pageStep;
}

void QRangeControl::setTracking(bool enable)
{
    Q_D(QRangeControl);
    d->tracking = enable;
}

bool QRangeControl::hasTracking() const
{
    Q_D(const QRangeControl);
    return d->tracking;
}

void QRangeControl::setSliderDown(bool down)
{
    Q_D(QRangeControl);
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

bool QRangeControl::isSliderDown() const
{
    Q_D(const QRangeControl);
    return d->pressed;
}

void QRangeControl::setSliderPosition(int position)
{
    Q_D(QRangeControl);
    position = bound(position);
    if (position == d->position)
        return;
    d->position = position;
    if (!d->tracking)
        d->update();
    if (d->pressed)
        emit sliderMoved(position);
    if (d->tracking && !d->blocktracking)
        triggerAction(SliderMove);
}

int QRangeControl::sliderPosition() const
{
    Q_D(const QRangeControl);
    return d->position;
}

void QRangeControl::setPosition(int pos)
{
    Q_D(QRangeControl);
    pos = qBound(d->minpos, pos, d->maxpos);
    if (pos == d->pos)
        return;
    d->pos = pos;
    emit positionChanged(d->pos);

    qreal scale = qreal(d->maximum - d->minimum) / qreal(d->maxpos - d->minpos);
    setSliderPosition(scale * d->pos + d->minimum);
}

int QRangeControl::position() const
{
    Q_D(const QRangeControl);
    return d->pos;
}

void QRangeControl::setMinimumPosition(int min)
{
    Q_D(QRangeControl);
    setPositionRange(min, d->maxpos);
}

int QRangeControl::minimumPosition() const
{
    Q_D(const QRangeControl);
    return d->maxpos;
}

void QRangeControl::setMaximumPosition(int max)
{
    Q_D(QRangeControl);
    setPositionRange(d->minpos, max);
}

int QRangeControl::maximumPosition() const
{
    Q_D(const QRangeControl);
    return d->maxpos;
}

int QRangeControl::value() const
{
    Q_D(const QRangeControl);
    return d->value;
}

void QRangeControl::setValue(int value)
{
    Q_D(QRangeControl);
    value = bound(value);
    if (d->value == value && d->position == value)
        return;
    d->value = value;
    if (d->position != value) {
        d->position = value; // ###
        if (d->pressed)
            emit sliderMoved((d->position = value));
    }
#ifndef QT_NO_ACCESSIBILITY
    QAccessible::updateAccessibility(this, 0, QAccessible::ValueChanged);
#endif
    sliderChange(SliderValueChange);
    emit valueChanged(value);
}

void QRangeControl::setPressValue(int value)
{
    Q_D(QRangeControl);
    d->pressValue = value;
}

int QRangeControl::pressValue() const
{
    Q_D(const QRangeControl);
    return d->pressValue;
}

void QRangeControl::setWrapping(bool b)
{
    Q_D(QRangeControl);
    d->wrapping = b;
}

bool QRangeControl::wrapping() const
{
    Q_D(const QRangeControl);
    return d->wrapping;
}

void QRangeControl::setStopMouseOverSlider(bool b)
{
    Q_D(QRangeControl);
    d->stopMouseOverSlider = b;
}

bool QRangeControl::stopMouseOverSlider() const
{
    Q_D(const QRangeControl);
    return d->stopMouseOverSlider;
}

bool QRangeControl::invertedAppearance() const
{
    Q_D(const QRangeControl);
    return d->invertedAppearance;
}

void QRangeControl::setInvertedAppearance(bool invert)
{
    Q_D(QRangeControl);
    d->invertedAppearance = invert;
    d->update();
}

bool QRangeControl::invertedControls() const
{
    Q_D(const QRangeControl);
    return d->invertedControls;
}

void QRangeControl::setInvertedControls(bool invert)
{
    Q_D(QRangeControl);
    d->invertedControls = invert;
}

void QRangeControl::triggerAction(SliderAction action)
{
    Q_D(QRangeControl);
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

void QRangeControl::setRepeatAction(SliderAction action, int thresholdTime, int repeatTime)
{
    Q_D(QRangeControl);
    if ((d->repeatAction = action) == SliderNoAction) {
        d->repeatActionTimer.stop();
    } else {
        d->repeatActionTime = repeatTime;
        d->repeatActionTimer.start(thresholdTime, this);
    }
}

QRangeControl::SliderAction QRangeControl::repeatAction() const
{
    return d_func()->repeatAction;
}

void QRangeControl::startRepeatActionTimer(int ms)
{
    d_func()->repeatActionTimer.start(ms, this);
}

bool QRangeControl::repeatActionTimerActive() const
{
    return d_func()->repeatActionTimer.isActive();
}

void QRangeControl::timerEvent(QTimerEvent *e)
{
    Q_D(QRangeControl);
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

void QRangeControl::sliderChange(SliderChange)
{
    Q_D(QRangeControl);
    d->update();
}

#ifndef QT_NO_WHEELEVENT

void QRangeControl::wheelEvent(QEvent * e)
{
    Q_D(QRangeControl);
    switch (e->type()) {
    case QEvent::Wheel: {
        QWheelEvent *we = dynamic_cast<QWheelEvent *>(e);
        d->wheelEventHandler(e, we->delta(), we->modifiers());
        break;
    }
    case QEvent::GraphicsSceneWheel: {
        QGraphicsSceneWheelEvent *we = dynamic_cast<QGraphicsSceneWheelEvent *>(e);
        d->wheelEventHandler(e, we->delta(), we->modifiers());
        break;
    }
    default:
        break;
    }
}

void QRangeControlPrivate::wheelEventHandler(QEvent *e, int delta, Qt::KeyboardModifiers modifiers)
{
    Q_Q(QRangeControl);
    e->ignore();

    int stepsToScroll = 0;
    qreal offset = qreal(delta) / 120;

    if ((modifiers & Qt::ControlModifier) || (modifiers & Qt::ShiftModifier)) {
        // Scroll one page regardless of delta:
        stepsToScroll = qBound(-pageStep, int(offset * pageStep), pageStep);
        offset_accumulated = 0;
    } else {
        // Calculate how many lines to scroll. Depending on what delta is (and
        // offset), we might end up with a fraction (e.g. scroll 1.3 lines). We can
        // only scroll whole lines, so we keep the reminder until next event.
        qreal stepsToScrollF = offset * QApplication::wheelScrollLines() * effectiveSingleStep();
        // Check if wheel changed direction since last event:
        if (offset_accumulated != 0 && (offset / offset_accumulated) < 0)
            offset_accumulated = 0;

        offset_accumulated += stepsToScrollF;
        stepsToScroll = qBound(-pageStep, int(offset_accumulated), pageStep);
        offset_accumulated -= int(offset_accumulated);
        if (stepsToScroll == 0)
            return;
    }

    if (invertedControls)
        stepsToScroll = -stepsToScroll;

    int prevValue = value;
    position = overflowSafeAdd(stepsToScroll); // value will be updated by triggerAction()
    q->triggerAction(QRangeControl::SliderMove);

    if (prevValue == value)
        offset_accumulated = 0;
    else
        e->accept();
}

#endif

void QRangeControl::keyPressEvent(QEvent *ev)
{
    Q_D(QRangeControl);
    if (ev->type() == QEvent::KeyPress)
        d->keyPressEventHandler(dynamic_cast<QKeyEvent *>(ev));
}

#ifdef QT_KEYPAD_NAVIGATION
/*!
    \internal

    Tells us if it there is currently a reachable widget by keypad navigation in
    a certain \a orientation.
    If no navigation is possible, occuring key events in that \a orientation may
    be used to interact with the value in the focussed widget, even though it
    currently has not the editFocus.

    \sa QWidgetPrivate::widgetInNavigationDirection(), QWidget::hasEditFocus()
*/
inline static bool canKeypadNavigate(Qt::Orientation orientation)
{
    return orientation == Qt::Horizontal?
            (QWidgetPrivate::widgetInNavigationDirection(QWidgetPrivate::DirectionEast)
                    || QWidgetPrivate::widgetInNavigationDirection(QWidgetPrivate::DirectionWest))
            :(QWidgetPrivate::widgetInNavigationDirection(QWidgetPrivate::DirectionNorth)
                    || QWidgetPrivate::widgetInNavigationDirection(QWidgetPrivate::DirectionSouth));
}
/*!
    \internal

    Checks, if the \a widget is inside a QTabWidget. If is is inside
    one, left/right key events will be used to switch between tabs in keypad
    navigation. If there is no QTabWidget, the horizontal key events can be used to
    interact with the value in the focussed widget, even though it currently has
    not the editFocus.

    \sa QWidget::hasEditFocus()
*/
inline static bool inTabWidget(QWidget *widget)
{
    for (QWidget *tabWidget = widget; tabWidget; tabWidget = tabWidget->parentWidget())
        if (qobject_cast<const QTabWidget*>(tabWidget))
            return true;
    return false;
}

void QRangeControl::saveValue()
{
    d->origValue = d->value;
}
#endif // QT_KEYPAD_NAVIGATION

void QRangeControlPrivate::keyPressEventHandler(QKeyEvent *ev)
{
    Q_Q(QRangeControl);
    QRangeControl::SliderAction action = QRangeControl::SliderNoAction;
#ifdef QT_KEYPAD_NAVIGATION
    if (ev->isAutoRepeat()) {
        if (firstRepeat.isNull())
            firstRepeat = QTime::currentTime();
        else if (1 == repeatMultiplier) {
            // This is the interval in milli seconds which one key repetition
            // takes.
            const int repeatMSecs = firstRepeat.msecsTo(QTime::currentTime());

            /**
             * The time it takes to currently navigate the whole slider.
             */
            const qreal currentTimeElapse = (qreal(maximum / singleStep) * repeatMSecs;

            /**
             * This is an arbitrarily determined constant in msecs that
             * specifies how long time it should take to navigate from the
             * start to the end(excluding starting key auto repeat).
             */
            const int SliderRepeatElapse = 2500;

            repeatMultiplier = currentTimeElapse / SliderRepeatElapse;
        }

    }
    else if (!firstRepeat.isNull()) {
        firstRepeat = QTime();
        repeatMultiplier = 1;
    }

#endif

    switch (ev->key()) {
#ifdef QT_KEYPAD_NAVIGATION
        case Qt::Key_Select:
            if (QApplication::keypadNavigationEnabled())
                setEditFocus(!hasEditFocus());
            else
                ev->ignore();
            break;
        case Qt::Key_Back:
            if (QApplication::keypadNavigationEnabled() && hasEditFocus()) {
                q->setValue(origValue);
                setEditFocus(false);
            } else
                ev->ignore();
            break;
#endif

        // It seems we need to use invertedAppearance for Left and right, otherwise, things look weird.
        case Qt::Key_Left:
#ifdef QT_KEYPAD_NAVIGATION
            // In QApplication::KeypadNavigationDirectional, we want to change the slider
            // value if there is no left/right navigation possible and if this slider is not
            // inside a tab widget.
            if (QApplication::keypadNavigationEnabled()
                    && (!hasEditFocus() && QApplication::navigationMode() == Qt::NavigationModeKeypadTabOrder
                    || orientation == Qt::Vertical
                    || !hasEditFocus() && (canKeypadNavigate(Qt::Horizontal) || inTabWidget(slider)))) {
                ev->ignore();
                return;
            }
            if (QApplication::keypadNavigationEnabled() && d->orientation == Qt::Vertical)
                action = d->invertedControls ? QRangeControl::SliderSingleStepSub : QRangeControl::SliderSingleStepAdd;
            else
#endif
            if (isRightToLeft())
                action = invertedAppearance ? QRangeControl::SliderSingleStepSub : QRangeControl::SliderSingleStepAdd;
            else
                action = !invertedAppearance ? QRangeControl::SliderSingleStepSub : QRangeControl::SliderSingleStepAdd;
            break;
        case Qt::Key_Right:
#ifdef QT_KEYPAD_NAVIGATION
            // Same logic as in Qt::Key_Left
            if (QApplication::keypadNavigationEnabled()
                    && (!hasEditFocus() && QApplication::navigationMode() == Qt::NavigationModeKeypadTabOrder
                    || orientation == Qt::Vertical
                    || !hasEditFocus() && (canKeypadNavigate(Qt::Horizontal) || inTabWidget(slider)))) {
                ev->ignore();
                return;
            }
            if (QApplication::keypadNavigationEnabled() && d->orientation == Qt::Vertical)
                action = invertedControls ? QRangeControl::SliderSingleStepAdd : QRangeControl::SliderSingleStepSub;
            else
#endif
            if (isRightToLeft())
                action = invertedAppearance ? QRangeControl::SliderSingleStepAdd : QRangeControl::SliderSingleStepSub;
            else
                action = !invertedAppearance ? QRangeControl::SliderSingleStepAdd : QRangeControl::SliderSingleStepSub;
            break;
        case Qt::Key_Up:
#ifdef QT_KEYPAD_NAVIGATION
            // In QApplication::KeypadNavigationDirectional, we want to change the slider
            // value if there is no up/down navigation possible.
            if (QApplication::keypadNavigationEnabled()
                    && (QApplication::navigationMode() == Qt::NavigationModeKeypadTabOrder
                    || orientation == Qt::Horizontal
                    || !hasEditFocus() && canKeypadNavigate(Qt::Vertical))) {
                ev->ignore();
                break;
            }
#endif
            action = invertedControls ? QRangeControl::SliderSingleStepSub : QRangeControl::SliderSingleStepAdd;
            break;
        case Qt::Key_Down:
#ifdef QT_KEYPAD_NAVIGATION
            // Same logic as in Qt::Key_Up
            if (QApplication::keypadNavigationEnabled()
                    && (QApplication::navigationMode() == Qt::NavigationModeKeypadTabOrder
                    || orientation == Qt::Horizontal
                    || !hasEditFocus() && canKeypadNavigate(Qt::Vertical))) {
                ev->ignore();
                break;
            }
#endif
            action = invertedControls ? QRangeControl::SliderSingleStepAdd : QRangeControl::SliderSingleStepSub;
            break;
        case Qt::Key_PageUp:
            action = invertedControls ? QRangeControl::SliderPageStepSub : QRangeControl::SliderPageStepAdd;
            break;
        case Qt::Key_PageDown:
            action = invertedControls ? QRangeControl::SliderPageStepAdd : QRangeControl::SliderPageStepSub;
            break;
        case Qt::Key_Home:
            action = QRangeControl::SliderToMinimum;
            break;
        case Qt::Key_End:
            action = QRangeControl::SliderToMaximum;
            break;
        default:
            ev->ignore();
            break;
    }
    if (action)
        q->triggerAction(action);
}

void QRangeControl::changeEvent(QEvent *ev)
{
    Q_D(QRangeControl);
    switch (ev->type()) {
    case QEvent::EnabledChange:
        if (!d->isEnabled()) {
            d->repeatActionTimer.stop();
            setSliderDown(false);
        }
        break;
    default:
        break;
    }
}

QT_END_NAMESPACE
