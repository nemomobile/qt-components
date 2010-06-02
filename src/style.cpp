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

#include "style.h"

#include <QtCore/QObject>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/private/qdeclarativeitemchangelistener_p.h>
#include <QtDeclarative/private/qdeclarativeitem_p.h>
#include <QtGui/QGraphicsAnchorLayout>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsWidget>
#include <QtGui/private/qgraphicsitem_p.h>

const char *ComponentPopulator::styleName = "default";


// ### QString vs. const char *
typedef QPair<QString, QString> Key;
typedef QHash<Key, ComponentPopulator *> StylePopulatorData;
Q_GLOBAL_STATIC(StylePopulatorData, stylePopulatorData);

void _q_registerComponentPopulator(const char *componentType,
                                   const char *styleName,
                                   ComponentPopulator *pop)
{
    Key key(componentType, styleName);
    StylePopulatorData *table = stylePopulatorData();
    table->insert(key, pop);

    // ### Do we need mutex to protect the table?
    // ### warning when registering multiple times?
}


ComponentStyle *ComponentStyle::self = 0;

void ComponentStyle::populate(QGraphicsObject *component, QObject *model)
{
    const char *type = component->metaObject()->className();

    // Try a populator for the current style, if not, fallback to 'default'
    ComponentPopulator *pop = lookupComponentPopulator(type, m_name);
    if (!pop)
        pop = lookupComponentPopulator(type, QLatin1String("default"));

    if (pop) {
        pop->populate(component, model);

        // Make the new children of 'component' follow its geometry.
        bindChildrenGeometry(component);

    } else {
        qWarning("Style::populate: couldn't find populator for component '%s' and style '%s'",
                 type, qPrintable(m_name));
        return;
    }

};

ComponentStyle::ComponentStyle(const QString &name, QObject *parent)
    : QObject(parent), m_name(name)
{
    ComponentStyle::self = this;
}

ComponentPopulator *ComponentStyle::lookupComponentPopulator(const char *componentType, const QString &name)
{
    Key key(componentType, name);
    StylePopulatorData *table = stylePopulatorData();
    return (*table)[key];
}

// Helper class for propagating the geometry of a qmldeclarativeitem
// to a graphics widget. When the item changes geometry, the widget
// will change.
class GraphicsWidgetToDeclarativeItemBinder : public QObject, public QDeclarativeItemChangeListener
{
public:

    GraphicsWidgetToDeclarativeItemBinder(QDeclarativeItem *item, QGraphicsWidget *widget)
        : QObject(), QDeclarativeItemChangeListener(), m_item(item), m_widget(widget) {
        QDeclarativeItemPrivate *p =
            static_cast<QDeclarativeItemPrivate *>(QGraphicsItemPrivate::get(m_item));
        p->addItemChangeListener(this, QDeclarativeItemPrivate::Geometry);
    }

    virtual ~GraphicsWidgetToDeclarativeItemBinder() {
        // ### This we can't access, we might have to do this in a
        // less private-ish fashion (not depending on those
        // changelisteners...)
        // QDeclarativeItemPrivate *p =
        //     static_cast<QDeclarativeItemPrivate *>(QGraphicsItemPrivate::get(m_item));
        // p->removeItemChangeListener(this, QDeclarativeItemPrivate::Geometry);
    }

    virtual void itemGeometryChanged(QDeclarativeItem *, const QRectF &newGeometry, const QRectF &) {
        m_widget->resize(newGeometry.size());
    }

    QDeclarativeItem *m_item;
    QGraphicsWidget *m_widget;
};


void ComponentStyle::bindChildrenGeometry(QGraphicsObject *component)
{
    // This function will bind the geometry of the QGraphicsWidget
    // children. The case for QDeclarativeItem children is treated in QmlStyle.

    // The parent is a QGraphicsWidget
    if (component->isWidget()) {
        QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(component);

        // If a layout is there, we assume the populator added it, and
        // configured the children objects inside.
        if (widget->layout()) {
            qWarning("LAYOUT OK!");
            return;
        }

        // If not, we we'll automatically make all toplevel children
        // have the parent's geometry.

        // ###
        QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
        l->setSpacing(0);
        widget->setLayout(l);
        QList<QGraphicsItem *> children = widget->childItems();
        for (int i = 0; i < children.count(); i++) {
            if (!children[i]->isWidget())
                continue;

            QGraphicsWidget *child = static_cast<QGraphicsWidget *>(children[i]);
            l->addAnchors(child, l);
        }

        return;
    }

    // The parent is a QDeclarativeItem, we then use declarative anchors
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component);
    if (item) {
        // Note that QGraphicsWidget doesn't imply QObject parenthood, thats why we use childItems.
        QList<QGraphicsItem *> children = item->childItems();
        for (int i = 0; i < children.count(); i++) {
            if (!children[i]->isWidget())
                continue;

            QGraphicsWidget *child = static_cast<QGraphicsWidget *>(children[i]);
            GraphicsWidgetToDeclarativeItemBinder *binder =
                new GraphicsWidgetToDeclarativeItemBinder(item, child);
            binder->setParent(child);
        }
    }

    // Only parents that are QGW and QDI are supported right now.
}

