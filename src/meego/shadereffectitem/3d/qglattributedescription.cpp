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

#include "qglattributedescription.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGLQGLAttributeDescription
    \brief The QGLQGLAttributeDescription class encapsulates information about an OpenGL attribute value's layout and type.
    \since 4.8
    \ingroup qt3d
    \ingroup qt3d::enablers

    OpenGL has many functions that take a pointer to vertex attribute
    values: \c{glVertexPointer()}, \c{glNormalPointer()},
    \c{glVertexAttribPointer()}, etc.  These functions typically
    take four arguments: tuple size (1, 2, 3, or 4), component type
    (e.g. GL_FLOAT), stride, and data pointer (\c{glNormalPointer()}
    does not use tuple size, assuming that it is 3).  When used with
    vertex buffers, the data pointer may be an offset into the vertex
    buffer instead.

    QGLQGLAttributeDescription encapsulates the vertex attribute() kind
    (QGL::Position, QGL::Normal, etc) with the type(), tupleSize(),
    and stride() information of an attribute.  The companion
    QGLAttributeValue class adds the data pointer.

    \sa QGLAttributeValue
*/

/*!
    \fn QGLQGLAttributeDescription::QGLQGLAttributeDescription()

    Constructs a null attribute description with default parameters of
    tupleSize() and stride() set to zero, type() set to GL_FLOAT,
    and attribute() set to QGL::Position.

    \sa isNull()
*/

/*!
    \fn QGLQGLAttributeDescription::QGLQGLAttributeDescription(QGL::VertexAttribute attribute, int tupleSize, GLenum type, int stride)

    Constructs an attribute description with the fields \a attribute,
    \a tupleSize, \a type, and \a stride.
*/

/*!
    \fn bool QGLQGLAttributeDescription::isNull() const

    Returns true if tupleSize() is zero, which indicates an unset
    attribute description; false otherwise.
*/

/*!
    \fn QGL::VertexAttribute QGLQGLAttributeDescription::attribute() const

    Returns the vertex attribute that this description applies to.
    The default value is QGL::Position.

    \sa setAttribute(), type()
*/

/*!
    \fn void QGLQGLAttributeDescription::setAttribute(QGL::VertexAttribute attribute)

    Sets the vertex \a attribute that this description applies to.

    \sa attribute()
*/

/*!
    \fn GLenum QGLQGLAttributeDescription::type() const

    Returns the component type for this attribute description.  The default
    value is GL_FLOAT.

    \sa setType(), sizeOfType(), attribute()
*/

/*!
    \fn void QGLQGLAttributeDescription::setType(GLenum type)

    Sets the component \a type for this attribute description.

    \sa type(), sizeOfType()
*/

/*!
    Returns the size in bytes of type().

    \sa type(), tupleSize()
*/
int QGLAttributeDescription::sizeOfType() const
{
    switch (m_type) {
    case GL_BYTE:           return int(sizeof(GLbyte));
    case GL_UNSIGNED_BYTE:  return int(sizeof(GLubyte));
    case GL_SHORT:          return int(sizeof(GLshort));
    case GL_UNSIGNED_SHORT: return int(sizeof(GLushort));
    case GL_INT:            return int(sizeof(GLint));
    case GL_UNSIGNED_INT:   return int(sizeof(GLuint));
    case GL_FLOAT:          return int(sizeof(GLfloat));
#if defined(GL_DOUBLE) && !defined(QT_OPENGL_ES)
    case GL_DOUBLE:         return int(sizeof(GLdouble));
#endif
    default:                return 0;
    }
}

/*!
    \fn int QGLQGLAttributeDescription::tupleSize() const

    Returns the tuple size of this attribute in components.  For example,
    a return value of 3 indicates a vector of 3-dimensional values.
    If tupleSize() is zero, then this attribute description is null.

    \sa setTupleSize(), isNull(), sizeOfType()
*/

/*!
    \fn void QGLQGLAttributeDescription::setTupleSize(int tupleSize)

    Sets the tuple size of this attribute in components to \a tupleSize.

    \sa tupleSize()
*/

/*!
    \fn int QGLQGLAttributeDescription::stride() const

    Returns the stride in bytes from one vertex element to the
    next for this attribute description.  The default value of 0 indicates
    that the elements are tightly packed within the data array.

    \sa setStride()
*/

/*!
    \fn void QGLQGLAttributeDescription::setStride(int stride)

    Sets the \a stride in bytes from one vertex element to the next
    for this attribute description.

    \sa stride()
*/

QT_END_NAMESPACE
