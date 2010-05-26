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

#include "qanglemodel.h"
#include "qanglemodel_p.h"

#include <QtCore/qmath.h>

static const qreal Q_PI   = qreal(3.14159265358979323846);   // pi
static const qreal Q_2PI  = qreal(6.28318530717958647693);   // 2*pi

QAngleModelPrivate::QAngleModelPrivate(QAngleModel *qq)
    : q_ptr(qq), origin(0, 0), position(0, 0), radius(0), angle(0), rotation(0)
{
}

QAngleModelPrivate::~QAngleModelPrivate()
{
}

static inline qreal normalize(qreal angle)
{
    return angle - 360 * qFloor(angle / 360);  // qreal modulus 360
}

static inline qreal toRadians(qreal angle)
{
    return angle * Q_2PI / 360;
}

static inline qreal fromRadians(qreal angle)
{
    return angle * 360 / Q_2PI;
}

/*!
  \internal

  Calculate position based on current origin, angle and radius.
*/
void QAngleModelPrivate::calculatePosition()
{
    Q_Q(QAngleModel);

    const qreal angleRadians = toRadians(angle + rotation);
    const qreal projectionInX = qCos(angleRadians) * radius;
    const qreal projectionInY = qSin(angleRadians) * radius;
    const qreal oldX = position.x();
    const qreal oldY = position.y();

    position.setX(origin.x() + projectionInX);
    position.setY(origin.y() + projectionInY);

    if (oldX != position.x())
        emit q->xChanged(position.x());
    if (oldY != position.y())
        emit q->yChanged(position.y());
}

/*!
  \internal

  Calculate angle and radius based on current origin and position.
*/
void QAngleModelPrivate::calculateAngleAndRadius()
{
    Q_Q(QAngleModel);

    const qreal projectionInX = position.x() - origin.x();
    const qreal projectionInY = position.y() - origin.y();
    const qreal oldAngle = angle;
    const qreal oldRadius = radius;

    radius = sqrt(projectionInX * projectionInX + projectionInY * projectionInY);

    if (radius == 0) {
        // Corner case
        angle = 0;
    } else if (projectionInX > 0) {
        if (projectionInY >= 0) {
            // First quadrant
            angle = qAsin(projectionInY / radius);
        } else {
            // Fourth quadrant
            angle = Q_2PI + qAsin(projectionInY / radius);
        }
    } else {
        // Second and Third quadrants
        angle = Q_PI - qAsin(projectionInY / radius);
    }

    angle = normalize(fromRadians(angle) - rotation);

    if (oldAngle != angle)
        emit q->angleChanged(angle);
    if (oldRadius != radius)
        emit q->radiusChanged(radius);
}

QAngleModel::QAngleModel(QObject *parent)
    : QObject(parent), d_ptr(new QAngleModelPrivate(this))
{
}

QAngleModel::~QAngleModel()
{
}

qreal QAngleModel::originX() const
{
    Q_D(const QAngleModel);
    return d->origin.x();
}

void QAngleModel::setOriginX(qreal x)
{
    Q_D(QAngleModel);
    if (x == d->origin.x())
        return;

    d->origin.setX(x);

    // ### Why not calculateAngle?
    // ### This can lead to problems in QML: { positionX: 20; originX: 10; }
    //     makes position become 30.
    //     Maybe let it be dirty and calculate in the getter, or use a enum
    //     to decide between origin ChangesPosition or ChangesAngle
    d->calculatePosition();
    emit originXChanged(x);
}

qreal QAngleModel::originY() const
{
    Q_D(const QAngleModel);
    return d->origin.y();
}

void QAngleModel::setOriginY(qreal y)
{
    Q_D(QAngleModel);
    if (y == d->origin.y())
        return;

    d->origin.setY(y);
    d->calculatePosition();
    emit originYChanged(y);
}

qreal QAngleModel::x() const
{
    Q_D(const QAngleModel);
    return d->position.x();
}

void QAngleModel::setX(qreal x)
{
    Q_D(QAngleModel);
    if (x == d->position.x())
        return;

    d->position.setX(x);
    d->calculateAngleAndRadius();
    emit xChanged(x);
}

qreal QAngleModel::y() const
{
    Q_D(const QAngleModel);
    return d->position.y();
}

void QAngleModel::setY(qreal y)
{
    Q_D(QAngleModel);
    if (y == d->position.y())
        return;

    d->position.setY(y);
    d->calculateAngleAndRadius();
    emit yChanged(y);
}

void QAngleModel::setXY(qreal x, qreal y)
{
    Q_D(QAngleModel);

    const bool xEqual = (x == d->position.x());
    const bool yEqual = (y == d->position.y());

    if (xEqual && yEqual)
        return;

    d->position.setX(x);
    d->position.setY(y);
    d->calculateAngleAndRadius();

    if (!xEqual)
        emit xChanged(x);
    if (!yEqual)
        emit yChanged(y);
}

qreal QAngleModel::radius() const
{
    Q_D(const QAngleModel);
    return d->radius;
}

void QAngleModel::setRadius(qreal radius)
{
    Q_D(QAngleModel);
    if (radius == d->radius)
        return;

    d->radius = radius;
    d->calculatePosition();
    emit radiusChanged(radius);
}

qreal QAngleModel::angle() const
{
    Q_D(const QAngleModel);
    return d->angle;
}

void QAngleModel::setAngle(qreal angle)
{
    Q_D(QAngleModel);

    angle = normalize(angle);
    if (angle == d->angle)
        return;

    d->angle = angle;
    d->calculatePosition();
    emit angleChanged(angle);
}

qreal QAngleModel::rotation() const
{
    Q_D(const QAngleModel);
    return d->rotation;
}

void QAngleModel::setRotation(qreal rotation)
{
    Q_D(QAngleModel);

    if (rotation == d->rotation)
        return;

    d->rotation = rotation;
    d->calculatePosition();
    emit rotationChanged(rotation);
}
