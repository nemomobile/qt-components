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

QRangeModelPrivate::QRangeModelPrivate(QAbstractSlider *slider)
    : slider(slider), graphicsObject(0)
{
}

QRangeModelPrivate::QRangeModelPrivate(QGraphicsObject *graphicsObject)
    : slider(0), graphicsObject(graphicsObject)
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
    invertedAppearance = false;
    invertedControls = false;
    wrapping = false;
    orientation = Qt::Horizontal;
    repeatAction = QRangeModel::SliderNoAction;
#ifdef QT_KEYPAD_NAVIGATION
    isAutoRepeating = false;
    repeatMultiplier = 1;
#endif
}

QRangeModel::QRangeModel(QAbstractSlider *parent)
    : QObject(*new QRangeModelPrivate(parent), parent)
{
    Q_D(QRangeModel);
    d->init();
}

QRangeModel::QRangeModel(QGraphicsObject *parent)
    : QObject(*new QRangeModelPrivate(parent), parent)
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
        //sliderChange(SliderRangeChange);
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
        sliderChange(SliderRangeChange);
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
    Q_Q(QRangeModel);
    singleStep = qAbs(single);
    pageStep = qAbs(page);
    q->sliderChange(QRangeModel::SliderStepsChange);
}

void QRangeModel::setOrientation(Qt::Orientation orientation)
{
    Q_D(QRangeModel);
    if (d->orientation == orientation)
        return;

    d->orientation = orientation;
}

Qt::Orientation QRangeModel::orientation() const
{
    Q_D(const QRangeModel);
    return d->orientation;
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
    if (!d->tracking)
        d->update();
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
#ifndef QT_NO_ACCESSIBILITY
    QAccessible::updateAccessibility(this, 0, QAccessible::ValueChanged);
#endif
    sliderChange(SliderValueChange);
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

bool QRangeModel::invertedAppearance() const
{
    Q_D(const QRangeModel);
    return d->invertedAppearance;
}

void QRangeModel::setInvertedAppearance(bool invert)
{
    Q_D(QRangeModel);
    d->invertedAppearance = invert;
    d->update();
}

bool QRangeModel::invertedControls() const
{
    Q_D(const QRangeModel);
    return d->invertedControls;
}

void QRangeModel::setInvertedControls(bool invert)
{
    Q_D(QRangeModel);
    d->invertedControls = invert;
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

void QRangeModel::sliderChange(SliderChange)
{
    Q_D(QRangeModel);
    d->update();
}

#ifndef QT_NO_WHEELEVENT

void QRangeModel::wheelEvent(QEvent * e)
{
    Q_D(QRangeModel);
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

void QRangeModelPrivate::wheelEventHandler(QEvent *e, int delta, Qt::KeyboardModifiers modifiers)
{
    Q_Q(QRangeModel);
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
    q->triggerAction(QRangeModel::SliderMove);

    if (prevValue == value)
        offset_accumulated = 0;
    else
        e->accept();
}

#endif

void QRangeModel::keyPressEvent(QEvent *ev)
{
    Q_D(QRangeModel);
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

void QRangeModel::saveValue()
{
    d->origValue = d->value;
}
#endif // QT_KEYPAD_NAVIGATION

void QRangeModelPrivate::keyPressEventHandler(QKeyEvent *ev)
{
    Q_Q(QRangeModel);
    QRangeModel::SliderAction action = QRangeModel::SliderNoAction;
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
                action = d->invertedControls ? QRangeModel::SliderSingleStepSub : QRangeModel::SliderSingleStepAdd;
            else
#endif
            if (isRightToLeft())
                action = invertedAppearance ? QRangeModel::SliderSingleStepSub : QRangeModel::SliderSingleStepAdd;
            else
                action = !invertedAppearance ? QRangeModel::SliderSingleStepSub : QRangeModel::SliderSingleStepAdd;
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
                action = invertedControls ? QRangeModel::SliderSingleStepAdd : QRangeModel::SliderSingleStepSub;
            else
#endif
            if (isRightToLeft())
                action = invertedAppearance ? QRangeModel::SliderSingleStepAdd : QRangeModel::SliderSingleStepSub;
            else
                action = !invertedAppearance ? QRangeModel::SliderSingleStepAdd : QRangeModel::SliderSingleStepSub;
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
            action = invertedControls ? QRangeModel::SliderSingleStepSub : QRangeModel::SliderSingleStepAdd;
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
            action = invertedControls ? QRangeModel::SliderSingleStepAdd : QRangeModel::SliderSingleStepSub;
            break;
        case Qt::Key_PageUp:
            action = invertedControls ? QRangeModel::SliderPageStepSub : QRangeModel::SliderPageStepAdd;
            break;
        case Qt::Key_PageDown:
            action = invertedControls ? QRangeModel::SliderPageStepAdd : QRangeModel::SliderPageStepSub;
            break;
        case Qt::Key_Home:
            action = QRangeModel::SliderToMinimum;
            break;
        case Qt::Key_End:
            action = QRangeModel::SliderToMaximum;
            break;
        default:
            ev->ignore();
            break;
    }
    if (action)
        q->triggerAction(action);
}

void QRangeModel::changeEvent(QEvent *ev)
{
    Q_D(QRangeModel);
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
