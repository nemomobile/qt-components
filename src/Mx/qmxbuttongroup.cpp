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

#include "qmxbuttongroup.h"

QMxButtonGroup::QMxButtonGroup(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), m_checkedItem(0), m_allowNoChecked(false)
{

}

QMxButtonGroup::~QMxButtonGroup()
{

}

void QMxButtonGroup::refresh()
{
    // ### Using brute-force here, we could do better...
    for (int i = 0; i < m_items.count(); i++) {
        QObject::disconnect(m_items[i], SIGNAL(clicked()), this, SLOT(onItemChecked()));
        QObject::disconnect(m_items[i], SIGNAL(destroyed()), this, SLOT(onItemDestroyed()));
    }
    m_items.clear();

    QDeclarativeItem *oldCheckedItem = m_checkedItem;
    m_checkedItem = 0;

    // Reconnect everyone
    connectChildrenItems(this);

    if (m_checkedItem != oldCheckedItem) {
        emit checkedItemChanged(m_checkedItem);
    }
}

void QMxButtonGroup::componentComplete()
{
    QDeclarativeItem::componentComplete();
    connectChildrenItems(this);

    // Emit only once after the checked item was decided (or don't if no
    // was checked).
    if (m_checkedItem) {
        emit checkedItemChanged(m_checkedItem);
    }
}

static bool isCheckableButton(QDeclarativeItem *item)
{
    // ### "Duck-typing" to identify button
    return (item->metaObject()->indexOfProperty("checked") != -1)
        && (item->metaObject()->indexOfProperty("checkable") != -1)
        && (item->metaObject()->indexOfSignal("clicked()") != -1);
}

void QMxButtonGroup::connectChildrenItems(QDeclarativeItem *item) {
    // ### We could do things in a different way, see related discussion in "ButtonModel"

    // Iterate through all graphics children, this includes QDeclarativeItem's.
    QList<QGraphicsItem *> graphicsChildren = item->childItems();
    for (int i = 0; i < graphicsChildren.count(); i++) {
        QGraphicsObject *o = graphicsChildren[i]->toGraphicsObject();
        if (!o) {
            continue;
        }

        // ### Only support QDI for now...
        QDeclarativeItem *child = qobject_cast<QDeclarativeItem *>(o);
        if (!child) {
            continue;
        }

        if (isCheckableButton(child)) {
            QObject::connect(child, SIGNAL(clicked()), this, SLOT(onItemChecked()));
            QObject::connect(child, SIGNAL(destroyed()), this, SLOT(onItemDestroyed()));
            m_items.append(child);

            // ### When multiple checked, the last will be the one used
            if (child->property("checked").toBool()) {
                if (m_checkedItem) {
                    m_checkedItem->setProperty("checked", false);
                }
                m_checkedItem = child;
            }

        } else if (child->childItems().count() > 0) {
            // If the child has more children, recurse
            connectChildrenItems(child);
        }

    }
}

void QMxButtonGroup::onItemChecked()
{
    // Just we connected in this slot, and we just connected QDItem
    QDeclarativeItem *item = static_cast<QDeclarativeItem *>(QObject::sender());

    // Non-checkable buttons do not affect other buttons
    if (!item->property("checkable").toBool()) {
        return;
    }

    // ### we rely on clicked happening after property 'checked' changed.

    // Item was checked! So we know it's different than m_checkedItem
    if (item->property("checked").toBool()) {
        // Uncheck the old checked item
        if (m_checkedItem) {
            m_checkedItem->setProperty("checked", false);
        }

        m_checkedItem = item;
        emit checkedItemChanged(m_checkedItem);

    } else {
        // Item was unchecked! So we know it's the m_checkedItem that was clicked...
        if (m_allowNoChecked) {
            m_checkedItem = 0;
            emit checkedItemChanged(m_checkedItem);
        } else {
            m_checkedItem->setProperty("checked", true);
        }
    }
}

void QMxButtonGroup::onItemDestroyed()
{
    // Just we connected in this slot, and we just connected QDItem
    QDeclarativeItem *item = static_cast<QDeclarativeItem *>(QObject::sender());
    m_items.removeOne(item);

    if (item == m_checkedItem) {
        m_checkedItem = 0;
        emit checkedItemChanged(m_checkedItem);
    }
}
