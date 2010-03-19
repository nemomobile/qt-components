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

#ifndef QMLSTYLE_H
#define QMLSTYLE_H

#include "style.h"

#include <QtCore/QHash>
#include <QtCore/QDir>

class QDeclarativeEngine;
class QDeclarativeComponent;

class QmlStyle : public Style
{
    Q_OBJECT

public:
    QmlStyle(QDeclarativeEngine *engine, const QString &path, QObject *parent = 0);

    virtual void populate(QGraphicsObject *component, QObject *model);

protected:
    QDeclarativeComponent *lookupQmlComponent(const char *componentType);
    void bindQmlChildGeometry(QGraphicsObject *component, QDeclarativeItem *child);

private:
    QDeclarativeEngine *m_engine;
    QDir m_path;
    QHash<const char *, QDeclarativeComponent *> m_componentsCache;
};


#endif
