/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mdeclarativemousefilter.h"
#include <QtCore/QEvent>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include <QGraphicsScene>
#include <QDebug>

static const int PressAndHoldDelay = 800;
// Threshold in QDeclarativeFlickable is 20, for press and hold we'll need 30 (30*30=900)
static const int FlickThresholdSquare = 900;

MDeclarativeMouseFilter::MDeclarativeMouseFilter(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    pressAndHoldTimerId(-1),
    delayedPressEvent(0),
    lastPos(QPoint(0,0))
{
    setFlag(ItemHasNoContents, true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

MDeclarativeMouseFilter::~MDeclarativeMouseFilter()
{
    if(delayedPressEvent) {
        delete delayedPressEvent;
        delayedPressEvent = 0;
    }
}

void MDeclarativeMouseFilter::itemChange(ItemChange change, const ItemChangeData &)
{
    if (change == QQuickItem::ItemParentHasChanged || change == QQuickItem::ItemSceneChange) {
        if (parentItem()) {
            parentItem()->setFiltersChildMouseEvents(true);
            setKeepMouseGrab(true);
        }
    }
}

bool MDeclarativeMouseFilter::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::GraphicsSceneMouseDoubleClick: {
        QGraphicsSceneMouseEvent *me = static_cast <QGraphicsSceneMouseEvent *>(event);
        clampMousePosition(me);

        QPointF mappedPos = static_cast<QGraphicsItem*>(parentItem())->mapFromItem(this, me->pos());
        MDeclarativeMouseEvent mdme(mappedPos);
        emit doubleClicked(&mdme);
        if (mdme.isFiltered())
            return true;
        break;
    }
    case QEvent::GraphicsSceneMousePress: {
        QGraphicsSceneMouseEvent *me = static_cast <QGraphicsSceneMouseEvent *>(event);
        clampMousePosition(me);

        QPointF mappedPos = static_cast<QGraphicsItem*>(parentItem())->mapFromItem(this, me->pos());
        lastPos.setX(mappedPos.x());
        lastPos.setY(mappedPos.y());
        setKeepMouseGrab(true);
        if (delayedPressEvent) {
            delete delayedPressEvent;
            delayedPressEvent = 0;
        }
        delayedPressEvent = copyMouseEvent(me);
        // Launch press and hold timer
        pressAndHoldTimerId = startTimer(PressAndHoldDelay);
        MDeclarativeMouseEvent mdme(mappedPos);
        emit pressed(&mdme);
        if (mdme.isFiltered()) {
            me->accept();
            return true;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseMove: {
        QGraphicsSceneMouseEvent *me = static_cast <QGraphicsSceneMouseEvent *>(event);
        QPointF dist = me->pos() - me->buttonDownPos(Qt::LeftButton);

        clampMousePosition(me);

        QPointF mappedPos = static_cast<QGraphicsItem*>(parentItem())->mapFromItem(this, me->pos());
        lastPos.setX(mappedPos.x());
        lastPos.setY(mappedPos.y());
        MDeclarativeMouseEvent mdme(mappedPos);

        if (pressAndHoldTimerId != -1 && (dist.x() * dist.x()) + (dist.y() * dist.y()) > FlickThresholdSquare) {
            killTimer(pressAndHoldTimerId);
            pressAndHoldTimerId = -1;
            if (abs(dist.x()) < abs(dist.y())) {
                setKeepMouseGrab(false);
                if (scene() && parentItem() && delayedPressEvent) {
                    scene()->sendEvent(parentItem(), delayedPressEvent);
                    emit delayedPressSent();
                    return true;
                }
            } else {
                emit horizontalDrag();

                // Reset the mouse sequence to apply selectByMouse changes from QML side
                if (scene() && parentItem() && delayedPressEvent) {
                    scene()->sendEvent(parentItem(), delayedPressEvent);
                    return true;
                }
            }
        } else if (pressAndHoldTimerId == -1){
            emit mousePositionChanged(&mdme);
            if (mdme.isFiltered())
                return true;
        }
        if (pressAndHoldTimerId != -1) {
            return true;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseRelease: {
        if (pressAndHoldTimerId != -1) {
            killTimer(pressAndHoldTimerId);
            pressAndHoldTimerId=-1;
        }
        QGraphicsSceneMouseEvent *me = static_cast <QGraphicsSceneMouseEvent *>(event);
        clampMousePosition(me);

        QPointF mappedPos = static_cast<QGraphicsItem*>(parentItem())->mapFromItem(this, me->pos());
        MDeclarativeMouseEvent mdme(mappedPos);

        emit released(&mdme);
        if (mdme.isFiltered()) {
            emit finished();
            return true;            
        }
        bool ret = QDeclarativeItem::event(event);
        emit finished();
        return ret;
    }
    case QEvent::UngrabMouse: {
        if (pressAndHoldTimerId != -1) {
            killTimer(pressAndHoldTimerId);
            pressAndHoldTimerId=-1;
        }

        setKeepMouseGrab(true);
        break;
    }
    default:
        break;
    }

    return QDeclarativeItem::event(event);
}

void MDeclarativeMouseFilter::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() == pressAndHoldTimerId) {
        killTimer(pressAndHoldTimerId);
        pressAndHoldTimerId = -1;
        MDeclarativeMouseEvent mdme(lastPos);

        emit pressAndHold(&mdme);
    }
}


QGraphicsSceneMouseEvent *MDeclarativeMouseFilter::copyMouseEvent (QGraphicsSceneMouseEvent *event)
{
    QGraphicsSceneMouseEvent *ret;
    ret = new QGraphicsSceneMouseEvent(event->type());
    ret->setAccepted(false);
    for (int i = 0x1; i <= 0x10; i <<= 1) {
        if (event->buttons() & i) {
            Qt::MouseButton button = Qt::MouseButton(i);
            ret->setButtonDownPos(button, QGraphicsItem::mapToItem(parentItem(),event->buttonDownPos(button)));
            ret->setButtonDownScenePos(button, event->buttonDownScenePos(button));
            ret->setButtonDownScreenPos(button, event->buttonDownScreenPos(button));
        }
    }
    ret->setButtons(event->buttons());
    ret->setButton(event->button());
    ret->setPos(QGraphicsItem::mapToItem(parentItem(),event->pos()));
    ret->setScenePos(event->scenePos());
    ret->setScreenPos(event->screenPos());
    ret->setLastPos(QGraphicsItem::mapToItem(parentItem(),event->lastPos()));
    ret->setLastScenePos(event->lastScenePos());
    ret->setLastScreenPos(event->lastScreenPos());
    ret->setModifiers(event->modifiers());
    return ret;
}

void MDeclarativeMouseFilter::clampMousePosition(QGraphicsSceneMouseEvent *me)
{
    // Clamp position in MouseFilter coordinates.
    // X position is clamped to MouseFilter area.
    // Y position is clamped to parent item area.
    QRectF targetRect = QRectF(0,-y(),width(),parentItem()->height());
    me->setPos(QPointF(qBound(targetRect.x(), me->pos().x(),targetRect.x()+targetRect.width()),
        qBound(targetRect.y(), me->pos().y(), targetRect.y()+targetRect.height()-1)));
    me->setLastPos(QPointF(qBound(targetRect.x(), me->lastPos().x(),targetRect.x()+targetRect.width()),
        qBound(targetRect.y(), me->lastPos().y(),targetRect.y()+targetRect.height()-1)));
    me->setButtonDownPos(Qt::LeftButton, QPointF(qBound(targetRect.x(), me->buttonDownPos(Qt::LeftButton).x(), targetRect.x()+targetRect.width()),
        qBound(targetRect.y(), me->buttonDownPos(Qt::LeftButton).y(), targetRect.y()+targetRect.height()-1)));
}
