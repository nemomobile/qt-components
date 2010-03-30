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
