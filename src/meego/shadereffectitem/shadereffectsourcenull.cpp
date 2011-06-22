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

#include "shadereffectsourcenull.h"
#include "shadereffect.h"
#include "glfunctions.h"

#include <QDebug>

ShaderEffectSourceNull::ShaderEffectSourceNull(QObject *parent)
    : QObject(parent)
    , m_sourceItem(0)
    , m_mipmap(None)
    , m_filtering(Nearest)
    , m_horizontalWrap(ClampToEdge)
    , m_verticalWrap(ClampToEdge)
    , m_margins(0, 0)
    , m_size(0, 0)
    , m_texture(0)
    , m_texture_mipmaps(false)
    , m_fbo(0)
    , m_multisampledFbo(0)
    , m_renderer(0)
    , m_refs(0)
    , m_dirtyTexture(true)
    , m_dirtySceneGraph(true)
    , m_multisamplingSupported(false)
    , m_checkedForMultisamplingSupport(false)
    , m_live(true)
    , m_hideOriginal(true)
{
}

ShaderEffectSourceNull::~ShaderEffectSourceNull()
{
}

void ShaderEffectSourceNull::setSourceItem(QDeclarativeItem *item)
{
    if (item == m_sourceItem)
        return;

    if (m_sourceItem) {
        disconnect(m_sourceItem, SIGNAL(widthChanged()), this, SLOT(markSourceSizeDirty()));
        disconnect(m_sourceItem, SIGNAL(heightChanged()), this, SLOT(markSourceSizeDirty()));

        if (m_refs)
            detachSourceItem();

        if (m_refs && m_hideOriginal) {
//            Q_ASSERT(m_renderer);
//            QxItemPrivate *d = QxItemPrivate::get(m_sourceItem);
//            d->derefFromEffectItem();
//            m_renderer->setRootNode(0);
        }
    }

    m_sourceItem = item;

    if (m_sourceItem && !m_renderer) {
//        m_renderer = m_context->createRenderer();
//        connect(m_renderer, SIGNAL(sceneGraphChanged()), this, SLOT(markSceneGraphDirty()));
    }

    if (m_sourceItem) {
        if (m_refs)
            attachSourceItem();

        if (m_refs && m_hideOriginal) {
//            Q_ASSERT(m_renderer);
//            QxItemPrivate *d = QxItemPrivate::get(m_sourceItem);
//            d->refFromEffectItem();
//            m_renderer->setRootNode(d->rootNode);
        }
        connect(m_sourceItem, SIGNAL(widthChanged()), this, SLOT(markSourceSizeDirty()));
        connect(m_sourceItem, SIGNAL(heightChanged()), this, SLOT(markSourceSizeDirty()));
    }

    updateSizeAndTexture();
    emit sourceItemChanged();
}

void ShaderEffectSourceNull::setSourceImage(const QUrl &url)
{
    if (url == m_sourceImage)
        return;
    m_sourceImage = url;
    updateSizeAndTexture();
    emit sourceImageChanged();
}

void ShaderEffectSourceNull::setMipmap(FilterMode mode)
{
    Q_UNUSED(mode);
}

void ShaderEffectSourceNull::setFiltering(FilterMode mode)
{
    Q_UNUSED(mode);
}

void ShaderEffectSourceNull::setHorizontalWrap(WrapMode mode)
{
    Q_UNUSED(mode);
}

void ShaderEffectSourceNull::setVerticalWrap(WrapMode mode)
{
    Q_UNUSED(mode);
}

void ShaderEffectSourceNull::setMargins(const QSize &size)
{
    Q_UNUSED(size);
}

void ShaderEffectSourceNull::setTextureSize(const QSize &size)
{
    Q_UNUSED(size);
}

void ShaderEffectSourceNull::setLive(bool s)
{
    Q_UNUSED(s);
}

void ShaderEffectSourceNull::setHideOriginal(bool hide)
{
    Q_UNUSED(hide);
}

void ShaderEffectSourceNull::bind() const
{
}

void ShaderEffectSourceNull::refFromEffectItem()
{        
}

void ShaderEffectSourceNull::derefFromEffectItem()
{
}

void ShaderEffectSourceNull::update()
{
}

void ShaderEffectSourceNull::grab()
{
}

void ShaderEffectSourceNull::markSceneGraphDirty()
{
}

void ShaderEffectSourceNull::markSourceSizeDirty()
{
}

void ShaderEffectSourceNull::updateSizeAndTexture()
{
    if (m_sourceItem) {
        QSize size = m_textureSize;
        if (size.isEmpty())
            size = m_sourceItem->boundingRect().size().toSize() + 2 * m_margins;
        if (size.width() < 1)
            size.setWidth(1);
        if (size.height() < 1)
            size.setHeight(1);
        if (m_fbo && m_fbo->size() != size) {
            delete m_fbo;
            delete m_multisampledFbo;
            m_fbo = m_multisampledFbo = 0;
        }
        if (m_size.width() != size.width()) {
            m_size.setWidth(size.width());
            emit widthChanged();
        }
        if (m_size.height() != size.height()) {
            m_size.setHeight(size.height());
            emit heightChanged();
        }
        m_dirtyTexture = true;
    } else {
        if (m_fbo) {
            delete m_fbo;
            delete m_multisampledFbo;
            m_fbo = m_multisampledFbo = 0;
        }
        if (!m_sourceImage.isEmpty()) {
            // TODO: Implement async loading and loading over network.
            QImageReader reader(m_sourceImage.toLocalFile());
            if (!m_textureSize.isEmpty())
                reader.setScaledSize(m_textureSize);
            QImage image = reader.read();
            if (image.isNull())
                qWarning() << reader.errorString();
            if (m_size.width() != image.width()) {
                m_size.setWidth(image.width());
                emit widthChanged();
            }
            if (m_size.height() != image.height()) {
                m_size.setHeight(image.height());
                emit heightChanged();
            }

            m_texture = upload(image.mirrored());
/*
            if (m_mipmap) {
                glBindTexture(GL_TEXTURE_2D, m_texture);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
*/
        } else {
            if (m_size.width() != 0) {
                m_size.setWidth(0);
                emit widthChanged();
            }
            if (m_size.height() != 0) {
                m_size.setHeight(0);
                emit heightChanged();
            }
        }
    }
}

void ShaderEffectSourceNull::swizzleBGRAToRGBA(QImage *image)
{
    Q_UNUSED(image);
}

uint ShaderEffectSourceNull::upload(const QImage &image, GLuint id)
{
    Q_UNUSED(image);
    Q_UNUSED(id);
    return 0;
}

void ShaderEffectSourceNull::attachSourceItem()
{
}

void ShaderEffectSourceNull::detachSourceItem()
{
}

