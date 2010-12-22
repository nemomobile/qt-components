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

#include "sdeclarativeframe.h"
#include "sframepool.h"
#include "../sdeclarative.h"

#include <qcoreapplication.h>
#include <qpainter.h>
#include <qgraphicsscene.h>

//#define Q_DEBUG_FRAME
#ifdef Q_DEBUG_FRAME
#include <qdebug.h>
#endif

class SDeclarativeFramePrivate
{
public:
    SDeclarativeFramePrivate() :
        frameType(SDeclarativeFrame::Undefined),
        frameLoadAttempted(false) {}

    ~SDeclarativeFramePrivate() {}

    void releaseFromFramePool();

public:
    QPixmap framePixmap;
    QString frameName;
    QString fileName;
    SDeclarativeFrame::FrameType frameType;
    bool frameLoadAttempted;
    QSize loadedSize;
};

void SDeclarativeFramePrivate::releaseFromFramePool()
{
    // Release previously loaded pixmap from frame pool
    SFramePool::release(fileName, frameType, loadedSize);
    framePixmap = QPixmap();
    frameLoadAttempted = false;
}

SDeclarativeFrame::SDeclarativeFrame(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    d_ptr(new SDeclarativeFramePrivate)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

SDeclarativeFrame::~SDeclarativeFrame()
{
    Q_D(SDeclarativeFrame);
    // Release from icon pool
    if (d->frameLoadAttempted)
        d->releaseFromFramePool();
}

QString SDeclarativeFrame::frameName() const
{
    Q_D(const SDeclarativeFrame);
    return d->frameName;
}

void SDeclarativeFrame::setFrameName(const QString &frameName)
{
    Q_D(SDeclarativeFrame);
    if (frameName != d->frameName) {
        if (d->frameLoadAttempted) {
            // Release previous one from frame pool
            d->releaseFromFramePool();
        }

        d->fileName = SDeclarative::resolveIconFileName(frameName);
        d->frameName = frameName;
        update();
        emit frameNameChanged(frameName);
    }
}

SDeclarativeFrame::FrameType SDeclarativeFrame::frameType() const
{
    Q_D(const SDeclarativeFrame);
    return d->frameType;
}

void SDeclarativeFrame::setFrameType(SDeclarativeFrame::FrameType type)
{
    Q_D(SDeclarativeFrame);
    if (d->frameType != type) {
        if (d->frameLoadAttempted) {
            // Release previous one from frame pool
            d->releaseFromFramePool();
        }

        d->frameType = type;
        emit frameTypeChanged(type);
    }
}

void SDeclarativeFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_D(SDeclarativeFrame);
    QSize rectSize(width(), height());

    if (rectSize.isEmpty())
        return;

    // If the rect size has changed, frame needs to be reloaded
    if (d->frameLoadAttempted && d->loadedSize != rectSize)
        d->releaseFromFramePool();

    // Load pixmap now that rect size is known
    if (!d->frameLoadAttempted && !d->fileName.isEmpty()) {
        // Get frame pixmap from application's frame pool
        d->framePixmap = SFramePool::get(d->fileName, d->frameType, rectSize);
        d->frameLoadAttempted = true;
        d->loadedSize = rectSize;
    }

    if (!d->framePixmap.isNull())
        painter->drawPixmap(QPoint(0, 0), d->framePixmap);

#ifdef Q_DEBUG_FRAME
    qDebug() << "SDeclarativeFrame::paint";
    qDebug() << "framename = " << frameName();

    qDebug() << "width = " << width();
    qDebug() << "height = " << height();
#endif
}
