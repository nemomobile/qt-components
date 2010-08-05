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
#include "qlineeditmodel.h"
#include "qrangemodel.h"

#ifdef Q_COMPONENTS_MX
#include "qmxwindow.h"
#include "qmxtoplevelitem.h"
#include "qmxbuttongroup.h"
#endif
#ifdef Q_COMPONENTS_MEEGO
#include "mdeclarativestatusbar.h"
#include "mdeclarativescreen.h"
#include "msnapshot.h"
#include <mcomponentdata.h>
#endif

class QtComponentsPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);

        if (!MComponentData::instance()) {
            // This is a workaround because we can't use a default
            // constructor for MComponentData
            int argc = 1;
            char *argv0 = "meegotouch";
            (void) new MComponentData(argc, &argv0);
        }
        engine->rootContext()->setContextProperty("screen", new MDeclarativeScreen);
        qmlRegisterUncreatableType<MDeclarativeScreen>("Qt",4,7,"Screen","");
        qWarning() << "QtComponents plugin initialized";
    }

    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("Qt.labs.components"));
        qmlRegisterType<QButtonModel>(uri, 1, 0, "ButtonModel");
        qmlRegisterType<QLineEditModel>(uri, 1, 0, "LineEditModel");
        qmlRegisterType<QLineEditLayoutHelper>(uri, 1, 0, "LineEditLayoutHelper");
        qmlRegisterType<QLineEditEventHelper>(uri, 1, 0, "LineEditEventHelper");
        qmlRegisterType<QRangeModel>(uri, 1, 0, "RangeModel");

        // ### Common high level style items
#ifdef Q_COMPONENTS_MX
        qmlRegisterType<QMxWindow>(uri, 1, 0, "WindowModel");
        qmlRegisterType<QMxTopLevelItem>(uri, 1, 0, "TopLevelItemHelper");

        // ### Temporary solution until we can access children items inside QML
        qmlRegisterType<QMxButtonGroup>(uri, 1, 0, "ButtonGroup");
#endif

#ifdef Q_COMPONENTS_MEEGO
        qmlRegisterType<MDeclarativeStatusBar>(uri, 1, 0, "StatusBar");
        qmlRegisterType<MSnapshot>(uri, 1, 0, "Snapshot");
#endif
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(qtcomponentsplugin, QtComponentsPlugin);
