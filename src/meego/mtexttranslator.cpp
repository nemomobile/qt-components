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

#include "mtexttranslator.h"

#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QFileInfo>

namespace {
  const QString i10nDir("/usr/share/l10n/meegotouch/");
}

// We will later use the separated mlocale implementation to get rid of the lmt dependency
#ifdef HAVE_MEEGOTOUCH
#include <MLocale>
#else
#ifdef HAVE_SYSTEMINFO
#include <QSystemInfo>
#endif
#endif

MTextTranslator::MTextTranslator()
{
    QCoreApplication *app = QCoreApplication::instance();
    Q_ASSERT(app);

    QFileInfo qmInfo;

#ifdef HAVE_MEEGOTOUCH
    MLocale mloc;
    QString locale = mloc.name();

    //locale names have this syntax: xx_YY@..., but we do only need the first part
    locale = locale.left(locale.indexOf('@'));
    //now check if we need the last part (this is true, when the appropriate file exists)
    qmInfo.setFile(QString("%1%2%3%4")
                     .arg(i10nDir)
                     .arg("common_")
                     .arg(locale)
                     .arg(".qm"));

    if (locale.contains('_') && !qmInfo.exists()) {
       locale = locale.left(locale.indexOf('_'));
    }

#else
#ifdef HAVE_SYSTEMINFO
    QtMobility::QSystemInfo *sysInfo = new QtMobility::QSystemInfo(this);
    QString locale = sysInfo->currentLanguage();
#else
    QString locale = QLocale::system().name();
#endif
#endif

    m_translator = new QTranslator(this);

    //fallback to default qm, when file not found
    qmInfo.setFile(QString("%1%2%3%4")
                   .arg(i10nDir)
                   .arg("common_")
                   .arg(locale)
                   .arg(".qm"));
    if (locale == "C" || !qmInfo.exists()) {
        m_translator->load(i10nDir + QString("libmeegotouch.qm"));
    } else {
        m_translator->load(i10nDir + QString("common_")+locale);
    }
    app->installTranslator(m_translator);
}

MTextTranslator::~MTextTranslator()
{
    delete m_translator;
}

QString MTextTranslator::translate(QString textId)
{
    QString text = qtTrId(textId.toLocal8Bit().data());

    // Use first length variant.
    const QChar TextVariantSeparator(0x9c, 0);
    text = text.left(text.indexOf(TextVariantSeparator));
    return text;
}


