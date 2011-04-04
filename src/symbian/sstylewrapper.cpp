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
#include "sdeclarative.h"

#include <QEvent>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>

#include <QDebug>

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#endif

SStyleWrapper::SStyleWrapper(QObject *parent)
    : QObject(parent),
      d_ptr(new SStyleWrapperPrivate(this))
{
}

SStyleWrapper::~SStyleWrapper()
{
    Q_D(SStyleWrapper);
    d->invalidateStyle();
}

QString SStyleWrapper::mode() const
{
    Q_D(const SStyleWrapper);
    return d->mode;
}

void SStyleWrapper::setMode(const QString &mode)
{
    Q_D(SStyleWrapper);
    if (mode == d->mode)
        return;
    d->mode = mode;

    emit currentStyleChanged();
    emit modeChanged();
}

QString SStyleWrapper::styleClass() const
{
    Q_D(const SStyleWrapper);
    return d->styleClass;
}

void SStyleWrapper::setStyleClass(const QString &styleClass)
{
    Q_D(SStyleWrapper);

    static QSet<QString> deprecationMessages;
    if (!deprecationMessages.contains(styleClass)) {
        deprecationMessages.insert(styleClass);
        qWarning() << QString(QLatin1String("warning: Style ('%1') is deprecated. Use platformStyle and privateStyle context properties instead")).arg(styleClass);
    }

    if (styleClass == d->styleClass)
        return;
    d->styleClass = styleClass;

    d->updateStyle();
    emit styleClassChanged();
}

QString SStyleWrapper::styleType() const
{
    Q_D(const SStyleWrapper);
    return d->styleType;
}

void SStyleWrapper::setStyleType(const QString &styleType)
{
    Q_D(SStyleWrapper);
    if (styleType == d->styleType)
        return;
    d->styleType = styleType;

    d->updateStyle();
    emit styleTypeChanged();
}

QString SStyleWrapper::styleObjectName() const
{
    Q_D(const SStyleWrapper);
    return d->styleObjectName;
}

void SStyleWrapper::setStyleObjectName(const QString &styleObjectName)
{
    Q_D(SStyleWrapper);
    if (styleObjectName == d->styleObjectName)
        return;
    d->styleObjectName = styleObjectName;

    d->updateStyle();
    emit styleObjectNameChanged();
}

QObject *SStyleWrapper::currentStyleAsObject() const
{
    return const_cast<SStyleWrapper *>(this);
}

qreal SStyleWrapper::preferredWidth() const
{
    Q_D(const SStyleWrapper);
    if (styleClass() == QLatin1String("Button"))
        return d->fetchLayoutParameter(QLatin1String("param-widget-chrome-height"));

    if (styleClass() == QLatin1String("ListHeading"))
        return d->fetchLayoutParameter(QLatin1String("param-screen-width"));

    // listItem width should be really controlled by the list user, but it defaults to screen width
    if (styleClass() == QLatin1String("ListItem"))
        return d->fetchLayoutParameter(QLatin1String("param-screen-width"));

    if (styleClass() == QLatin1String("ListItemText"))
        return d->fetchLayoutParameter(QLatin1String("param-screen-width"));

    if (styleClass() == QLatin1String("ListItemImage")) {

        const int imageSize = property("imageSize").toInt();

        switch (imageSize) {
        case SDeclarative::Small:
            return d->listItemImageProperty(QLatin1String("iconSmallWidth")).toReal();
        case SDeclarative::Medium:
            return d->listItemImageProperty(QLatin1String("iconMediumWidth")).toReal();
        case SDeclarative::Large:
            return d->listItemImageProperty(QLatin1String("iconLargeWidth")).toReal();
        case SDeclarative::ImagePortrait:
            return d->listItemImageProperty(QLatin1String("imageWidth")).toReal();
        case SDeclarative::Undefined:
        default:
            return 0;
        }
    }
    if (styleClass() == QLatin1String("Dialog"))
        return qMin(d->dialogProperty(QLatin1String("appRectWidth")).toReal(),
            d->dialogProperty(QLatin1String("appRectHeight")).toReal())
            - 4 * d->fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));

    if (styleClass() == QLatin1String("Slider")) {
        if (d->mode == QLatin1String("horizontal"))
            return 150; // TODO: get from layout
        else
            return d->fetchLayoutParameter(QLatin1String("param-widget-slider-thumb-width"));
    }

    if (styleClass() == QLatin1String("ProgressBar"))
        return 150; // TODO: get from layout

    if (styleClass() == QLatin1String("ContextMenu")) {
        qreal width = d->fetchLayoutParameter(QLatin1String("param-screen-width"));
        qreal height = d->fetchLayoutParameter(QLatin1String("param-screen-height"));
        if (width > height) {
            return 2 * d->faderProperty(QLatin1String("appRectWidth")).toReal() / 3;
        }
        else {
            return d->faderProperty(QLatin1String("appRectWidth")).toReal()
                    - 4 * d->fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));
        }
    }

    if (styleClass() == QLatin1String("Menu"))
        return d->fetchLayoutParameter(QLatin1String("param-screen-width"));

    if (styleClass() == QLatin1String("TabBar"))
        return d->fetchLayoutParameter(QLatin1String("param-screen-width"));

    if (styleClass() == QLatin1String("StatusBar"))
        return d->fetchLayoutParameter(QLatin1String("param-screen-width"));

    if (styleClass() == QLatin1String("ToolBar"))
        return d->fetchLayoutParameter(QLatin1String("param-screen-width"));

    if (styleClass() == QLatin1String("BusyIndicator"))
        return 40;

    return 50;
}

qreal SStyleWrapper::preferredHeight() const
{
    Q_D(const SStyleWrapper);

    if (styleClass() == QLatin1String("Button") && styleObjectName() == QLatin1String("TabButton"))
        return 46; // TODO: get from layout. tab-bar-height: nHD 46

    if (styleClass() == QLatin1String("Button"))
        return qreal(2 / 3 * d->fetchLayoutParameter(QLatin1String("param-widget-chrome-height")));

    if (styleClass() == QLatin1String("ListHeading"))
        // Conversion to whole number to avoid sub-pixel alignment of items
        return qRound(d->listHeadingProperty(QLatin1String("marginTop")).toReal()
            + d->fetchLayoutParameter(QLatin1String("param-text-height-secondary"))
            + d->listHeadingProperty(QLatin1String("marginBottom")).toReal());

    if (styleClass() == QLatin1String("ListItem"))
        // Conversion to whole number to avoid sub-pixel alignment of items
        return qRound(d->listItemProperty(QLatin1String("marginTop")).toReal()
            + d->fetchLayoutParameter(QLatin1String("param-text-height-primary"))
            + d->listItemProperty(QLatin1String("verticalSpacing")).toReal()
            + d->fetchLayoutParameter(QLatin1String("param-text-height-secondary"))
            + d->listItemProperty(QLatin1String("marginBottom")).toReal());

    if (styleClass() == QLatin1String("ListItemText")) {
        // height for ListItem specified supported part
        if (styleObjectName() == QLatin1String("Title"))
            return d->fetchLayoutParameter(QLatin1String("param-text-height-primary"));
        if (styleObjectName() == QLatin1String("SubTitle"))
            return d->fetchLayoutParameter(QLatin1String("param-text-height-secondary"));
        if (styleObjectName() == QLatin1String("Heading"))
            return d->fetchLayoutParameter(QLatin1String("param-text-height-title"));
    }

    if (styleClass() == QLatin1String("ListItemImage")) {

        const int imageSize = property("imageSize").toInt();

        switch (imageSize) {
        case SDeclarative::Small:
            return d->listItemImageProperty(QLatin1String("iconSmallHeight")).toReal();
        case SDeclarative::Medium:
            return d->listItemImageProperty(QLatin1String("iconMediumHeight")).toReal();
        case SDeclarative::Large:
            return d->listItemImageProperty(QLatin1String("iconLargeHeight")).toReal();
        case SDeclarative::ImagePortrait:
            return d->listItemImageProperty(QLatin1String("imageHeight")).toReal();
        case SDeclarative::Undefined:
        default:
            return 0;
        }
    }
    if (styleClass() == QLatin1String("Dialog"))
        return preferredWidth();

    if (styleClass() == QLatin1String("Slider")) {
        if (d->mode == QLatin1String("horizontal"))
            return d->fetchLayoutParameter(QLatin1String("param-widget-slider-thumb-width"));
        else
            return 150; // TODO: get from layout
    }

    if (styleClass() == QLatin1String("ProgressBar"))
        return d->fetchLayoutParameter(QLatin1String("param-widget-progress-bar-height"));

    if (styleClass() == QLatin1String("TabBar"))
        return 46; // TODO: get from layout. tab-bar-height: nHD 46

    if (styleClass() == QLatin1String("MenuContent")) {
        return d->faderProperty(QLatin1String("appRectHeight")).toReal()
            - 4 * d->fetchLayoutParameter(QLatin1String("param-margin-gene-screen"));
    }
    if (styleClass() == QLatin1String("ToolBar"))
        return d->fetchLayoutParameter(QLatin1String("param-widget-toolbar-height"));

    if (styleClass() == QLatin1String("BusyIndicator"))
        return 40;

    if (styleClass() == QLatin1String("StatusBar"))
        return d->fetchLayoutParameter(QLatin1String("param-statusbar-height"));

    return 50;
}

// ### This property getter is a workaround to the following situation:
//
// - QML warns when when we bind to properties that doesn't have either a
//   NOTIFY signal or a CONSTANT mark.
//
// - libmeegotouch MStyle classes have properties with the data we want, but do not
//   have NOTIFY signal (they don't need since they write only once), neither
//   CONSTANT mark (because they have WRITE, which they use to dynamically fill the
//   properties from a set of CSS files).
//
// - Since MStyle is a QObject we do want to export it to QML instead of having to
//   create wrapper for each proper as we did before.
//
// This workaround avoids the warning by accessing the property in C++ (via the get()
// method). But we still need some signal to notify the properties that use data from
// get() that something change. The trick is to make the SStyleWrapper have a property
// that points to itself, and NOTIFY change when the style changes (what we want).
QVariant SStyleWrapper::get(const QString &propertyName)
{
    Q_D(const SStyleWrapper);
    if (propertyName.isEmpty())
        return QVariant();

    QVariant ret;
    // properties for each component
    if (styleClass() == QLatin1String("Button"))
        ret = d->buttonProperty(propertyName);
    else if (styleClass() == QLatin1String("TextArea"))
        ret = d->textComponentProperty(propertyName);
    else if (styleClass() == QLatin1String("TextField"))
        ret = d->textComponentProperty(propertyName);
    else if (styleClass() == QLatin1String("TitleBar"))
        ret = d->titleBarProperty(propertyName);
    else if (styleClass() == QLatin1String("ListHeading"))
        ret = d->listHeadingProperty(propertyName);
    else if (styleClass() == QLatin1String("ListItem"))
        ret = d->listItemProperty(propertyName);
    else if (styleClass() == QLatin1String("ListItemText"))
        ret = d->listItemTextProperty(propertyName);
    else if (styleClass() == QLatin1String("CheckBox"))
        ret = d->checkBoxProperty(propertyName);
    else if (styleClass() == QLatin1String("Dialog"))
        ret = d->dialogProperty(propertyName);
    else if (styleClass() == QLatin1String("ScrollBar"))
        ret = d->scrollBarProperty(propertyName);
    else if (styleClass() == QLatin1String("ProgressBar"))
        ret = d->progressBarProperty(propertyName);
    else if (styleClass() == QLatin1String("ListItemImage"))
        ret = d->listItemImageProperty(propertyName);
    else if (styleClass() == QLatin1String("RadioButton"))
        ret = d->radioButtonProperty(propertyName);
    else if (styleClass() == QLatin1String("Slider"))
        ret = d->sliderProperty(propertyName);
    else if (styleClass() == QLatin1String("ToolTip"))
        ret = d->toolTipProperty(propertyName);
    else if (styleClass() == QLatin1String("MenuContent"))
        ret = d->menuContentProperty(propertyName);
    else if (styleClass() == QLatin1String("Menu"))
        ret = d->menuProperty(propertyName);
    else if (styleClass() == QLatin1String("ChoiceList"))
        ret = d->choiceListProperty(propertyName);
    else if (styleClass() == QLatin1String("Fader"))
        ret = d->faderProperty(propertyName);
    else if (styleClass() == QLatin1String("TabBar"))
        ret = d->tabBarProperty(propertyName);
    else if (styleClass() == QLatin1String("ToolButton"))
        ret = d->toolButtonProperty(propertyName);
    else if (styleClass() == QLatin1String("SectionScroller"))
        ret = d->sectionScrollerProperty(propertyName);
    else if (styleClass() == QLatin1String("ToolBar"))
        ret = d->toolBarProperty(propertyName);
    else if (styleClass() == QLatin1String("BusyIndicator"))
        ret = d->busyIndicatorProperty(propertyName);
    else if (styleClass() == QLatin1String("StatusBar"))
        ret = d->statusBarProperty(propertyName);

    // this is for querying something else than component specific values...
    /*if(ret.isNull())
        ret = d->fetchLayoutParameter(propertyName);
    */

    return ret;
}

bool SStyleWrapper::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    if (event->type() == QEvent::Resize) {
        QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
        if (resizeEvent->oldSize().isValid()) {
            QMetaObject::invokeMethod(this, "_q_desktopWorkareaChanged", Qt::QueuedConnection);
        }
    }
    return false;
}

void SStyleWrapper::play(int effect)
{
    Q_D(SStyleWrapper);
    d->play(effect);
}

qreal SStyleWrapper::textWidth(const QString &text, const QFont &font) const
{
    QFontMetricsF metrics(font);
    return metrics.width(text);
}

qreal SStyleWrapper::fontHeight(const QFont &font) const
{
    QFontMetricsF metrics(font);
    return metrics.height();
}

#include "moc_sstylewrapper.cpp"
