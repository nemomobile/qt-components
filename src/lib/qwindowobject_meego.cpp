#include <qwindowobject_p.h>
#include <qdeclarativewindow.h>
#include <MApplicationWindow>

#include <stdio.h>

#define mwin qobject_cast<MApplicationWindow *>(window->window())

QWindowObject::QWindowObject(QDeclarativeWindow *parent)
    : QObject(parent), activeWindow(false), window(parent)
{
}



QWindowObject::Orientation QWindowObject::orientation() const
{
    Orientation o = UnknownOrientation;
    switch (mwin->orientationAngle()) {
    case M::Angle0:
        o = Landscape;
        break;
    case M::Angle90:
        o = PortraitInverted;
        break;
    case M::Angle180:
        o = LandscapeInverted;
        break;
    case M::Angle270:
        o = Portrait;
        break;
    }
    printf(">>> orientation: %d\n", o);
    return o;
}

void QWindowObject::setOrientation(Orientation orientation)
{
    M::OrientationAngle angle = M::Angle0;
    switch(orientation) {
    case Landscape:
    case UnknownOrientation:
        angle = M::Angle0;
        break;
    case PortraitInverted:
        angle = M::Angle90;
        break;
    case LandscapeInverted:
        angle = M::Angle180;
        break;
    case Portrait:
        angle = M::Angle270;
        break;
    }
    printf(">>> setorientation: %d\n", orientation);
    if (angle != mwin->orientationAngle()) {
        mwin->setOrientationAngle(angle);
        window->adjustRootObject();
    }
}

bool QWindowObject::isOrientationLocked() const
{
    return mwin->isOrientationAngleLocked();
}

void QWindowObject::setOrientationLocked(bool locked)
{
    mwin->setOrientationAngleLocked(locked);
    emit orientationLockedChanged();
}

