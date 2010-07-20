#include <qwindowobject_p.h>
#include <qdeclarativewindow.h>
#include <qdebug.h>

#include <stdio.h>

#define win qobject_cast<QGraphicsView *>(_window)

QWindowObject::QWindowObject(QDeclarativeWindow *parent)
    : QObject(parent), activeWindow(false), window(parent)
{
}


QWindowObject::Orientation QWindowObject::orientation() const
{
    return Portrait;
}

void QWindowObject::setOrientation(Orientation orientation)
{
    Q_UNUSED(orientation);
    qWarning() << "setOrientation not implemented for desktop";
}

bool QWindowObject::isOrientationLocked() const
{
    return true;
}

void QWindowObject::setOrientationLocked(bool locked)
{
    Q_UNUSED(locked);
    qWarning() << "setOrientationLocked not implemented for desktop";
}

