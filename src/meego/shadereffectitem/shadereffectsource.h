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

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class ShaderEffectBuffer;

class ShaderEffectSource : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeItem *sourceItem READ sourceItem WRITE setSourceItem NOTIFY sourceItemChanged)
    Q_PROPERTY(QRectF sourceRect READ sourceRect WRITE setSourceRect NOTIFY sourceRectChanged)
    Q_PROPERTY(QSize textureSize READ textureSize WRITE setTextureSize NOTIFY textureSizeChanged)
    Q_PROPERTY(bool live READ isLive WRITE setLive NOTIFY liveChanged)
    Q_PROPERTY(bool hideSource READ hideSource WRITE setHideSource NOTIFY hideSourceChanged)
    Q_PROPERTY(WrapMode wrapMode READ wrapMode WRITE setWrapMode NOTIFY wrapModeChanged)
    Q_ENUMS(WrapMode)
    Q_ENUMS(Format)

public:
    enum WrapMode {
            ClampToEdge,
            RepeatHorizontally,
            RepeatVertically,
            Repeat
        };

    enum Format {
        Alpha = GL_ALPHA,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };

    ShaderEffectSource(QDeclarativeItem *parent = 0);
    virtual ~ShaderEffectSource();

    QDeclarativeItem *sourceItem() const { return m_sourceItem.data(); }
    void setSourceItem(QDeclarativeItem *item);

    QRectF sourceRect() const { return m_sourceRect; };
    void setSourceRect(const QRectF &rect);

    QSize textureSize() const { return m_textureSize; }
    void setTextureSize(const QSize &size);

    bool isLive() const { return m_live; }
    void setLive(bool s);

    bool hideSource() const { return m_hideSource; }
    void setHideSource(bool hide);

    WrapMode wrapMode() const { return m_wrapMode; };
    void setWrapMode(WrapMode mode);

    bool isActive() const { return m_refs; }
    void bind() const;
    void refFromEffectItem();
    void derefFromEffectItem();
    void updateBackbuffer();

    ShaderEffectBuffer* fbo() { return m_fbo; }
    bool isDirtyTexture() { return m_dirtyTexture; }
    bool isMirrored() { return m_mirrored; }

    Q_INVOKABLE void grab();

Q_SIGNALS:
    void sourceItemChanged();
    void sourceRectChanged();
    void textureSizeChanged();
    void formatChanged();
    void liveChanged();
    void hideSourceChanged();
    void activeChanged();
    void repaintRequired();
    void wrapModeChanged();

public Q_SLOTS:
    void markSceneGraphDirty();
    void markSourceSizeDirty();

private:
    void updateSizeAndTexture();
    void attachSourceItem();
    void detachSourceItem();

private:
    QPointer<QDeclarativeItem> m_sourceItem;
    WrapMode m_wrapMode;
    QRectF m_sourceRect;
    QSize m_textureSize;
    Format m_format;
    QSize m_size;

    ShaderEffectBuffer *m_fbo;
    ShaderEffectBuffer *m_multisampledFbo;
    int m_refs;
    bool m_dirtyTexture : 1;
    bool m_dirtySceneGraph : 1;
    bool m_multisamplingSupported : 1;
    bool m_checkedForMultisamplingSupport : 1;
    bool m_live : 1;
    bool m_hideSource : 1;
    bool m_mirrored : 1;
};

QT_END_HEADER

QT_END_NAMESPACE


#endif // SHADEREFFECTSOURCE_H
