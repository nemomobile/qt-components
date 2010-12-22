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

#include "siconpool.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QPixmap>
#include <QSizeF>
#include <QHash>

//#define Q_DEBUG_ICON
#ifdef Q_DEBUG_ICON
#include <QDebug>
#endif

/*!
  \class SIconPoolKey
 */
struct SIconPoolKey {
public:
    SIconPoolKey(const QString &filename, const QSize &size, Qt::AspectRatioMode mode)
        : mFilename(filename), mSize(size), mMode(mode) {}

    bool operator==(const SIconPoolKey &other) const {
        return other.mFilename == mFilename && other.mSize == mSize && other.mMode == mMode;
    }

    QString mFilename;
    QSize mSize;
    Qt::AspectRatioMode mMode;
};

/*!
  \class SIconPoolValue
 */
struct SIconPoolValue {
    SIconPoolValue()
        : mPixmap(), mRefCount(0) {}
    SIconPoolValue(const QPixmap &pixmap)
        : mPixmap(pixmap), mRefCount(1) {}
    QPixmap mPixmap;
    int mRefCount;
};

uint qHash(const SIconPoolKey &key)
{
    return qHash(key.mFilename);
}

typedef QHash<SIconPoolKey, SIconPoolValue> SIconPoolData;
Q_GLOBAL_STATIC(SIconPoolData, poolData);

QPixmap SIconPool::get(const QString &filename, const QSize &size, Qt::AspectRatioMode mode)
{
    QPixmap pixmap;

    if (!filename.isEmpty() && !size.isEmpty()) {
        SIconPoolKey key(filename, size, mode);
        SIconPoolData *pool = poolData();

        if (pool->contains(key)) {
            SIconPoolValue value = pool->value(key);
            ++value.mRefCount;
            pixmap = value.mPixmap;
            pool->insert(key, value);
        } else {
            pixmap = loadIcon(filename, size, mode);
            pool->insert(key, SIconPoolValue(pixmap));
        }
#ifdef Q_DEBUG_ICON
        qDebug() << "SIconPool::get()" << key.mFilename << pool->value(key).mRefCount;
#endif
    }
    return pixmap;
}

/**
* This is just a temp solution to get something visible - final icon loading architecture is worked on in SVG team.
*/
QPixmap SIconPool::loadIcon(const QString &filename, const QSize &size, Qt::AspectRatioMode mode)
{
    QPixmap pm;
    // SVG? Use QSvgRenderer
    if (filename.endsWith(".svg")) {
        QSvgRenderer *renderer = getSvgRenderer(filename);

        if (renderer->isValid()) {
            QSize renderSize = renderer->defaultSize();

            // a bit of a hack: should we expose the aspect ratio flag?
            if (filename.contains("screen_bg")) {
                mode = Qt::KeepAspectRatioByExpanding;
            }
            renderSize.scale(size, mode);

            pm = QPixmap(renderSize);
            pm.fill(QColor(0, 0, 0, 0));
            QPainter painter(&pm);
            renderer->render(&painter, QRectF(QPointF(), renderSize));
        }
    } else {
        // Otherwise load with QPixmap
        pm.load(filename);
        if (!pm.isNull()) {
            pm = pm.scaled(size, mode, Qt::SmoothTransformation);
        }
    }

#ifdef Q_DEBUG_ICON
    if (pm.isNull()) {
        qDebug() << "Fail to load icon: " << filename;
    }
#endif

    return pm;
}

void SIconPool::release(const QString &filename, const QSize &size, Qt::AspectRatioMode mode)
{
    SIconPoolKey key(filename, size, mode);
    SIconPoolData *pool = poolData();
    if (pool->contains(key)) {
        SIconPoolValue value = pool->value(key);
        // Delete if last reference
        if (!--value.mRefCount) {
            pool->remove(key);
        } else {
            // Update dereferenced value in pool
            pool->insert(key, value);
        }

#ifdef Q_DEBUG_ICON
        qDebug() << "SIconPool::release()" << key.mFilename << value.mRefCount;
#endif
    }
}

QSize SIconPool::defaultSize(const QString &filename)
{
    QSize defSize;

    // Get the default size from svg renderer or pixmap size
    if (!filename.isEmpty()) {
        // SVG? Use QSvgRenderer
        if (filename.endsWith(".svg")) {
            QSvgRenderer *svgRenderer = getSvgRenderer(filename);
            if (svgRenderer->isValid()) {
                defSize = svgRenderer->defaultSize();
            }
        } else {
            // Otherwise load with QPixmap
            QPixmap pixmap;
            pixmap.load(filename);
            defSize = pixmap.size();
        }
    }

    return defSize;
}

QSvgRenderer *SIconPool::getSvgRenderer(const QString &filename)
{
    static QString lastSvgFileName;
    static QSvgRenderer *lastSvgRenderer = 0;

    if (lastSvgFileName == filename)
        return lastSvgRenderer;

    delete lastSvgRenderer;
    lastSvgRenderer = new QSvgRenderer(filename);
    lastSvgFileName = filename;
    return lastSvgRenderer;
}

#ifdef ICON_POOL_UNIT_TEST

int SIconPool::totalCount()
{
    SIconPoolData *pool = poolData();
    return pool->count();
}

int SIconPool::count(const QString &filename, const QSize &size, Qt::AspectRatioMode mode)
{
    SIconPoolKey key(filename, size, mode);
    SIconPoolData *pool = poolData();
    if (pool->contains(key)) {
        SIconPoolValue value = pool->value(key);
        return value.mRefCount;
    } else {
        return 0;
    }
}

#endif // ICON_POOL_UNIT_TEST
