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

#include <QtDeclarative>
#include <QApplication>

#include "mdeclarativestatusbar.h"
#include "mdeclarativescreen.h"
#include "mdeclarativeinputcontext.h"
#include "msnapshot.h"
#include "mpagestatus.h"
#include "mdialogstatus.h"
#include "mpageorientation.h"
#include "minversemousearea.h"
#include "mdeclarativeimageprovider.h"
#include "mdeclarativeimplicitsizeitem.h"
#include "mdeclarativemaskeditem.h"
#include "mthemeplugin.h"
#include "mtexttranslator.h"
#include "mwindowstate.h"
#include "mdeclarativemousefilter.h"
#include "mscrolldecoratorsizer.h"
#include "mdeclarativeimattributeextension.h"
#include "mdeclarativeimobserver.h"
#include "mtoolbarvisibility.h"
#include <QDeclarativePropertyMap>
#include <QFont>

#ifdef HAVE_SHADER
#include "shadereffectitem/shadereffectitem.h"
#include "shadereffectitem/shadereffectsource.h"
#else
#include "shadereffectitem/shadereffectitemnull.h"
#include "shadereffectitem/shadereffectsourcenull.h"
#endif

class MeeGoPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        Q_ASSERT(uri == QLatin1String("com.meego") || uri == QLatin1String("com.nokia.meego") || uri == QLatin1String("Qt.labs.components.native"));

        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);

        engine->addImageProvider(QLatin1String("theme"), new MDeclarativeImageProvider);

        engine->rootContext()->setContextProperty("screen", MDeclarativeScreen::instance());
        qmlRegisterUncreatableType<MDeclarativeScreen>(uri, 1, 0, "Screen", "");

        engine->rootContext()->setContextProperty("platformWindow", MWindowState::instance());
        qmlRegisterUncreatableType<MWindowState>(uri, 1, 0, "WindowState", "");

        engine->rootContext()->setContextProperty("theme", new MThemePlugin);
        qmlRegisterUncreatableType<MThemePlugin>(uri, 1, 0, "Theme", "");

        engine->rootContext()->setContextProperty("inputContext", new MDeclarativeInputContext);
        qmlRegisterUncreatableType<MDeclarativeInputContext>(uri, 1, 0, "InputContext", "");
        
        engine->rootContext()->setContextProperty("textTranslator", new MTextTranslator);
        qmlRegisterUncreatableType<MTextTranslator>(uri, 1, 0, "TextTranslator", "");

        // Disable cursor blinking + make double tapping work the way it is done in lmt.
        QApplication *app = qobject_cast<QApplication*>(QApplication::instance());
        if (app) {
            app->setCursorFlashTime(0);
            app->setDoubleClickInterval(MEEGOTOUCH_DOUBLETAP_INTERVAL);
        } 

        engine->rootContext()->setContextProperty("UiConstants", uiConstants());
    }

    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("com.meego") || uri == QLatin1String("com.nokia.meego") || uri == QLatin1String("Qt.labs.components.native"));
        qmlRegisterType<MDeclarativeStatusBar>(uri, 1, 0, "StatusBarInternal");
        qmlRegisterType<MSnapshot>(uri, 1, 0, "Snapshot");

        qmlRegisterUncreatableType<MPageStatus>(uri, 1, 0, "PageStatus", "");
        qmlRegisterUncreatableType<MDialogStatus>(uri, 1, 0, "DialogStatus", "");
        qmlRegisterUncreatableType<MWindowState>(uri, 1, 0, "WindowState","");
        qmlRegisterUncreatableType<MPageOrientation>(uri, 1, 0, "PageOrientation", "");
        qmlRegisterUncreatableType<MToolBarVisibility>(uri, 1, 0, "ToolBarVisibility", "");
        qmlRegisterUncreatableType<MTextTranslator>(uri, 1, 0, "TextTranslator", "");

        // Custom primitives
        qmlRegisterType<MDeclarativeImplicitSizeItem>(uri, 1, 0, "ImplicitSizeItem");
        qmlRegisterType<MDeclarativeMaskedItem>(uri, 1, 0, "MaskedItem");
        qmlRegisterType<MInverseMouseArea>(uri, 1, 0, "InverseMouseArea");
        qmlRegisterType<MDeclarativeMouseFilter>(uri, 1, 0, "MouseFilter");
        qmlRegisterType<MDeclarativeMouseEvent>(uri, 1, 0, "MMouseEvent");
        qmlRegisterType<MDeclarativeIMAttributeExtension>(uri, 1, 0, "SipAttributes");
        qmlRegisterType<MDeclarativeIMObserver>(uri, 1, 0, "InputMethodObserver");

        qmlRegisterType<MScrollDecoratorSizer>(uri, 1, 0, "ScrollDecoratorSizerCPP");

        // shader effect item (to be removed when supported in QML)
#ifdef HAVE_SHADER
        qmlRegisterType<ShaderEffectItem>(uri, 1, 0, "ShaderEffectItem");
        qmlRegisterType<ShaderEffectSource>(uri, 1, 0, "ShaderEffectSource");
#else
        qmlRegisterType<ShaderEffectItemNull>(uri, 1, 0, "ShaderEffectItem");
        qmlRegisterType<ShaderEffectSourceNull>(uri, 1, 0, "ShaderEffectSource");
#endif

        qmlRegisterType<MInverseMouseArea>(uri, 1, 0, "InverseMouseArea");

    }

    QDeclarativePropertyMap *uiConstants() {
        QDeclarativePropertyMap *uiConstantsData = new QDeclarativePropertyMap();
        uiConstantsData->insert("DefaultMargin", QVariant(16));
        uiConstantsData->insert("ButtonSpacing", QVariant(6));
        uiConstantsData->insert("HeaderDefaultHeightPortrait", QVariant(72));
        uiConstantsData->insert("HeaderDefaultHeightLandscape", QVariant(46));
        uiConstantsData->insert("HeaderDefaultTopSpacingPortrait", QVariant(20));
        uiConstantsData->insert("HeaderDefaultBottomSpacingPortrait", QVariant(20));
        uiConstantsData->insert("HeaderDefaultTopSpacingLandscape", QVariant(16));
        uiConstantsData->insert("HeaderDefaultBottomSpacingLandscape", QVariant(14));
        uiConstantsData->insert("ListItemHeightSmall", QVariant(64));
        uiConstantsData->insert("ListItemHeightDefault", QVariant(80));

        QFont bodyTextFont;
        bodyTextFont.setFamily("Nokia Pure Text Light");
        bodyTextFont.setPixelSize(24);
        uiConstantsData->insert("BodyTextFont", QVariant(bodyTextFont));

        QFont headerFont;
        headerFont.setFamily("Nokia Pure Text Light");
        headerFont.setPixelSize(32);
        uiConstantsData->insert("HeaderFont", QVariant(headerFont));

        QFont groupHeaderFont;
        groupHeaderFont.setFamily("Nokia Pure Text");
        groupHeaderFont.setPixelSize(18);
        groupHeaderFont.setBold(true);
        uiConstantsData->insert("GroupHeaderFont", QVariant(groupHeaderFont));

        QFont titleFont;
        titleFont.setFamily("Nokia Pure Text");
        titleFont.setPixelSize(26);
        titleFont.setBold(true);
        uiConstantsData->insert("TitleFont", QVariant(titleFont));

        QFont smallTitleFont;
        smallTitleFont.setFamily("Nokia Pure Text");
        smallTitleFont.setPixelSize(24);
        smallTitleFont.setBold(true);
        uiConstantsData->insert("SmallTitleFont", QVariant(titleFont));

        QFont fieldLabelFont;
        fieldLabelFont.setFamily("Nokia Pure Text Light");
        fieldLabelFont.setPixelSize(22);
        uiConstantsData->insert("FieldLabelFont", QVariant(fieldLabelFont));
        uiConstantsData->insert("FieldLabelColor", QVariant(QColor("#505050")));

        QFont subTitleFont;
        subTitleFont.setFamily("Nokia Pure Text Light");
        subTitleFont.setPixelSize(22);
        uiConstantsData->insert("SubtitleFont", QVariant(subTitleFont));

        return uiConstantsData;
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(meegoplugin, MeeGoPlugin);
