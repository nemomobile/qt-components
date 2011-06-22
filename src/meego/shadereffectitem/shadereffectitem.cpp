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

#include "shadereffectitem.h"
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

ShaderEffectItem::ShaderEffectItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
    , m_mesh_resolution(1, 1)
    , m_blending(true)
    , m_program_dirty(true)
    , m_active(true)
    , m_respects_matrix(false)
    , m_respects_opacity(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    GeometryDataUploader::setUseBuffers(true);
    m_geometry = Utilities::createTexturedRectGeometry(QRectF(0, 0, 1, 1), QSize(1, 1), QRectF(0, 1, 1, -1));
    GeometryDataUploader::registerGeometry(m_geometry);
}

ShaderEffectItem::~ShaderEffectItem()
{
    GeometryDataUploader::unregisterGeometry(m_geometry);
    delete m_geometry;
}

void ShaderEffectItem::componentComplete()
{
    //m_node.setRect(QRectF(QPointF(), size()));
    updateProperties();
    QDeclarativeItem::componentComplete();
}

void ShaderEffectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_active) return;

    QGLContext* context = const_cast<QGLContext*>(QGLContext::currentContext());

    if (context){
        painter->save();
        painter->beginNativePainting();
        QMatrix4x4 combinedMatrix = QMatrix4x4(painter->transform());
        renderEffect(context, combinedMatrix);
        painter->endNativePainting();
        painter->restore();
    } else {
        qWarning() << "ShaderEffectItem::paint - OpenGL not available";
    }
}

void ShaderEffectItem::renderEffect(QGLContext* context, const QMatrix4x4& matrix)
{
    if (!m_geometry)
        return;

    // UGLY HACK
    /* renderEffect is called sometimes with a scale matrix
       and sometimes with a shear matrix
       by trial-and-error, we find that the scale matrix results in
       what has become known as the 'evil twin' - a duplicate
       magnifier squashed and in the wrong place.
       here we filter out the calls that cause the problem
       */
    if (matrix.column(0).x()!=0 && matrix.column(0).x()!=1) {
        return;
    }

    if (!m_program.isLinked())
        updateShaderProgram();

    m_program.bind();

    QPaintDevice *d = context->device();
    QMatrix4x4 combinedMatrix;
    combinedMatrix.scale(2.0/d->width(), -2.0/d->height(),1.0);
    combinedMatrix.translate(-d->width()/2.0, -d->height()/2.0 );
    combinedMatrix*=matrix;

    updateEffectState(combinedMatrix);

    for (int i = 0; i < m_attributeNames.size(); ++i)
        m_program.enableAttributeArray(m_attributes.at(i));

    Geometry *g = m_geometry;

    GeometryDataUploader::bind();
    GeometryDataUploader::upload();

    const QGL::VertexAttribute *attributes = requiredFields();
    int offset = 0;
    for (; *attributes != QGL::VertexAttribute(-1); ++attributes) {
        QGLAttributeValue attr = g->attributeValue(*attributes);
        if (!attr.isNull()) {
#if defined(QT_OPENGL_ES_2)
            GLboolean normalize = attr.type() != GL_FLOAT;
#else
            GLboolean normalize = attr.type() != GL_FLOAT && attr.type() != GL_DOUBLE;
#endif
            if (normalize)
                qWarning() << "ShaderEffectItem::renderEffect - vertex coord normalization not supported!";

//            glVertexAttribPointer(*attributes, attr.tupleSize(), attr.type(), normalize, attr.stride(),
//                                 GeometryDataUploader::vertexData(g, offset));
            m_program.setAttributeArray(*attributes, GL_FLOAT, GeometryDataUploader::vertexData(g, offset), attr.tupleSize(), attr.stride());
            offset += attr.tupleSize() * attr.sizeOfType();
        } else {
            qWarning("Attribute required by effect is missing.");
        }
    }

    QPair<int, int> indexRange;

    bool drawElements = m_geometry->indexCount();

    if (drawElements)
        indexRange = QPair<int, int>(0, m_geometry->indexCount());
    else
        indexRange = QPair<int, int>(0, m_geometry->vertexCount());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glDepthMask(true);
#if defined(QT_OPENGL_ES)
    glClearDepthf(0);
#else
    glClearDepth(0);
#endif
    glClearColor(0, 0, 0, 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    if (m_blending){
        glEnable(GL_BLEND);
        glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    if (drawElements)
        glDrawElements(GLenum(g->drawingMode()), indexRange.second - indexRange.first, g->indexType(), GeometryDataUploader::indexData(g));
    else
        glDrawArrays(GLenum(g->drawingMode()), indexRange.first, indexRange.second - indexRange.first);

    glDepthMask(false);
    glDisable(GL_DEPTH_TEST);

    GeometryDataUploader::release();

    for (int i = 0; i < m_attributeNames.size(); ++i)
        m_program.disableAttributeArray(m_attributes.at(i));

}

void ShaderEffectItem::updateEffectState(const QMatrix4x4& matrix)
{
    for (int i = m_sources.size() - 1; i >= 0; --i) {
        const ShaderEffectItem::SourceData &source = m_sources.at(i);
        if (!source.source)
            continue;

        glActiveTexture(GL_TEXTURE0 + i);
        source.source->bind();
    }

    if (m_respects_opacity)
        m_program.setUniformValue("qt_Opacity", (float) effectiveOpacity());

    if (m_respects_matrix){
        m_program.setUniformValue("qt_ModelViewProjectionMatrix", matrix);
    }

    QSet<QByteArray>::const_iterator it;
    for (it = m_uniformNames.begin(); it != m_uniformNames.end(); ++it) {
        const QByteArray &name = *it;
        QVariant v = property(name.constData());

        switch (v.type()) {
        case QVariant::Color:
            m_program.setUniformValue(name.constData(), qvariant_cast<QColor>(v));
            break;
        case QVariant::Double:
            m_program.setUniformValue(name.constData(), (float) qvariant_cast<double>(v));
            break;
        case QVariant::Transform:
            m_program.setUniformValue(name.constData(), qvariant_cast<QTransform>(v));
            break;
        case QVariant::Int:
            m_program.setUniformValue(name.constData(), v.toInt());
            break;
        case QVariant::Size:
        case QVariant::SizeF:
            m_program.setUniformValue(name.constData(), v.toSizeF());
            break;
        case QVariant::Point:
        case QVariant::PointF:
            m_program.setUniformValue(name.constData(), v.toPointF());
            break;
        case QVariant::Rect:
        case QVariant::RectF:
        {
            QRectF r = v.toRectF();
            m_program.setUniformValue(name.constData(), r.x(), r.y(), r.width(), r.height());
        }
            break;
        case QVariant::Vector3D:
            m_program.setUniformValue(name.constData(), qvariant_cast<QVector3D>(v));
            break;
        default:
            break;
        }
    }
}

const QGL::VertexAttribute *ShaderEffectItem::requiredFields() const
{
//    if (m_program_dirty)
//        updateShaderProgram();
    return m_attributes.constData();
}

void ShaderEffectItem::updateGeometry()
{
    if (!m_geometry)
        return;

    int vmesh = m_mesh_resolution.height();
    int hmesh = m_mesh_resolution.width();

    Geometry *g = m_geometry;
    g->setVertexCount((vmesh + 1) * (hmesh + 1));
    g->setIndexCount(vmesh * 2 * (hmesh + 2));

    struct V { float x, y, tx, ty; };

    V *vdata = (V *) g->vertexData();

    QRectF dstRect = boundingRect();
    QRectF srcRect(0, 1, 1, -1);
    for (int iy = 0; iy <= vmesh; ++iy) {
        float fy = iy / float(vmesh);
        float y = float(dstRect.top()) + fy * float(dstRect.height());
        float ty = float(srcRect.top()) + fy * float(srcRect.height());
        for (int ix = 0; ix <= hmesh; ++ix) {
            float fx = ix / float(hmesh);
            vdata->x = float(dstRect.left()) + fx * float(dstRect.width());
            vdata->y = y;
            vdata->tx = float(srcRect.left()) + fx * float(srcRect.width());
            vdata->ty = ty;
            ++vdata;
        }
    }

    quint16 *indices = (quint16 *)g->ushortIndexData();
    int i = 0;
    for (int iy = 0; iy < vmesh; ++iy) {
        *(indices++) = i;
        for (int ix = 0; ix <= hmesh; ++ix) {
            *(indices++) = i++;
            *(indices++) = i + hmesh;
        }
        *(indices++) = i + hmesh;
    }
    Q_ASSERT(indices == g->ushortIndexData() + g->indexCount());

    //markDirty(Node::DirtyGeometry);
}


void ShaderEffectItem::setFragmentShader(const QString &code)
{
    if (m_fragment_code.constData() == code.constData())
        return;
    m_fragment_code = code;
    if (isComponentComplete()) {
        reset();
        updateProperties();
    }
    emit fragmentShaderChanged();
}

void ShaderEffectItem::setVertexShader(const QString &code)
{
    if (m_vertex_code.constData() == code.constData())
        return;
    m_vertex_code = code;
    if (isComponentComplete()) {
        reset();
        updateProperties();
    }
    emit vertexShaderChanged();
}

void ShaderEffectItem::setBlending(bool enable)
{
    if (m_blending == enable)
        return;

    m_blending = enable;
    m_changed = true;
    emit blendingChanged();
}

void ShaderEffectItem::setActive(bool enable)
{
    if (m_active == enable)
        return;

    if (m_active) {
        for (int i = 0; i < m_sources.size(); ++i) {
            ShaderEffectSource *source = m_sources.at(i).source;
            if (!source)
                continue;
            disconnect(source, SIGNAL(repaintRequired()), this, SLOT(markDirty()));
            source->derefFromEffectItem();
        }
        //setPaintNode(0);
    }

    m_active = enable;

    if (m_active) {
        //setPaintNode(&m_node);
        for (int i = 0; i < m_sources.size(); ++i) {
            ShaderEffectSource *source = m_sources.at(i).source;
            if (!source)
                continue;
            source->refFromEffectItem();
            connect(source, SIGNAL(repaintRequired()), this, SLOT(markDirty()));
        }
    }

    emit activeChanged();
    markDirty();
}

void ShaderEffectItem::setMeshResolution(const QSize &size)
{
    if (size == m_mesh_resolution)
        return;

    m_mesh_resolution = size;
    updateGeometry();
}

void ShaderEffectItem::preprocess()
{
    for (int i = 0; i < m_sources.size(); ++i) {
        ShaderEffectSource *source = m_sources.at(i).source;
        if (source)
            source->update();
    }
}

void ShaderEffectItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    // QUARK TODO: connect this to some suitable signal ?
    if (newGeometry.size() != oldGeometry.size())
        updateGeometry();
    QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
}

void ShaderEffectItem::changeSource(int index)
{
    Q_ASSERT(index >= 0 && index < m_sources.size());
    QVariant v = property(m_sources.at(index).name.constData());
    setSource(v, index);
}

void ShaderEffectItem::markDirty() {
    update();
}

void ShaderEffectItem::setSource(QVariant var, int index)
{
    Q_ASSERT(index >= 0 && index < m_sources.size());

    SourceData &source = m_sources[index];

    if (m_active && source.source) {
        disconnect(source.source, SIGNAL(repaintRequired()), this, SLOT(markDirty()));
        source.source->derefFromEffectItem();
    }

    enum SourceType { Url, Item, Source, Other };
    SourceType sourceType = Other;
    QObject *obj = 0;

    if (!var.isValid()) {
        sourceType = Source; // Causes source to be set to null.
    } else if (var.type() == QVariant::Url || var.type() == QVariant::String) {
        sourceType = Url;
    } else if (var.type() == QMetaType::QObjectStar) {
        obj = qVariantValue<QObject *>(var);
        if (qobject_cast<QDeclarativeItem *>(obj))
            sourceType = Item;
        else if (!obj || qobject_cast<ShaderEffectSource *>(obj)) // Interpret null as ShaderEffectItemSource.
            sourceType = Source;
    }

    switch (sourceType) {
    case Url:
        {
            QUrl url = var.type() == QVariant::Url ? var.toUrl() : QUrl(var.toString());
            if (source.ownedByEffect && !url.isEmpty() && source.source->sourceImage() == url)
                break;
            if (source.ownedByEffect)
                delete source.source;
            source.source = new ShaderEffectSource;
            //source.source->setSceneGraphContext(QSGContext::current);
            source.ownedByEffect = true;
            source.source->setSourceImage(url);
        }
        break;
    case Item:
        {
        if (source.ownedByEffect && source.source->sourceItem() == obj)
            break;
        if (source.ownedByEffect)
            delete source.source;
        source.source = new ShaderEffectSource;
        //source.source->setSceneGraphContext(QSGContext::current);
        source.ownedByEffect = true;
        source.source->setSourceItem(static_cast<QDeclarativeItem *>(obj));
        }
        break;
    case Source:
        if (obj == source.source)
            break;
        if (source.ownedByEffect)
            delete source.source;
        source.source = static_cast<ShaderEffectSource *>(obj);
        source.ownedByEffect = false;
        break;
    default:
        qWarning("Could not assign source of type '%s' to property '%s'.", var.typeName(), source.name.constData());
        break;
    }

    if (m_active && source.source) {
        source.source->refFromEffectItem();
        connect(source.source, SIGNAL(repaintRequired()), this, SLOT(markDirty()));
    }
}

void ShaderEffectItem::disconnectPropertySignals()
{
    disconnect(this, 0, this, SLOT(markDirty()));
    for (int i = 0; i < m_sources.size(); ++i) {
        SourceData &source = m_sources[i];
        disconnect(this, 0, source.mapper, 0);
        disconnect(source.mapper, 0, this, 0);
    }
}

void ShaderEffectItem::connectPropertySignals()
{
    QSet<QByteArray>::const_iterator it;
    for (it = m_uniformNames.begin(); it != m_uniformNames.end(); ++it) {
        int pi = metaObject()->indexOfProperty(it->constData());
        if (pi >= 0) {
            QMetaProperty mp = metaObject()->property(pi);
            if (!mp.hasNotifySignal())
                qWarning("ShaderEffectItem: property '%s' does not have notification method!", it->constData());
            QByteArray signalName("2");
            signalName.append(mp.notifySignal().signature());
            connect(this, signalName, this, SLOT(markDirty()));
        } else {
            qWarning("ShaderEffectItem: '%s' does not have a matching property!", it->constData());
        }
    }
    for (int i = 0; i < m_sources.size(); ++i) {
        SourceData &source = m_sources[i];
        int pi = metaObject()->indexOfProperty(source.name.constData());
        if (pi >= 0) {
            QMetaProperty mp = metaObject()->property(pi);
            QByteArray signalName("2");
            signalName.append(mp.notifySignal().signature());
            connect(this, signalName, source.mapper, SLOT(map()));
            source.mapper->setMapping(this, i);
            connect(source.mapper, SIGNAL(mapped(int)), this, SLOT(changeSource(int)));
        } else {
            qWarning("ShaderEffectItem: '%s' does not have a matching source!", source.name.constData());
        }
    }
}

void ShaderEffectItem::reset()
{
    disconnectPropertySignals();

    m_program.removeAllShaders();
    m_attributeNames.clear();
    m_attributes.clear();
    m_uniformNames.clear();
    for (int i = 0; i < m_sources.size(); ++i) {
        const SourceData &source = m_sources.at(i);
        if (m_active && source.source)
            source.source->derefFromEffectItem();
        delete source.mapper;
        if (source.ownedByEffect)
            delete source.source;
    }

    m_sources.clear();

    //m_node.markDirty(Node::DirtyMaterial);
    m_program_dirty = true;

}

void ShaderEffectItem::updateProperties()
{
    QString vertexCode = m_vertex_code;
    QString fragmentCode = m_fragment_code;
    if (vertexCode.isEmpty())
        vertexCode = QString::fromLatin1(qt_default_vertex_code);
    if (fragmentCode.isEmpty())
        fragmentCode = QString::fromLatin1(qt_default_fragment_code);

    lookThroughShaderCode(vertexCode);
    lookThroughShaderCode(fragmentCode);

    for (int i = 0; i < m_sources.size(); ++i) {
        QVariant v = property(m_sources.at(i).name);
        setSource(v, i); // Property exists.
    }

    // Append an 'end of array' marker so that m_attributes.constData() can be returned in requiredFields().
    m_attributes.append(QGL::VertexAttribute(-1));
    connectPropertySignals();
}


void ShaderEffectItem::updateShaderProgram()
{
    QString vertexCode = m_vertex_code;
    QString fragmentCode = m_fragment_code;
    if (vertexCode.isEmpty())
        vertexCode = QString::fromLatin1(qt_default_vertex_code);
    if (fragmentCode.isEmpty())
        fragmentCode = QString::fromLatin1(qt_default_fragment_code);

    m_program.addShaderFromSourceCode(QGLShader::Vertex, vertexCode);
    m_program.addShaderFromSourceCode(QGLShader::Fragment, fragmentCode);

    for (int i = 0; i < m_attributeNames.size(); ++i)
        m_program.bindAttributeLocation(m_attributeNames.at(i), m_attributes.at(i));

    if (!m_program.link()) {
        qWarning("ShaderEffectItem: Shader compilation failed:");
        qWarning() << m_program.log();
    }

    if (!m_attributes.contains(QGL::Position))
        qWarning("ShaderEffectItem: Missing reference to \'qt_Vertex\'.");
    if (!m_attributes.contains(QGL::TextureCoord0))
        qWarning("ShaderEffectItem: Missing reference to \'qt_MultiTexCoord0\'.");
    if (!m_respects_matrix)
        qWarning("ShaderEffectItem: Missing reference to \'qt_ModelViewProjectionMatrix\'.");

    if (m_program.isLinked()) {
        m_program.bind();
        for (int i = 0; i < m_sources.size(); ++i)
            m_program.setUniformValue(m_sources.at(i).name.constData(), i);
    }

    m_program_dirty = false;
}

void ShaderEffectItem::lookThroughShaderCode(const QString &code)
{
    // Regexp for matching attributes and uniforms.
    // In human readable form: attribute|uniform [lowp|mediump|highp] <type> <name>
    static QRegExp re(QLatin1String("\\b(attribute|uniform)\\b\\s*\\b(?:lowp|mediump|highp)?\\b\\s*\\b(\\w+)\\b\\s*\\b(\\w+)"));
    Q_ASSERT(re.isValid());

    int pos = -1;
    while ((pos = re.indexIn(code, pos + 1)) != -1) {
        QString decl = re.cap(1); // uniform or attribute
        QString type = re.cap(2); // type
        QString name = re.cap(3); // variable name

        if (decl == QLatin1String("attribute")) {
            if (name == QLatin1String("qt_Vertex")) {
                m_attributeNames.append(name.toLatin1());
                m_attributes.append(QGL::Position);
            } else if (name == QLatin1String("qt_MultiTexCoord0")) {
                m_attributeNames.append(name.toLatin1());
                m_attributes.append(QGL::TextureCoord0);
            } else {
                // TODO: Support user defined attributes.
                qWarning("ShaderEffectItem: Attribute \'%s\' not recognized.", qPrintable(name));
            }
        } else {
            Q_ASSERT(decl == QLatin1String("uniform"));

            if (name == QLatin1String("qt_ModelViewProjectionMatrix")){
                m_respects_matrix = true;
            } else if (name == QLatin1String("qt_Opacity")) {
                m_respects_opacity = true;
            } else {
                m_uniformNames.insert(name.toLatin1());
                if (type == QLatin1String("sampler2D")) {
                    SourceData d;
                    d.mapper = new QSignalMapper;
                    d.source = 0;
                    d.name = name.toLatin1();
                    d.ownedByEffect = false;
                    m_sources.append(d);
                }
            }
        }
    }
}
