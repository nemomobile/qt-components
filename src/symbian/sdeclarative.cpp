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
#include <QCoreApplication>
#include <QLocale>
#include <QTime>
#include <QTimer>
#if defined(Q_OS_WIN)
#include <qt_windows.h>
#elif defined(Q_OS_SYMBIAN)
#include <e32std.h>
#endif

//#define Q_DEBUG_SDECLARATIVE

static const int MINUTE_MS = 60*1000;

class SDeclarativePrivate
{
public:
    SDeclarativePrivate() :
        mListInteractionMode(SDeclarative::TouchInteraction) {}

    void updateTimeFormat();

    SDeclarative::InteractionMode mListInteractionMode;
    QTimer timer;
    static QString timeFormat;
};

QString SDeclarativePrivate::timeFormat;

void SDeclarativePrivate::updateTimeFormat()
{
    QString timeSeparator;
#if defined(Q_OS_WIN)
    wchar_t data[10];
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME, data, 10);
    timeSeparator = QString::fromWCharArray(data);
#elif defined(Q_OS_SYMBIAN)
    TExtendedLocale loc;
    loc.LoadSystemSettings();
    // From TLocale 'S0 XX S1 YY S2 ZZ S3', we need S1 separator
    timeSeparator = QChar(loc.GetLocale()->TimeSeparator(1));
#else
    timeSeparator = QLatin1Char(':');
#endif
    timeFormat = QString("h%1mm").arg(timeSeparator);
}

SDeclarative::SDeclarative(QObject *parent) :
    QObject(parent),
    d_ptr(new SDeclarativePrivate)
{
    d_ptr->updateTimeFormat();
    d_ptr->timer.start(MINUTE_MS);
    connect(&d_ptr->timer, SIGNAL(timeout()), this, SIGNAL(currentTimeChanged()));
    QCoreApplication::instance()->installEventFilter(this);
}

SDeclarative::~SDeclarative()
{
    d_ptr->timer.stop();
}

SDeclarative::InteractionMode SDeclarative::listInteractionMode() const
{
    return d_ptr->mListInteractionMode;
}

void SDeclarative::setListInteractionMode(SDeclarative::InteractionMode mode)
{
    if (d_ptr->mListInteractionMode != mode) {
        d_ptr->mListInteractionMode = mode;
        emit listInteractionModeChanged();
    }
}

QString SDeclarative::currentTime()
{
    return QLocale().toString(QTime::currentTime(), SDeclarativePrivate::timeFormat);
}

bool SDeclarative::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == QCoreApplication::instance()) {
        if (event->type() == QEvent::ApplicationActivate) {
            d_ptr->updateTimeFormat();
            emit currentTimeChanged();
            d_ptr->timer.start(MINUTE_MS);
        } else if (event->type() == QEvent::ApplicationDeactivate) {
            d_ptr->timer.stop();
        } else if (event->type() == QEvent::LocaleChange) {
            d_ptr->updateTimeFormat();
        }
    }
    return QObject::eventFilter(obj, event);
}
