/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "sdeclarativeicon.h"
#include "siconpool.h"
#include "sdeclarative.h"

#include <QCoreApplication>
#include <QPainter>
#include <QGraphicsScene>
#include <QSvgRenderer>
#include <QPixmap>
#include <QSizeF>

//#define Q_DEBUG_ICON
#ifdef Q_DEBUG_ICON
#include <QDebug>
#endif

class SDeclarativeIconPrivate
{
public:
    SDeclarativeIconPrivate() :
        iconLoadAttempted(false) {}

    void releaseFromIconPool();

public:
    QPixmap pixmap;
    QString iconName;
    QString fileName;
    QColor iconColor;
    bool iconLoadAttempted;
    QSize loadedSize;
};

void SDeclarativeIconPrivate::releaseFromIconPool()
{
    // Release previously loaded pixmap from icon pool
    SIconPool::release(fileName, loadedSize, Qt::KeepAspectRatio, iconColor);
    pixmap = QPixmap();
    iconLoadAttempted = false;
}

SDeclarativeIcon::SDeclarativeIcon(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    d_ptr(new SDeclarativeIconPrivate)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

SDeclarativeIcon::~SDeclarativeIcon()
{
    Q_D(SDeclarativeIcon);
    // Release from icon pool
    if (d->iconLoadAttempted)
        d->releaseFromIconPool();
}

QString SDeclarativeIcon::iconName() const
{
    Q_D(const SDeclarativeIcon);
    return d->iconName;
}

void SDeclarativeIcon::setIconName(const QString &name)
{
    Q_D(SDeclarativeIcon);
    if (d->iconName != name) {

        if (d->iconLoadAttempted) {
            // Release previous one from icon pool
            d->releaseFromIconPool();
        }

        d->fileName = SDeclarative::resolveIconFileName(name);
        d->iconName = name;
        update();
        emit iconNameChanged(name);
    }
}

QColor SDeclarativeIcon::iconColor() const
{
    Q_D(const SDeclarativeIcon);
    return d->iconColor;
}

void SDeclarativeIcon::setIconColor(const QColor &color)
{
    Q_D(SDeclarativeIcon);
    if (d->iconColor != color) {
        if (d->iconLoadAttempted) {
            // Release previous one from icon pool
            d->releaseFromIconPool();
        }

        d->iconColor = color;

        update();
        emit iconColorChanged(color);
    }
}

void SDeclarativeIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_D(SDeclarativeIcon);
    QSize rectSize(width(), height());

    if (rectSize.isEmpty())
        return;

    // If the rect size has changed, icon needs to be reloaded
    if (d->iconLoadAttempted && d->loadedSize != rectSize)
        d->releaseFromIconPool();

    // Load pixmap now that rect size is known
    if (!d->iconLoadAttempted && !d->fileName.isEmpty()) {
        // Get pixmap from application's icon pool
        d->pixmap = SIconPool::get(d->fileName, rectSize, Qt::KeepAspectRatio, d->iconColor);
        d->iconLoadAttempted = true;
        d->loadedSize = rectSize;
    }

    // Center pixmap rect
    QRect rect(QPoint(0, 0), d->pixmap.size());
    // Round to nearest integer pixel
    rect.moveCenter(QPoint(width() / 2 + 0.5, height() / 2 + 0.5));

    // Give only topleft coordinates - no pixmap scaling when drawing
    painter->drawPixmap(rect.topLeft().x(), rect.topLeft().y(), d->pixmap);

#ifdef Q_DEBUG_ICON
    qDebug() << "SDeclarativeIcon::paint";
    qDebug() << "iconname = " << iconName();
    qDebug() << "width = " << width();
    qDebug() << "height = " << height();
#endif
}
