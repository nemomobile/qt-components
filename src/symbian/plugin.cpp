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

#include "sdeclarative.h"
#include "sstylefactory.h"
#include "sdeclarativeicon.h"
#include "sdeclarativefocusscopeitem.h"
#include "sdeclarativeimplicitsizeitem.h"
#include "sdeclarativeinputcontext.h"
#include "sdeclarativeimageprovider.h"
#include "sdeclarativemaskedimage.h"
#include "sdeclarativescreen.h"
#include "sdeclarativescreen_p.h"
#include "sdeclarativeindicatorcontainer.h"
#include "sdeclarativenetworkindicator.h"
#include "sbatteryinfo.h"
#include "snetworkinfo.h"
#include "spopupmanager.h"
#include "smousegrabdisabler.h"
#include "sdeclarativemagnifier.h"
#include "sdeclarativesharedstatusbar.h"
#include "ssnapshot.h"

#include <QCoreApplication>
#include <QtDeclarative>
#include <QDeclarativeView>


static const int VERSION_MAJOR = 1;
static const int VERSION_MINOR = 0;

static void tryToDisableSystemRotation(const QDeclarativeEngine *engine)
{
    QDeclarativeView *declarativeView = 0;
    const QWidgetList &widgets = QApplication::allWidgets();
    for (int i = 0; i < widgets.count() && !declarativeView; i++) {
         QDeclarativeView *tempView = qobject_cast<QDeclarativeView *>(widgets.at(i));
         if (tempView && tempView->engine() == engine)
             declarativeView = tempView;
    }

    if (!declarativeView)
        return;

    engine->rootContext()->setProperty("declarativeViewPtr", qVariantFromValue(qobject_cast<QObject *>(declarativeView)));

    // If resize mode set from componenttest
    if (declarativeView->property("orientationMethod").toBool())
        return;
// Do not use sensor orientation method with simulator
#ifndef Q_WS_SIMULATOR
    // Qt::WA_SymbianNoSystemRotation cannot be set if view is already open
    if(!declarativeView->isVisible())
        declarativeView->setAttribute(Qt::WA_SymbianNoSystemRotation);
#endif
}


class SymbianPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:

    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {

        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);
        context = engine->rootContext();
        tryToDisableSystemRotation(engine);

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

        screen = new SDeclarativeScreen(engine, context); // context as parent
        context->setContextProperty("screen", screen);

        inputContext = new SDeclarativeInputContext(screen, context); // context as parent
        context->setContextProperty("inputContext", inputContext);

        style = new SStyleFactory(screen, context);
        context->setContextProperty("platformStyle", style->platformStyle());
        context->setContextProperty("privateStyle", style->privateStyle());

        SDeclarative *declarative = new SDeclarative(context);
        context->setContextProperty("symbian", declarative);

        SDeclarativeImageProvider *imageProvider = new SDeclarativeImageProvider;
        engine->addImageProvider(QLatin1String("theme"), imageProvider);

        // make status of graphics sharing available for 'symbian' -context property
        if (imageProvider->graphicsSharing())
            declarative->setGraphicsSharing(true);

        SPopupManager *popupManager = new SPopupManager(context);
        context->setContextProperty("platformPopupManager", popupManager);

        SBatteryInfo *batteryInfo = new SBatteryInfo(context);
        context->setContextProperty("privateBatteryInfo", batteryInfo);

        SNetworkInfo *networkInfo = new SNetworkInfo(context);
        context->setContextProperty("privateNetworkInfo", networkInfo);

        QObject::connect(engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
        QObject::connect(screen, SIGNAL(displayChanged()), this, SLOT(resetScreen()));
        QObject::connect(style->platformStyle(), SIGNAL(fontParametersChanged()), this, SLOT(resetPlatformStyle()));
        QObject::connect(style->platformStyle(), SIGNAL(layoutParametersChanged()), this, SLOT(resetPlatformStyle()));
        QObject::connect(style->platformStyle(), SIGNAL(colorParametersChanged()), this, SLOT(resetPlatformStyle()));
        QObject::connect(style->privateStyle(), SIGNAL(layoutParametersChanged()), this, SLOT(resetPrivateStyle()));
        QObject::connect(style->privateStyle(), SIGNAL(colorParametersChanged()), this, SLOT(resetPrivateStyle()));

#if defined(Q_OS_SYMBIAN) && QT_VERSION >= 0x040704
        Q_DECL_IMPORT void qt_s60_setPartialScreenInputMode(bool enable);
        qt_s60_setPartialScreenInputMode(true);

        Q_DECL_IMPORT void qt_s60_setPartialScreenAutomaticTranslation(bool enable);
        qt_s60_setPartialScreenAutomaticTranslation(false);
#endif
    }

    void registerTypes(const char *uri) {

        // enables the use of QtQuick 1.1 version (revision 1) of QDeclarativeItem
        qmlRegisterRevision<QDeclarativeItem, 1>(uri, 1, 1);

        qmlRegisterType<SDeclarativeIcon>(uri, 1, 1, "Icon");
        qmlRegisterType<SDeclarativeMaskedImage>(uri, 1, 1, "MaskedImage");
        qmlRegisterType<SDeclarativeImplicitSizeItem>(uri, 1, 1, "ImplicitSizeItem");
        qmlRegisterType<SDeclarativeFocusScopeItem>(uri, 1, 1, "FocusScopeItem");
        qmlRegisterType<SDeclarativeIndicatorContainer>(uri, 1, 1, "UniversalIndicators");
        qmlRegisterType<SDeclarativeNetworkIndicator>(uri, 1, 1, "NetworkIndicator");
        qmlRegisterType<SMouseGrabDisabler>(uri, 1, 1, "MouseGrabDisabler");
        qmlRegisterType<SDeclarativeMagnifier>(uri, 1, 1, "Magnifier");
        qmlRegisterType<SDeclarativeSharedStatusBar>(uri, 1, 1, "SharedStatusBar");
        qmlRegisterType<Snapshot>(uri, 1, 1, "Snapshot");
        qmlRegisterUncreatableType<SDeclarative>(uri, 1, 1, "Symbian", "");
        qmlRegisterUncreatableType<SDeclarativeScreen>(uri, 1, 1, "Screen", "");
        qmlRegisterUncreatableType<SDialogStatus>(uri, 1, 1, "DialogStatus", "");
        qmlRegisterUncreatableType<SPageOrientation>(uri, 1, 1, "PageOrientation", "");
        qmlRegisterUncreatableType<SPageStatus>(uri, 1, 1, "PageStatus", "");
        qmlRegisterUncreatableType<SBatteryInfo>(uri, 1, 1, "BatteryInfo", "");
        qmlRegisterUncreatableType<SNetworkInfo>(uri, 1, 1, "NetworkInfo", "");
        qmlRegisterUncreatableType<SDeclarativeInputContext>(uri, 1, 1, "InputContext", "");
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
    SDeclarativeInputContext *inputContext;
    SDeclarativeScreen *screen;
    SStyleFactory *style;
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(symbianplugin, SymbianPlugin)
