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

#include "ssnapshot.h"
#include <qpainter.h>
#include <qgraphicsscene.h>

Snapshot::Snapshot(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
    , m_width(0)
    , m_height(0)
{
    setFlag(ItemHasNoContents, false);
    setFlag(ItemIgnoresParentOpacity, true);
    setOpacity(0);
}

Snapshot::~Snapshot()
{
}

void Snapshot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    if (smooth()) {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    } else {
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    }

    painter->drawPixmap(0, 0, m_snapshot);
    painter->restore();
}

void Snapshot::take()
{
    QGraphicsScene *s = scene();
    if (!s)
        return;

    m_snapshot = QPixmap(width(), height());
    QPainter painter(&m_snapshot);
    QRectF r(0, 0, snapshotWidth(), snapshotHeight());
    s->render(&painter, r, r);
}

void Snapshot::free()
{
    m_snapshot = QPixmap();
}

void Snapshot::setSnapshotWidth(int width)
{
    if (m_width == width)
        return;

    m_width = width;
    emit snapshotWidthChanged();
}

void Snapshot::setSnapshotHeight(int height)
{
    if (m_height == height)
        return;

    m_height = height;
    emit snapshotHeightChanged();
}

int Snapshot::snapshotWidth() const
{
    return m_width;
}

int Snapshot::snapshotHeight() const
{
    return m_height;
}
