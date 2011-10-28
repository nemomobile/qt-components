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

#ifndef MLOCALEWRAPPER_H
#define MLOCALEWRAPPER_H

#ifdef HAVE_MEEGOTOUCH
#include <MLocale>
#include <QStringList>
#endif

#include <QDebug>
#include <QDateTime>
#include <QStringList>

/*
* This class is a wrapper around MLocale
*
* It exports the most common localization methods, so that
* they can used in meegotouch's Qt-Components implementation
* For information about specific methods, check the MLocale docs
*
* Api changes:
*   - Collation is not supported yet (i.e. setCollation(), collation(), collator())
*   - indexBucket() is not supported yet
*   - connectSettings() and disconnectSettings is not supported
*   - textDirection() and defaultLayoutDirection() are not supported
*   - localeScript() is not supported
*   - only a formatNumber() for integer is supported
*   - when a formatNumber for float/double values is needed, use formatDouble()
*   - at moment, the ok feedback for the toNumber methods is not supported
*   - functions that asks for a MCalender in their parameter list are not supported
*   - settingsChanged() signal is not supported yet
*
*/

class MLocaleWrapper : public QObject {
    Q_OBJECT

    Q_ENUMS(DateType)
    Q_ENUMS(TimeType)
    Q_ENUMS(Category)
    Q_ENUMS(Collation)
    Q_ENUMS(CalendarType)
    Q_ENUMS(TimeFormat24h)
    Q_ENUMS(Weekday)

public:
    MLocaleWrapper(QObject *parent=0) : QObject(parent)
#ifdef HAVE_MEEGOTOUCH
      , ml(new MLocale)
#endif
    {

    }

    ~MLocaleWrapper()
    {
#ifdef HAVE_MEEGOTOUCH
        if(ml) {
            delete ml;
            ml = 0;
        }
#endif
    }


    enum DateType {
        DateNone,
        DateShort,
        DateMedium,
        DateLong,
        DateFull,
        DateYearAndMonth
    };


    enum TimeType {
        TimeNone,
        TimeShort,
        TimeMedium,
        TimeLong,
        TimeFull
    };

    enum Category {MLcMessages, MLcTime, MLcCollate,
                   MLcNumeric, MLcMonetary, MLcName,
                   MLcTelephone
                  };


    enum Collation {DefaultCollation, PhonebookCollation, PinyinCollation,
                    TraditionalCollation, StrokeCollation, DirectCollation,
                    PosixCollation, Big5hanCollation, Gb2312hanCollation
                   };


    enum CalendarType {DefaultCalendar, GregorianCalendar, IslamicCalendar,
                       ChineseCalendar, IslamicCivilCalendar, HebrewCalendar,
                       JapaneseCalendar, BuddhistCalendar, PersianCalendar,
                       CopticCalendar, EthiopicCalendar
                      };


    enum TimeFormat24h {LocaleDefaultTimeFormat24h,
                        TwelveHourTimeFormat24h,
                        TwentyFourHourTimeFormat24h};

    enum Weekday {Monday = 1, Tuesday, Wednesday, Thursday, Friday,
                  Saturday, Sunday
                 };


    enum Comparison {LessThan = -1, Equal = 0, GreaterThan = 1};


    enum DateSymbolContext {DateSymbolFormat, DateSymbolStandalone};


    enum DateSymbolLength {DateSymbolAbbreviated, DateSymbolWide, DateSymbolNarrow};


    enum PhoneNumberGrouping {
        DefaultPhoneNumberGrouping,
        NoPhoneNumberGrouping,
        NorthAmericanPhoneNumberGrouping
    };

    // without localization support, an error message is thrown whenever Qml tries to access a method from MLocale
    // this is what we want
#ifdef HAVE_MEEGOTOUCH


    Q_INVOKABLE void setLocale(QString localeName)
    {
        if (ml)
            delete ml;
        ml = new MLocale(localeName);

        emit isValidChanged();
        emit calendarTypeChanged();
        emit timeFormat24hChanged();
        emit defaultTimeFormat24hChanged();
        emit languageEndonymChanged();
        emit countryEndonymChanged();
        emit decimalPointChanged();
        emit localeScriptsChanged();
        emit exemplarCharactersIndexChanged();
        emit languageChanged();
        emit countryChanged();
        emit scriptChanged();
        emit variantChanged();
        emit nameChanged();
        emit dataPathsChanged();
        emit translationPathsChanged();

    }

    // properties
    Q_PROPERTY(bool valid READ isValid NOTIFY isValidChanged FINAL)
    Q_PROPERTY(int calendarType READ calendarType WRITE setCalendarType NOTIFY calendarTypeChanged)
    Q_PROPERTY(int timeFormat24h READ timeFormat24h WRITE setTimeFormat24h NOTIFY timeFormat24hChanged)
    Q_PROPERTY(int defaultTimeFormat24h READ defaultTimeFormat24h NOTIFY defaultTimeFormat24hChanged FINAL)
    Q_PROPERTY(QString languageEndonym READ languageEndonym NOTIFY languageEndonymChanged FINAL)
    Q_PROPERTY(QString countryEndonym READ countryEndonym NOTIFY countryEndonymChanged FINAL)
    Q_PROPERTY(QString decimalPoint READ decimalPoint NOTIFY decimalPointChanged FINAL)
    Q_PROPERTY(QStringList localeScripts READ localeScripts NOTIFY localeScriptsChanged FINAL)
    Q_PROPERTY(QStringList exemplarCharactersIndex READ exemplarCharactersIndex NOTIFY exemplarCharactersIndexChanged FINAL)
    Q_PROPERTY(QString language READ language NOTIFY languageChanged FINAL)
    Q_PROPERTY(QString country READ country NOTIFY countryChanged FINAL)
    Q_PROPERTY(QString script READ script NOTIFY scriptChanged FINAL)
    Q_PROPERTY(QString variant READ variant NOTIFY variantChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
    Q_PROPERTY(QStringList dataPaths READ dataPaths NOTIFY dataPathsChanged FINAL)
    Q_PROPERTY(QStringList translationPaths READ translationPaths WRITE setTranslationPaths NOTIFY translationPathsChanged)

    bool isValid() const                                                      {Q_ASSERT(ml); return ml->isValid();}

    //ToDo: Return enums (CalendarType, TimeFormat24h) instead of int; unfortunately this doesn'tcurrently work
    int calendarType() const                                                  {Q_ASSERT(ml); return (int) ml->calendarType();}
    int timeFormat24h() const                                                 {Q_ASSERT(ml); return (int) ml->timeFormat24h();}
    int defaultTimeFormat24h() const                                          {Q_ASSERT(ml); return (int) ml->defaultTimeFormat24h();}
    QString languageEndonym() const                                           {Q_ASSERT(ml); return ml->languageEndonym();}
    QString countryEndonym() const                                            {Q_ASSERT(ml); return ml->countryEndonym();}
    QString decimalPoint() const                                              {Q_ASSERT(ml); return ml->decimalPoint();}
    QStringList localeScripts() const                                         {Q_ASSERT(ml); return ml->localeScripts();}
    QStringList exemplarCharactersIndex() const                               {Q_ASSERT(ml); return ml->exemplarCharactersIndex();}
    QString language() const                                                  {Q_ASSERT(ml); return ml->language();}
    QString country() const                                                   {Q_ASSERT(ml); return ml->country();}
    QString script() const                                                    {Q_ASSERT(ml); return ml->script();}
    QString variant() const                                                   {Q_ASSERT(ml); return ml->variant();}
    QString name() const                                                      {Q_ASSERT(ml); return ml->name();}

    //ToDo: Use CalendarType instead of int
    void setCalendarType(int calendar)
    {
        Q_ASSERT(ml);
        if (calendar != ml->calendarType()) {
            ml->setCalendarType((MLocale::CalendarType) calendar);
            emit calendarTypeChanged();
        }
    }

    void setTimeFormat24h(int timeFormat24h)
    {
        Q_ASSERT(ml);
        if (timeFormat24h != ml->timeFormat24h()) {
            ml->setTimeFormat24h((MLocale::TimeFormat24h) timeFormat24h);
            emit timeFormat24hChanged();
        }
    }

    QStringList dataPaths()
    {
        const QStringList sl = MLocale::dataPaths();
#ifdef MLOCALEWRAPPER_DEBUG
        foreach(const QString &s, sl) {
            qDebug() << "dataPath: " << s;
        }
 #endif
        return sl;
    }

    QStringList translationPaths()
    {
        const QStringList sl =  MLocale::translationPaths();
#ifdef MLOCALEWRAPPER_DEBUG
        foreach(const QString &s, sl) {
            qDebug() << "translationPath: " << s;
        }
 #endif
        return sl;
    }

     void setTranslationPaths(const QStringList &translationPaths)
     {
         Q_ASSERT(ml);
         if (translationPaths != ml->translationPaths()) {
            MLocale::setTranslationPaths(translationPaths);
            emit translationPathsChanged();
         }
     }

    // invokables
    Q_INVOKABLE QString toLower(const QString &string) const                  {Q_ASSERT(ml); return ml->toLower(string);}
    Q_INVOKABLE QString toUpper(const QString &string) const                  {Q_ASSERT(ml); return ml->toUpper(string);}

    Q_INVOKABLE QString categoryLanguage(Category category) const             {Q_ASSERT(ml); return ml->categoryLanguage((MLocale::Category) category);}
    Q_INVOKABLE QString categoryCountry(Category category) const              {Q_ASSERT(ml); return ml->categoryCountry((MLocale::Category) category);}
    Q_INVOKABLE QString categoryScript(Category category) const               {Q_ASSERT(ml); return ml->categoryScript((MLocale::Category) category);}
    Q_INVOKABLE QString categoryVariant(Category category) const              {Q_ASSERT(ml); return ml->categoryVariant((MLocale::Category) category);}
    Q_INVOKABLE QString categoryName(Category category) const                 {Q_ASSERT(ml); return ml->categoryName((MLocale::Category) category);}

    Q_INVOKABLE QString formatNumber(int i) const                             {Q_ASSERT(ml); return ml->formatNumber(i);}
    Q_INVOKABLE qlonglong toInt(const QString &s) const                       {Q_ASSERT(ml); return ml->toInt(s);} //ToDo: evaluate ok feedback
    Q_INVOKABLE QString formatDouble(double i,
                                     int maxPrecision = -1) const             {Q_ASSERT(ml); return ml->formatNumber(i, maxPrecision);}
    Q_INVOKABLE QString formatDouble(double i, int maxPrecision,
                                               int minPrecision) const        {Q_ASSERT(ml); return ml->formatNumber(i, maxPrecision, minPrecision);}
    Q_INVOKABLE double toDouble(const QString &s) const                       {Q_ASSERT(ml); return ml->toDouble(s);} //ToDo: evaluate ok feedback
    Q_INVOKABLE QString formatPercent(double i) const                         {Q_ASSERT(ml); return ml->formatPercent(i);}
    Q_INVOKABLE QString formatCurrency(double amount,
                                       const QString &currency) const         {Q_ASSERT(ml); return ml->formatCurrency(amount, currency);}

    Q_INVOKABLE QString toLocalizedNumbers(const QString &text) const         {Q_ASSERT(ml); return ml->toLocalizedNumbers(text);}

    Q_INVOKABLE QString formatDateTime(const QDateTime &dateTime,
                                       DateType dateType = DateLong,
                                       TimeType timeType = TimeLong,
                                       CalendarType calendarType = DefaultCalendar) const
                                                                              {Q_ASSERT(ml); return ml->formatDateTime(dateTime,
                                                                                                                       (MLocale::DateType) dateType,
                                                                                                                       (MLocale::TimeType) timeType,
                                                                                                                       (MLocale::CalendarType) calendarType);}


    Q_INVOKABLE QString formatDateTime(const QDateTime &dateTime,
                                       CalendarType calendarType) const       {Q_ASSERT(ml); return ml->formatDateTime(dateTime,
                                                                                                                       (MLocale::CalendarType) calendarType);}
    Q_INVOKABLE QString formatDateTimeICU(const QDateTime &dateTime,
                                  const QString &formatString) const          {Q_ASSERT(ml); return ml->formatDateTimeICU(dateTime, formatString);}


    Q_INVOKABLE QString formatDateTime(const QDateTime &dateTime,
                               const QString &formatString) const             {Q_ASSERT(ml); return ml->formatDateTime(dateTime, formatString);}


    Q_INVOKABLE QString formatPhoneNumber( const QString& phoneNumber,
                               PhoneNumberGrouping grouping =
                               DefaultPhoneNumberGrouping ) const             {Q_ASSERT(ml); return ml->formatPhoneNumber(phoneNumber,
                                                                                                                          (MLocale::PhoneNumberGrouping) grouping);}
    Q_INVOKABLE QString icuFormatString( DateType dateType = DateLong,
                              TimeType timeType = TimeLong,
                              CalendarType calendarType = DefaultCalendar) const
                                                                              {Q_ASSERT(ml); return ml->icuFormatString((MLocale::DateType) dateType,
                                                                                                                        (MLocale::TimeType) timeType,
                                                                                                                        (MLocale::CalendarType) calendarType);}
    Q_INVOKABLE QDateTime parseDateTime(const QString &dateTime,
                            DateType dateType = DateLong,
                            TimeType timeType = TimeLong,
                            CalendarType calendarType = DefaultCalendar) const
                                                                              {Q_ASSERT(ml); return ml->parseDateTime(dateTime,
                                                                                                                      (MLocale::DateType) dateType,
                                                                                                                      (MLocale::TimeType) timeType,
                                                                                                                      (MLocale::CalendarType) calendarType);}

    Q_INVOKABLE QDateTime parseDateTime(const QString &dateTime,
                                        CalendarType calendarType) const      {Q_ASSERT(ml); return ml->parseDateTime(dateTime,
                                                                                                                      (MLocale::CalendarType) calendarType);}

    Q_INVOKABLE void installTrCatalog(const QString &name)                    {Q_ASSERT(ml); ml->installTrCatalog(name); MLocale::setDefault(*ml);}
    Q_INVOKABLE void removeTrCatalog(const QString &name)                     {Q_ASSERT(ml); ml->removeTrCatalog(name);  MLocale::setDefault(*ml);}
    Q_INVOKABLE bool isInstalledTrCatalog(const QString &name) const          {Q_ASSERT(ml); return ml->isInstalledTrCatalog(name);}


    //functions that are static in MLocale
    Q_INVOKABLE QString toLocalizedNumbers(const QString &text,
                                           const QString &targetDigits)       {return MLocale::toLocalizedNumbers(text, targetDigits);}

    Q_INVOKABLE QString toLatinNumbers(const QString &text)                   {return MLocale::toLatinNumbers(text);}

    Q_INVOKABLE void addTranslationPath(const QString &path)                  {MLocale::addTranslationPath(path); emit translationPathsChanged();}
    Q_INVOKABLE void removeTranslationPath(const QString &path)               {MLocale::removeTranslationPath(path); emit translationPathsChanged();}

    Q_INVOKABLE QString languageEndonym(const QString &locale)                {return MLocale::languageEndonym(locale);}

    //ToDo: Return Qt::LayoutDirection instead of int; unfortunately this doesn't currently work
    Q_INVOKABLE int directionForText(const QString &text)                     {return (int) MLocale::directionForText(text);}


Q_SIGNALS:
    void isValidChanged();
    void calendarTypeChanged();
    void timeFormat24hChanged();
    void defaultTimeFormat24hChanged();
    void languageEndonymChanged();
    void countryEndonymChanged();
    void decimalPointChanged();
    void localeScriptsChanged();
    void exemplarCharactersIndexChanged();
    void languageChanged();
    void countryChanged();
    void scriptChanged();
    void variantChanged();
    void nameChanged();
    void dataPathsChanged();
    void translationPathsChanged();

private:
    MLocale *ml;
#endif

};

#endif //MDECLARATIVEINPUTCONTEXT_H
