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

#include "qbuttonmodel.h"
#include "qapplication.h"
#include "qbuttonmodel_p.h"
#include "private/qabstractbutton_p.h"
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
QButtonModelPrivate::QButtonModelPrivate(QAbstractButton *button, QSizePolicy::ControlType type) :
    button(button),
#ifndef QT_NO_SHORTCUT
    shortcutId(0),
#endif
    checkable(false), checked(false), autoRepeat(false), autoExclusive(false),
    down(false), blockRefresh(false), pressed(false),
#ifndef QT_NO_BUTTONGROUP
    group(0),
#endif
    autoRepeatDelay(AUTO_REPEAT_DELAY),
    autoRepeatInterval(AUTO_REPEAT_INTERVAL),
    controlType(type)
{
}

/*!
    \internal
*/
QButtonModelPrivate::QButtonModelPrivate(QGraphicsObject *graphicsObject) : graphicsObject(graphicsObject)
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

QList<QAbstractButton *>QButtonModelPrivate::queryButtonList() const
{
#ifndef QT_NO_BUTTONGROUP
    if (group)
        return group->d_func()->buttonList;
#endif

    QList<QAbstractButton*>candidates = qFindChildren<QAbstractButton *>(parent);
    if (autoExclusive) {
        for (int i = candidates.count() - 1; i >= 0; --i) {
            QAbstractButton *candidate = candidates.at(i);
            if (!candidate->autoExclusive()
#ifndef QT_NO_BUTTONGROUP
                || candidate->group()
#endif
                )
                candidates.removeAt(i);
        }
    }
    return candidates;
}

QAbstractButton *QButtonModelPrivate::queryCheckedButton() const
{
#ifndef QT_NO_BUTTONGROUP
    if (group)
        return group->d_func()->checkedButton;
#endif

    QList<QAbstractButton *> buttonList = queryButtonList();
    if (!autoExclusive || buttonList.count() == 1) // no group
        return 0;

    for (int i = 0; i < buttonList.count(); ++i) {
        QAbstractButton *b = buttonList.at(i);
        if (b->d_func()->checked && b != button)
            return b;
    }
    return checked  ? const_cast<QAbstractButton *>(button) : 0;
}

void QButtonModelPrivate::click()
{
    Q_Q(QButtonModel);

    down = false;
    blockRefresh = true;
    bool changeState = true;
    if (checked && queryCheckedButton() == button) {
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
        button->nextCheckState();
        if (!guard)
            return;
    }
    blockRefresh = false;
    refresh();
    button->repaint(); //flush paint event before invoking potentially expensive operation
    QApplication::flush();
    if (guard)
        emitReleased();
    if (guard)
        emitClicked();
}

void QButtonModelPrivate::refresh()
{
    if (blockRefresh)
        return;
    button->update();
#ifndef QT_NO_ACCESSIBILITY
    QAccessible::updateAccessibility(button, 0, QAccessible::StateChanged);
#endif
}

/*!
    Constructs an button control with a \a parent.
*/
QButtonModel::QButtonModel(QAbstractButton *parent)
    : QObject(*new QButtonModelPrivate(parent), parent)
{
    Q_D(QButtonModel);
    d->init();
}

/*!
    Constructs an button control with a \a parent.
*/
QButtonModel::QButtonModel(QGraphicsObject *parent)
    : QObject(*new QButtonModelPrivate(parent), parent)
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

void QButtonModel::mousePressEventHandler(QEvent *event)
{
    Q_D(QButtonModel);
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (e->button() != Qt::LeftButton) {
            e->ignore();
            return;
        }

        if (d->button->hitButton(e->pos())) {
            d->button->setDown(true);
            d->pressed = true;
            d->button->repaint(); //flush paint event before invoking potentially expensive operation
            QApplication::flush();
            d->emitPressed();
            e->accept();
        } else {
            e->ignore();
        }
    } else if (event->type() == QEvent::GraphicsSceneMousePress) {
        //QGV IMPLEMENTATION
    }
}

void QButtonModel::mouseReleaseEventHandler(QEvent *event)
{
    Q_D(QButtonModel);
    if (event->type() == QEvent::MouseButtonRelease) {
        d->pressed = false;
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (e->button() != Qt::LeftButton) {
            e->ignore();
            return;
        }

        if (!d->down) {
            e->ignore();
            return;
        }

        if (d->button->hitButton(e->pos())) {
            d->repeatTimer.stop();
            d->click();
            e->accept();
        } else {
            d->button->setDown(false);
            e->ignore();
        }
    } else if(event->type() == QEvent::GraphicsSceneMouseRelease) {
        //QGV IMPLEMENTATION
    }
}

void QButtonModel::mouseMoveEventHandler(QEvent *event)
{
    Q_D(QButtonModel);
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (!(e->buttons() & Qt::LeftButton) || !d->pressed) {
            e->ignore();
            return;
        }

        if (d->button->hitButton(e->pos()) != d->down) {
            d->button->setDown(!d->down);
            d->button->repaint(); //flush paint event before invoking potentially expensive operation
            QApplication::flush();
            if (d->down)
                d->emitPressed();
            else
                d->emitReleased();
            e->accept();
        } else if (!d->button->hitButton(e->pos())) {
            e->ignore();
        }
    } else if(event->type() == QEvent::GraphicsSceneMouseMove) {
        //QGV IMPLEMENTATION
    }
}

QT_END_NAMESPACE
