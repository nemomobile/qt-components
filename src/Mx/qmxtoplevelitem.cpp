/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include "qmxtoplevelitem.h"
#include "qmxtoplevelitem_p.h"

#include <QGraphicsScene>

QMxTopLevelItemPrivate::QMxTopLevelItemPrivate() :
    targetItem(0), transformDirty(0), keepInside(0)
{
}

QMxTopLevelItemPrivate::~QMxTopLevelItemPrivate()
{
}

void QMxTopLevelItemPrivate::clearDependencyList()
{
    Q_Q(QMxTopLevelItem);
    for (int i = dependencyList.count() - 1; i >= 0; --i) {
        dependencyList.takeAt(i)->disconnect(this);
    }
    q->setTransform(QTransform());
    q->setOpacity(1);
    q->setVisible(false);
}

/*!
  \internal

  Set data bindings between the TopLevelItem and all the items it depend upon,
  including the targetItem and its ancestors.
*/
void QMxTopLevelItemPrivate::initDependencyList()
{
    Q_Q(QMxTopLevelItem);

    if (!targetItem || !targetItem->parentItem())
        return;

    // ### We are not listening for childrenChange signals in our parent
    // but that seems a little bit too much overhead. It can be done if
    // required in the future.
    setZFromSiblings();

    // The width of the TopLevelItem is the width of the targetItem
    connect(targetItem, SIGNAL(widthChanged()), SLOT(updateWidthFromTarget()));
    connect(targetItem, SIGNAL(heightChanged()), SLOT(updateHeightFromTarget()));

    // Now bind to events that may change the position and/or transforms of
    // the TopLevelItem

    // ### If we had access to QDeclarativeItem private we could do this
    //     in a better way, by adding ourselves to the changeListeners list
    //     in each item.
    //     The benefit is that we could update our data based on the change
    //     rather than having to recalculate the whole tree.

    QDeclarativeItem *item = targetItem;
    while (item->parentItem()) {
        dependencyList << item;

        // We listen for events that can change the visibility and/or geometry
        // of the targetItem or its ancestors.
        connect(item, SIGNAL(opacityChanged()), SLOT(updateOpacity()));
        connect(item, SIGNAL(visibleChanged()), SLOT(updateVisible()));

        // ### We are not listening to changes in the "transform" property

        // 'updateTransform' may be expensive, so instead of calling it several
        // times, we call the schedule method instead, that also compresses
        // these events.
        connect(item, SIGNAL(xChanged()), SLOT(scheduleUpdateTransform()));
        connect(item, SIGNAL(yChanged()), SLOT(scheduleUpdateTransform()));
        connect(item, SIGNAL(rotationChanged()), SLOT(scheduleUpdateTransform()));
        connect(item, SIGNAL(scaleChanged()), SLOT(scheduleUpdateTransform()));
        connect(item, SIGNAL(transformOriginChanged(TransformOrigin)), SLOT(scheduleUpdateTransform()));

        // parentChanged() may be emitted from destructors and other sensible code regions.
        // By making this connection Queued we wait for the control to reach the event loop
        // allow for the scene to be in a stable state before doing our changes.
        connect(item, SIGNAL(parentChanged()), SLOT(updateParent()), Qt::QueuedConnection);

        item = item->parentItem();
    }

    // 'item' is the root item in the scene, make it our parent
    q->setParentItem(item);

    // Note that we did not connect to signals regarding opacity, visibility or
    // transform changes of our parent, that's because we take that effect
    // automatically, as it is our parent

    // OTOH we need to listen to changes in its size to re-evaluate the keep-inside
    // functionality.
    connect(item, SIGNAL(widthChanged()), SLOT(updateWidthFromTarget()));

    // Call slots for the first time
    updateHeightFromTarget();
    updateTransform();
    updateOpacity();
    updateVisible();
}

void QMxTopLevelItemPrivate::scheduleUpdateTransform()
{
    if (transformDirty)
        return;

    transformDirty = 1;
    QMetaObject::invokeMethod(this, "updateTransform", Qt::QueuedConnection);
}

void QMxTopLevelItemPrivate::updateTransform()
{
    Q_ASSERT(targetItem);
    Q_Q(QMxTopLevelItem);
    q->setTransform(targetItem->itemTransform(q->parentItem()));
    updateWidthFromTarget();
    transformDirty = 0;
}

void QMxTopLevelItemPrivate::updateOpacity()
{
    Q_ASSERT(targetItem);
    Q_Q(QMxTopLevelItem);
    q->setOpacity(targetItem->effectiveOpacity());
}

void QMxTopLevelItemPrivate::updateVisible()
{
    Q_ASSERT(targetItem);
    Q_Q(QMxTopLevelItem);
    q->setVisible(targetItem->isVisibleTo(q->parentItem()));
}

void QMxTopLevelItemPrivate::updateParent()
{
    Q_ASSERT(targetItem);
    clearDependencyList();
    initDependencyList();
}

void QMxTopLevelItemPrivate::updateWidthFromTarget()
{
    Q_ASSERT(targetItem);
    Q_Q(QMxTopLevelItem);

    // Reset position and size to those of the targetItem
    qreal newX = 0;
    qreal newWidth = targetItem->width();

    if (!keepInside) {
        q->setX(newX);
        q->setWidth(newWidth);
        return;
    }

    const QTransform screenToParentTransform = q->parentItem()->sceneTransform().inverted();
    const QTransform itemToParentTransform = q->QGraphicsItem::transform();

    if (screenToParentTransform.isRotating() || itemToParentTransform.isRotating()) {
        qWarning() << "QMxTopLevelItem: KeepInside feature is not supported together with rotation transforms";
        q->setX(newX);
        q->setWidth(newWidth);
        return;
    }

    // We use a compromise solution here. The real deal is that we have a scene with items
    // and possibly one or more views to that scene. We don't know exactly where these
    // views are, but we need a reference to constrain the items.
    // So we assume from current "qml runtime" implementation that usually the scene
    // has only one view and that view has the size of the root-item in the scene.
    const qreal screenWidth = q->parentItem()->width();

    // Now we map to _root item coordinates_ the following info:
    //  1) The edges of the "screen".
    //  2) The edges of our transformed item, ie. the place where our targetItem is.
    //  3) The respective widths
    const qreal leftScreenEdgeAtParent = screenToParentTransform.map(QPointF(0, 0)).x();
    const qreal rightScreenEdgeAtParent = screenToParentTransform.map(QPointF(screenWidth, 0)).x();
    const qreal leftItemEdgeAtParent = itemToParentTransform.map(QPointF(0, 0)).x();
    const qreal rightItemEdgeAtParent = itemToParentTransform.map(QPointF(newWidth, 0)).x();
    const qreal screenWidthAtParent = rightScreenEdgeAtParent - leftScreenEdgeAtParent;
    const qreal itemWidthAtParent = rightItemEdgeAtParent - leftItemEdgeAtParent;

    // Now with all sizes in the same coordinate system we can apply offsets to
    // our item's width and/or position to keep it inside the screen.
    if (itemWidthAtParent > screenWidthAtParent) {
        // Is the item too large?
        newWidth *= screenWidthAtParent / itemWidthAtParent;
        newX = leftScreenEdgeAtParent - leftItemEdgeAtParent;
    } else if (leftScreenEdgeAtParent > leftItemEdgeAtParent) {
        // Does item go beyond left edge ?
        newX = leftScreenEdgeAtParent - leftItemEdgeAtParent;
    } else if (rightItemEdgeAtParent > rightScreenEdgeAtParent) {
        // Does item go beyond right edge ?
        newX = rightScreenEdgeAtParent - rightItemEdgeAtParent;
    }

    q->setX(newX);
    q->setWidth(newWidth);
}

void QMxTopLevelItemPrivate::updateHeightFromTarget()
{
    Q_ASSERT(targetItem);
    Q_Q(QMxTopLevelItem);
    q->setHeight(targetItem->height());
}

void QMxTopLevelItemPrivate::setZFromSiblings()
{
    Q_Q(QMxTopLevelItem);
    int maxZ = 0;

    const QList<QGraphicsItem *> siblings = q->parentItem()->childItems();
    for (int i = siblings.count() - 1; i >= 0; --i) {
        // Skip other topLevelItems
        QGraphicsObject *obj = siblings[i]->toGraphicsObject();
        if (qobject_cast<QMxTopLevelItem *>(obj))
            continue;

        if (siblings[i]->zValue() > maxZ)
            maxZ = siblings[i]->zValue();
    }

    q->setZValue(maxZ + 1);
}

QMxTopLevelItem::QMxTopLevelItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), d_ptr(new QMxTopLevelItemPrivate)
{
    d_ptr->q_ptr = this;
}

QMxTopLevelItem::QMxTopLevelItem(QMxTopLevelItemPrivate &dd, QDeclarativeItem *parent) :
    QDeclarativeItem(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

QMxTopLevelItem::~QMxTopLevelItem()
{

}

bool QMxTopLevelItem::keepInside() const
{
    Q_D(const QMxTopLevelItem);
    return d->keepInside;
}

void QMxTopLevelItem::setKeepInside(bool keepInside)
{
    Q_D(QMxTopLevelItem);
    if (keepInside == d->keepInside)
        return;

    d->keepInside = keepInside;

    if (d->targetItem)
        d->updateWidthFromTarget();
    emit keepInsideChanged(keepInside);
}

QVariant QMxTopLevelItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(QMxTopLevelItem);

    if (d->targetItem == 0) {
        // The original parent of this item (declared in QML) will be our
        // 'target' item, ie, the item which size and position we will follow.

        // ### To find that parent, we could listen to ItemParentChange but that
        // does not work due to the fact QDeclarativeItem::data adds children
        // w/o trigerring such event. (QGraphicsItem::get(child)->setParentItemHelper())
        // Our workarround is to assume that the parent is set before the item is
        // added to the scene, so we listen for the latter and get the info we need.
        if (change == ItemSceneHasChanged)
            d->targetItem = parentItem();

        // Let the changes be finished before we start initDependencyList
        QMetaObject::invokeMethod(d, "initDependencyList", Qt::QueuedConnection);
    }
    return QDeclarativeItem::itemChange(change, value);
}
