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

#ifndef SDATETIME_H
#define SDATETIME_H

#include <QtCore/QObject>
#include <QtDeclarative/qdeclarativeitem.h>

class SDateTime : public QObject
{
    Q_OBJECT
    Q_ENUMS(TimeUnit HourMode)

public:
    explicit SDateTime(QObject *parent = 0);
    virtual ~SDateTime();

    Q_INVOKABLE static QString longMonthName(int month);
    Q_INVOKABLE static bool isLeapYear(int year);
    Q_INVOKABLE static int daysInMonth(int year, int month);
    Q_INVOKABLE static QString amText();
    Q_INVOKABLE static QString pmText();
    Q_INVOKABLE static int hourMode();

    enum TimeUnit {
        Hours = 1,
        Minutes = 2,
        Seconds = 4,
        All = 7
    };

    enum HourMode {
        TwelveHours = 1,
        TwentyFourHours = 2
    };

private:
    Q_DISABLE_COPY(SDateTime)
};

QML_DECLARE_TYPE(SDateTime)
#endif // SDATETIME_H
