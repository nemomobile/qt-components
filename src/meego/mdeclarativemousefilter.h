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

#ifndef MDECLARATIVEMOUSEFILTER_H
#define MDECLARATIVEMOUSEFILTER_H

#include <QDeclarativeItem>

class QGraphicsSceneMouseEvent;

class MDeclarativeMouseEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(bool filtered READ isFiltered WRITE setFiltered)

public:
    MDeclarativeMouseEvent()
        : _x(0), _y(0), _filtered(false) {}
    MDeclarativeMouseEvent(int x, int y)
        : _x(x), _y(y), _filtered(false) {}
    MDeclarativeMouseEvent(const QPointF &point)
        : _x(point.x()), _y(point.y()), _filtered(false) {}

    int x() const { return _x; }
    int y() const { return _y; }

    // only for internal usage
    void setX(int x) { _x = x; }
    void setY(int y) { _y = y; }

    bool isFiltered() { return _filtered; }
    void setFiltered(bool filtered) { _filtered = filtered; }

private:
    int _x;
    int _y;
    bool _filtered;
};

class MDeclarativeMouseFilter : public QDeclarativeItem
{
    Q_OBJECT

public:
    explicit MDeclarativeMouseFilter(QDeclarativeItem *parent = 0);
    ~MDeclarativeMouseFilter();

private:
    QGraphicsSceneMouseEvent *copyMouseEvent (QGraphicsSceneMouseEvent *event);

signals:
    void mousePositionChanged(MDeclarativeMouseEvent *mouse);

    void pressed(MDeclarativeMouseEvent *mouse);
    void delayedPressSent();
    void pressAndHold(MDeclarativeMouseEvent *mouse);
    void released(MDeclarativeMouseEvent *mouse);
    void finished(); // sent after release has been handled in the target item
    void horizontalDrag(); // for initiating the selection
    void doubleClicked(MDeclarativeMouseEvent *mouse);
public slots:

protected:
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    virtual bool event(QEvent *event);
    void itemChange(ItemChange, const ItemChangeData &);
#else
    virtual bool sceneEvent(QEvent *event);
    QVariant itemChange(GraphicsItemChange, const QVariant &);
#endif
    void timerEvent(QTimerEvent *);
    void clampMousePosition(QGraphicsSceneMouseEvent *me);

private:
    int pressAndHoldTimerId;
    QGraphicsSceneMouseEvent *delayedPressEvent;
    QPoint lastPos;
};

#endif // MDECLARATIVEMOUSEFILTER_H
