/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include <QtDeclarative>

#include "qbuttonmodel.h"
#include "qdatetimemodel.h"
#include "qlineeditmodel.h"
#include "qrangemodel.h"

#include "qdeclarativewindow.h"
#include "qdeclarativetoplevelitem.h"
#include "qdeclarativebuttongroup.h"

class QtComponentsPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        /*
        QString style(qgetenv("COMPSTYLE"));
        if (style == "") {
            new TestStyle(engine);
        } else {
            QString path = QString("data/qmlstyle/%1").arg(style);
            qWarning() << "Loading theme from: " << path;
            new QmlStyle(path, engine, engine);
        }
        */
        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);
    }

    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("QtComponents"));
        qmlRegisterType<QButtonModel>(uri, 1, 0, "ButtonModel");
        qmlRegisterType<QDateTimeModel>(uri, 1, 0, "DateTimeModel");
        qmlRegisterType<QLineEditModel>(uri, 1, 0, "LineEditModel");
        qmlRegisterType<QLineEditLayoutHelper>(uri, 1, 0, "LineEditLayoutHelper");
        qmlRegisterType<QLineEditEventHelper>(uri, 1, 0, "LineEditEventHelper");
        qmlRegisterType<QRangeModel>(uri, 1, 0, "RangeModel");

        // ### Common high level style items
        qmlRegisterType<QtDeclarativeWindow>(uri, 1, 0, "WindowModel");
        qmlRegisterType<QtDeclarativeTopLevelItem>(uri, 1, 0, "TopLevelItemHelper");

        // ### Temporary solution until we can access children items inside QML
        qmlRegisterType<QtDeclarativeButtonGroup>(uri, 1, 0, "ButtonGroup");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(qtcomponentsplugin, QtComponentsPlugin);
