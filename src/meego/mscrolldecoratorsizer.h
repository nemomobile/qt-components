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

#ifndef MSCROLLDECORATORSIZER_H
#define MSCROLLDECORATORSIZER_H

#include <QDeclarativeItem>

class MScrollDecoratorSizer : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(qreal positionRatio READ positionRatio WRITE setPositionRatio NOTIFY positionRatioChanged)
    Q_PROPERTY(qreal sizeRatio READ sizeRatio WRITE setSizeRatio NOTIFY sizeRatioChanged)
    Q_PROPERTY(qreal maxPosition READ maxPosition WRITE setMaxPosition NOTIFY maxPositionChanged)
    Q_PROPERTY(qreal minSize READ minSize WRITE setMinSize NOTIFY minSizeChanged)

    Q_PROPERTY(int position READ position NOTIFY positionChanged FINAL)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged FINAL)

public:
    MScrollDecoratorSizer(QDeclarativeItem *parent = 0);
    virtual ~MScrollDecoratorSizer();

    qreal positionRatio() const;
    void setPositionRatio(qreal val);

    qreal sizeRatio() const;
    void setSizeRatio(qreal val);

    qreal maxPosition() const;
    void setMaxPosition(qreal val);

    qreal minSize() const;
    void setMinSize(qreal val);

    int position() const;
    int size() const;


Q_SIGNALS:
    void positionRatioChanged();
    void sizeRatioChanged();
    void maxPositionChanged();
    void minSizeChanged();
    void positionChanged();
    void sizeChanged();

protected:
    void recompute();
    qreal m_positionRatio;
    qreal m_sizeRatio;
    qreal m_maxPosition;
    qreal m_minSize;

    int m_position;
    int m_size;

};

#endif //MSCROLLDECORATORSIZER_H
