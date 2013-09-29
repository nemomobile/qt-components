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

#include "mdeclarativescreen.h"
#include "mdeclarativeinputcontext.h"
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
#include "mdeclarativeview.h"
#include "mtoolbarvisibility.h"
#include "mdatetimehelper.h"

#include <QQmlPropertyMap>
#include <QFont>

#include "kernel/common.h"

#include "i18n/mlocalewrapper.h"

class MeeGoPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.nokia.meego")

public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        Q_ASSERT(uri == QLatin1String("com.meego") || uri == QLatin1String("com.nokia.meego") || uri == QLatin1String("Qt.labs.components.native"));

        if(uri == QLatin1String("com.meego")) {
            qWarning() << "'import com.meego' is DEPRECATED and may cause 'Error: Cannot assign QObject* to PageStack_QMLTYPE_42*', missing icons, widgets and other problems;"
                          "use 'import com.nokia.meego' instead";
        }

        QDeclarativeExtensionPlugin::initializeEngine(engine, uri);

        // If plugin was initialized once, do not initialize it again
        if(!engine->imageProvider(QLatin1String("theme"))) {
            engine->rootContext()->setContextProperty("dateTime", new MDateTimeHelper(engine->rootContext()));
            engine->addImageProvider(QLatin1String("theme"), new MDeclarativeImageProvider);

            engine->rootContext()->setContextProperty("screen", MDeclarativeScreen::instance());
            qmlRegisterUncreatableType<MDeclarativeScreen>(uri, SINCE_VERSION(1, 0), "Screen", "");

            engine->rootContext()->setContextProperty("platformWindow", MWindowState::instance());
            qmlRegisterUncreatableType<MWindowState>(uri, SINCE_VERSION(1, 0), "WindowState", "");

            engine->rootContext()->setContextProperty("theme", new MThemePlugin);
            qmlRegisterUncreatableType<MThemePlugin>(uri, SINCE_VERSION(1, 0), "Theme", "");

            engine->rootContext()->setContextProperty("inputContext", new MDeclarativeInputContext);
            qmlRegisterUncreatableType<MDeclarativeInputContext>(uri, SINCE_VERSION(1, 0), "InputContext", "");

            engine->rootContext()->setContextProperty("textTranslator", new MTextTranslator);
            qmlRegisterUncreatableType<MTextTranslator>(uri, SINCE_VERSION(1, 0), "TextTranslator", "");

            // Disable cursor blinking + make double tapping work the way it is done in lmt.
            QApplication *app = qobject_cast<QApplication*>(QApplication::instance());
            if (app) {
                app->setCursorFlashTime(0);
                app->setDoubleClickInterval(MEEGOTOUCH_DOUBLETAP_INTERVAL);
            }

            MLocaleWrapper *locale = new MLocaleWrapper;
            engine->rootContext()->setContextProperty("locale", locale);
            engine->rootContext()->setContextProperty("UiConstants", uiConstants(locale));
            qmlRegisterUncreatableType<MLocaleWrapper>(uri, 1, 0, "Locale", "");
        }
    }

    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("com.meego") || uri == QLatin1String("com.nokia.meego") || uri == QLatin1String("Qt.labs.components.native"));
        qmlRegisterUncreatableType<MDateTimeHelper>(uri, SINCE_VERSION(1, 0), "DateTime", "");
        qmlRegisterUncreatableType<MPageStatus>(uri, SINCE_VERSION(1, 0), "PageStatus", "");
        qmlRegisterUncreatableType<MDialogStatus>(uri, SINCE_VERSION(1, 0), "DialogStatus", "");
        qmlRegisterUncreatableType<MWindowState>(uri, SINCE_VERSION(1, 0), "WindowState","");
        qmlRegisterUncreatableType<MPageOrientation>(uri, SINCE_VERSION(1, 0), "PageOrientation", "");
        qmlRegisterUncreatableType<MToolBarVisibility>(uri, SINCE_VERSION(1, 0), "ToolBarVisibility", "");
        qmlRegisterUncreatableType<MTextTranslator>(uri, SINCE_VERSION(1, 0), "TextTranslator", "");

        // Custom primitives
        qmlRegisterType<MDeclarativeImplicitSizeItem>(uri, SINCE_VERSION(1, 0), "ImplicitSizeItem");
        qmlRegisterType<MInverseMouseArea>(uri, SINCE_VERSION(1, 0), "InverseMouseArea");
        qmlRegisterType<MDeclarativeMouseFilter>(uri, SINCE_VERSION(1, 0), "MouseFilter");
        qmlRegisterType<MDeclarativeMouseEvent>(uri, SINCE_VERSION(1, 0), "MMouseEvent");
        qmlRegisterType<MDeclarativeIMAttributeExtension>(uri, SINCE_VERSION(1, 0), "SipAttributes");
        qmlRegisterType<MDeclarativeIMObserver>(uri, SINCE_VERSION(1, 0), "InputMethodObserver");

        qmlRegisterType<MScrollDecoratorSizer>(uri, SINCE_VERSION(1, 0), "ScrollDecoratorSizerCPP");
        qmlRegisterType<MInverseMouseArea>(uri, SINCE_VERSION(1, 0), "InverseMouseArea");

    }

    QDeclarativePropertyMap *uiConstants(MLocaleWrapper *locale = 0) {

        QString defaultFontFamily      = QLatin1String("Open Sans");
        QString defaultFontFamilyLight = QLatin1String("Open Sans");

        // use arial when language is set to farsi
        if (locale && locale->language() == QLatin1String("fa")) {
            defaultFontFamily = QLatin1String("Arial");
            defaultFontFamilyLight = QLatin1String("Arial");
        }

        QDeclarativePropertyMap *uiConstantsData = new QDeclarativePropertyMap();
        uiConstantsData->insert("DefaultFontFamily", defaultFontFamily);
        uiConstantsData->insert("DefaultFontFamilyLight", defaultFontFamilyLight);
        uiConstantsData->insert("DefaultFontFamilyBold", "Nokia Pure Text Bold");
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

        uiConstantsData->insert("IndentDefault", QVariant(16)); // For left and right
        uiConstantsData->insert("GroupHeaderHeight", QVariant(40));

        QFont bodyTextFont;
        bodyTextFont.setFamily(defaultFontFamilyLight);
        bodyTextFont.setPixelSize(24);
        uiConstantsData->insert("BodyTextFont", QVariant(bodyTextFont));

        QFont headerFont;
        headerFont.setFamily(defaultFontFamilyLight);
        headerFont.setPixelSize(32);
        uiConstantsData->insert("HeaderFont", QVariant(headerFont));

        QFont groupHeaderFont;
        groupHeaderFont.setFamily(defaultFontFamily);
        groupHeaderFont.setPixelSize(18);
        groupHeaderFont.setBold(true);
        uiConstantsData->insert("GroupHeaderFont", QVariant(groupHeaderFont));

        QFont titleFont;
        titleFont.setFamily(defaultFontFamily);
        titleFont.setPixelSize(26);
        titleFont.setBold(true);
        uiConstantsData->insert("TitleFont", QVariant(titleFont));

        QFont smallTitleFont;
        smallTitleFont.setFamily(defaultFontFamily);
        smallTitleFont.setPixelSize(24);
        smallTitleFont.setBold(true);
        uiConstantsData->insert("SmallTitleFont", QVariant(smallTitleFont));

        QFont fieldLabelFont;
        fieldLabelFont.setFamily(defaultFontFamilyLight);
        fieldLabelFont.setPixelSize(22);
        uiConstantsData->insert("FieldLabelFont", QVariant(fieldLabelFont));
        uiConstantsData->insert("FieldLabelColor", QVariant(QColor("#505050")));

        QFont subTitleFont;
        subTitleFont.setFamily(defaultFontFamilyLight);
        subTitleFont.setPixelSize(22);
        uiConstantsData->insert("SubtitleFont", QVariant(subTitleFont));

        QFont itemInfoFont;
        itemInfoFont.setFamily(defaultFontFamilyLight);
        itemInfoFont.setPixelSize(18);
        uiConstantsData->insert("InfoFont", QVariant(itemInfoFont));

        return uiConstantsData;
    }
};

#include "plugin.moc"

