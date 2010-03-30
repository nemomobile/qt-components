/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
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
** $QT_END_LICENSE$
**
****************************************************************************/
#include "mxwindow.h"
#include <QPoint>
#include <QApplication>
#include <QSize>
#include <QWidget>
#include <QDebug>

QmlMxLikeScreen::QmlMxLikeScreen(QDeclarativeItem* parent)
    : QDeclarativeItem(parent), moveValid(false), resizeValid(true)
{
    foreach (QWidget *widget, QApplication::topLevelWidgets())
        widget->setWindowFlags(widget->windowFlags() | Qt::FramelessWindowHint);
}

void QmlMxLikeScreen::resetMove()
{
    moveValid = false;
}

void QmlMxLikeScreen::resetResize()
{
    resizeValid = false;
}

void QmlMxLikeScreen::dragPointMove(int x, int y)
{
    QPoint newPoint(x,y);
    if(moveValid){
        QWidget* window = scene()->views().first()->window();
        QPoint delta = newPoint - lastMovePoint;
        window->move(window->pos() + delta);
        newPoint -= delta;
    }else{
        moveValid = true;
    }
    lastMovePoint = newPoint;
}

void QmlMxLikeScreen::dragPointResize(int x, int y)
{
    QPoint newPoint(x,y);
    if(resizeValid){
        QWidget* window = scene()->views().first()->window();
        QPoint delta = newPoint - lastResizePoint;
        window->resize(window->size() + QSize(delta.x(), delta.y()));
        newPoint -= delta;//Because it's anchored to the corner, it moves too!
    }else{
        resizeValid = true;
    }
    lastResizePoint = newPoint;
}
