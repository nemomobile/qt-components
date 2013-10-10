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

#include "minversemousearea.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


// Threshold in QDeclarativeFlickable is 20
static const int FlickThresholdSquare = 400;

MInverseMouseArea::MInverseMouseArea(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_pressed(false),
      m_enabled(true)
{

}

MInverseMouseArea::~MInverseMouseArea()
{
    if (scene())
        scene()->removeEventFilter(this);
}

bool MInverseMouseArea::isEnabled() const
{
    return m_enabled;
}

void MInverseMouseArea::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        if (!enabled)
            m_pressed = false;

        m_enabled = enabled;
        emit enabledChanged();
    }
}

void MInverseMouseArea::itemChange(ItemChange change, const ItemChangeData &data)
{
    switch (change) {
    case QGraphicsItem::ItemSceneChange: {
        QGraphicsScene *oldScene = scene();

        if (oldScene)
            oldScene->removeEventFilter(this);

        m_pressed = false;

        if (QQuickWindow *newScene = data.window) {
            if (newScene)
                newScene->installEventFilter(this);
        }
        break;
    }
    case QGraphicsItem::ItemVisibleHasChanged: {
        if (!isVisible())
            m_pressed = false;
        break;
    }
    default:
        break;
    }

    QQuickItem::itemChange(change, data);
}

bool MInverseMouseArea::isClickedOnSoftwareInputPanel(QGraphicsSceneMouseEvent *event) const
{
    Q_UNUSED(event);
    // FIXME: How do we solve this with Qt5?
    return false;
}

bool MInverseMouseArea::eventFilter(QObject *obj, QEvent *ev)
{
    Q_UNUSED(obj);

    if (!m_enabled || !isVisible())
        return false;    
    switch (ev->type()) {
    case QEvent::GraphicsSceneMousePress: {
        QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent *>(ev);
        QPointF mappedPos = mapToRootItem(me->scenePos());

        m_pressed = !isUnderMouse() && !isClickedOnSoftwareInputPanel(me);

        if (m_pressed)
            emit pressedOutside(mappedPos.x(), mappedPos.y());
        break;
    }
    case QEvent::GraphicsSceneMouseMove: {
        if (m_pressed) {
            QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent *>(ev);
            const QPointF &dist = me->scenePos() - me->buttonDownScenePos(Qt::LeftButton);

            if (dist.x() * dist.x() + dist.y() * dist.y() > FlickThresholdSquare)
                m_pressed = false;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseRelease: {
        QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent *>(ev);
        QPointF mappedPos = mapToRootItem(me->scenePos());

        if (m_pressed) {
            m_pressed = false;
            emit clickedOutside(mappedPos.x(), mappedPos.y());
        }
        break;
    }
    default:
        break;
    }

    return false;
}

QPointF MInverseMouseArea::mapToRootItem(QPointF pos) {
    QPointF mappedPos = pos;
    QDeclarativeItem *rootItem = parentItem();

    while (rootItem->parentItem()) {
        if (rootItem->objectName() == "windowContent") {
            break;
        }
        rootItem = rootItem->parentItem();
    }

    if (rootItem) {
        mappedPos = rootItem->mapFromScene(pos);
    }

    return mappedPos;
}
