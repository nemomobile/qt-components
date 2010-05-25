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

#ifndef QANGLEMODEL_H
#define QANGLEMODEL_H

#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarative.h>

class QAngleModelPrivate;

class QAngleModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal originX READ originX WRITE setOriginX NOTIFY originXChanged)
    Q_PROPERTY(qreal originY READ originY WRITE setOriginY NOTIFY originYChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    QAngleModel(QObject *parent = 0);
    virtual ~QAngleModel();

    qreal originX() const;
    void setOriginX(qreal x);

    qreal originY() const;
    void setOriginY(qreal x);

    qreal x() const;
    void setX(qreal x);

    qreal y() const;
    void setY(qreal y);

    Q_INVOKABLE void setXY(qreal x, qreal y);

    qreal radius() const;
    void setRadius(qreal radius);

    qreal angle() const;
    void setAngle(qreal angle);

    qreal rotation() const;
    void setRotation(qreal rotation);

Q_SIGNALS:
    void originXChanged(qreal x);
    void originYChanged(qreal y);
    void xChanged(qreal x);
    void yChanged(qreal y);
    void radiusChanged(qreal radius);
    void angleChanged(qreal angle);
    void rotationChanged(qreal rotation);

protected:
    QAngleModel(QAngleModelPrivate &dd, QObject *parent = 0);
    QScopedPointer<QAngleModelPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QAngleModel)
    Q_DISABLE_COPY(QAngleModel)
};

QML_DECLARE_TYPE(QAngleModel)

#endif // QANGLEMODEL_H
