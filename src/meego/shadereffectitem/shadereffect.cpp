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

#include "shadereffect.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>


ShaderEffect::ShaderEffect(QObject *parent)
    : QGraphicsEffect(parent)
    , m_changed(true)
{
}

ShaderEffect::~ShaderEffect()
{
}

void ShaderEffect::draw (QPainter* painter)
{
    QGLContext* context = const_cast<QGLContext*>(QGLContext::currentContext());

    if (context){
        updateRenderTargets();
    } else if(m_renderTargets.count() > 0) {
        qWarning() << "ShaderEffect::draw - OpenGL not available";
    }

    if (m_renderTargets.count() == 0 || !hideOriginal()){
        drawSource(painter);
    }
}

void ShaderEffect::updateRenderTargets()
{
    if (!m_changed) return;

    m_changed = false;

    int count = m_renderTargets.count();
    for (int i=0; i < count; i++){
        if (m_renderTargets[i]->isLive() || m_renderTargets[i]->isDirtyTexture()){
            m_renderTargets[i]->update();
            QGLFramebufferObject* target = m_renderTargets[i]->fbo();
            if (target && target->isValid() && target->width() > 0 && target->height() > 0) {
                QPainter p(target);
                p.setCompositionMode(QPainter::CompositionMode_Clear);
                p.fillRect(QRect(QPoint(0,0), target->size()), Qt::transparent);
                p.setCompositionMode(QPainter::CompositionMode_SourceOver);

                QSize textureSize = m_renderTargets[i]->textureSize();
                if (!textureSize.isEmpty()){
                    qreal xscale = qreal(textureSize.width()) / qreal(m_renderTargets[i]->sourceItem()->width());
                    qreal yscale = qreal(textureSize.height()) / qreal(m_renderTargets[i]->sourceItem()->height());
                    p.scale(xscale, yscale);
                }

                drawSource(&p);
                p.end();
            }
        }
    }
}

void ShaderEffect::sourceChanged (ChangeFlags flags)
{
    Q_UNUSED(flags);
    int count = m_renderTargets.count();
    for (int i=0; i < count; i++){
        if (m_renderTargets[i]->isLive()) {
            m_renderTargets[i]->markSceneGraphDirty();
        }
    }
    m_changed = true;
}

void ShaderEffect::addRenderTarget(ShaderEffectSource* target)
{
    if (!m_renderTargets.contains(target)){
        m_renderTargets.append(target);
    }
}

void ShaderEffect::removeRenderTarget(ShaderEffectSource* target)
{
    int index = m_renderTargets.indexOf(target);
    if (index >= 0)
        m_renderTargets.remove(index);
    else
        qWarning() << "ShaderEffect::removeRenderTarget - did not find target.";
}

bool ShaderEffect::hideOriginal() const
{
    if (m_renderTargets.count() == 0)
        return false;

    // Hmm, if there is even one source that says "do not hide original" we don't hide it.
    int count = m_renderTargets.count();
    for (int i=0; i < count; i++){
        if (!m_renderTargets[i]->hideOriginal()){
            return false;
        }
    }
    return true;
}
