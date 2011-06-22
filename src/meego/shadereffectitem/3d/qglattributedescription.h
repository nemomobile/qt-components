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

#ifndef QGLAttributeDescription_H
#define QGLAttributeDescription_H

#include <QtOpenGL/qgl.h>
#include "qt3dglobal.h"
#include "qglnamespace.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class Q_QT3D_EXPORT QGLAttributeDescription
{
public:
    QGLAttributeDescription();
    QGLAttributeDescription(QGL::VertexAttribute attribute,
                            int tupleSize, GLenum type, int stride);

    bool isNull() const;

    QGL::VertexAttribute attribute() const;
    void setAttribute(QGL::VertexAttribute attribute);

    GLenum type() const;
    void setType(GLenum type);

    int sizeOfType() const;

    int tupleSize() const;
    void setTupleSize(int tupleSize);

    int stride() const;
    void setStride(int stride);

private:
    QGL::VertexAttribute m_attribute;
    GLenum m_type;
    int m_tupleSize;
    int m_stride;
};

inline QGLAttributeDescription::QGLAttributeDescription()
    : m_attribute(QGL::Position), m_type(GL_FLOAT),
      m_tupleSize(0), m_stride(0)
{
}

inline QGLAttributeDescription::QGLAttributeDescription
        (QGL::VertexAttribute attribute, int tupleSize, GLenum type, int stride)
    : m_attribute(attribute), m_type(type),
      m_tupleSize(tupleSize), m_stride(stride)
{
    Q_ASSERT(tupleSize >= 1 && tupleSize <= 4);
}

inline bool QGLAttributeDescription::isNull() const
{
    return m_tupleSize == 0;
}

inline QGL::VertexAttribute QGLAttributeDescription::attribute() const
{
    return m_attribute;
}

inline void QGLAttributeDescription::setAttribute(QGL::VertexAttribute attribute)
{
    m_attribute = attribute;
}

inline GLenum QGLAttributeDescription::type() const
{
    return m_type;
}

inline void QGLAttributeDescription::setType(GLenum type)
{
    m_type = type;
}

inline int QGLAttributeDescription::tupleSize() const
{
    return m_tupleSize;
}

inline void QGLAttributeDescription::setTupleSize(int tupleSize)
{
    Q_ASSERT(tupleSize >= 1 && tupleSize <= 4);
    m_tupleSize = tupleSize;
}

inline int QGLAttributeDescription::stride() const
{
    return m_stride;
}

inline void QGLAttributeDescription::setStride(int stride)
{
    m_stride = stride;
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
