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

#include "shadereffectsource.h"
#include "shadereffect.h"
#include "glfunctions.h"

#include <QDebug>

ShaderEffectSource::ShaderEffectSource(QObject *parent)
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
//    m_context = QSGContext::current;
}

ShaderEffectSource::~ShaderEffectSource()
{
    if (m_refs && m_sourceItem)
        detachSourceItem();

//    if (m_refs && m_sourceItem)
//        QxItemPrivate::get(m_sourceItem)->derefFromEffectItem();

    delete m_fbo;
    delete m_multisampledFbo;
    glDeleteTextures(1, &m_texture);

}

void ShaderEffectSource::setSourceItem(QDeclarativeItem *item)
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
    emit repaintRequired();
}

void ShaderEffectSource::setSourceImage(const QUrl &url)
{
    if (url == m_sourceImage)
        return;
    m_sourceImage = url;
    updateSizeAndTexture();
    emit sourceImageChanged();
    emit repaintRequired();
}

void ShaderEffectSource::setMipmap(FilterMode mode)
{
    if (mode == m_mipmap)
        return;
    m_mipmap = mode;
    if (m_mipmap != None) {
        qWarning() << "ShaderEffectSource::setMipmap - mipmaps (for now) are NOT supported!";

        // Mipmap enabled, need to reallocate the textures.
//        if (m_fbo && !m_fbo->format().mipmap()) {
//            Q_ASSERT(m_sourceItem);
//            delete m_fbo;
//            m_fbo = 0;
//            m_dirtyTexture = true;
//        } else if (m_texture) {
//            Q_ASSERT(!m_sourceImage.isEmpty());
//            if (!m_texture_mipmaps)
//                updateSizeAndTexture();
//        }
    }
    emit mipmapChanged();
    emit repaintRequired();
}

void ShaderEffectSource::setFiltering(FilterMode mode)
{
    if (mode == m_filtering)
        return;
    m_filtering = mode;
    emit filteringChanged();
    emit repaintRequired();
}

void ShaderEffectSource::setHorizontalWrap(WrapMode mode)
{
    if (mode == m_horizontalWrap)
        return;
    m_horizontalWrap = mode;
    emit horizontalWrapChanged();
    emit repaintRequired();
}

void ShaderEffectSource::setVerticalWrap(WrapMode mode)
{
    if (mode == m_verticalWrap)
        return;
    m_verticalWrap = mode;
    emit verticalWrapChanged();
    emit repaintRequired();
}

void ShaderEffectSource::setMargins(const QSize &size)
{
    if (size == m_margins)
        return;
    m_margins = size;
    updateSizeAndTexture();
    emit marginsChanged();
    emit repaintRequired();
}

void ShaderEffectSource::setTextureSize(const QSize &size)
{
    if (size == m_textureSize)
        return;
    m_textureSize = size;
    updateSizeAndTexture();
    emit textureSizeChanged();
    emit repaintRequired();

    if (m_sourceItem) {
        ShaderEffect* effect = qobject_cast<ShaderEffect*> (m_sourceItem->graphicsEffect());
        if (effect)
            effect->m_changed = true;
    }
}

void ShaderEffectSource::setLive(bool s)
{
    if (s == m_live)
        return;
    m_live = s;

    emit liveChanged();
    emit repaintRequired();
}

void ShaderEffectSource::setHideOriginal(bool hide)
{
    if (hide == m_hideOriginal)
        return;

//    if (m_refs && m_sourceItem && m_hideOriginal) {
//        Q_ASSERT(m_renderer);
//        m_renderer->setRootNode(0);
//        QxItemPrivate *d = QxItemPrivate::get(m_sourceItem);
//        d->derefFromEffectItem();
//    }

    m_hideOriginal = hide;

//    if (m_refs && m_sourceItem && m_hideOriginal) {
//        Q_ASSERT(m_renderer);
//        QxItemPrivate *d = QxItemPrivate::get(m_sourceItem);
//        d->refFromEffectItem();
//        m_renderer->setRootNode(d->rootNode);
//    }

    emit hideOriginalChanged();
    emit repaintRequired();
}

void ShaderEffectSource::bind() const
{
//    qDebug() << "ShaderEffectSource::bind()";
    bool linear = m_filtering == Linear;

    GLint filtering = GL_NEAREST;
    switch (m_mipmap) {
    case Nearest:
        filtering = linear ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
        break;
    case Linear:
        filtering = linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
        break;
    default:
        filtering = linear ? GL_LINEAR : GL_NEAREST;
        break;
    }

    GLuint hwrap = m_horizontalWrap == Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    GLuint vwrap = m_verticalWrap == Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;

#if !defined(QT_OPENGL_ES_2)
    glEnable(GL_TEXTURE_2D);
#endif
    if (m_fbo) {
        glBindTexture(GL_TEXTURE_2D, m_fbo->texture());
    } else if (m_texture) {
        glBindTexture(GL_TEXTURE_2D, m_texture);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hwrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vwrap);
}

void ShaderEffectSource::refFromEffectItem()
{
      if (m_refs++ == 0) {
          attachSourceItem();
          emit activeChanged();
      }


//    if (m_refs++ == 0) {
//        if (m_sourceItem && m_hideOriginal) {
//            Q_ASSERT(m_renderer);
//            QxItemPrivate *d = QxItemPrivate::get(m_sourceItem);
//            d->refFromEffectItem();
//            m_renderer->setRootNode(d->rootNode);
//        }
//        emit activeChanged();
//    }
}

void ShaderEffectSource::derefFromEffectItem()
{
    if (--m_refs == 0) {
        detachSourceItem();
        emit activeChanged();
    }
    Q_ASSERT(m_refs >= 0);

//    if (--m_refs == 0) {
//        if (m_sourceItem && m_hideOriginal) {
//            Q_ASSERT(m_renderer);
//            m_renderer->setRootNode(0);
//            QxItemPrivate *d = QxItemPrivate::get(m_sourceItem);
//            d->derefFromEffectItem();
//        }
//        emit activeChanged();
//    }
//    Q_ASSERT(m_refs >= 0);
}

void ShaderEffectSource::update()
{
//    qDebug() << "ShaderEffectSource::update()";
    QSize size = m_sourceItem->boundingRect().toRect().size();
    if (!m_textureSize.isEmpty())
        size = m_textureSize;

    if (size.height() > 0 && size.width() > 0){
        QGLFramebufferObjectFormat format;
        format.setInternalTextureFormat(GLenum(GL_RGBA));

        if (!m_fbo) {
            m_fbo =  new QGLFramebufferObject(size, format);
        } else {
            if (m_fbo->size() != size) {
                delete m_fbo;
                m_fbo = 0;
                m_fbo =  new QGLFramebufferObject(size, format);
            }
        }
    }

//    Q_ASSERT(m_refs);
//    QxItemPrivate *src = m_sourceItem ? QxItemPrivate::get(m_sourceItem) : 0;
//    Node::DirtyFlags dirtyFlags = src ? src->transformNode.dirtyFlags() : Node::DirtyFlags(0);
//    if (!m_dirtyTexture && (!(m_dirtySceneGraph || dirtyFlags) || !m_live))
//        return;

//    if (m_sourceItem) {
//        Q_ASSERT(m_hideOriginal == (m_renderer->rootNode() != 0));
//        if (!m_hideOriginal) {
//            src->refFromEffectItem();
//            m_renderer->setRootNode(src->rootNode);
//        }
//        Q_ASSERT(src->rootNode && src->rootNode == m_renderer->rootNode());
//        Q_ASSERT(m_renderer);

//        const QGLContext *ctx = m_context->glContext();

//        if (!m_checkedForMultisamplingSupport) {
//            QList<QByteArray> extensions = QByteArray((const char *)glGetString(GL_EXTENSIONS)).split(' ');
//            m_multisamplingSupported = extensions.contains("GL_EXT_framebuffer_multisample")
//                                       && extensions.contains("GL_EXT_framebuffer_blit");
//            m_checkedForMultisamplingSupport = true;
//        }

//        if (!m_fbo) {
//            if (ctx->format().sampleBuffers() && m_multisamplingSupported) {
//                // If mipmapping was just enabled, m_fbo might be 0 while m_multisampledFbo != 0.
//                if (!m_multisampledFbo) {
//                    QGLFramebufferObjectFormat format;
//                    format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
//                    format.setSamples(ctx->format().samples());
//                    m_multisampledFbo = new QGLFramebufferObject(m_size, format);
//                }
//                {
//                    QGLFramebufferObjectFormat format;
//                    format.setAttachment(QGLFramebufferObject::NoAttachment);
//                    format.setMipmap(m_mipmap != None);
//                    m_fbo = new QGLFramebufferObject(m_size, format);
//                }
//            } else {
//                QGLFramebufferObjectFormat format;
//                format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
//                format.setMipmap(m_mipmap != None);
//                m_fbo = new QGLFramebufferObject(m_size, format);
//            }
//        }

//        Q_ASSERT(m_size == m_fbo->size());
//        Q_ASSERT(m_multisampledFbo == 0 || m_size == m_multisampledFbo->size());

//        QRectF r(0, 0, m_sourceItem->width(), m_sourceItem->height());
//        r.adjust(-m_margins.width(), -m_margins.height(), m_margins.width(), m_margins.height());
//        m_renderer->setDeviceRect(m_size);
//        m_renderer->setProjectMatrixToRect(r);
//        m_renderer->setClearColor(Qt::transparent);

//        if (m_multisampledFbo) {
//            m_renderer->renderScene(BindableFbo(const_cast<QGLContext *>(m_context->glContext()), m_multisampledFbo));
//            QRect r(0, 0, m_size.width(), m_size.height());
//            QGLFramebufferObject::blitFramebuffer(m_fbo, r, m_multisampledFbo, r);
//        } else {
//            m_renderer->renderScene(BindableFbo(const_cast<QGLContext *>(m_context->glContext()), m_fbo));
//        }
//        if (m_mipmap != None) {
//            QGLFramebufferObject::bindDefault();
//            glBindTexture(GL_TEXTURE_2D, m_fbo->texture());
//            m_context->renderer()->glGenerateMipmap(GL_TEXTURE_2D);
//        }

//        if (!m_hideOriginal) {
//            m_renderer->setRootNode(0);
//            src->derefFromEffectItem();
//            src->transformNode.markDirty(dirtyFlags);
//        }

//        m_dirtySceneGraph = false;
//    }
    m_dirtyTexture = false;
//    emit repaintRequired();
}

void ShaderEffectSource::grab()
{
    m_dirtyTexture = true;
    emit repaintRequired();
}

void ShaderEffectSource::markSceneGraphDirty()
{
    m_dirtySceneGraph = true;
    emit repaintRequired();
}

void ShaderEffectSource::markSourceSizeDirty()
{
    Q_ASSERT(m_sourceItem);
    if (m_textureSize.isEmpty())
        updateSizeAndTexture();
    if (m_refs)
        emit repaintRequired();
}

void ShaderEffectSource::updateSizeAndTexture()
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

            if (m_mipmap) {
                glBindTexture(GL_TEXTURE_2D, m_texture);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

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

void ShaderEffectSource::swizzleBGRAToRGBA(QImage *image)
{
    const int width = image->width();
    const int height = image->height();
    for (int i = 0; i < height; ++i) {
        uint *p = (uint *) image->scanLine(i);
        for (int x = 0; x < width; ++x)
            p[x] = ((p[x] << 16) & 0xff0000) | ((p[x] >> 16) & 0xff) | (p[x] & 0xff00ff00);
    }
}

uint ShaderEffectSource::upload(const QImage &image, GLuint id)
{
    // image not already scheduled, upload normally...
    while (glGetError() != GL_NO_ERROR) {}

    if (id == 0)
        glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);

    QImage i = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

#ifdef QT_OPENGL_ES
    swizzleBGRAToRGBA(&i);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i.width(), i.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, i.constBits());
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.constBits());
#endif

    // Gracefully fail in case of an error...
    GLuint error = glGetError();
    if (error != GL_NO_ERROR) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &id);
        return 0;
    }

    return id;
}

void ShaderEffectSource::attachSourceItem()
{
    if (!m_sourceItem)
        return;

    ShaderEffect* effect = qobject_cast<ShaderEffect*> (m_sourceItem->graphicsEffect());

    if (!effect){
        effect = new ShaderEffect();
        m_sourceItem->setGraphicsEffect(effect);
    }

    if (effect)
        effect->addRenderTarget(this);

    m_sourceItem->update();
}

void ShaderEffectSource::detachSourceItem()
{
    if (!m_sourceItem)
        return;

    ShaderEffect* effect = qobject_cast<ShaderEffect*> (m_sourceItem->graphicsEffect());

    if (effect)
        effect->removeRenderTarget(this);

    delete m_fbo;
    m_fbo = 0;
    delete m_multisampledFbo;
    m_multisampledFbo = 0;
    m_dirtyTexture = true;
}

