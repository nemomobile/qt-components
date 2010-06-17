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

#include "qprogressbarmodel.h"
#include "qprogressbarmodel_p.h"

QProgressBarModelPrivate::QProgressBarModelPrivate()
    : m_progressValue(0), q_ptr(0)
{
}

QProgressBarModel::QProgressBarModel(QObject *parent)
    : QObject(parent), d_ptr(new QProgressBarModelPrivate)
{
}

QProgressBarModel::QProgressBarModel(QProgressBarModelPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

QProgressBarModel::~QProgressBarModel()
{
}

int QProgressBarModel::value() const
{
    Q_D(const QProgressBarModel);
    return d->m_progressValue;
}

void QProgressBarModel::setValue(int value)
{
    Q_D(QProgressBarModel);
    if (d->m_progressValue == value)
        return;
    d->m_progressValue = value;
    emit valueChanged();
}

void QProgressBarModel::start()
{
    emit running();
}

void QProgressBarModel::pause()
{
    emit paused();
}

void QProgressBarModel::resume()
{
    emit running();
}

void QProgressBarModel::reset()
{
    Q_D(QProgressBarModel);
    d->m_progressValue = 0;
    emit canceled();
}
