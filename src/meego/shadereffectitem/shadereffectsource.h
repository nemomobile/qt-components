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

#ifndef SHADEREFFECTSOURCE_H
#define SHADEREFFECTSOURCE_H

#include <QDeclarativeItem>
#include <QtOpenGL>
#include <QPointer>

class Renderer;
class QSGContext;

class ShaderEffectSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeItem *sourceItem READ sourceItem WRITE setSourceItem NOTIFY sourceItemChanged)
    Q_PROPERTY(QUrl sourceImage READ sourceImage WRITE setSourceImage NOTIFY sourceImageChanged)
    Q_PROPERTY(FilterMode mipmap READ mipmap WRITE setMipmap NOTIFY mipmapChanged)
    Q_PROPERTY(FilterMode filtering READ filtering WRITE setFiltering NOTIFY filteringChanged)
    Q_PROPERTY(WrapMode horizontalWrap READ horizontalWrap WRITE setHorizontalWrap NOTIFY horizontalWrapChanged)
    Q_PROPERTY(WrapMode verticalWrap READ verticalWrap WRITE setVerticalWrap NOTIFY verticalWrapChanged)
    Q_PROPERTY(QSize margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(QSize textureSize READ textureSize WRITE setTextureSize NOTIFY textureSizeChanged)
    Q_PROPERTY(int width READ width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)
    Q_PROPERTY(bool live READ isLive WRITE setLive NOTIFY liveChanged)
    Q_PROPERTY(bool hideOriginal READ hideOriginal WRITE setHideOriginal NOTIFY hideOriginalChanged)
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)
    Q_ENUMS(FilterMode)
    Q_ENUMS(WrapMode)

public:
    enum FilterMode
    {
        None,
        Nearest,
        Linear
    };

    enum WrapMode
    {
        Repeat,
        ClampToEdge
    };

    ShaderEffectSource(QObject *parent = 0);
    virtual ~ShaderEffectSource();

    QSGContext *sceneGraphContext() const { return m_context; }
    void setSceneGraphContext(QSGContext *context) { m_context = context; }

    QDeclarativeItem *sourceItem() const { return m_sourceItem.data(); }
    void setSourceItem(QDeclarativeItem *item);

    QUrl sourceImage() const { return m_sourceImage; }
    void setSourceImage(const QUrl &url);

    FilterMode mipmap() const { return m_mipmap; }
    void setMipmap(FilterMode mode);

    FilterMode filtering() const { return m_filtering; }
    void setFiltering(FilterMode mode);

    WrapMode horizontalWrap() const { return m_horizontalWrap; }
    void setHorizontalWrap(WrapMode mode);

    WrapMode verticalWrap() const { return m_verticalWrap; }
    void setVerticalWrap(WrapMode mode);

    QSize margins() const { return m_margins; }
    void setMargins(const QSize &size);

    QSize textureSize() const { return m_textureSize; }
    void setTextureSize(const QSize &size);

    int width() const { return m_size.width(); }
    int height() const { return m_size.height(); }

    bool isLive() const { return m_live; }
    void setLive(bool s);

    bool hideOriginal() const { return m_hideOriginal; }
    void setHideOriginal(bool hide);

    bool isActive() const { return m_refs; }

    void bind() const;

    void refFromEffectItem();
    void derefFromEffectItem();
    void update();

    QGLFramebufferObject* fbo() { return m_fbo; }
    bool isDirtyTexture() { return m_dirtyTexture == true;}

    Q_INVOKABLE void grab();

Q_SIGNALS:
    void sourceItemChanged();
    void sourceImageChanged();
    void mipmapChanged();
    void filteringChanged();
    void horizontalWrapChanged();
    void verticalWrapChanged();
    void marginsChanged();
    void textureSizeChanged();
    void widthChanged();
    void heightChanged();
    void liveChanged();
    void hideOriginalChanged();
    void activeChanged();

    void repaintRequired();

public Q_SLOTS:
    void markSceneGraphDirty();
    void markSourceSizeDirty();

private:
    void updateSizeAndTexture();

    void attachSourceItem();
    void detachSourceItem();

    static void swizzleBGRAToRGBA(QImage *image);
    uint upload(const QImage &image, GLuint id = 0);

    QPointer<QDeclarativeItem> m_sourceItem;
    QUrl m_sourceImage;
    FilterMode m_mipmap;
    FilterMode m_filtering;
    WrapMode m_horizontalWrap;
    WrapMode m_verticalWrap;
    QSize m_margins;
    QSize m_textureSize;
    QSize m_size;

    uint m_texture;
    bool m_texture_mipmaps;

    QGLFramebufferObject *m_fbo;
    QGLFramebufferObject *m_multisampledFbo;
    Renderer *m_renderer;
    QSGContext *m_context;
    int m_refs;
    uint m_dirtyTexture : 1; // Causes update no matter what.
    uint m_dirtySceneGraph : 1; // Causes update if not static.
    uint m_multisamplingSupported : 1;
    uint m_checkedForMultisamplingSupport : 1;
    uint m_live : 1;
    uint m_hideOriginal : 1;
};
#endif // SHADEREFFECTSOURCE_H
