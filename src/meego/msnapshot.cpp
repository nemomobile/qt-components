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

#include "msnapshot.h"
#include <qpainter.h>
#include <qgraphicsscene.h>

MSnapshot::MSnapshot(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), sWidth(0), sHeight(0)
{
    setFlag(ItemHasNoContents, false);
    setFlag(ItemIgnoresParentOpacity, true);
    setOpacity(0);
}

MSnapshot::~MSnapshot()
{
}

void MSnapshot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->drawPixmap(0, 0, snapshot);
    painter->restore();
}

void MSnapshot::take()
{
    QGraphicsScene *s = scene();
    if (!s)
        return;
    snapshot = QPixmap(width(), height());
    QPainter painter(&snapshot);
    QRectF r(0, 0, snapshotWidth(), snapshotHeight());
    s->render(&painter, r, r);
}

void MSnapshot::free()
{
    snapshot = QPixmap();
}

void MSnapshot::setSnapshotWidth(int width)
{
    if (sWidth != width) {
        sWidth = width;
        emit snapshotWidthChanged();
    }
}

void MSnapshot::setSnapshotHeight(int height)
{
    if (sHeight != height) {
        sHeight = height;
        emit snapshotHeightChanged();
    }
}

int MSnapshot::snapshotWidth() const
{
    return sWidth;
}

int MSnapshot::snapshotHeight() const
{
    return sHeight;
}
