/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QDATETIMEMODEL_P_H
#define QDATETIMEMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Components API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qdatetime.h>
#include <QtCore/private/qdatetime_p.h>


QT_BEGIN_NAMESPACE

class QDateTimeModelPrivate : public QDateTimeParser
{
    Q_DECLARE_PUBLIC(QDateTimeModel)
public:
    QDateTimeModelPrivate(QDateTimeModel *qq);

    void readLocaleSettings();
    void updateTimeSpec();
    void setRange(const QDateTime &min, const QDateTime &max);

    virtual QVariant getZeroVariant() const;

    QDateTime value;
    QDateTime minimum;
    QDateTime maximum;

    QDateTimeModel *q_ptr;
};

QT_END_NAMESPACE

#endif // QDATETIMEMODEL_P_H
