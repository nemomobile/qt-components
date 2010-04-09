/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QRANGEMODEL_P_H
#define QRANGEMODEL_P_H

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

#include "qrangemodel.h"

QT_BEGIN_NAMESPACE

class QRangeModelPrivate : public QObject
{
public:
    Q_DECLARE_PUBLIC(QRangeModel)

    QRangeModelPrivate(QRangeModel *qq);
    virtual ~QRangeModelPrivate();

    void init();

    bool isSedated;
    bool signalsBlocked;

    void setSteps(qreal single, qreal page);

    qreal minimum, maximum, pageStep, value, position, pressValue;
    qreal pos, minpos, maxpos;
    qreal singleStep;

    uint tracking : 1;
    uint blocktracking :1;

    QRangeModel *q_ptr;

    inline qreal positionFromValue() {
        qreal scale =  qreal(maximum - minimum) / qreal(maxpos - minpos);
        return minpos + ((value - minimum) / scale); // ###
    }

    inline qreal valueFromPosition() {
        qreal scale = qreal(maximum - minimum) / qreal(maxpos - minpos);
        return minimum + ((pos - minpos) * scale); // ###
    }
};

QT_END_NAMESPACE

#endif // QRANGEMODEL_P_H
