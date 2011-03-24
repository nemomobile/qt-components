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

#include "sstyleengine.h"
#include "sdeclarative.h"
#include "sdeclarativescreen.h"

#include <QObject>
#include <QFile>
#include <qmath.h>

// Parameters for magic unit value (RnD feature)
// -> See loadParameters documentation for details
static const qreal MID_POINT = 3.5;
static const qreal MID_VALUE = 3.2;
static const qreal RANGE = 1.0;
static const qreal ATAN_FACTOR = 1.5;

class SStyleEnginePrivate
{
    Q_DECLARE_PUBLIC(SStyleEngine)
public:
    SStyleEnginePrivate() {}

    enum ParameterType {
        ParameterType_Integer,
        ParameterType_Unit,
        ParameterType_Color
    };

    void init();
    bool updateLayoutParameters();
    void loadParameters(const QString &filePath, ParameterType type);
    void resolveFont();    
    void _q_displayChanged();

public:
    SStyleEngine *q_ptr;
    SDeclarativeScreen *screen;

    QHash<QString, int> layoutParameters;
    QHash<QString, QColor> colorParameters;
    QHash<QString, QString> fontFamilyParameters;
    QString displayConfig;
};

void SStyleEnginePrivate::init()
{
    updateLayoutParameters();
    loadParameters(QLatin1String(":/params/colors/default.params"), ParameterType_Color);
    resolveFont();
}

bool SStyleEnginePrivate::updateLayoutParameters()
{
    QString longEdge = QString::number(qMax(screen->displayWidth(), screen->displayHeight()));
    QString shortEdge = QString::number(qMin(screen->displayWidth(), screen->displayHeight()));
    QString ppi = QString::number(qRound(screen->dpi() / 5.0) * 5); // round to closest 5
    QString newDisplayConfig = longEdge + QLatin1Char('_') + shortEdge + QLatin1Char('_') + ppi;
    
    if (displayConfig != newDisplayConfig) {
        layoutParameters.clear();
        QString layoutFile = QLatin1String(":/params/layouts/") + newDisplayConfig + QLatin1String(".params");
        if (QFile::exists(layoutFile))
            loadParameters(layoutFile, ParameterType_Integer);
        else
            loadParameters(QLatin1String(":/params/layouts/fallback.params"), ParameterType_Unit);
        displayConfig = newDisplayConfig;
        return true;
    }
    return false;
}

void SStyleEnginePrivate::loadParameters(const QString &filePath, ParameterType type)
{
    qreal unit(0.0);
    if (type == ParameterType_Unit) {
        // Magic unit formula
        //
        // This is an RnD feature that makes it possible to scale the
        // components to any arbitrary screen resolution/dpi. There should
        // be device specific parameter files for all real device configuration
        // and this function should not be hit from real hardware.
        //
        // The arctan function calculates the "primary text height" in
        // millimeters and all the parameters are defined in "units"
        // that's a fourth of the "primary text height"
        //
        // The magic unit formula can be controlled with four parameters:
        // - MID_POINT: the inch size of the reference display
        // - MID_VALUE: "primary text height" in millimeters at MID_POINT
        // - RANGE: the range of "primary text height" in millimeters
        // - ATAN_FACTOR: controls the "steepness" of the arctan curve
        qreal inchSize = qSqrt(screen->height() * screen->height()
            + screen->width() * screen->width()) / screen->dpi();
        qreal pthMm = MID_VALUE + RANGE * qAtan(ATAN_FACTOR * (inchSize - MID_POINT)) / M_PI;
        unit = 0.25 * pthMm * screen->dpi() / 25.4;
    }

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith(QLatin1Char('#')))
                continue;

            int colonId = line.indexOf(QLatin1Char(':'));
            if (colonId < 0)
                return;

            QVariant value;
            QString valueStr = line.mid(colonId + 1).trimmed();
            if (type == ParameterType_Color) {
                // do implicit string-to-color conversion
                QColor color(valueStr);
                colorParameters.insert(line.left(colonId).trimmed(), color);
            } else {
                int intVal(-1);
                bool ok(false);
                if (type == ParameterType_Unit && valueStr.endsWith(QLatin1String("un"))) {
                    valueStr.chop(2);
                    qreal temp = valueStr.toFloat(&ok);
                    if (ok)
                        intVal = qRound(unit * temp);
                } else {
                    int temp = valueStr.toInt(&ok);
                    if (ok)
                        intVal = temp;
                }
                layoutParameters.insert(line.left(colonId).trimmed(), intVal);
            }
        }
        file.close();
    }
}

void SStyleEnginePrivate::resolveFont()
{
#ifdef Q_OS_SYMBIAN
    QString fontFamily = QFont().defaultFamily();
#else
    QString fontFamily = QLatin1String("Nokia Sans");
#endif
    fontFamilyParameters.insert(QLatin1String("font-family-regular"), fontFamily);
}

void SStyleEnginePrivate::_q_displayChanged() 
{
    Q_Q(SStyleEngine);
    if (updateLayoutParameters())
        emit q->layoutParametersChanged();
}

SStyleEngine::SStyleEngine(SDeclarativeScreen *screen, QObject *parent)
    : QObject(parent),
      d_ptr(new SStyleEnginePrivate())
{
    Q_D(SStyleEngine);
    d->q_ptr = this;
    d->screen = screen;
    // Screen size can change on desktop (RnD feature)
    QObject::connect(screen, SIGNAL(displayChanged()), this, SLOT(_q_displayChanged()));
    d->init();
}

SStyleEngine::~SStyleEngine()
{
}

int SStyleEngine::layoutParameter(const QString &parameter) const
{
    Q_D(const SStyleEngine);
    // default to invalid value (-1)
    return d->layoutParameters.value(parameter, -1);
}

QColor SStyleEngine::colorParameter(const QString &parameter) const
{
    Q_D(const SStyleEngine);
    return d->colorParameters.value(parameter);
}

QString SStyleEngine::fontFamilyParameter(const QString &parameter) const
{
    Q_D(const SStyleEngine);
    return d->fontFamilyParameters.value(parameter);
}

#include "moc_sstyleengine.cpp"
