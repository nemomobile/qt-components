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

#ifndef QGLATTRIBUTEVALUE_H
#define QGLATTRIBUTEVALUE_H

#include "qglattributedescription.h"
#include "qcustomdataarray.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QGLVertexBuffer;

class Q_QT3D_EXPORT QGLAttributeValue
{
public:
    QGLAttributeValue();
    QGLAttributeValue(const QArray<float>& array);
    QGLAttributeValue(const QArray<QVector2D>& array);
    QGLAttributeValue(const QArray<QVector3D>& array);
    QGLAttributeValue(const QArray<QVector4D>& array);
    QGLAttributeValue(const QArray<QColor4ub>& array);
    QGLAttributeValue(const QCustomDataArray& array);
    QGLAttributeValue(int tupleSize, GLenum type, int stride, const void *data, int count = 0);
    QGLAttributeValue(int tupleSize, GLenum type, int stride, int offset, int count = 0);
    QGLAttributeValue(const QGLAttributeDescription& description, const void *data, int count = 0);
    QGLAttributeValue(const QGLAttributeDescription& description, int offset, int count = 0);

    bool isNull() const;

    const QGLAttributeDescription &description() const;
    GLenum type() const;
    int sizeOfType() const;
    int tupleSize() const;
    int stride() const;
    int offset() const;
    const void *data() const;
    const float *floatData() const;
    int count() const;

private:
    QGLAttributeDescription m_description;
    const void *m_data;
    int m_count;

    void setStride(int stride) { m_description.setStride(stride); }
    void setOffset(int offset)
        { m_data = reinterpret_cast<const void *>(offset); }

    friend class QGLVertexBuffer;
};

inline QGLAttributeValue::QGLAttributeValue()
    : m_data(0), m_count(0)
{
}

inline QGLAttributeValue::QGLAttributeValue(const QArray<float>& array)
    : m_description(QGL::Position, 1, GL_FLOAT, 0), m_data(array.constData()), m_count(array.count())
{
}

inline QGLAttributeValue::QGLAttributeValue(const QArray<QVector2D>& array)
    : m_description(QGL::Position, 2, GL_FLOAT, 0), m_data(array.constData()), m_count(array.count())
{
}

inline QGLAttributeValue::QGLAttributeValue(const QArray<QVector3D>& array)
    : m_description(QGL::Position, 3, GL_FLOAT, 0), m_data(array.constData()), m_count(array.count())
{
}

inline QGLAttributeValue::QGLAttributeValue(const QArray<QVector4D>& array)
    : m_description(QGL::Position, 4, GL_FLOAT, 0), m_data(array.constData()), m_count(array.count())
{
}

inline QGLAttributeValue::QGLAttributeValue(const QArray<QColor4ub>& array)
    : m_description(QGL::Position, 4, GL_UNSIGNED_BYTE, 0), m_data(array.constData()), m_count(array.count())
{
}

inline QGLAttributeValue::QGLAttributeValue
        (int tupleSize, GLenum type, int stride, const void *data, int count)
    : m_description(QGL::Position, tupleSize, type, stride), m_data(data), m_count(count)
{
}

inline QGLAttributeValue::QGLAttributeValue
        (int tupleSize, GLenum type, int stride, int offset, int count)
    : m_description(QGL::Position, tupleSize, type, stride),
      m_data(reinterpret_cast<const void *>(offset)), m_count(count)
{
}

inline QGLAttributeValue::QGLAttributeValue
        (const QGLAttributeDescription& description, const void *data, int count)
    : m_description(description), m_data(data), m_count(count)
{
}

inline QGLAttributeValue::QGLAttributeValue
        (const QGLAttributeDescription& description, int offset, int count)
    : m_description(description),
      m_data(reinterpret_cast<const void *>(offset)), m_count(count)
{
}

inline bool QGLAttributeValue::isNull() const
{
    return m_description.isNull();
}

inline const QGLAttributeDescription &QGLAttributeValue::description() const
{
    return m_description;
}

inline GLenum QGLAttributeValue::type() const
{
    return m_description.type();
}

inline int QGLAttributeValue::tupleSize() const
{
    return m_description.tupleSize();
}

inline int QGLAttributeValue::stride() const
{
    return m_description.stride();
}

inline int QGLAttributeValue::offset() const
{
    return int(reinterpret_cast<size_t>(m_data));
}

inline const void *QGLAttributeValue::data() const
{
    return m_data;
}

inline const float *QGLAttributeValue::floatData() const
{
    return reinterpret_cast<const float *>(m_data);
}

inline int QGLAttributeValue::count() const
{
    return m_count;
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
