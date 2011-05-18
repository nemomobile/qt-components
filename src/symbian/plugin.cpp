/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "sdeclarative.h"
#include "sstylefactory.h"
#include "sstylewrapper.h"
#include "sdeclarativeicon.h"
#include "sdeclarativefocusscopeitem.h"
#include "sdeclarativeimplicitsizeitem.h"
#include "sdeclarativeimageprovider.h"
#include "sdeclarativemaskedimage.h"
#include "sdeclarativescreen.h"
#include "sbatteryinfo.h"
#include "snetworkinfo.h"
#include "spopupmanager.h"

#include <QCoreApplication>
#include <QtDeclarative>

static const int VERSION_MAJOR = 1;
static const int VERSION_MINOR = 0;

class SymbianPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:

    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);
        context = engine->rootContext();

        // QVariant.toInt() defaults to zero if QVariant is invalid.
        int versionMajor = context->property("symbianComponentsVersionMajor").toInt();
        int versionMinor = context->property("symbianComponentsVersionMinor").toInt();
        if (versionMajor > VERSION_MAJOR ||
            (versionMajor == VERSION_MAJOR && versionMinor >= VERSION_MINOR)) {
            // Either newer or this version of plugin already initialized.
            // The same plugin might initialized twice: once from 
            // "import com.nokia.symbian", and another time from
            // "import Qt.labs.components.native".
            return;
        } else {
            // Intentionally override possible older version of the plugin.
            context->setProperty("symbianComponentsVersionMajor", VERSION_MAJOR);
            context->setProperty("symbianComponentsVersionMinor", VERSION_MINOR);
        }

        engine->addImageProvider(QLatin1String("theme"), new SDeclarativeImageProvider);

        screen = new SDeclarativeScreen(engine, context); // context as parent
        context->setContextProperty("screen", screen);

        style = new SStyleFactory(screen, context);
        context->setContextProperty("platformStyle", style->platformStyle());
        context->setContextProperty("privateStyle", style->privateStyle());

        SDeclarative *declarative = new SDeclarative(context);
        context->setContextProperty("symbian", declarative);

        SPopupManager *popupManager = new SPopupManager(context);
        context->setContextProperty("platformPopupManager", popupManager);

        SBatteryInfo *batteryInfo = new SBatteryInfo(context);
        context->setContextProperty("batteryInfo", batteryInfo);

        SNetworkInfo *networkInfo = new SNetworkInfo(context);
        context->setContextProperty("networkInfo", networkInfo);

        QObject::connect(engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
        QObject::connect(screen, SIGNAL(displayChanged()), this, SLOT(resetScreen()));
        QObject::connect(style->platformStyle(), SIGNAL(fontParametersChanged()), this, SLOT(resetPlatformStyle()));
        QObject::connect(style->platformStyle(), SIGNAL(layoutParametersChanged()), this, SLOT(resetPlatformStyle()));
        QObject::connect(style->platformStyle(), SIGNAL(colorParametersChanged()), this, SLOT(resetPlatformStyle()));
        QObject::connect(style->privateStyle(), SIGNAL(layoutParametersChanged()), this, SLOT(resetPrivateStyle()));
        QObject::connect(style->privateStyle(), SIGNAL(colorParametersChanged()), this, SLOT(resetPrivateStyle()));
    }

    void registerTypes(const char *uri) {
        qmlRegisterType<SDeclarativeIcon>(uri, 1, 0, "Icon");
        qmlRegisterType<SDeclarativeMaskedImage>(uri, 1, 0, "MaskedImage");
        qmlRegisterType<SStyleWrapper>(uri, 1, 0, "Style");
        qmlRegisterType<SDeclarativeImplicitSizeItem>(uri, 1, 0, "ImplicitSizeItem");
        qmlRegisterType<SDeclarativeFocusScopeItem>(uri, 1, 0, "FocusScopeItem");
        qmlRegisterUncreatableType<SDeclarative>(uri, 1, 0, "Symbian", "");
        qmlRegisterUncreatableType<SDeclarativeScreen>(uri, 1, 0, "Screen", "");
        qmlRegisterUncreatableType<SDialogStatus>(uri, 1, 0, "DialogStatus", "");
        qmlRegisterUncreatableType<SPageOrientation>(uri, 1, 0, "PageOrientation", "");
        qmlRegisterUncreatableType<SPageStatus>(uri, 1, 0, "PageStatus", "");
        qmlRegisterUncreatableType<SBatteryInfo>(uri, 1, 0, "BatteryInfo", "");
        qmlRegisterUncreatableType<SNetworkInfo>(uri, 1, 0, "NetworkInfo", "");
    }

public slots:

    void resetScreen() {
        context->setContextProperty("screen", screen);
    }

    void resetPlatformStyle() {
        context->setContextProperty("platformStyle", style->platformStyle());
    }

    void resetPrivateStyle() {
        context->setContextProperty("privateStyle", style->privateStyle());
    }

private:
    QDeclarativeContext *context;
    SDeclarativeScreen *screen;
    SStyleFactory *style;
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(symbianplugin, SymbianPlugin)
