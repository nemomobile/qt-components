/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QObject>

#include "mscrolldecoratorsizer.h"

MScrollDecoratorSizer::MScrollDecoratorSizer(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
    m_positionRatio(0.0f),
    m_sizeRatio(0.0f),
    m_maxPosition(0.0f),
    m_minSize(0.0f),
    m_position(0),
    m_size(0)
{
}

MScrollDecoratorSizer::~MScrollDecoratorSizer()
{
}


qreal MScrollDecoratorSizer::positionRatio() const
{
    return m_positionRatio;
}

void MScrollDecoratorSizer::setPositionRatio(qreal val)
{
    if (val != m_positionRatio) {
        m_positionRatio = val;
        emit positionRatioChanged();
        recompute();
    }
}

qreal MScrollDecoratorSizer::sizeRatio() const
{
    return m_sizeRatio;
}

void MScrollDecoratorSizer::setSizeRatio(qreal val)
{
    if (val != m_sizeRatio) {
        m_sizeRatio = val;
        emit sizeRatioChanged();
        recompute();
    }
}

qreal MScrollDecoratorSizer::maxPosition() const
{
    return m_maxPosition;
}

void MScrollDecoratorSizer::setMaxPosition(qreal val)
{
    if (val != m_maxPosition) {
        m_maxPosition = val;
        emit maxPositionChanged();
        recompute();
    }
}

qreal  MScrollDecoratorSizer::minSize() const
{
    return m_minSize;
}

void  MScrollDecoratorSizer::setMinSize(qreal val)
{
    if (val != m_minSize) {
        m_minSize = val;
        emit minSizeChanged();
        recompute();
    }
}

int MScrollDecoratorSizer::position() const
{
    return m_position;
}

int MScrollDecoratorSizer::size() const
{
    return m_size;
}

void MScrollDecoratorSizer::recompute()
{


    // size underflow
    qreal sizeUnderflow =  (m_sizeRatio * m_maxPosition) < m_minSize ? m_minSize - (m_sizeRatio * m_maxPosition) : 0;

    // raw start and end position considering minimum size
    qreal rawStartPos =  m_positionRatio * (m_maxPosition - sizeUnderflow);
    qreal rawEndPos   = (m_positionRatio + m_sizeRatio) * (m_maxPosition - sizeUnderflow) + sizeUnderflow;

    // overshoot amount at start and end
    qreal overshootStart = rawStartPos < 0 ? -rawStartPos : 0;
    qreal overshootEnd   = rawEndPos > m_maxPosition ? rawEndPos - m_maxPosition : 0;

    // overshoot adjusted start and end
    qreal adjStartPos = rawStartPos + overshootStart;
    qreal adjEndPos   = rawEndPos - overshootStart - overshootEnd;

    // final position and size of thumb
    int pos  =  (int) 0.5 + (adjStartPos + m_minSize > m_maxPosition ? m_maxPosition - m_minSize : adjStartPos);
    int s    = (int)  0.5 + ((adjEndPos - pos) < m_minSize ? m_minSize : (adjEndPos - pos));

    if (pos != m_position) {
        m_position = pos;
        emit positionChanged();
    }

    if (s != m_size) {
        m_size = s;
        emit sizeChanged();
    }

}


