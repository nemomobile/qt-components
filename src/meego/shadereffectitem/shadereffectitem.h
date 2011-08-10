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

#ifndef SHADEREFFECTITEM_H
#define SHADEREFFECTITEM_H

#include <QDeclarativeItem>
#include <QtOpenGL>
#include "shadereffectsource.h"
#include "scenegraph/qsggeometry.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class ShaderEffectItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QString fragmentShader READ fragmentShader WRITE setFragmentShader NOTIFY fragmentShaderChanged)
    Q_PROPERTY(QString vertexShader READ vertexShader WRITE setVertexShader NOTIFY vertexShaderChanged)
    Q_PROPERTY(bool blending READ blending WRITE setBlending NOTIFY blendingChanged)
    Q_PROPERTY(QSize meshResolution READ meshResolution WRITE setMeshResolution NOTIFY meshResolutionChanged)

public:
    ShaderEffectItem(QDeclarativeItem* parent = 0);
    ~ShaderEffectItem();

    virtual void componentComplete();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    QString fragmentShader() const { return m_fragment_code; }
    void setFragmentShader(const QString &code);

    QString vertexShader() const { return m_vertex_code; }
    void setVertexShader(const QString &code);

    bool blending() const { return m_blending; }
    void setBlending(bool enable);

    QSize meshResolution() const { return m_meshResolution; }
    void setMeshResolution(const QSize &size);

    void preprocess();

Q_SIGNALS:
    void fragmentShaderChanged();
    void vertexShaderChanged();
    void blendingChanged();
    void activeChanged();
    void meshResolutionChanged();

protected:
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private Q_SLOTS:
    void changeSource(int index);
    void handleVisibilityChange();
    void markDirty();

private:
    void checkViewportUpdateMode();
    void renderEffect(QPainter *painter, const QMatrix4x4 &matrix);
    void updateEffectState(const QMatrix4x4 &matrix);
    void updateGeometry();
    void bindGeometry();
    void setSource(const QVariant &var, int index);
    void disconnectPropertySignals();
    void connectPropertySignals();
    void reset();
    void updateProperties();
    void updateShaderProgram();
    void lookThroughShaderCode(const QString &code);
    bool active() const { return m_active; }
    void setActive(bool enable);

private:
    QString m_fragment_code;
    QString m_vertex_code;
    QGLShaderProgram m_program;
    QVector<const char *> m_attributeNames;
    QSet<QByteArray> m_uniformNames;
    QSize m_meshResolution;
    QSGGeometry m_geometry;

    struct SourceData
    {
        QSignalMapper *mapper;
        QPointer<ShaderEffectSource> source;
        QPointer<QDeclarativeItem> item;
        QByteArray name;
    };

    QVector<SourceData> m_sources;

    bool m_changed : 1;
    bool m_blending : 1;
    bool m_program_dirty : 1;
    bool m_active : 1;
    bool m_respectsMatrix : 1;
    bool m_respectsOpacity : 1;
    bool m_checkedViewportUpdateMode : 1;
    bool m_checkedOpenGL : 1;
    bool m_checkedShaderPrograms : 1;
    bool m_hasShaderPrograms : 1;
    bool m_mirrored : 1;
    bool m_defaultVertexShader : 1;
};

QT_END_HEADER

QT_END_NAMESPACE

#endif // SHADEREFFECTITEM_H
