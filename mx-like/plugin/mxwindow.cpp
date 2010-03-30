#include "mxwindow.h"
#include <QPoint>
#include <QApplication>
#include <QSize>
#include <QWidget>

QmlMxLikeScreen::QmlMxLikeScreen(QDeclarativeItem* parent)
    : QDeclarativeItem(parent)
{
    foreach (QWidget *widget, QApplication::topLevelWidgets())
        widget->setWindowFlags(widget->windowFlags() | Qt::FramelessWindowHint);
}

void QmlMxLikeScreen::dragPointMove(int x, int y, bool first)
{
    QPoint newPoint(x,y);
    if(!first){
        //Only works for a single window right now - but that's correct for moblin UX?
        QWidget* window = QApplication::topLevelWidgets().first();
        window->move(window->pos() + newPoint - lastMovePoint);
    }
    lastMovePoint = newPoint;
}

void QmlMxLikeScreen::dragPointResize(int x, int y, bool first)
{
    QPoint newPoint(x,y);
    if(!first){
        //Only works for a single window right now - but that's correct for moblin UX?
        QWidget* window = QApplication::topLevelWidgets().first();
        QPoint delta = newPoint - lastResizePoint;
        window->resize(window->size() + QSize(delta.x(), delta.y()));
    }
    lastResizePoint = newPoint;
}
