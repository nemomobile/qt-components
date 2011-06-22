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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "3d/qglattributedescription.h"
#include "3d/qarray.h"
#include <QtCore/qvector.h>
//#include <qglfunctions.h>
#include <QGLBuffer>

//#include "qmlscene_global.h"
class QGLAttributeValue;

class Geometry
{
public:
    Geometry(); // Creates a null geometry.
    Geometry(const QVector<QGLAttributeDescription> &description, GLenum indexType = GL_UNSIGNED_SHORT);
    ~Geometry();

    void *vertexData();
    const void *constVertexData() const { return m_vertex_data.constData(); }
    const QArray<uchar> &vertexArray() const { return m_vertex_data; }

    void setVertexDescription(const QVector<QGLAttributeDescription> &description);
    const QVector<QGLAttributeDescription> &vertexDescription() const { return m_vertex_description; }

    int vertexCount() const { return m_vertex_data.size() / m_vertex_stride; }
    void setVertexCount(int count) {
        int currSize = m_vertex_data.size();
        int newSize = count * m_vertex_stride;
        if (newSize > currSize)
            m_vertex_data.extend(newSize - currSize); // Do not initialize newly allocated memory
        else if (newSize < currSize)
            m_vertex_data.resize(newSize);
    }

    void *indexData();
    ushort *ushortIndexData();
    uint *uintIndexData();
    const void *constIndexData() const { return m_index_data.constData(); }
    const ushort *constUshortIndexData() const;
    const uint *constUintIndexData() const;
    const QArray<uchar> &indexArray() const { return m_index_data; }

    void setIndexType(GLenum indexType);
    GLenum indexType() const { return m_index_type; }

    int indexCount() const { return m_index_data.size() / m_index_stride; }
    void setIndexCount(int count) {
        int currSize = m_index_data.size();
        int newSize = count * m_index_stride;
        if (newSize > currSize)
            m_index_data.extend(newSize - currSize); // Do not initialize newly allocated memory
        else if (newSize < currSize)
            m_index_data.resize(newSize);
    }

    QGL::DrawingMode drawingMode() const { return m_mode; }
    void setDrawingMode(QGL::DrawingMode mode) { m_mode = mode; }

    int stride() const { return m_vertex_stride; }

    QGLAttributeValue attributeValue(QGL::VertexAttribute attribute) const;

    bool isNull() const { return m_vertex_description.isEmpty(); }

protected:
    QArray<uchar> m_vertex_data;
    QArray<uchar> m_index_data;

    QVector<QGLAttributeDescription> m_vertex_description; // QGLAttributeDescription should have offset and normalized, must assume tightly packed data for now.
    GLenum m_index_type;
    QGL::DrawingMode m_mode;
    int m_vertex_stride;
    int m_index_stride;
};

class GeometryDataUploader
{
public:
    static void registerGeometry(const Geometry *);
    static void unregisterGeometry(const Geometry *);

    static void bind();
    static void release();
    static void upload();

    static const void *vertexData(const Geometry *g, int offset = 0);
    static const void *indexData(const Geometry *g);

    static void setUseBuffers(bool b) { m_use_buffers = b; }
    static bool useBuffers() { return m_use_buffers; }

    static void markVertexDirty() { m_vertex_dirty = true; }
    static void markIndexDirty() { m_index_dirty = true; }

private:
    static void addGeometryVertex(const Geometry *);
    static void addGeometryIndex(const Geometry *);

    static void clearVertexData() { m_vertex_offsets.clear(); m_vertex_data.clear(); }
    static void clearIndexData() { m_index_offsets.clear(); m_index_data.clear(); }

    static bool m_use_buffers;
    static QSet<const Geometry *> m_geometries;
    static QGLBuffer m_vertex_buffer;
    static QGLBuffer m_index_buffer;
    static QArray<uchar> m_vertex_data;
    static QArray<uchar> m_index_data;
    static QHash<const Geometry *, int> m_vertex_offsets;
    static QHash<const Geometry *, int> m_index_offsets;
    static bool m_vertex_bound;
    static bool m_index_bound;
    static bool m_vertex_dirty;
    static bool m_index_dirty;
};


#endif
