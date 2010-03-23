/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qmlstyle.h"

#include <QtDeclarative/private/qdeclarativeanchors_p.h>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeItem>
#include <QtGui/QGraphicsObject>


QmlStyle::QmlStyle(QDeclarativeEngine *engine, const QString &path, QObject *parent)
    : ComponentStyle("qml", parent), m_engine(engine), m_path(path)
{
    //
}

void QmlStyle::populate(QGraphicsObject *component, QObject *model)
{
    const char *type = component->metaObject()->className();

    QDeclarativeComponent *qmlComponent = lookupQmlComponent(type);
    if (qmlComponent) {

        // We expose the component and the model to the QML context. The QML implementation
        // will bind properties of its primitives and that model/component.
        // ### TO DO: Do we need to expose the component? Should we decorate or subclass the
        //            model to add any information that is missing?
        Q_ASSERT(component);
        QDeclarativeContext *context = new QDeclarativeContext(m_engine->rootContext(), component);
        context->setContextProperty("component", component);

        if (model) {
            // ### should we always set the model, or only when there's one? I rather set only when exists, so
            // we can spot bugs earlier in qml code that depends on unexisting model or a null model in
            // widget creation.
            context->setContextProperty("model", model);
        }

        // ###
        QObject *obj = qmlComponent->create(context);
        QDeclarativeItem *child = qobject_cast<QDeclarativeItem *>(obj);

        // ### That's what QDeclarativeItem::setParent does...
        obj->setParent(component);
        static_cast<QGraphicsItem *>(child)->setParentItem(component);

        bindQmlChildGeometry(component, child);

    } else {
        qWarning("QmlStyle::populate: couldn't find Qml style for '%s' in path '%s'",
                 type, qPrintable(m_path.path()));

        // Fallback to a default
        ComponentPopulator *pop = lookupComponentPopulator(type, QLatin1String("default"));
        if (pop) {
            pop->populate(component, model);
        } else {
            qWarning("QmlStyle::populate: couldn't find populator for "
                     "component '%s' and fallback style 'default'", type);
        }

        bindChildrenGeometry(component);
    }
}

QDeclarativeComponent *QmlStyle::lookupQmlComponent(const char *componentType)
{
    // First look in the cache...
    if (m_componentsCache.contains(componentType)) {
        return m_componentsCache[componentType];
    }

    QString componentFile = QString("%1.qml").arg(componentType);
    if (!m_path.exists(componentFile)) {
        qWarning() << componentFile;
        // ### we could cache the negative here as well :-P
        return 0;
    }

    QDeclarativeComponent *c =
        new QDeclarativeComponent(m_engine, m_path.absoluteFilePath(componentFile), this);

    if (c->isError()) {
        QList<QDeclarativeError> errors = c->errors();
        foreach (const QDeclarativeError &error, errors) {
            qWarning() << error;
        }
        return 0;
    }

    m_componentsCache.insert(componentType, c);
    return c;
}

void QmlStyle::bindQmlChildGeometry(QGraphicsObject *component, QDeclarativeItem *child)
{
    // The parent is a QGraphicsWidget
    if (component->isWidget()) {
        qWarning("Bind for a QGraphicsWidget parent not implemented yet...");
        // ### Not there yet

        // ### make a QGraphicsLayoutItem to bridge the geometry and
        // use a 1-child only layout.
    }

    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component);
    if (item) {
        child->anchors()->setFill(item);
    }

    // Only parents that are QGW and QDI are supported right now.
}


