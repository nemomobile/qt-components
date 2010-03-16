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

#include "qprogressbarmodel.h"

QProgressBarModel::QProgressBarModel(QObject *parent) :
    QObject(parent),
    m_progressValue(0)
{
    m_progressAnimation.setTargetObject(this);
    m_progressAnimation.setPropertyName("currentValue");
    m_progressAnimation.setStartValue(0);
    m_progressAnimation.setEndValue(100);
    m_progressAnimation.setDuration(5000);

    connect(&m_progressAnimation, SIGNAL(finished()), SIGNAL(finished()));
}

QProgressBarModel::~QProgressBarModel()
{}

int QProgressBarModel::currentValue()
{
    return m_progressValue;
}

void QProgressBarModel::setCurrentValue(int value)
{
    if (m_progressValue == value)
        return;
    m_progressValue = value;
    emit currentValueChanged();
}

void QProgressBarModel::start()
{
    m_progressAnimation.start();
    emit running();
}

void QProgressBarModel::pause()
{
    m_progressAnimation.pause();
    emit paused();
}

void QProgressBarModel::resume()
{
    m_progressAnimation.resume();
    emit running();
}

void QProgressBarModel::cancel()
{
    m_progressAnimation.stop(); // needed?
    m_progressAnimation.setCurrentTime(0);
    m_progressValue = 0;
    emit canceled();
}
