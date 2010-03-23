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
#include <QtGui/QGraphicsAnchorLayout>
#include <QtGui/QGraphicsLayoutItem>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsWidget>

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

        if (child->width() == 0) {
            qWarning() << "Warning: No default width in" << qmlComponent->url().path();
            child->setWidth(50);
        }
        if (child->height() == 0) {
            qWarning() << "Warning: No default height in" << qmlComponent->url().path();
            child->setHeight(50);
        }

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

class LayoutItemWrapper : public QObject, public QGraphicsLayoutItem
{
public:
    LayoutItemWrapper(QDeclarativeItem *item, QGraphicsLayoutItem *parent)
        : QObject(item), QGraphicsLayoutItem(), m_item(item) {
    }

    virtual void setGeometry(const QRectF &rect) {
        m_item->setPos(rect.x(), rect.y());
        m_item->setSize(rect.size());
    }

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const {
        // ###
        if (which == Qt::MinimumSize)
            return QSizeF(0, 0);
        else if (which == Qt::MaximumSize)
            return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

        // ### Preferred case, try to use QML information
        return QSizeF(m_item->implicitWidth(), m_item->implicitHeight());
    }

private:
    QDeclarativeItem *m_item;
};

void QmlStyle::bindQmlChildGeometry(QGraphicsObject *component, QDeclarativeItem *child)
{
    // The parent is a QGraphicsWidget
    if (component->isWidget()) {
        QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(component);

        // ### Implement something lightweighter than QGAL :)
        QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
        l->setSpacing(0);
        widget->setLayout(l);
        LayoutItemWrapper *wrapper = new LayoutItemWrapper(child, l);
        l->addAnchors(wrapper, l);
        return;
    }

    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component);
    if (item) {
        item->setWidth(child->width());
        item->setHeight(child->height());
        child->anchors()->setFill(item);
    }

    // Only parents that are QGW and QDI are supported right now.
}


