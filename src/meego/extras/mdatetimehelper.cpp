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

#include "mdatetimehelper.h"
#include <QDate>
#include <QLocale>
#if defined(Q_OS_WIN)
#include <qt_windows.h>
#elif defined(HAVE_MEEGOTOUCH)
#include <MLocale>
#endif

MDateTimeHelper::MDateTimeHelper(QObject *parent) : QObject(parent) 
{
}

MDateTimeHelper::~MDateTimeHelper() 
{
}

QString MDateTimeHelper::shortMonthName(int month) 
{
    return QDate::shortMonthName(month);
}

bool MDateTimeHelper::isLeapYear(int year) 
{
    return QDate::isLeapYear(year);
}

int MDateTimeHelper::daysInMonth(int year, int month) 
{
    return QDate(year, month, 1).daysInMonth();
}

int MDateTimeHelper::currentYear() 
{
    return QDate::currentDate().year();
}

QString MDateTimeHelper::amText()
{
    return QLocale().amText();
}

QString MDateTimeHelper::pmText()
{
    return QLocale().pmText();
}

int MDateTimeHelper::hourMode()
{
    bool format12h = false;
#if defined(Q_OS_WIN)
    wchar_t data[10];
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, data, 10);
    format12h = QString::fromWCharArray(data).startsWith(QLatin1Char('h'));
#elif defined(HAVE_MEEGOTOUCH)
    format12h = (MLocale().timeFormat24h() == MLocale::TwelveHourTimeFormat24h ? true :
                (MLocale().timeFormat24h() == MLocale::TwentyFourHourTimeFormat24h ? false :
                MLocale().defaultTimeFormat24h() == MLocale::TwelveHourTimeFormat24h));
#endif
    return format12h ? TwelveHours : TwentyFourHours;
}
#include "moc_mdatetimehelper.cpp"
