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

#define QT_NO_CAST_FROM_ASCII
#define QT_NO_CAST_TO_ASCII

#include "sstylewrapper.h"
#include "sstylewrapper_p.h"

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

#ifdef HAVE_MOBILITY
#include <QFeedbackEffect>
#endif //HAVE_MOBILITY

static const qreal DEFAULT_DP_PER_DPI = 160.0;

// Use static variables. There are a lot of SStyleWrapper instances
// which like to share the same data.
struct SStyleWrapperData
{
    SStyleWrapperData() :
        screenAreaDependentParametersUpToDate(false),
        instancesNotifiedOnScreenChange(0),
        graphicsView(0) { }
    QHash<QString, quint32> colorMappings;
    QHash<QString, qreal> layoutParameterMappings;
    bool screenAreaDependentParametersUpToDate;
    int instancesNotifiedOnScreenChange;
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

        const qreal dpValue = styleData()->screen->property("dpi").value<qreal>() / DEFAULT_DP_PER_DPI;
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

                int dpIndex = value.indexOf(QLatin1String("dp"));
                if (dpIndex != -1) {
                    value = value.left(dpIndex);
                }

                bool ok = false;
                qreal valueInReal = (qreal)value.toDouble(&ok);
                if (dpIndex != -1) {
                    valueInReal *= dpValue;
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
    // Cannot use the screen.size() because the geometry is not updated early enough on application start.
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
    updateOnScreenAreaChange(false)
{
}

SStyleWrapperPrivate::~SStyleWrapperPrivate()
{
    if (updateOnScreenAreaChange)
        if ((--styleData()->instancesNotifiedOnScreenChange) == 0)
            styleData()->screenAreaDependentParametersUpToDate = false;
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
                QObject::connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), q_ptr, SLOT(_q_desktopWorkareaChanged()), Qt::QueuedConnection);
#else
                if (styleData()->graphicsView)
                    styleData()->graphicsView->installEventFilter(q_ptr);
#endif
                updateOnScreenAreaChange = true;
                styleData()->instancesNotifiedOnScreenChange++;
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
#ifdef HAVE_MOBILITY
    QtMobility::QFeedbackEffect::playThemeEffect(static_cast<QtMobility::QFeedbackEffect::ThemeEffect>(effect));
#else
    Q_UNUSED( effect );
#endif //HAVE_MOBILITY
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

#ifndef Q_OS_SYMBIAN
        // Reloads the layout parameters when the display changes.
        // In Symbian, the orientation switch does not result a "display change".
        if (styleData()->screen)
            QObject::connect(styleData()->screen, SIGNAL(displayChanged()), q, SLOT(_q_displayChanged()));
#endif // !Q_OS_SYMBIAN
    }
}

QVariant SStyleWrapperPrivate::buttonProperty(const QString &propertyName) const
{
    // TabButton overrides some properties
    if (styleObjectName == QLatin1String("TabButton")) {
        QVariant ret = tabButtonProperty(propertyName);
        if (!ret.isNull())
            return ret;
    }

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary, fetchLayoutParameter(QLatin1String("param-text-height-tiny")));

    if (propertyName == QLatin1String("font_secondary"))
        return fetchFont(SStyleWrapper::Secondary, fetchLayoutParameter(QLatin1String("param-text-height-secondary")));

    if (propertyName == QLatin1String("background"))
        return buttonPropertyBackground();

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
    if (propertyName == QLatin1String("background"))
        return textComponentBackground();

    if (propertyName == QLatin1String("placeholderColor") || propertyName == QLatin1String("color"))
        return fetchThemeColor(QLatin1String("qtc_text_component_normal"));

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Secondary, fetchLayoutParameter(QLatin1String("param-text-height-tiny"))); // magic

    if (propertyName == QLatin1String("selectionTextColor"))
        return fetchThemeColor(QLatin1String("qtc_text_component_selected"));

    if (propertyName == QLatin1String("selectionColor"))
        return fetchThemeColor(QLatin1String("qtc_text_component_marker_normal"));

    if (propertyName == QLatin1String("leftMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("rightMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("topMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    if (propertyName == QLatin1String("bottomMargin"))
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

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Title);

    return QVariant();
}

QVariant SStyleWrapperPrivate::listHeadingProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_list_heading_normal"));

    if (propertyName == QLatin1String("marginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("marginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("marginTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    if (propertyName == QLatin1String("marginBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::listItemProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_list_normal"));

    if (propertyName == QLatin1String("faderBackground")) {
        if (mode == QLatin1String("default"))
            return imagePath(QLatin1String("qtg_fr_list_normal"));
        if (mode == QLatin1String("pressed"))
            return imagePath(QLatin1String("qtg_fr_list_pressed"));
        if (mode == QLatin1String("focused"))
            return imagePath(QLatin1String("qtg_fr_list_highlight"));
        if (mode == QLatin1String("disabled"))
            return imagePath(QLatin1String("qtg_fr_list_disabled"));
    }

    if (propertyName == QLatin1String("horizontalSpacing"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("verticalSpacing"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    if (propertyName == QLatin1String("marginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("marginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    if (propertyName == QLatin1String("marginTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));

    if (propertyName == QLatin1String("marginBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::listItemImageProperty(const QString &propertyName) const
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

QVariant SStyleWrapperPrivate::listItemTextProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("textColor")) {
        if (styleObjectName == QLatin1String("Title")) {
            if (mode == QLatin1String("default"))
                return fetchThemeColor(QLatin1String("qtc_list_item_title_normal"));
            else if (mode == QLatin1String("pressed"))
                return fetchThemeColor(QLatin1String("qtc_list_item_pressed"));
            else if (mode == QLatin1String("disabled"))
                return fetchThemeColor(QLatin1String("qtc_list_item_disabled"));
            else if (mode == QLatin1String("focused"))
                return fetchThemeColor(QLatin1String("qtc_list_item_highlight"));
        }
        if (styleObjectName == QLatin1String("SubTitle")) {
            if (mode == QLatin1String("default"))
                return fetchThemeColor(QLatin1String("qtc_list_item_content_normal"));
            else if (mode == QLatin1String("pressed"))
                return fetchThemeColor(QLatin1String("qtc_list_item_pressed"));
            else if (mode == QLatin1String("disabled"))
                return fetchThemeColor(QLatin1String("qtc_list_item_disabled"));
            else if (mode == QLatin1String("focused"))
                return fetchThemeColor(QLatin1String("qtc_list_item_highlight"));
        }
        if (styleObjectName == QLatin1String("Heading"))
            return fetchThemeColor(QLatin1String("qtc_list_item_title_normal"));
    }

    if (propertyName == QLatin1String("font")) {
        if (styleObjectName == QLatin1String("Title"))
            return fetchFont(SStyleWrapper::Primary);
        if (styleObjectName == QLatin1String("SubTitle"))
            return fetchFont(SStyleWrapper::Secondary);
        if (styleObjectName == QLatin1String("Heading"))
            return fetchFont(SStyleWrapper::PrimarySmall);
    }

    if (propertyName == QLatin1String("textHeight")) {
        if (styleObjectName == QLatin1String("Title"))
            return fetchLayoutParameter(QLatin1String("param-text-height-primary"));
        if (styleObjectName == QLatin1String("SubTitle"))
            return fetchLayoutParameter(QLatin1String("param-text-height-secondary"));
        if (styleObjectName == QLatin1String("Heading"))
            return fetchLayoutParameter(QLatin1String("param-text-height-tiny"));
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
        if (mode == QLatin1String("pressed"))
            return imagePath(QLatin1String("qtg_graf_checkbox_pressed"));
        else if (mode == QLatin1String("checked"))
            return imagePath(QLatin1String("qtg_graf_checkbox_normal_selected"));
        else if (mode == QLatin1String("unchecked"))
            return imagePath(QLatin1String("qtg_graf_checkbox_normal_unselected"));
        else if (mode == QLatin1String("disabled"))
            return imagePath(QLatin1String("qtg_graf_checkbox_disabled_unselected"));
        else if (mode == QLatin1String("checkedAndDisabled"))
            return imagePath(QLatin1String("qtg_graf_checkbox_disabled_selected"));
    }

    if (propertyName == QLatin1String("padding"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("iconWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("iconHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::sectionScrollerProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("backgroundHeight"))
        return 192;     // TODO: 24 * padding-medium
    if (propertyName == QLatin1String("backgroundImage"))
        return imagePath(QLatin1String("qtg_fr_popup_transparent"));
    if (propertyName == QLatin1String("backgroundWidth"))
        return 264;     // TODO: portrait screen width - 8 * padding-large
    if (propertyName == QLatin1String("smallSingleIndexWidth"))
        return 100;     // TODO: get from layout
    if (propertyName == QLatin1String("smallFont"))
        return fetchFont(SStyleWrapper::Secondary, 30);
    if (propertyName == QLatin1String("largeFont"))
        return fetchFont(SStyleWrapper::Secondary, 200);
    if (propertyName == QLatin1String("popupRightMargin"))
        return 50;      // TODO: get from layout
    if (propertyName == QLatin1String("dividerImage"))
        return imagePath(QLatin1String("qtg_fr_popup_divider"));
    if (propertyName == QLatin1String("dividerBufferSize"))
        return 40;
    if (propertyName == QLatin1String("dividerHeight"))
        return 1;       // TODO: get from layout
    if (propertyName == QLatin1String("graphicsSizeTiny"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-tiny"));
    if (propertyName == QLatin1String("indexLeftMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));
    if (propertyName == QLatin1String("indexRightMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));
    if (propertyName == QLatin1String("textHeight"))
        return 59;      // TODO: menu-item-height - round(graphic-size-tiny / 4)
    if (propertyName == QLatin1String("TextMargin"))
        return 40;      // TODO: get from layout
    if (propertyName == QLatin1String("touchAreaWidth"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-interactive-width"));
    if (propertyName == QLatin1String("highlightedTextColor"))
        return QColor(Qt::white);
    if (propertyName == QLatin1String("textColor"))
        return QColor(194, 194, 185, 255);

    return QVariant();
}

QVariant SStyleWrapperPrivate::dialogProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary);

    if (propertyName == QLatin1String("titleTextColor"))
        return fetchThemeColor(QLatin1String("qtc_popup_heading_normal"));

    if (propertyName == QLatin1String("titleBackground"))
        return imagePath(QLatin1String("qtg_fr_popup_heading"));

    if (propertyName == QLatin1String("popupBackground"))
        return imagePath(QLatin1String("qtg_fr_popup"));

    if (propertyName == QLatin1String("titleTextHeight"))
        return fetchLayoutParameter(QLatin1String("param-text-height-primary"));

    if (propertyName == QLatin1String("titleMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-popup"));

    if (propertyName == QLatin1String("screenMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    if (propertyName == QLatin1String("buttonHeight"))
        return fetchLayoutParameter(QLatin1String("param-widget-popup-softkey-height"));

    if (propertyName == QLatin1String("contentMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-popup-list"));

    if (propertyName == QLatin1String("buttonMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-center-align"));

    if (propertyName == QLatin1String("appRectHeight"))
        return faderProperty(propertyName);

    if (propertyName == QLatin1String("appRectWidth"))
        return faderProperty(propertyName);

    if (propertyName == QLatin1String("maxWidth")) {
        qreal width = qMin(fetchLayoutParameter(QLatin1String("param-screen-width")),
            faderProperty(QLatin1String("appRectWidth")).toReal());
        return width - 4 * fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));
    }

    if (propertyName == QLatin1String("maxHeight")) {
        qreal width = qMin(fetchLayoutParameter(QLatin1String("param-screen-height")),
            faderProperty(QLatin1String("appRectHeight")).toReal());
        return width - 4 * fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));
    }

    return QVariant();
}

QVariant SStyleWrapperPrivate::scrollBarProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("thickness")) {
        if (mode == QLatin1String("indicative"))
            return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-indicative-width"));
        else if (mode == QLatin1String("interactive"))
            return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-interactive-width"));
    }

    if (propertyName == QLatin1String("trackColor"))
        return fetchThemeColor(QLatin1String("qtc_scrollbar_track"));

    if (propertyName == QLatin1String("handleColor")) {
        if (mode == QLatin1String("default"))
            return fetchThemeColor(QLatin1String("qtc_scrollbar_handle_released"));
        else if (mode == QLatin1String("pressed"))
            return fetchThemeColor(QLatin1String("qtc_scrollbar_handle_pressed"));
    }

    if (propertyName == QLatin1String("minHandleSize")) {
        if (mode == QLatin1String("indicative"))
            return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-indicative-minimum-handle"));
        else if (mode == QLatin1String("interactive"))
            return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-interactive-minimum-handle"));
    }

    if (propertyName == QLatin1String("hideTimeout"))
        //TODO: Extract from a common effects styling file
        return 2000;

    if (propertyName == QLatin1String("keyEventSize"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-interactive-key-event-size"));

    if (propertyName == QLatin1String("touchAreaThickness"))
        return fetchLayoutParameter(QLatin1String("param-touch-area-gene-primary-small"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::progressBarProperty(const QString &propertyName) const
{
    // Graphic tiles
    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_progressbar_track"));

    if (propertyName == QLatin1String("contentsBackground"))
        return imagePath(QLatin1String("qtg_fr_progressbar_fill"));

    if (propertyName == QLatin1String("indeterminateIcon"))
        return QLatin1String("qtg_graf_progressbar_wait");

    if (propertyName == QLatin1String("indeterminateMask"))
        return QLatin1String("qtg_fr_progressbar_mask");

    // Sizes and measurements
    if ((propertyName == QLatin1String("height")))
        return fetchLayoutParameter(QLatin1String("param-widget-progress-bar-height"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::radioButtonProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("iconName")) {
        if (mode == QLatin1String("checked") || mode == QLatin1String("checkedAndFocused"))
            return imagePath(QLatin1String("qtg_graf_radiobutton_normal_selected"));
        else if (mode == QLatin1String("pressed"))
            return imagePath(QLatin1String("qtg_graf_radiobutton_pressed"));
        else if (mode == QLatin1String("normal") || mode == QLatin1String("focused"))
            return imagePath(QLatin1String("qtg_graf_radiobutton_normal_unselected"));
        else if (mode == QLatin1String("disabled"))
            return imagePath(QLatin1String("qtg_graf_radiobutton_disabled_unselected"));
        else if (mode == QLatin1String("checkedAndDisabled"))
            return imagePath(QLatin1String("qtg_graf_radiobutton_disabled_selected"));
        else
            return imagePath(QLatin1String("qtg_graf_radiobutton_normal_unselected"));
    }

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary, fetchLayoutParameter(QLatin1String("param-text-height-secondary")));

    if (propertyName == QLatin1String("textColor")) {
        if (mode == QLatin1String("pressed"))
            return fetchThemeColor(QLatin1String("qtc_list_item_pressed"));
        else if (mode == QLatin1String("focused") || mode == QLatin1String("checkedAndFocused"))
            return fetchThemeColor(QLatin1String("qtc_list_item_highlight"));
        else if (mode == QLatin1String("disabled") || mode == QLatin1String("checkedAndDisabled"))
            return fetchThemeColor(QLatin1String("qtc_list_item_disabled"));
        else
            return fetchThemeColor(QLatin1String("qtc_list_item_title_normal"));
    }

    if (propertyName == QLatin1String("background")) {
        if (mode == QLatin1String("pressed"))
            return imagePath(QLatin1String("qtg_fr_list_pressed"));
        else if (mode == QLatin1String("focused") || mode == QLatin1String("checkedAndFocused"))
            return imagePath(QLatin1String("qtg_fr_list_highlight"));
        else if (mode == QLatin1String("disabled") || mode == QLatin1String("checkedAndDisabled"))
            return imagePath(QLatin1String("qtg_fr_list_disabled"));
        else
            return imagePath(QLatin1String("qtg_fr_list_normal"));
    }

    if (propertyName == QLatin1String("spacing"))
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

QVariant SStyleWrapperPrivate::sliderProperty(const QString &propertyName) const
{
    // Graphic tiles
    if (propertyName == QLatin1String("trackBackground")) {
        if (mode == QLatin1String("horizontal"))
            return imagePath(QLatin1String("qtg_fr_slider_h_track_normal"));
        else
            return imagePath(QLatin1String("qtg_fr_slider_v_track_normal"));
    }
    // TODO: pressed background graphics do not exist yet, returning normal graphics here
    else if (propertyName == QLatin1String("trackPressedBackground")) {
        if (mode == QLatin1String("horizontal"))
            return imagePath(QLatin1String("qtg_fr_slider_h_track_normal"));
        else
            return imagePath(QLatin1String("qtg_fr_slider_v_track_normal"));
    } else if (propertyName == QLatin1String("handleIcon")) {
        if (mode == QLatin1String("horizontal"))
            return imagePath(QLatin1String("qtg_graf_slider_h_handle_normal"));
        else
            return imagePath(QLatin1String("qtg_graf_slider_v_handle_normal"));
    } else if (propertyName == QLatin1String("handlePressed")) {
        if (mode == QLatin1String("horizontal"))
            return imagePath(QLatin1String("qtg_graf_slider_h_handle_pressed"));
        else
            return imagePath(QLatin1String("qtg_graf_slider_v_handle_pressed"));
    }

    // Sizes and measurements
    if (propertyName == QLatin1String("handleHeight")) {
        if (mode == QLatin1String("horizontal"))
            return fetchLayoutParameter(QLatin1String("param-widget-slider-thumb-width")) * 2;
        else
            return fetchLayoutParameter(QLatin1String("param-widget-slider-thumb-width"));
    } else if (propertyName == QLatin1String("handleWidth")) {
        if (mode == QLatin1String("horizontal"))
            return fetchLayoutParameter(QLatin1String("param-widget-slider-thumb-width"));
        else
            return fetchLayoutParameter(QLatin1String("param-widget-slider-thumb-width")) * 2;
    } else if (propertyName == QLatin1String("handleTouchAreaWidth")) {
        if (mode == QLatin1String("horizontal"))
            return 30; // TODO: get from layout
        else
            return fetchLayoutParameter(QLatin1String("param-touch-area-gene-primary-medium"));
    } else if (propertyName == QLatin1String("handleTouchAreaHeight")) {
        if (mode == QLatin1String("horizontal"))
            return fetchLayoutParameter(QLatin1String("param-touch-area-gene-primary-medium"));
        else
            return 30; // TODO: get from layout
    } else if (propertyName == QLatin1String("trackHeight")) {
        if (mode == QLatin1String("horizontal"))
            return 20; // TODO: get from layout
        else
            return 20; // TODO: get from layout
    }

    // Margins
    if (propertyName == QLatin1String("marginLeft")) {
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));
    } else if (propertyName == QLatin1String("marginRight")) {
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));
    } else if (propertyName == QLatin1String("marginTop")) {
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));
    } else if (propertyName == QLatin1String("marginBottom")) {
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));
    } else if (propertyName == QLatin1String("toolTipSpacing")) {
        return 20; // TODO: get from layout
    }

    return QVariant();
}

QVariant SStyleWrapperPrivate::toolTipProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_popup")); // TODO: real graphics missing

    if (propertyName == QLatin1String("horizontalMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("verticalMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-vertical"));

    if (propertyName == QLatin1String("spacing"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Secondary, fetchLayoutParameter(QLatin1String("param-text-height-tiny")));

    if (propertyName == QLatin1String("color"))
        return fetchThemeColor(QLatin1String("qtc_popup_preview_normal"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::menuContentProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("itemHeight"))
        return QVariant(fetchLayoutParameter(QLatin1String("param-text-height-primary"))
                        + fetchLayoutParameter(QLatin1String("param-margin-gene-top"))
                        + fetchLayoutParameter(QLatin1String("param-margin-gene-bottom")));

    if (propertyName == QLatin1String("listMargin"))
        return QVariant(fetchLayoutParameter(QLatin1String("param-margin-gene-popup-list")));

    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_popup"));

    if (propertyName == QLatin1String("titleTextHeight"))
        return fetchLayoutParameter(QLatin1String("param-text-height-secondary"));

    if (propertyName == QLatin1String("margin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-popup-list"));

    if (propertyName == QLatin1String("titleBackground"))
        return imagePath(QLatin1String("qtg_fr_popup_heading"));

    if (propertyName == QLatin1String("titleTextColor"))
        return fetchThemeColor(QLatin1String("qtc_popup_heading_normal"));

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary);

    if (propertyName == QLatin1String("itemBackground")) {
        if (mode == QLatin1String("default"))
            return imagePath(QLatin1String("qtg_fr_list_popup_normal"));
        else if (mode == QLatin1String("pressed"))
            return imagePath(QLatin1String("qtg_fr_list_pressed"));
    }

    if (propertyName == QLatin1String("drillDownImage"))
        return imagePath(QLatin1String("qtg_graf_drill_down_indicator.svg"));

    if (propertyName == QLatin1String("indicatorWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("indicatorHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("itemMarginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("itemMarginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    if (propertyName == QLatin1String("itemMarginTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));

    if (propertyName == QLatin1String("itemMarginBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    if (propertyName == QLatin1String("color"))
        return fetchThemeColor(QLatin1String("qtc_popup_list_title_normal"));

    if (propertyName == QLatin1String("screenMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    if (propertyName == QLatin1String("appRectHeight"))
        return faderProperty(propertyName);

    if (propertyName == QLatin1String("appRectWidth"))
        return faderProperty(propertyName);

    return QVariant();
}

QVariant SStyleWrapperPrivate::menuProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("appRectHeight"))
        return faderProperty(propertyName);

    return QVariant();
}

QVariant SStyleWrapperPrivate::choiceListProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("textHeight"))
        return fetchLayoutParameter(QLatin1String("param-text-height-secondary"));

    if (propertyName == QLatin1String("textColor")) {
        if (mode == QLatin1String("normal"))
            return fetchThemeColor(QLatin1String("qtc_combobox_normal"));
        else if (mode == QLatin1String("pressed"))
            return fetchThemeColor(QLatin1String("qtc_combobox_pressed"));
        else if (mode == QLatin1String("latched"))
            return fetchThemeColor(QLatin1String("qtc_combobox_latched"));
        else if (mode == QLatin1String("disabled"))
            return fetchThemeColor(QLatin1String("qtc_combobox_disabled"));
        else
            return fetchThemeColor(QLatin1String("qtc_combobox_normal"));
    }

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary);

    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_list_heading_normal"));

    if (propertyName == QLatin1String("marginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("marginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    if (propertyName == QLatin1String("marginTop"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-top"));

    if (propertyName == QLatin1String("marginBottom"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    if (propertyName == QLatin1String("marginMiddle"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("appRectHeight"))
        return faderProperty(propertyName);

    // List indicator related properties
    if (propertyName == QLatin1String("listIndicatorImage"))
        return imagePath(QLatin1String("qtg_graf_drill_down_indicator"));

    if (propertyName == QLatin1String("listIndicatorWidth"))
        return fetchLayoutParameter(QLatin1String("param-widget-combobox-height"));

    if (propertyName == QLatin1String("listIndicatorHeight"))
        return fetchLayoutParameter(QLatin1String("param-widget-combobox-height"));


    // Drop down list related properties
    if (propertyName == QLatin1String("listItemHeight"))
        return fetchLayoutParameter(QLatin1String("param-text-height-primary"))
            + fetchLayoutParameter(QLatin1String("param-margin-gene-top"))
            + fetchLayoutParameter(QLatin1String("param-margin-gene-bottom"));

    if ( propertyName == QLatin1String("listItemTextColor") )
        return fetchThemeColor(QLatin1String("qtc_list_item_title_normal"));

    if (propertyName == QLatin1String("listBackground"))
        return imagePath(QLatin1String("qtg_fr_list_normal"));

    if (propertyName == QLatin1String("selectionIndicatorWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("selectionIndicatorImage"))
        return imagePath(QLatin1String("qtg_graf_choice_list_indicator"));

    if (propertyName == QLatin1String("scrollbarWidth"))
        return fetchLayoutParameter(QLatin1String("param-widget-scroll-bar-indicative-width"));

    if (propertyName == QLatin1String("listMargin"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-popup-list"));

    if (propertyName == QLatin1String("listItemBackground"))
        return imagePath(QLatin1String("qtg_fr_choice_list_normal"));

    if (propertyName == QLatin1String("listItemFont"))
        return fetchFont(SStyleWrapper::Primary);

    return QVariant();
}

QVariant SStyleWrapperPrivate::faderProperty(const QString &propertyName) const
{
    QRect appRect;
#ifdef Q_OS_SYMBIAN
    CEikonEnv *eikonEnv = CEikonEnv::Static();
    const TRect appArea = static_cast<CEikAppUi*>(eikonEnv->AppUi())->ClientRect();
    appRect.setRect(appArea.iTl.iX, appArea.iTl.iY, appArea.Width(), appArea.Height());
#else
    QWidgetList widgetList = QApplication::allWidgets();
    // resize all graphicsviews (usually just one of them)
    foreach (QWidget *w, widgetList) {
        QGraphicsView *gv = qobject_cast<QGraphicsView*>(w);
        if (gv)
            appRect.setSize(gv->size());
    }
#endif //Q_OS_SYMBIAN

    if (propertyName == QLatin1String("appRectHeight"))
        return appRect.height();

    if (propertyName == QLatin1String("appRectWidth"))
        return appRect.width();

    return QVariant();
}

QVariant SStyleWrapperPrivate::toolButtonProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("background")) {
        if (mode == QLatin1String("pressed"))
            return imagePath(QLatin1String("qtg_fr_toolbutton_pressed"));
        else if (mode == QLatin1String("disabled"))
            return imagePath(QLatin1String("qtg_fr_toolbutton_disabled"));
        else if (mode == QLatin1String("checked"))
            return imagePath(QLatin1String("qtg_fr_toolbutton_latched"));
        else
            return imagePath(QLatin1String("qtg_fr_toolbutton_normal"));
    }

    // TODO: IconMargins and TextMargins are picked randomly
    if (propertyName == QLatin1String("iconMarginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("iconMarginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));

    if (propertyName == QLatin1String("textMarginLeft"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-left"));

    if (propertyName == QLatin1String("textMarginLeftInner"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-middle-horizontal"));

    if (propertyName == QLatin1String("textMarginRight"))
        return fetchLayoutParameter(QLatin1String("param-margin-gene-right"));
    // End of suspicious style values

    if (propertyName == QLatin1String("iconButtonWidth") || propertyName == QLatin1String("iconButtonHeight"))
        return fetchLayoutParameter(QLatin1String("param-widget-toolbar-height"));

    if (propertyName == QLatin1String("iconWidth"))
        return fetchLayoutParameter(QLatin1String("param-widget-toolbar-height")) / 2;

    if (propertyName == QLatin1String("iconHeight"))
        return fetchLayoutParameter(QLatin1String("param-widget-toolbar-height")) / 2;

    if (propertyName == QLatin1String("font"))
        return fetchFont(SStyleWrapper::Primary, fetchLayoutParameter(QLatin1String("param-text-height-secondary")));

    if (propertyName == QLatin1String("textButtonWidth")) {
        if (fetchLayoutParameter(QLatin1String("param-screen-width")) < fetchLayoutParameter(QLatin1String("param-screen-height")))
            return (fetchLayoutParameter(QLatin1String("param-screen-width")) - 2 * fetchLayoutParameter(QLatin1String("param-widget-toolbar-height")) - 20) / 2; // TODO: fetch 4*5(margin) from layout
        else
            return (fetchLayoutParameter(QLatin1String("param-screen-width")) - 2 * fetchLayoutParameter(QLatin1String("param-widget-toolbar-height")) - 110) / 2; // TODO: fetch 22*5(margin) from layout
    }

    return QVariant();
}

QVariant SStyleWrapperPrivate::toolBarProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_toolbar"));

    if (propertyName == QLatin1String("toolbar-height"))
        return fetchLayoutParameter(QLatin1String("param-widget-toolbar-height"));

    if (propertyName == QLatin1String("toolbar-width"))
        return fetchLayoutParameter(QLatin1String("param-screen-width"));

    if (propertyName == QLatin1String("padding-large"))
        return fetchLayoutParameter(QLatin1String("param-padding-large"));

    if (propertyName == QLatin1String("border-margin")) {
        if (fetchLayoutParameter(QLatin1String("param-screen-width")) > fetchLayoutParameter(QLatin1String("param-screen-height")))
            return fetchLayoutParameter(QLatin1String("param-padding-large"));
        else
            return 0;
    }
    return QVariant();     
}

QVariant SStyleWrapperPrivate::busyIndicatorProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("source"))
        return imagePath(QLatin1String("qtg_graf_busyindicator_"));
    if (propertyName == QLatin1String("frameCount"))
        return 10;
    if (propertyName == QLatin1String("duration"))
        return 1000;

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
        return imagePath(QLatin1String("qtg_fr_pushbutton_pressed"));
    else if (mode == QLatin1String("checked"))
        return imagePath(QLatin1String("qtg_fr_pushbutton_latched"));
    else
        return imagePath(QLatin1String("qtg_fr_pushbutton_normal"));
}

QVariant SStyleWrapperPrivate::buttonPropertyTextColor() const
{
    if (mode == QLatin1String("pressed"))
        return fetchThemeColor(QLatin1String("qtc_button_pressed"));
    else if (mode == QLatin1String("checked"))
        return fetchThemeColor(QLatin1String("qtc_button_latched"));
    else if (mode == QLatin1String("default"))
        return fetchThemeColor(QLatin1String("qtc_button_normal"));
    else
        return QVariant();
}

QVariant SStyleWrapperPrivate::textComponentBackground() const
{
    if (mode == QLatin1String("selected"))
        return imagePath(QLatin1String("qtg_fr_textfield_highlighted"));
    else if (mode == QLatin1String("default"))
        return imagePath(QLatin1String("qtg_fr_textfield_editable"));
    else
        return QVariant();
}

QVariant SStyleWrapperPrivate::tabButtonProperty(const QString &propertyName) const
{
    Q_UNUSED(propertyName)

    if (propertyName == QLatin1String("iconWidth"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("iconHeight"))
        return fetchLayoutParameter(QLatin1String("param-graphic-size-primary-small"));

    if (propertyName == QLatin1String("textColor"))
        return QColor(Qt::black); // TODO: no color group defined yet

    if (propertyName == QLatin1String("background")) {
        if (mode == QLatin1String("checked"))
            return imagePath(QLatin1String("qtg_fr_tab_active"));
        else if (mode == QLatin1String("pressed"))
            return imagePath(QLatin1String("qtg_fr_tab_passive_pressed"));
        else
            return imagePath(QLatin1String("qtg_fr_tab_passive_normal"));
    }

    return QVariant();
}


QVariant SStyleWrapperPrivate::tabBarProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_tab_bar"));

    return QVariant();
}

QVariant SStyleWrapperPrivate::statusBarProperty(const QString &propertyName) const
{
    if (propertyName == QLatin1String("background"))
        return imagePath(QLatin1String("qtg_fr_statusbar.svg"));
    if (propertyName == QLatin1String("batteryFull"))
        return imagePath(QLatin1String("qtg_graf_battery_level_full.svg"));
    if (propertyName == QLatin1String("batteryMedium"))
        return imagePath(QLatin1String("qtg_graf_battery_level_medium.svg"));
    if (propertyName == QLatin1String("batteryLow"))
        return imagePath(QLatin1String("qtg_graf_battery_level_low.svg"));
    if (propertyName == QLatin1String("signalGprs"))
        return imagePath(QLatin1String("qtg_graf_signal_gprs_att_icon.svg"));
    if (propertyName == QLatin1String("signalGeneric"))
        return imagePath(QLatin1String("qtg_graf_signal_icon.svg"));
    if (propertyName == QLatin1String("signalBackground"))
        return imagePath(QLatin1String("qtg_graf_signal_level_bg.svg"));
    if (propertyName == QLatin1String("signalFull"))
        return imagePath(QLatin1String("qtg_graf_signal_level_full.svg"));
    if (propertyName == QLatin1String("batteryBackground"))
        return imagePath(QLatin1String("qtg_graf_battery_level_bg.svg"));
    if (propertyName == QLatin1String("height"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-height"));
    if (propertyName == QLatin1String("signalHeight"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-signal-height"));
    if (propertyName == QLatin1String("signalWidth"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-signal-width"));
    if (propertyName == QLatin1String("signalLevelWidth"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-signallevel-width"));
    if (propertyName == QLatin1String("signalLevelHeight"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-signallevel-height"));
    if (propertyName == QLatin1String("batteryLevelWidth"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-batterylevel-width"));
    if (propertyName == QLatin1String("batteryLevelHeight"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-batterylevel-height"));
    if (propertyName == QLatin1String("timeWidth"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-time-width"));
    if (propertyName == QLatin1String("timeHeight"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-time-height"));
    if (propertyName == QLatin1String("rowMargins"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-row-margins"));
    if (propertyName == QLatin1String("rowSpacing"))
        return fetchLayoutParameter(QLatin1String("param-statusbar-row-spacing"));

    return QVariant();
}

QString SStyleWrapperPrivate::imagePath(const QString &fileName) const
{
    return QLatin1String("image://theme/") + fileName;
}
