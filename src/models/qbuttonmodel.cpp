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

#include "qbuttonmodel.h"
#include "qapplication.h"
#include "qbuttonmodel_p.h"
#include "qevent.h"
#include "qdebug.h"
#ifndef QT_NO_ACCESSIBILITY
#include "qaccessible.h"
#endif

QT_BEGIN_NAMESPACE

#define AUTO_REPEAT_DELAY  300
#define AUTO_REPEAT_INTERVAL 100

/*!
    \internal
*/
QButtonModelPrivate::QButtonModelPrivate() :
    QObjectPrivate(),
#ifndef QT_NO_SHORTCUT
    shortcutId(0),
#endif
    checkable(false), checked(false), autoRepeat(false), autoExclusive(false),
    buttonDown(false), mousePressed(false), mouseOver(false),
#ifndef QT_NO_BUTTONGROUP
    group(0),
#endif
    autoRepeatDelay(AUTO_REPEAT_DELAY),
    autoRepeatInterval(AUTO_REPEAT_INTERVAL)
{
}

void QButtonModelPrivate::init()
{
}

void QButtonModelPrivate::emitClicked()
{
    Q_Q(QButtonModel);
    QPointer<QButtonModel> guard(q);
    emit q->clicked(checked);
#ifndef QT_NO_BUTTONGROUP
    if (guard && group) {
        emit group->buttonClicked(group->id(button));
        if (guard && group)
            emit group->buttonClicked(button);
    }
#endif
}

void QButtonModelPrivate::emitPressed()
{
    Q_Q(QButtonModel);
    QPointer<QButtonModel> guard(q);
    emit q->pressed();
#ifndef QT_NO_BUTTONGROUP
    if (guard && group) {
        emit group->buttonPressed(group->id(button));
        if (guard && group)
            emit group->buttonPressed(button);
    }
#endif
}

void QButtonModelPrivate::emitReleased()
{
    Q_Q(QButtonModel);
    QPointer<QButtonModel> guard(q);
    emit q->released();
#ifndef QT_NO_BUTTONGROUP
    if (guard && group) {
        emit group->buttonReleased(group->id(button));
        if (guard && group)
            emit group->buttonReleased(button);
    }
#endif
}

QList<QButtonModel *>QButtonModelPrivate::queryButtonList() const
{
    Q_Q(const QButtonModel);

#ifndef QT_NO_BUTTONGROUP
    if (group)
        return group->d_func()->buttonList;
#endif

    // ##### this relies on the assumption that the following
    //       _QObject_ tree exists:
    //
    //                    (Parent Canvas Item)
    //        (CanvasButton 1) --/   \-- (CanvasButton 2)
    //  (Model 1) --/                             \-- (Model 2)

    QList<QButtonModel *> candidates;

    // Go up
    const QObject *canvasButton = q->parent();
    if (!canvasButton)
        return candidates;

    const QObject *canvasParent = canvasButton->parent();
    if (!canvasParent)
        return candidates;

    // Go down
    const QObjectList otherItems = canvasParent->children();
    foreach (QObject *item, otherItems) {
        const QObjectList itemChildren = item->children();
        foreach(QObject *childItem, itemChildren) {
            QButtonModel *otherModel = qobject_cast<QButtonModel *>(childItem);
            if (otherModel && otherModel->autoExclusive())
                candidates << otherModel;
        }
    }
    return candidates;
}

QButtonModel *QButtonModelPrivate::queryCheckedButton() const
{
#ifndef QT_NO_BUTTONGROUP
    if (group)
        return group->d_func()->checkedButton;
#endif

    Q_Q(const QButtonModel);
    QList<QButtonModel *> buttonList = queryButtonList();
    if (!autoExclusive || buttonList.count() == 1) // no group
        return 0;

    for (int i = 0; i < buttonList.count(); ++i) {
        QButtonModel *b = buttonList.at(i);
        if (b->d_func()->checked && b != q)
            return b;
    }
    return checked ? const_cast<QButtonModel *>(q) : 0;
}

void QButtonModelPrivate::notifyChecked() const
{
#ifndef QT_NO_BUTTONGROUP
    Q_Q(QButtonModel);
    if (group) {
        QButtonModel *previous = group->d_func()->checkedButton;
        group->d_func()->checkedButton = q;
        if (group->d_func()->exclusive && previous && previous != q)
            previous->nextCheckState();
    } else
#endif
    if (autoExclusive) {
        if (QButtonModel *b = queryCheckedButton())
            b->setChecked(false);
    }
}

void QButtonModelPrivate::click()
{
    Q_Q(QButtonModel);

    buttonDown = false;
    bool changeState = true;
    if (checked && queryCheckedButton() == q) {
        // the checked button of an exclusive or autoexclusive group cannot be unchecked
#ifndef QT_NO_BUTTONGROUP
        if (group ? group->d_func()->exclusive : autoExclusive)
#else
        if (autoExclusive)
#endif
            changeState = false;
    }

    QPointer<QButtonModel> guard(q);

    if (changeState) {
        q->nextCheckState();
        if (!guard)
            return;
    }

    if (guard)
        emitReleased();
    if (guard)
        emitClicked();
}

/*!
    Constructs an button control with a \a parent.
*/
QButtonModel::QButtonModel(QObject *parent)
    : QObject(*new QButtonModelPrivate(), parent)
{
    Q_D(QButtonModel);
    d->init();
}

/*!
    \internal
*/
QButtonModel::QButtonModel(QButtonModelPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(QButtonModel);
    d->init();
}

/*!
    Destroys the button control.
*/
QButtonModel::~QButtonModel()
{
}

#ifndef QT_NO_SHORTCUT
/*!
\property QButtonModel::shortcut
\brief the mnemonic associated with the button
*/

void QButtonModel::setShortcut(const QKeySequence &key)
{
    Q_D(QButtonModel);
    if (d->shortcutId != 0)
        releaseShortcut(d->shortcutId);
    d->shortcut = key;
    d->shortcutId = grabShortcut(key);
}

QKeySequence QButtonModel::shortcut() const
{
    Q_D(const QButtonModel);
    return d->shortcut;
}
#endif // QT_NO_SHORTCUT

/*!
\property QButtonModel::checkable
\brief whether the button is checkable

By default, the button is not checkable.

\sa checked
*/
void QButtonModel::setCheckable(bool checkable)
{
    Q_D(QButtonModel);
    if (d->checkable == checkable)
        return;

    d->checkable = checkable;

    // ### Forcing an order on how properties should be set cause
    // trouble to declarative. If we do both "checkable: true" and
    // "checked: true" and the other is reverse, we lose information.
    d->checked = false;
    emit checkableChanged();
}

bool QButtonModel::isCheckable() const
{
    Q_D(const QButtonModel);
    return d->checkable;
}

/*!
\property QButtonModel::checked
\brief whether the button is checked

Only checkable buttons can be checked. By default, the button is unchecked.

\sa checkable
*/
void QButtonModel::setChecked(bool checked)
{
    Q_D(QButtonModel);
    if (!d->checkable || d->checked == checked) {
//################################
//        if (!d->blockRefresh)
//            checkStateSet();
        return;
    }

    if (!checked && d->queryCheckedButton() == this) {
        // the checked button of an exclusive or autoexclusive group cannot be  unchecked
#ifndef QT_NO_BUTTONGROUP
        if (d->group ? d->group->d_func()->exclusive : d->autoExclusive)
            return;
        if (d->group)
            d->group->d_func()->detectCheckedButton();
#else
        if (d->autoExclusive)
            return;
#endif
    }

    QPointer<QButtonModel> guard(this);

    d->checked = checked;
    //##############################################
    // if (!d->blockRefresh)
    //     checkStateSet();

    if (guard && checked)
        d->notifyChecked();
    if (guard)
        emit toggled(checked);
}

bool QButtonModel::isChecked() const
{
    Q_D(const QButtonModel);
    return d->checked;
}

/*!
  \property QButtonModel::down
  \brief whether the button is pressed down

  If this property is true, the button is pressed down. The signals
  pressed() and clicked() are not emitted if you set this property
  to true. The default is false.
*/

void QButtonModel::setButtonDown(bool down)
{
    Q_D(QButtonModel);
    if (d->buttonDown == down)
        return;
    d->buttonDown = down;
    if (d->autoRepeat && d->buttonDown)
        d->repeatTimer.start(d->autoRepeatDelay, this);
    else
        d->repeatTimer.stop();
    emit buttonDownChanged();
}

bool QButtonModel::buttonDown() const
{
    Q_D(const QButtonModel);
    return d->buttonDown;
}

/*!
\property QButtonModel::autoRepeat
\brief whether autoRepeat is enabled

If autoRepeat is enabled, then the pressed(), released(), and clicked() signals are emitted at
regular intervals when the button is down. autoRepeat is off by default.
The initial delay and the repetition interval are defined in milliseconds by \l
autoRepeatDelay and \l autoRepeatInterval.

Note: If a button is pressed down by a shortcut key, then auto-repeat is enabled and timed by the
system and not by this class. The pressed(), released(), and clicked() signals will be emitted
like in the normal case.
*/

void QButtonModel::setAutoRepeat(bool autoRepeat)
{
    Q_D(QButtonModel);
    if (d->autoRepeat == autoRepeat)
        return;
    d->autoRepeat = autoRepeat;
    if (d->autoRepeat && d->buttonDown)
        d->repeatTimer.start(d->autoRepeatDelay, this);
    else
        d->repeatTimer.stop();
}

bool QButtonModel::autoRepeat() const
{
    Q_D(const QButtonModel);
    return d->autoRepeat;
}

/*!
    \property QButtonModel::autoRepeatDelay
    \brief the initial delay of auto-repetition
    \since 4.2

    If \l autoRepeat is enabled, then autoRepeatDelay defines the initial
    delay in milliseconds before auto-repetition kicks in.

    \sa autoRepeat, autoRepeatInterval
*/

void QButtonModel::setAutoRepeatDelay(int autoRepeatDelay)
{
    Q_D(QButtonModel);
    d->autoRepeatDelay = autoRepeatDelay;
}

int QButtonModel::autoRepeatDelay() const
{
    Q_D(const QButtonModel);
    return d->autoRepeatDelay;
}

/*!
    \property QButtonModel::autoRepeatInterval
    \brief the interval of auto-repetition
    \since 4.2

    If \l autoRepeat is enabled, then autoRepeatInterval defines the
    length of the auto-repetition interval in millisecons.

    \sa autoRepeat, autoRepeatDelay
*/

void QButtonModel::setAutoRepeatInterval(int autoRepeatInterval)
{
    Q_D(QButtonModel);
    d->autoRepeatInterval = autoRepeatInterval;
}

int QButtonModel::autoRepeatInterval() const
{
    Q_D(const QButtonModel);
    return d->autoRepeatInterval;
}



/*!
\property QButtonModel::autoExclusive
\brief whether auto-exclusivity is enabled

If auto-exclusivity is enabled, checkable buttons that belong to the
same parent widget behave as if they were part of the same
exclusive button group. In an exclusive button group, only one button
can be checked at any time; checking another button automatically
unchecks the previously checked one.

The property has no effect on buttons that belong to a button
group.

autoExclusive is off by default, except for radio buttons.

\sa QRadioButton
*/
void QButtonModel::setAutoExclusive(bool autoExclusive)
{
    Q_D(QButtonModel);
    d->autoExclusive = autoExclusive;
}

bool QButtonModel::autoExclusive() const
{
    Q_D(const QButtonModel);
    return d->autoExclusive;
}

#ifndef QT_NO_BUTTONGROUP
/*!
  Returns the group that this button belongs to.

  If the button is not a member of any QButtonGroup, this function
  returns 0.

  \sa QButtonGroup
*/
QButtonGroup *QButtonModel::group() const
{
    Q_D(const QButtonModel);
    return d->group;
}
#endif // QT_NO_BUTTONGROUP

void QButtonModel::setMousePressed(bool mousePressed)
{
    Q_D(QButtonModel);

    if (d->mousePressed == mousePressed)
        return;
    d->mousePressed = mousePressed;

    // We don't care about clicks outside the button
    if (d->mouseOver) {
        if (mousePressed) {
            // Press event
            setButtonDown(true);
            d->emitPressed();
        } else {
            // Release event
            setButtonDown(false);
            d->click();
        }
    }
    emit mousePressedChanged();
}

bool QButtonModel::mousePressed() const
{
    Q_D(const QButtonModel);
    return d->mousePressed;
}

void QButtonModel::setMouseOver(bool mouseOver)
{
    Q_D(QButtonModel);

    if (d->mouseOver == mouseOver)
        return;
    d->mouseOver = mouseOver;

    // We don't care about hover with mouse released
    if (d->mousePressed) {
        if (mouseOver) {
            // HoverIn event
            setButtonDown(true);
            d->emitPressed();
        } else {
            // HoverOut event
            setButtonDown(false);
            d->emitReleased();
        }
    }
    emit mouseOverChanged();
}

bool QButtonModel::mouseOver() const
{
    Q_D(const QButtonModel);
    return d->mouseOver;
}

void QButtonModel::nextCheckState()
{
    if (isCheckable())
        setChecked(!isChecked());
}

// void QButtonModel::mousePressEventHandler(QEvent *event)
// {
//     Q_D(QButtonModel);
//     if (event->type() == QEvent::MouseButtonPress) {
//         QMouseEvent *e = static_cast<QMouseEvent *>(event);
//         if (e->button() != Qt::LeftButton) {
//             e->ignore();
//             return;
//         }

//         if (d->button->hitButton(e->pos())) {
//             d->button->setDown(true);
//             d->pressed = true;
//             d->button->repaint(); //flush paint event before invoking potentially expensive operation
//             QApplication::flush();
//             d->emitPressed();
//             e->accept();
//         } else {
//             e->ignore();
//         }
//     } else if (event->type() == QEvent::GraphicsSceneMousePress) {
//         //QGV IMPLEMENTATION
//     }
// }

// void QButtonModel::mouseReleaseEventHandler(QEvent *event)
// {
//     Q_D(QButtonModel);
//     if (event->type() == QEvent::MouseButtonRelease) {
//         d->pressed = false;
//         QMouseEvent *e = static_cast<QMouseEvent *>(event);
//         if (e->button() != Qt::LeftButton) {
//             e->ignore();
//             return;
//         }

//         if (!d->down) {
//             e->ignore();
//             return;
//         }

//         if (d->button->hitButton(e->pos())) {
//             d->repeatTimer.stop();
//             d->click();
//             e->accept();
//         } else {
//             d->button->setDown(false);
//             e->ignore();
//         }
//     } else if(event->type() == QEvent::GraphicsSceneMouseRelease) {
//         //QGV IMPLEMENTATION
//     }
// }

// void QButtonModel::mouseMoveEventHandler(QEvent *event)
// {
//     Q_D(QButtonModel);
//     if (event->type() == QEvent::MouseMove) {
//         QMouseEvent *e = static_cast<QMouseEvent *>(event);
//         if (!(e->buttons() & Qt::LeftButton) || !d->pressed) {
//             e->ignore();
//             return;
//         }

//         if (d->button->hitButton(e->pos()) != d->down) {
//             d->button->setDown(!d->down);
//             d->button->repaint(); //flush paint event before invoking potentially expensive operation
//             QApplication::flush();
//             if (d->down)
//                 d->emitPressed();
//             else
//                 d->emitReleased();
//             e->accept();
//         } else if (!d->button->hitButton(e->pos())) {
//             e->ignore();
//         }
//     } else if(event->type() == QEvent::GraphicsSceneMouseMove) {
//         //QGV IMPLEMENTATION
//     }
// }

QT_END_NAMESPACE
