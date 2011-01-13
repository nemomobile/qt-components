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

#define QT_NO_CAST_FROM_ASCII
#define QT_NO_CAST_TO_ASCII

#include "sstylewrapper.h"
#include "sstylewrapper_p.h"
#include "sdeclarativeframe.h"

#include <QFile>
#include <QTextStream>
#include <QGraphicsObject>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QPointer>

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#endif

// Use static variables. There are a lot of SStyleWrapper instances
// which like to share the same data.
struct SStyleWrapperData
{
    SStyleWrapperData() : screenAreaDependentParametersUpToDate(false), graphicsView(0) { }
    QHash<QString, quint32> colorMappings;
    QHash<QString, qreal> layoutParameterMappings;
    bool screenAreaDependentParametersUpToDate;
    QGraphicsView *graphicsView;
    QPointer<QObject> screen;
};
Q_GLOBAL_STATIC(SStyleWrapperData, styleData)

static void loadVariables(const QString &filename)
{
    if (!styleData()->screen) {
        qDebug() << "SStylewrapper:loadVariables fails - screen does no exist";
        return;
    }

    QFile file(filename);

    // Index only if given CSS file exists in theme.
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        const qreal unitValue = styleData()->screen->property("unit").value<qreal>();
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();

            if (line.startsWith(QLatin1String("qtc_"))) {

                // Extract name and value from line
                QString name = line.mid(0, line.indexOf(QLatin1Char(':'))).trimmed();
                QString value;
                if (line.at(line.indexOf(QLatin1Char(':')) + 1) == QLatin1Char('#')) {
                    // qtc_conv_list_received_normal:#B5B5B5;
                    int startIndex = line.indexOf(QLatin1Char('#'));
                    int endIndex = line.indexOf(QLatin1Char(';'));
                    value = line.mid(startIndex + 1, endIndex - startIndex - 1).trimmed();
                } else if (line.indexOf(QLatin1String("var(")) >= 0) {
                    // qtc_conv_list_received_normal:var(qtc_conv_list_sent_normal);
                    int startIndex = line.indexOf(QLatin1String("var(")) + 4;
                    int endIndex = line.indexOf(QLatin1Char(')'));
                    value = line.mid(startIndex, endIndex - startIndex).trimmed();
                }
                bool ok = false;
                styleData()->colorMappings.insert(name, (quint32)value.toUInt(&ok, 16)); // Might cause compiler warning in 64 bit systems
            }

            if (line.startsWith(QLatin1String("param-"))) {
                QString name = line.mid(0, line.indexOf(QLatin1Char(':'))).trimmed();
                QString value;
                int startIndex = line.indexOf(QLatin1Char(':'));
                int endIndex = line.indexOf(QLatin1Char(';'));
                value = line.mid(startIndex + 1, endIndex - startIndex - 1).trimmed();

                int unitIndex = value.indexOf(QLatin1String("un"));
                if (unitIndex != -1) {
                    value = value.left(unitIndex);
                }

                bool ok = false;
                qreal valueInReal = (qreal)value.toDouble(&ok);
                if (unitIndex != -1) {
                    valueInReal *= unitValue;
                }

                if (ok) {
                    styleData()->layoutParameterMappings.insert(name, valueInReal);
                }
            }
        }
        file.close();
    }
    return;
}

static void setScreenSizeParameters()
{
    QSize screenSize;
#ifdef Q_OS_SYMBIAN
    screenSize = QApplication::desktop()->screenGeometry().size();
#else
    if (styleData()->graphicsView) {
        screenSize = styleData()->graphicsView->size();
    }
#endif

    styleData()->layoutParameterMappings.insert(QLatin1String("param-screen-width"), screenSize.width());
    styleData()->layoutParameterMappings.insert(QLatin1String("param-screen-height"), screenSize.height());
    styleData()->layoutParameterMappings.insert(QLatin1String("param-screen-short-edge"), qMin(screenSize.width(), screenSize.height()));
    styleData()->layoutParameterMappings.insert(QLatin1String("param-screen-long-edge"), qMax(screenSize.width(), screenSize.height()));
}


SStyleWrapperPrivate::SStyleWrapperPrivate(SStyleWrapper *qq) :
    q_ptr(qq),
    mode(QLatin1String("default")),
    styleClass(),
    styleType(),
    styleObjectName(),
    updateOnScreenAreaChange(false),
    listeningDisplayChange(false)
{
}

SStyleWrapperPrivate::~SStyleWrapperPrivate()
{
}

QFont SStyleWrapperPrivate::fetchFont(SStyleWrapper::FontRole role, qreal textPaneHeight) const
{
    QFont result;

    qreal pixelSize = 1;
    switch (role) {
    case SStyleWrapper::Primary:
        result.setFamily(QLatin1String("Nokia Sans"));
        result.setWeight(75);
        pixelSize = fetchLayoutParameter(QLatin1String("param-text-height-primary"));
        break;
    case SStyleWrapper::Undefined:
    case SStyleWrapper::Secondary:
        result.setFamily(QLatin1String("Nokia Sans"));
        result.setWeight(50);
        pixelSize = fetchLayoutParameter(QLatin1String("param-text-height-secondary"));
        break;
    case SStyleWrapper::Title:
        result.setFamily(QLatin1String("Nokia Sans Title SemiBold"));
        result.setWeight(50);
        pixelSize = fetchLayoutParameter(QLatin1String("param-text-height-title"));
        break;
    case SStyleWrapper::PrimarySmall:
        result.setFamily(QLatin1String("Nokia Sans"));
        result.setWeight(75);
        pixelSize = fetchLayoutParameter(QLatin1String("param-text-height-tiny"));
        break;
    case SStyleWrapper::Digital:
        result.setFamily(QLatin1String("OCR A Extended"));
        result.setWeight(50);
        pixelSize = fetchLayoutParameter(QLatin1String("param-text-height-secondary"));
        break;
    default:
        break;
    }

    const qreal downSizeMultiplier = 0.80; //magic
    if (textPaneHeight > 0)
        result.setPixelSize(int(textPaneHeight * downSizeMultiplier));
    else
        result.setPixelSize(int(pixelSize * downSizeMultiplier));

    return result;
}

QColor SStyleWrapperPrivate::fetchThemeColor(const QString &colorName) const
{
    QColor resultColor;

    if (!styleData()->colorMappings.count()) {
        initScreenPtr();
        loadVariables(QLatin1String(":/colors/widgetcolorgroup.css"));
    }

    if (styleData()->colorMappings.contains(colorName))
        resultColor = QColor(styleData()->colorMappings.value(colorName));

    if (!resultColor.isValid())
        qDebug() << "Fail to map color: " << colorName ;

    return resultColor;
}

qreal SStyleWrapperPrivate::fetchLayoutParameter(const QString &layoutParameterName) const
{
    qreal resultValue = 0.f;
    bool ok = false;

    if (!styleData()->graphicsView) {
        QWidgetList rootWidgets = QApplication::topLevelWidgets();
        foreach (QWidget *widget, rootWidgets) {
            if (QGraphicsView *view = qobject_cast<QGraphicsView *>(widget)) {
                styleData()->graphicsView = view;
                break;
            }
        }
    }

    initScreenPtr();
    if (!styleData()->layoutParameterMappings.count()) {
        loadVariables(QLatin1String(":/layouts/globalparameters.css"));
        setScreenSizeParameters();
        styleData()->screenAreaDependentParametersUpToDate = true;
    }

    if (styleData()->layoutParameterMappings.contains(layoutParameterName)) {
        if (layoutParameterName == QLatin1String("param-screen-width") || layoutParameterName == QLatin1String("param-screen-height")) {

            if (!styleData()->screenAreaDependentParametersUpToDate) {
                setScreenSizeParameters();
                styleData()->screenAreaDependentParametersUpToDate = true;
            }

            // register to auto-fetch when orientation changes
            if (!updateOnScreenAreaChange) {
#ifdef Q_OS_SYMBIAN
                QObject::connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), q_ptr, SLOT(desktopWorkareaChanged()), Qt::QueuedConnection);
#else
                if (styleData()->graphicsView)
                    styleData()->graphicsView->installEventFilter(q_ptr);
#endif
                updateOnScreenAreaChange = true;
            }
        }

        resultValue = styleData()->layoutParameterMappings.value(layoutParameterName);
        ok  = true;
    }

    if (!ok)
        qDebug() << "Fail to load layout parameter: " << layoutParameterName ;

    return resultValue;
}

void SStyleWrapperPrivate::_q_displayChanged()
{
    Q_Q(SStyleWrapper);
    // this forces re-read of the layout parametes when next time needed.
    styleData()->layoutParameterMappings.clear();

    QMetaObject::invokeMethod(q, "_q_desktopWorkareaChanged", Qt::QueuedConnection);
}

void SStyleWrapperPrivate::play(int effect)
{
    Q_UNUSED(effect);
}

void SStyleWrapperPrivate::updateStyle()
{
    Q_Q(SStyleWrapper);
    invalidateStyle();
    emit q->currentStyleChanged();
}

void SStyleWrapperPrivate::invalidateStyle()
{
}

void SStyleWrapperPrivate::initScreenPtr() const
{
    Q_Q(const SStyleWrapper);
    if (!styleData()->screen) {
        QDeclarativeContext *context = QDeclarativeEngine::contextForObject(q);
        if (context)
            styleData()->screen = qVariantValue<QObject *>(context->contextProperty(QLatin1String("screen")));
    }

#ifndef Q_OS_SYMBIAN
    // Reloads the layout parameters when the unit value changes
    // in symbian, the orientation switch does not result a "display change".
    if (styleData()->screen && !listeningDisplayChange) {
        QObject::connect(styleData()->screen, SIGNAL(displayChanged()), q, SLOT(_q_displayChanged()));
        listeningDisplayChange = true;
    }
#endif // !Q_OS_SYMBIAN
}

QVariant SStyleWrapperPrivate::buttonProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary, fetchLayoutParameter(QLatin1String("param-text-height-tiny")));

    if (propertyName == QLatin1String("font_secondary"))
        return fetchFont(SStyleWrapper::Secondary, fetchLayoutParameter(QLatin1String("param-text-height-secondary")));

    if (propertyName == QLatin1String("background"))
        return buttonPropertyBackground();

    if (propertyName == QLatin1String("frameType"))
        return SDeclarativeFrame::NinePieces;

    if (propertyName == QLatin1String("textColor"))
        return buttonPropertyTextColor();

    if (propertyName == QLatin1String("iconMarginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("iconMarginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    if (propertyName == QLatin1String("iconMarginTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));

    if (propertyName == QLatin1String("iconMarginBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    if (propertyName == QLatin1String("iconWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-function"));

    if (propertyName == QLatin1String("iconHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-function"));

    if (propertyName == QLatin1String("textMarginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("textMarginLeftInner"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("textMarginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::textComponentProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("frameName"))
        return textComponentFrameName();

    if (propertyName == QLatin1String("frameType"))
        return SDeclarativeFrame::NinePieces;

    if (propertyName == QLatin1String("placeholderColor") || propertyName == QLatin1String("textColor"))
        return fetchThemeColor(QLatin1String("qtc_text_component_normal"));

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Secondary, fetchLayoutParameter(QLatin1String("param-text-height-tiny"))); // magic

    if (propertyName == QLatin1String("selectionTextColor"))
        return fetchThemeColor(QLatin1String("qtc_text_component_selected"));

    if (propertyName == QLatin1String("selectionColor"))
        return fetchThemeColor(QLatin1String("qtc_text_component_marker_normal"));

    if (propertyName == QLatin1String("paddingLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("paddingRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("paddingTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    if (propertyName == QLatin1String("paddingBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::titleBarProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("height"))
        return fetchLayoutParameter(QLatin1String("param-widget-chrome-height")) * 2 / 3;

    if (propertyName == QLatin1String("textColor"))
        return fetchThemeColor(QLatin1String("qtc_button_normal"));

    if (propertyName == QLatin1String("frame"))
        return QLatin1String("qtg_fr_titlebar_normal");

    if (propertyName == QLatin1String("frameType"))
        return SDeclarativeFrame::ThreePiecesHorizontal;

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Title);

    return QVariant();
}

QVariant SStyleWrapperPrivate::listItemProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("mainTextColor"))
        return listItemPropertyMainTextColor();

    if (propertyName == QLatin1String("subTextColor"))
        return listItemPropertySubTextColor();

    if (propertyName == QLatin1String("frame"))
        return QLatin1String("qtg_fr_list_normal");

    if (propertyName == QLatin1String("frameType"))
        return SDeclarativeFrame::NinePieces;

    if (propertyName == QLatin1String("frameFader"))
        return QLatin1String("qtg_fr_list_pressed");

    if (propertyName == QLatin1String("frameFaderType"))
        return SDeclarativeFrame::NinePieces;

    if (propertyName == QLatin1String("mainTextFont"))
        return fetchFont(SStyleWrapper::Primary);

    if (propertyName == QLatin1String("subTextFont"))
        return fetchFont(SStyleWrapper::Secondary);

    if (propertyName == QLatin1String("marginHorizontalMiddle"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("marginVerticalMiddle"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    if (propertyName == QLatin1String("marginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("marginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    if (propertyName == QLatin1String("marginTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));

    if (propertyName == QLatin1String("marginBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    if (propertyName == QLatin1String("primaryTextHeight"))
        return fetchLayoutParameter(QLatin1String("param-text-height-primary"));

    if (propertyName == QLatin1String("secondaryTextHeight"))
        return fetchLayoutParameter(QLatin1String("param-text-height-secondary"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::basicListItemImageProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("iconSmallWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("iconSmallHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("iconMediumWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-medium"));

    if (propertyName == QLatin1String("iconMediumHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-medium"));

    if (propertyName == QLatin1String("iconLargeWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-large"));

    if (propertyName == QLatin1String("iconLargeHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-large"));

    if (propertyName == QLatin1String("imageWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-image-portrait"));

    if (propertyName == QLatin1String("imageHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-image-portrait"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::pageContainerProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("backgroundImage")) {
        if (mode == QLatin1String("landscape"))
            return QLatin1String("qtg_graf_screen_bg_lsc");
        else
            return QLatin1String("qtg_graf_screen_bg_prt");
    }
    return QVariant();
}

QVariant SStyleWrapperPrivate::checkBoxProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Secondary);

    if (propertyName == QLatin1String("textColor"))
        return fetchThemeColor(QLatin1String("qtc_default_main_pane_normal"));

    if (propertyName == QLatin1String("iconName")) {
        if (mode == QLatin1String("selected"))
            return QLatin1String("qtg_small_selected");
        else
            return QLatin1String("qtg_small_unselected"); // Default
    }

    if (propertyName == QLatin1String("padding"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("iconWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("iconHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("topMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));

    if (propertyName == QLatin1String("bottomMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::dialogProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary);

    if (propertyName == QLatin1String("titleTextColor"))
        return fetchThemeColor(QLatin1String("qtc_popup_heading_normal"));

    if (propertyName == QLatin1String("titleBarBackground"))
        return QLatin1String("qtg_fr_popup_heading");

    if (propertyName == QLatin1String("titleBarFrameType"))
        return SDeclarativeFrame::ThreePiecesHorizontal;

    if (propertyName == QLatin1String("popupBackground"))
        return QLatin1String("qtg_fr_popup");

    if (propertyName == QLatin1String("popupFrameType"))
        return SDeclarativeFrame::NinePieces;

    if (propertyName == QLatin1String("titleTextHeight"))
        return fetchLayoutParameter(QLatin1String("param-text-height-primary"));

    if (propertyName == QLatin1String("margin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-popup"));

    if (propertyName == QLatin1String("screenMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    if (propertyName == QLatin1String("buttonHeight"))
        return fetchLayoutParameter(QLatin1String("param-widget-popup-softkey-height"));

    if (propertyName == QLatin1String("contentMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-popup-list"));

    if (propertyName == QLatin1String("buttonMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-center-align"));

    QRect appRect;
#ifdef Q_OS_SYMBIAN
    CEikonEnv *eikonEnv = CEikonEnv::Static();
    const TRect newApplicationAreaSymbian = static_cast<CEikAppUi *>(eikonEnv->AppUi())->ClientRect();
    appRect.setRect(newApplicationAreaSymbian.iTl.iX, newApplicationAreaSymbian.iTl.iY, newApplicationAreaSymbian.Width(), newApplicationAreaSymbian.Height());
#else
    QWidgetList widgetList = QApplication::allWidgets();
    // resize all graphicsviews (usually just one of them)
    foreach (QWidget *widget, widgetList) {
        QGraphicsView *view = qobject_cast<QGraphicsView *>(widget);
        if (view) {
            appRect.setSize(view->size());
            break;
        }
    }
#endif //Q_OS_SYMBIAN

    if (propertyName == QLatin1String("verticalCenter"))
        return appRect.height() / 2;

    if (propertyName == QLatin1String("maxWidth"))
        return appRect.width() - 2 * fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    if (propertyName == QLatin1String("maxHeight"))
        return appRect.height() - 2 * fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::scrollBarProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("interactiveScrollbarWidth"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-interactive-width"));

    if (propertyName == QLatin1String("indicativeScrollbarWidth"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-indicative-width"));

    if (propertyName == QLatin1String("scrollbarTrackColor"))
        return fetchThemeColor(QLatin1String("qtc_scrollbar_track"));

    if (propertyName == QLatin1String("scrollbarHandlePressedColor"))
        return fetchThemeColor(QLatin1String("qtc_scrollbar_handle_pressed"));

    if (propertyName == QLatin1String("scrollbarHandleReleasedColor"))
        return fetchThemeColor(QLatin1String("qtc_scrollbar_handle_released"));

    if (propertyName == QLatin1String("minInteractiveHandleSize"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-interactive-minimum-handle"));

    if (propertyName == QLatin1String("minIndicativeHandleSize"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-indicative-minimum-handle"));

    if (propertyName == QLatin1String("hideTimeout"))
        //TODO: Extract from a common effects styling file
        return 2000;

    if (propertyName == QLatin1String("interactiveKeyEventSize"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-interactive-key-event-size"));

    if (propertyName == QLatin1String("interactiveScrollbarTouchAreaThickness"))
        return fetchLayoutParameter(QLatin1String("param-touch-area-gene-primary-small"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::progressBarProperty(const QString &propertyName) const
{
    // Graphic tiles
    if (propertyName == QLatin1String("backgroundFrame"))
        return QLatin1String("qtg_fr_progbar_h_frame");

    if ((propertyName == QLatin1String("backgroundType")) || (propertyName == QLatin1String("contentsType")))
        return SDeclarativeFrame::ThreePiecesHorizontal;

    if (propertyName == QLatin1String("contentsFrame"))
        return QLatin1String("qtg_fr_progbar_h_filled");

    if (propertyName == QLatin1String("indeterminateIcon"))
        return QLatin1String("qtg_graf_progbar_h_wait");

    if (propertyName == QLatin1String("indeterminateMask"))
        return QLatin1String("qtg_fr_progbar_h_mask");

    // Sizes and measurements
    if ((propertyName == QLatin1String("height")))
        return fetchLayoutParameter(QLatin1String("param-widget-progress-bar-height"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::radioButtonProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("iconName")) {
        if (mode == QLatin1String("checked") || mode == QLatin1String("pressed"))
            return QLatin1String("qtg_small_radio_selected");
        else if (mode == QLatin1String("default"))
            return QLatin1String("qtg_small_radio_unselected");
        else if (mode == QLatin1String("focused"))
            return QLatin1String("qtg_small_radio_unselected_highlight");
        else if (mode == QLatin1String("checkedanddisabled"))
            return QLatin1String("qtg_small_radio_selected_disabled");
        else if (mode == QLatin1String("checkedandfocused"))
            return QLatin1String("qtg_small_radio_selected_highlight");
        else // disabled
            return QLatin1String("qtg_small_radio_unselected_disabled");
    }

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary, fetchLayoutParameter(QLatin1String("param-text-height-secondary")));

    // TODO: popup list item colors are not handled separately yet, popup item background missing
    if (propertyName == QLatin1String("textColor")) {
        if (mode == QLatin1String("pressed"))
            return fetchThemeColor(QLatin1String("qtc_list_item_pressed"));
        else if (mode == QLatin1String("focused") || mode == QLatin1String("checkedandfocused"))
            return fetchThemeColor(QLatin1String("qtc_list_item_highlight"));
        else if (mode == QLatin1String("disabled") || mode == QLatin1String("checkedanddisabled"))
            return fetchThemeColor(QLatin1String("qtc_list_item_disabled"));
        else
            return fetchThemeColor(QLatin1String("qtc_list_item_title_normal"));
    }

    if (propertyName == QLatin1String("paddingLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("marginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("marginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    if (propertyName == QLatin1String("marginTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));

    if (propertyName == QLatin1String("marginBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    if (propertyName == QLatin1String("iconWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("iconHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::toolTipProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("toolTipBackground"))
        return QLatin1String("qtg_fr_popup_preview");

    if (propertyName == QLatin1String("toolTipFrameType"))
        return SDeclarativeFrame::NinePieces;

    if (propertyName == QLatin1String("horizontalMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("verticalMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    if (propertyName == QLatin1String("screenMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Secondary, fetchLayoutParameter(QLatin1String("param-text-height-tiny")));

    if (propertyName == QLatin1String("fontColor"))
        return fetchThemeColor(QLatin1String("qtc_popup_preview_normal"));

    return QVariant();
}

void SStyleWrapperPrivate::_q_desktopWorkareaChanged()
{
    Q_Q(SStyleWrapper);
    styleData()->screenAreaDependentParametersUpToDate = false;

    // we could de-register the change notifications here and re-register them again when
    // screen dependent layout parameter is fetched. currently there is no need.
    emit q->currentStyleChanged();
}

QVariant SStyleWrapperPrivate::buttonPropertyBackground() const
{
    if (mode == QLatin1String("pressed"))
        return QLatin1String("qtg_fr_btn_pressed");
    else if (mode == QLatin1String("focused"))
        return QLatin1String("qtg_fr_btn_highlight");
    else if (mode == QLatin1String("checked"))
        return QLatin1String("qtg_fr_btn_latched");
    else
        return QLatin1String("qtg_fr_btn_normal");
}

QVariant SStyleWrapperPrivate::buttonPropertyTextColor() const
{
    if (mode == QLatin1String("pressed"))
        return fetchThemeColor(QLatin1String("qtc_button_pressed"));
    else if (mode == QLatin1String("focused"))
        return fetchThemeColor(QLatin1String("qtc_button_latched"));
    else if (mode == QLatin1String("checked"))
        return fetchThemeColor(QLatin1String("qtc_button_latched"));
    else if (mode == QLatin1String("default"))
        return fetchThemeColor(QLatin1String("qtc_button_normal"));
    else
        return QVariant();
}

QVariant SStyleWrapperPrivate::textComponentFrameName() const
{
    if (mode == QLatin1String("selected"))
        return QLatin1String("qtg_fr_text_component_highlight");
    else if (mode == QLatin1String("default"))
        return QLatin1String("qtg_fr_text_component_normal");
    else
        return QVariant();
}

QVariant SStyleWrapperPrivate::listItemPropertyMainTextColor() const
{
    if (mode == QLatin1String("pressed"))
        return fetchThemeColor(QLatin1String("qtc_list_item_pressed"));
    else if (mode == QLatin1String("default"))
        return fetchThemeColor(QLatin1String("qtc_list_item_title_normal"));
    else
        return QVariant();
}

QVariant SStyleWrapperPrivate::listItemPropertySubTextColor() const
{
    if (mode == QLatin1String("pressed"))
        return fetchThemeColor(QLatin1String("qtc_list_item_pressed"));
    else if (mode == QLatin1String("default"))
        return fetchThemeColor(QLatin1String("qtc_list_item_content_normal"));
    else
        return QVariant();
}
