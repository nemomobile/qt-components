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

#ifndef STYLE_H
#define STYLE_H

#include <QtCore/QObject>

class QGraphicsObject;
class QDeclarativeItem;
class QGraphicsWidget;

class ComponentPopulator
{
public:
    virtual void populate(QGraphicsObject *component, QObject *model) = 0;
    static const char *styleName;
};

void _q_registerComponentPopulator(const char *componentType,
                                   const char *styleName,
                                   ComponentPopulator *pop);

template<typename W, typename P>
struct ComponentPopulatorInitializer { };

#define STYLE_REGISTER_COMPONENT_POPULATOR(W, P)                        \
    template<>                                                          \
    struct ComponentPopulatorInitializer<W, P>                          \
    {                                                                   \
        ComponentPopulatorInitializer() {                               \
            printf("Registering '%s' for component '%s'.\n", #P, #W);   \
            const char *compType = W::staticMetaObject.className();     \
            const char *styleName = P::styleName;                       \
            _q_registerComponentPopulator(compType, styleName, new P);  \
        }                                                               \
    };                                                                  \
    static const ComponentPopulatorInitializer<W, P> componentPopulatorInitializer ## W ## __ ## P;


class ComponentStyle : public QObject
{
    Q_OBJECT

public:
    static ComponentStyle *instance() { return self; }
    virtual void populate(QGraphicsObject *component, QObject *model);

protected:
    ComponentStyle(const QString &name, QObject *parent = 0);

    ComponentPopulator *lookupComponentPopulator(const char *componentType, const QString &styleName);
    void bindChildrenGeometry(QGraphicsObject *component);

private:
    static ComponentStyle *self;
    QString m_name;
};

#endif // STYLE_H
