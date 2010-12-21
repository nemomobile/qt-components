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

#include "mdeclarativeiconglow.h"

#include <QPainter>
#include <QDeclarativeEngine>
#include <QDeclarativeImageProvider>

MDeclarativeIconGlow::MDeclarativeIconGlow(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), m_glowRadius(0), m_glowColor(Qt::white)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

MDeclarativeIconGlow::~MDeclarativeIconGlow()
{
}

QUrl MDeclarativeIconGlow::source() const
{
    return m_source;
}

void MDeclarativeIconGlow::setSource(const QUrl &source)
{
    if (m_source == source)
        return;

    m_source = source;

    m_icon = QImage();
    update();

    emit sourceChanged(m_source);
}

QSize MDeclarativeIconGlow::sourceSize() const
{
    return m_sourceSize;
}

void MDeclarativeIconGlow::setSourceSize(const QSize &size)
{
    if (m_sourceSize == size)
        return;

    m_sourceSize = size;

    m_icon = QImage();
    update();

    emit sourceSizeChanged(m_sourceSize);
}

int MDeclarativeIconGlow::glowRadius() const
{
    return m_glowRadius;
}

void MDeclarativeIconGlow::setGlowRadius(int glowRadius)
{
    if (m_glowRadius == glowRadius)
        return;

    m_glowRadius = glowRadius;
    update();
}

QColor MDeclarativeIconGlow::glowColor() const
{
    return m_glowColor;
}

void MDeclarativeIconGlow::setGlowColor(QColor glowColor)
{
    if (m_glowColor == glowColor)
        return;

    m_glowColor = glowColor;
    update();
}

QRectF MDeclarativeIconGlow::boundingRect() const
{
    return QRectF(-m_glowRadius, -m_glowRadius,
                  width() + 2 * m_glowRadius, height() + 2 * m_glowRadius);
}

static void blur(const QImage *source, QImage *destination, int radius, const QColor &color);

void MDeclarativeIconGlow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_icon.isNull() && !loadIcon())
        return;

    // Generate glow image
    // This is slow, I'm relying on QGraphicsItem cache system here
    QImage scaledImage = m_icon.scaled(QSize(width(), height()),
                                       Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation);
    QImage glowImage(boundingRect().size().toSize(), QImage::Format_ARGB32);
    blur(&scaledImage, &glowImage, m_glowRadius, m_glowColor);

    // Blit to screen
    painter->drawImage(boundingRect().toRect(), glowImage);
}

bool MDeclarativeIconGlow::loadIcon()
{
    if (m_source.isEmpty())
        return false;

    const QString scheme = m_source.scheme();

    if (scheme == "file") {
        // Load from filesystem
        m_icon = QImage(m_source.path());

        if (m_icon.isNull())
            return false;

    } else if (scheme == "image") {
        QDeclarativeEngine *engine = qmlEngine(this);
        Q_ASSERT(engine);

        QDeclarativeImageProvider *provider = engine->imageProvider(m_source.host());
        if (!provider)
            return false;

        const QString id(m_source.path().mid(1));
        QSize loadedSize;
        const QSize requestedSize = m_sourceSize.isEmpty() ? QSize(0, 0) : m_sourceSize;

        if (provider->imageType() == QDeclarativeImageProvider::Image) {
            m_icon = provider->requestImage(id, &loadedSize, requestedSize);
        } else {
            m_icon = provider->requestPixmap(id, &loadedSize, requestedSize).toImage();
        }

    } else {
        qWarning("QML IconGlow: Protocl \"%s\" is unknown", qPrintable(scheme));
        return false;
    }

    setImplicitWidth(m_icon.width());
    setImplicitHeight(m_icon.height());

    return true;
}

////////////////////////////////////////////////////
// glow generation copied from LibMeegoTouch IconButton
// src/views/mbuttoniconview.cpp
////////////////////////////////////////////////////

// blur image
static void blur(const QImage *source, QImage *destination, int radius, const QColor &color)
{
    QSize s = destination->size();
    int width = s.width();
    int height = s.height();

    QImage tmp(s, QImage::Format_ARGB32);

    qreal GlowColorR  = color.redF();
    qreal GlowColorG  = color.greenF();
    qreal GlowColorB  = color.blueF();

    int total, alpha;
    QRgb *buffer;

    // horizontal
    for (int y = 0; y < height; ++y) {
        total = 0;
        buffer = (QRgb *)tmp.scanLine(y);

        // Process entire window for first pixel
        //for (int kx = -radius; kx <= radius; ++kx)
        if (y >= radius && y < height - radius) {
            total = qAlpha(source->pixel(0, y - radius));

            alpha = total / (radius * 2 + 1);
            *buffer = qRgba(0, 0, 0, alpha);
            buffer++;

            // Subsequent pixels just update window total
            for (int x = 1; x < width; ++x) {
                // Subtract pixel leaving window
                if (x - radius * 2 - 1 >= 0) // && x - radius*2 < source->size().width())
                    total -= qAlpha(source->pixel(x - radius * 2 - 1, y - radius));

                // Add pixel entering window
                if (x > 0 && x < source->size().width())
                    total += qAlpha(source->pixel(x, y - radius));

                alpha = total / (radius * 2 + 1);

                *buffer = qRgba(0, 0, 0, alpha);
                buffer++;
            }
        } else {
            for (int x = 0; x < width; ++x) {
                *buffer = qRgba(0, 0, 0, 0);
                buffer++;
            }
        }
    }

    qreal r, g, b, a;
    // vertical
    for (int x = 0; x < width; ++x) {
        total = 0;

        // Process entire window for first pixel
        //for (int ky = -radius; ky <= radius; ++ky)
        for (int ky = 0; ky <= radius; ++ky)
            total += qAlpha(tmp.pixel(x, ky));

        alpha = total / (radius * 2 + 1);

        r = alpha * GlowColorR * 2;
        g = alpha * GlowColorG * 2;
        b = alpha * GlowColorB * 2;
        a = alpha * 2;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (a > 255) a = 255;

        buffer = (QRgb *)destination->scanLine(0);
        *(buffer + x) = qRgba(r, g, b, a);

        // Subsequent pixels just update window total
        for (int y = 1; y < height; ++y) {
            // Subtract pixel leaving window
            if (y - radius - 1 >= 0)
                total -= qAlpha(tmp.pixel(x, y - radius - 1));

            // Add pixel entering window
            if (y + radius < height)
                total += qAlpha(tmp.pixel(x, y + radius));

            alpha = total / (radius * 2 + 1);

            r = alpha * GlowColorR * 2;
            g = alpha * GlowColorG * 2;
            b = alpha * GlowColorB * 2;
            a = alpha * 2;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (a > 255) a = 255;

            buffer = (QRgb *)destination->scanLine(y);
            *(buffer + x) = qRgba(r, g, b, a);
        }
    }
}
