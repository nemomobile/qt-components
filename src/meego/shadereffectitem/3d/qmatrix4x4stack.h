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

#ifndef QMATRIX4X4STACK_H
#define QMATRIX4X4STACK_H

#include "qt3dglobal.h"
#include <QtGui/qmatrix4x4.h>
#include <QtCore/qscopedpointer.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QMatrix4x4StackPrivate;

class Q_QT3D_EXPORT QMatrix4x4Stack
{
public:
    QMatrix4x4Stack();
    ~QMatrix4x4Stack();

    const QMatrix4x4 &top() const;

    void push();
    void pop();

    void setToIdentity();

    void translate(qreal x, qreal y, qreal z);
    void translate(const QVector3D& vector);
    void scale(qreal x, qreal y, qreal z);
    void scale(qreal factor);
    void scale(const QVector3D& vector);
    void rotate(qreal angle, qreal x, qreal y, qreal z);
    void rotate(qreal angle, const QVector3D& vector);
    void rotate(const QQuaternion &quaternion);

    QMatrix4x4Stack& operator=(const QMatrix4x4& matrix);
    QMatrix4x4Stack& operator*=(const QMatrix4x4& matrix);

    operator const QMatrix4x4 &() const;

    bool isDirty() const;
    void setDirty(bool dirty);

private:
    Q_DISABLE_COPY(QMatrix4x4Stack)
    Q_DECLARE_PRIVATE(QMatrix4x4Stack)

    QScopedPointer<QMatrix4x4StackPrivate> d_ptr;

    friend class QGLPainter;
};

inline QMatrix4x4Stack::operator const QMatrix4x4 &() const
{
    return top();
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
