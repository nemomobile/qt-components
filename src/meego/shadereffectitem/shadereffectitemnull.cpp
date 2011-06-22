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

#include "shadereffectitemnull.h"
#include "shadereffect.h"
#include "utilities.h"
#include "glfunctions.h"
#include "3d/qglattributevalue.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QtOpenGL>


static const char qt_default_vertex_code[] =
        "uniform highp mat4 qt_ModelViewProjectionMatrix;\n"
        "attribute highp vec4 qt_Vertex;\n"
        "attribute highp vec2 qt_MultiTexCoord0;\n"
        "varying highp vec2 qt_TexCoord0;\n"
        "void main(void)\n"
        "{\n"
            "qt_TexCoord0 = qt_MultiTexCoord0;\n"
            "gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;\n"
        "}\n";

static const char qt_default_fragment_code[] =
        "varying highp vec2 qt_TexCoord0;\n"
        "uniform sampler2D source;\n"
        "void main(void)\n"
        "{\n"
            "gl_FragColor = texture2D(source, qt_TexCoord0.st);\n"
        "}\n";


ShaderEffectItemNull::ShaderEffectItemNull(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
    , m_mesh_resolution(1, 1)
    , m_blending(true)
    , m_program_dirty(true)
    , m_active(true)
    , m_respects_matrix(false)
    , m_respects_opacity(false)
{
}

ShaderEffectItemNull::~ShaderEffectItemNull()
{
}

void ShaderEffectItemNull::componentComplete()
{
    QDeclarativeItem::componentComplete();
}

void ShaderEffectItemNull::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    Q_UNUSED(painter);
}

void ShaderEffectItemNull::renderEffect(QGLContext* context, const QMatrix4x4& matrix)
{
    Q_UNUSED(context);
    Q_UNUSED(matrix);
}

void ShaderEffectItemNull::updateEffectState(const QMatrix4x4& matrix)
{
    Q_UNUSED(matrix);
}

const QGL::VertexAttribute *ShaderEffectItemNull::requiredFields() const
{
    return m_attributes.constData();
}

void ShaderEffectItemNull::updateGeometry()
{
}


void ShaderEffectItemNull::setFragmentShader(const QString &code)
{
    Q_UNUSED(code);
}

void ShaderEffectItemNull::setVertexShader(const QString &code)
{
    Q_UNUSED(code);
}

void ShaderEffectItemNull::setBlending(bool enable)
{
    Q_UNUSED(enable);
}

void ShaderEffectItemNull::setActive(bool enable)
{
    Q_UNUSED(enable);
}

void ShaderEffectItemNull::setMeshResolution(const QSize &size)
{
    Q_UNUSED(size);
}

void ShaderEffectItemNull::preprocess()
{
}

void ShaderEffectItemNull::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_UNUSED(newGeometry);
    Q_UNUSED(oldGeometry);
}

void ShaderEffectItemNull::changeSource(int index)
{
    Q_UNUSED(index);
}

void ShaderEffectItemNull::markDirty() {
}

void ShaderEffectItemNull::setSource(QVariant var, int index)
{
    Q_UNUSED(var);
    Q_UNUSED(index);
}

void ShaderEffectItemNull::disconnectPropertySignals()
{
}

void ShaderEffectItemNull::connectPropertySignals()
{
}

void ShaderEffectItemNull::reset()
{
}

void ShaderEffectItemNull::updateProperties()
{
}


void ShaderEffectItemNull::updateShaderProgram()
{
}

void ShaderEffectItemNull::lookThroughShaderCode(const QString &code)
{
    Q_UNUSED(code);
}
