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

#include "sframepool.h"
#include "siconpool.h"

#include <QPainter>
#include <QPixmap>
#include <QSizeF>
#include <QHash>

//#define Q_DEBUG_FRAME
#ifdef Q_DEBUG_FRAME
#include <QDebug>
#endif

static const QLatin1String extension_tl("_tl");
static const QLatin1String extension_t("_t");
static const QLatin1String extension_tr("_tr");
static const QLatin1String extension_l("_l");
static const QLatin1String extension_c("_c");
static const QLatin1String extension_r("_r");
static const QLatin1String extension_bl("_bl");
static const QLatin1String extension_b("_b");
static const QLatin1String extension_br("_br");

/*!
  \class SFramePoolKey
 */
struct SFramePoolKey {
public:
    SFramePoolKey(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size)
        : mFilename(filename), mType(type), mSize(size) {}

    bool operator==(const SFramePoolKey &other) const {
        return other.mFilename == mFilename && other.mType == mType && other.mSize == mSize;
    }

    QString mFilename;
    SDeclarativeFrame::FrameType mType;
    QSize mSize;
};

/*!
  \class SFramePoolValue
 */
struct SFramePoolValue {
    SFramePoolValue()
        : mPixmap(), mRefCount(0) {}
    SFramePoolValue(const QPixmap &pixmap)
        : mPixmap(pixmap), mRefCount(1) {}
    QPixmap mPixmap;
    int mRefCount;
};

uint qHash(const SFramePoolKey &key)
{
    return qHash(key.mFilename);
}

typedef QHash<SFramePoolKey, SFramePoolValue> SFramePoolData;
Q_GLOBAL_STATIC(SFramePoolData, poolData);

QPixmap SFramePool::get(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size)
{
    QPixmap pixmap;

    if (!filename.isEmpty() && !size.isEmpty() && type != SDeclarativeFrame::Undefined) {
        SFramePoolKey key(filename, type, size);
        SFramePoolData *pool = poolData();

        if (pool->contains(key)) {
            SFramePoolValue value = pool->value(key);
            ++value.mRefCount;
            pixmap = value.mPixmap;
            pool->insert(key, value);
        } else {
            pixmap = loadFrame(filename, type, size);
            pool->insert(key, SFramePoolValue(pixmap));
        }
#ifdef Q_DEBUG_FRAME
        qDebug() << "SFramePool::get()" << key.mFilename << pool->value(key).mRefCount;
#endif
    }
    return pixmap;
}

void SFramePool::release(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size)
{
    // One piece frame uses icon pool directly...
    if (type == SDeclarativeFrame::OnePiece) {
        SIconPool::release(filename, size, Qt::IgnoreAspectRatio);
    } else {
        SFramePoolKey key(filename, type, size);
        SFramePoolData *pool = poolData();
        if (pool->contains(key)) {
            SFramePoolValue value = pool->value(key);
            // Delete if last reference
            if (!--value.mRefCount) {
                pool->remove(key);
            } else {
                // Update dereferenced value
                pool->insert(key, value);
            }
#ifdef Q_DEBUG_FRAME
            qDebug() << "SFramePool::release()" << key.mFilename << value.mRefCount;
#endif
        }
    }
}

/**
* This is just a temp solution to get something visible - final icon/frame loading architecture is worked on in SVG team.
*/
QPixmap SFramePool::loadFrame(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size)
{
    QPixmap pm;

    // If frame is one piece, use icon pool directly
    if (type == SDeclarativeFrame::OnePiece) {
        pm = SIconPool::get(filename, size, Qt::IgnoreAspectRatio);
    } else {

        // Generate frame in the given size
        pm = QPixmap(size);
        pm.fill(QColor(0, 0, 0, 0));
        QPainter painter(&pm);

        switch (type) {
        case SDeclarativeFrame::ThreePiecesHorizontal: {

            int border = SIconPool::defaultSize(createFileName(filename, extension_l)).width();

            QRect rect_l = QRect(0, 0, border, size.height());
            QRect rect_c = QRect(border, 0, size.width() - 2 * border, size.height());
            QRect rect_r = QRect(size.width() - border, 0, border, size.height());

            QPixmap l = SIconPool::get(createFileName(filename, extension_l), rect_l.size(), Qt::KeepAspectRatio);
            QPixmap c = SIconPool::get(createFileName(filename, extension_c), rect_c.size(), Qt::IgnoreAspectRatio);
            QPixmap r = SIconPool::get(createFileName(filename, extension_r), rect_r.size(), Qt::KeepAspectRatio);

            // Draw the loaded pixmaps in the frame pixmap
            painter.drawPixmap(rect_l, l);
            painter.drawPixmap(rect_c, c);
            painter.drawPixmap(rect_r, r);

            SIconPool::release(createFileName(filename, extension_l), rect_l.size(), Qt::KeepAspectRatio);
            SIconPool::release(createFileName(filename, extension_c), rect_c.size(), Qt::IgnoreAspectRatio);
            SIconPool::release(createFileName(filename, extension_r), rect_r.size(), Qt::KeepAspectRatio);

            break;
        }
        case SDeclarativeFrame::ThreePiecesVertical: {

            int border = SIconPool::defaultSize(createFileName(filename, extension_t)).height();

            QRect rect_t = QRect(0, 0, size.width(), border);
            QRect rect_c = QRect(0, border, size.width(), size.height() - 2 * border);
            QRect rect_b = QRect(0, size.height() - border, size.width(), border);

            QPixmap t = SIconPool::get(createFileName(filename, extension_t), rect_t.size(), Qt::KeepAspectRatio);
            QPixmap c = SIconPool::get(createFileName(filename, extension_c), rect_c.size(), Qt::IgnoreAspectRatio);
            QPixmap b = SIconPool::get(createFileName(filename, extension_b), rect_b.size(), Qt::KeepAspectRatio);

            // Draw the loaded pixmaps in the frame pixmap
            painter.drawPixmap(QRect(0, 0, size.width(), border), t);
            painter.drawPixmap(QRect(0, border, size.width(), size.height() - 2 * border), c);
            painter.drawPixmap(QRect(0, size.height() - border, size.width(), border), b);

            SIconPool::release(createFileName(filename, extension_t), rect_t.size(), Qt::KeepAspectRatio);
            SIconPool::release(createFileName(filename, extension_c), rect_c.size(), Qt::IgnoreAspectRatio);
            SIconPool::release(createFileName(filename, extension_b), rect_b.size(), Qt::KeepAspectRatio);

            break;
        }
        case SDeclarativeFrame::NinePieces: {

            int border = SIconPool::defaultSize(createFileName(filename, extension_tl)).width();

            QRect rect_tl = QRect(0, 0, border, border);
            QRect rect_t = QRect(border, 0, size.width() - 2 * border, border);
            QRect rect_tr = QRect(size.width() - border, 0, border, border);
            QRect rect_l = QRect(0, border, border, size.height() - 2 * border);
            QRect rect_c = QRect(border, border, size.width() - 2 * border, size.height() - 2 * border);
            QRect rect_r = QRect(size.width() - border, border, border, size.height() - 2 * border);
            QRect rect_bl = QRect(0, size.height() - border, border, border);
            QRect rect_b = QRect(border, size.height() - border, size.width() - 2 * border, border);
            QRect rect_br = QRect(size.width() - border, size.height() - border, border, border);

            QPixmap tl = SIconPool::get(createFileName(filename, extension_tl), rect_tl.size(), Qt::KeepAspectRatio);
            QPixmap t = SIconPool::get(createFileName(filename, extension_t), rect_t.size(), Qt::IgnoreAspectRatio);
            QPixmap tr = SIconPool::get(createFileName(filename, extension_tr), rect_tr.size(), Qt::KeepAspectRatio);
            QPixmap l = SIconPool::get(createFileName(filename, extension_l), rect_l.size(), Qt::IgnoreAspectRatio);
            QPixmap c = SIconPool::get(createFileName(filename, extension_c), rect_c.size(), Qt::IgnoreAspectRatio);;
            QPixmap r = SIconPool::get(createFileName(filename, extension_r), rect_r.size(), Qt::IgnoreAspectRatio);
            QPixmap bl = SIconPool::get(createFileName(filename, extension_bl), rect_bl.size(), Qt::KeepAspectRatio);
            QPixmap b = SIconPool::get(createFileName(filename, extension_b), rect_b.size(), Qt::IgnoreAspectRatio);
            QPixmap br = SIconPool::get(createFileName(filename, extension_br), rect_br.size(), Qt::KeepAspectRatio);

            // Draw the loaded pixmaps in the frame pixmap
            painter.drawPixmap(rect_tl, tl);
            painter.drawPixmap(rect_t, t);
            painter.drawPixmap(rect_tr , tr);
            painter.drawPixmap(rect_l, l);
            painter.drawPixmap(rect_c, c);
            painter.drawPixmap(rect_r, r);
            painter.drawPixmap(rect_bl, bl);
            painter.drawPixmap(rect_b, b);
            painter.drawPixmap(rect_br, br);

            SIconPool::release(createFileName(filename, extension_tl), rect_tl.size(), Qt::KeepAspectRatio);
            SIconPool::release(createFileName(filename, extension_t), rect_t.size(), Qt::IgnoreAspectRatio);
            SIconPool::release(createFileName(filename, extension_tr), rect_tr.size(), Qt::KeepAspectRatio);
            SIconPool::release(createFileName(filename, extension_l), rect_l.size(), Qt::IgnoreAspectRatio);
            SIconPool::release(createFileName(filename, extension_c), rect_c.size(), Qt::IgnoreAspectRatio);
            SIconPool::release(createFileName(filename, extension_r), rect_r.size(), Qt::IgnoreAspectRatio);
            SIconPool::release(createFileName(filename, extension_bl), rect_bl.size(), Qt::KeepAspectRatio);
            SIconPool::release(createFileName(filename, extension_b), rect_b.size(), Qt::IgnoreAspectRatio);
            SIconPool::release(createFileName(filename, extension_br), rect_br.size(), Qt::KeepAspectRatio);

            break;
        }
        default:
            break;
        }

        painter.end();
    }

#ifdef Q_DEBUG_FRAME
    if (pm.isNull()) {
        qDebug() << "Fail to load frame: " << filename;
    }
#endif

    return pm;
}

QString SFramePool::createFileName(const QString &filename, const QString &suffix)
{
    QString ret = filename;

    int index = filename.lastIndexOf(QChar('.'));
    // Append suffix before extension
    if (index > 0)
        ret.insert(index, suffix);

    return ret;
}

#ifdef FRAME_POOL_UNIT_TEST

int SFramePool::totalCount()
{
    SFramePoolData *pool = poolData();
    return pool->count();
}

int SFramePool::count(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size)
{
    SFramePoolKey key(filename, type, size);
    SFramePoolData *pool = poolData();
    if (pool->contains(key)) {
        SFramePoolValue value = pool->value(key);
        return value.mRefCount;
    } else {
        return 0;
    }
}

#endif // FRAME_POOL_UNIT_TEST
