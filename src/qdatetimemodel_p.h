/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QDATETIMEMODEL_P_H
#define QDATETIMEMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/private/qdatetime_p.h"

#include "qdebug.h"

class QDateTimeModelPrivate : public QObjectPrivate, public QDateTimeParser
{
    Q_DECLARE_PUBLIC(QDateTimeModel)
public:
    QDateTimeModelPrivate();

    void init(const QVariant &variant);
    void readLocaleSettings();

    void emitSignals(EmitPolicy ep, const QVariant &old);

    QString textFromValue(const QVariant &value) const;
    QVariant valueFromText(const QString &text) const;

    virtual void interpret(EmitPolicy ep);
    virtual void clearCache() const;

    QDateTime validateAndInterpret(QString &input, int &, QValidator::State &state, bool fixup = false) const;
    void clearSection(int index);

    //virtual QString displayText() const { return edit->text(); } // this is from QDateTimeParser

    void updateCache(const QVariant &val, const QString &str) const;

    void updateTimeSpec();
    virtual QDateTime getMinimum() const { return minimum.toDateTime(); }
    virtual QDateTime getMaximum() const { return maximum.toDateTime(); }
    virtual QLocale locale() const { return q_func()->locale(); }
    QString valueToText(const QVariant &var) const { return textFromValue(var); }
    QString getAmPmText(AmPm ap, Case cs) const;
    int cursorPosition() const { return edit ? edit->cursorPosition() : -1; }

    virtual QVariant getZeroVariant() const;
    virtual void setRange(const QVariant &min, const QVariant &max);

    mutable bool cacheGuard;

    QString defaultDateFormat;
    QString defaultTimeFormat;
    QString defaultDateTimeFormat;
    QString unreversedFormat;

    mutable QVariant conflictGuard;
};

#endif // QDATETIMEMODEL_P_H
