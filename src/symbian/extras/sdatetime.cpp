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

#include "sdatetime.h"
#include <QDate>
#include <QLocale>
#if defined(Q_OS_WIN)
#include <qt_windows.h>
#elif defined(Q_OS_SYMBIAN)
#include <e32std.h>
#endif

SDateTime::SDateTime(QObject *parent) : QObject(parent)
{
}

SDateTime::~SDateTime()
{
}

QString SDateTime::longMonthName(int month)
{
    return QDate::longMonthName(month);
}

bool SDateTime::isLeapYear(int year)
{
    return QDate::isLeapYear(year);
}

int SDateTime::daysInMonth(int year, int month)
{
    return QDate(year, month, 1).daysInMonth();
}

QString SDateTime::amText()
{
    return QLocale().amText();
}

QString SDateTime::pmText()
{
    return QLocale().pmText();
}

int SDateTime::hourMode()
{
    bool format12h = false;
#if defined(Q_OS_WIN)
    wchar_t data[10];
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, data, 10);
    format12h = QString::fromWCharArray(data).startsWith(QLatin1Char('h'));
#elif defined(Q_OS_SYMBIAN)
    TExtendedLocale loc;
    loc.LoadSystemSettings();
    format12h = loc.GetLocale()->TimeFormat() == ETime12;
#endif
    return format12h ? TwelveHours : TwentyFourHours;
}

#include "moc_sdatetime.cpp"
