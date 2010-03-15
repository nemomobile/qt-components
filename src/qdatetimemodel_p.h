/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
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
