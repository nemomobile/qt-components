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

#include "qprogressbarmodel.h"

QProgressBarModel::QProgressBarModel(QObject *parent) :
    QObject(parent),
    m_progressValue(0)
{
}

QProgressBarModel::~QProgressBarModel()
{}

int QProgressBarModel::value() const
{
    return m_progressValue;
}

void QProgressBarModel::setValue(int value)
{
    if (m_progressValue == value)
        return;
    m_progressValue = value;
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
    m_progressValue = 0;
    emit canceled();
}
