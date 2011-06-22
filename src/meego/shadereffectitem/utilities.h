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

#ifndef UTILITIES_H
#define UTILITIES_H

//#include "areaallocator.h"
//#include "adaptationlayer.h"

//#include "qmlscene_global.h"
#include "3d/qglattributedescription.h"

#include <QtCore/QSharedPointer>
#include <QtGui/QColor>

class TextMaskMaterial;
class FlatColorMaterial;
class VertexColorMaterial;
class Geometry;
class Renderer;

class QPixmap;
class QImage;
class QSize;
class QFontEngine;
class QRectF;

//class SubTexture2D
//{
//public:
//    SubTexture2D(TextureAtlasInterface *atlas, const QRect &allocatedRect);
//    SubTexture2D(const QGLTexture2DConstPtr &texture);
//    ~SubTexture2D();

//    bool hasOwnTexture() const { return m_source.right() == 1; }
//    bool isValid() const { return hasOwnTexture() || !m_allocated.isNull(); }
//    QRectF sourceRect() const { return m_source; }
//    QGLTexture2DConstPtr texture() const { return m_texture; }
//private:
//    QRect m_allocated;
//    QRectF m_source;
//    TextureAtlasInterface *m_atlas;
//    QGLTexture2DConstPtr m_texture;
//};

//typedef QSharedPointer<const SubTexture2D> SubTexture2DConstPtr;
//typedef QSharedPointer<SubTexture2D> SubTexture2DPtr;

class Utilities
{
public:
//    static SubTexture2DPtr getTextureForImage(const QImage &image, bool clampToEdge, bool dynamic = true);
//    static SubTexture2DPtr getTextureForPixmap(const QPixmap &pixmap, bool clampToEdge, bool dynamic = true);

    static void setupRectGeometry(Geometry *geometry, const QRectF &rect, const QSize &textureSize = QSize(), const QRectF &sourceRect = QRectF());
    static QVector<QGLAttributeDescription> &getRectGeometryDescription();
    static QVector<QGLAttributeDescription> &getTexturedRectGeometryDescription();
    static QVector<QGLAttributeDescription> &getColoredRectGeometryDescription();
    static Geometry *createRectGeometry(const QRectF &rect);
    static Geometry *createTexturedRectGeometry(const QRectF &rect, const QSize &textureSize, const QRectF &sourceRect);

    static QColor mix(const QColor &c1, const QColor c2, qreal factor) {
        qreal inv = 1 - factor;
        return QColor::fromRgbF(c1.redF() * factor + c2.redF() * inv,
                                c1.greenF() * factor + c2.greenF() * inv,
                                c1.blueF() * factor + c2.blueF() * inv,
                                c1.alphaF() * factor + c2.alphaF() * inv);
    }
};


#endif // UTILITIES_H
