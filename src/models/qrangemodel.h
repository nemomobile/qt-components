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

#ifndef QRANGEMODEL_H
#define QRANGEMODEL_H

#include <QtCore/qobject.h>
#include <QtGui/qgraphicsitem.h>
#include <QtGui/qabstractslider.h>
#include <QtDeclarative/qdeclarative.h>

class QRangeModelPrivate;

class QRangeModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal minimumValue READ minimum WRITE setMinimum NOTIFY rangeChanged)
    Q_PROPERTY(qreal maximumValue READ maximum WRITE setMaximum NOTIFY rangeChanged)
    Q_PROPERTY(qreal singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(qreal pageStep READ pageStep WRITE setPageStep)
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(qreal position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qreal positionAtMinimum READ positionAtMinimum WRITE setPositionAtMinimum NOTIFY positionRangeChanged)
    Q_PROPERTY(qreal positionAtMaximum READ positionAtMaximum WRITE setPositionAtMaximum NOTIFY positionRangeChanged)
    Q_PROPERTY(bool tracking READ hasTracking WRITE setTracking)

public:
    QRangeModel(QObject *parent = 0);
    virtual ~QRangeModel();

    void sedate();
    void awake();

    void setMinimum(qreal);
    qreal minimum() const;

    void setMaximum(qreal);
    qreal maximum() const;

    void setRange(qreal min, qreal max);

    void setSingleStep(qreal);
    qreal singleStep() const;

    void setPageStep(qreal);
    qreal pageStep() const;

    void setTracking(bool enable);
    bool hasTracking() const;

    void setSliderPosition(qreal);
    qreal sliderPosition() const;

    void setPositionAtMinimum(qreal);
    qreal positionAtMinimum() const;

    void setPositionAtMaximum(qreal);
    qreal positionAtMaximum() const;

    void setPositionRange(qreal min, qreal max);

    void setPosition(qreal position);
    qreal position() const;

    qreal value() const;

public Q_SLOTS:
    void singleStepAdd();
    void singleStepSub();
    void pageStepAdd();
    void pageStepSub();
    void toMinimum();
    void toMaximum();
    void setValue(qreal);

Q_SIGNALS:
    void sliderMoved(qreal position);

    void valueChanged(qreal value);
    void positionChanged(qreal position);

    void rangeChanged(qreal min, qreal max);
    void positionRangeChanged(qreal min, qreal max);

protected:
    QRangeModel(QRangeModelPrivate &dd, QObject *parent);
    QRangeModelPrivate* d_ptr;

private:
    Q_DISABLE_COPY(QRangeModel)
    Q_DECLARE_PRIVATE(QRangeModel)

};

QML_DECLARE_TYPE(QRangeModel);

#endif // QRANGEMODEL_H
