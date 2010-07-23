/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include <mdeclarativewindow.h>
#include <mdeviceprofile.h>

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

class MDeclarativeWindowPrivate
{
public:
    MDeclarativeWindowPrivate(MDeclarativeWindow *qq);

    void initContextSubscriber();
    void _q_isCoveredChanged();
    void _q_updateOrientationAngle();


    MDeclarativeWindow *q;
    MDeclarativeWindow::Orientation orientation;
    bool orientationLocked;
    bool isCovered;

#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty topEdgeProperty;
    ContextProperty isCoveredProperty;
#endif
};

MDeclarativeWindowPrivate::MDeclarativeWindowPrivate(MDeclarativeWindow *qq)
        : q(qq)
        , orientation(MDeclarativeWindow::Portrait)
        , orientationLocked(false)
        , isCovered(false)
#ifdef HAVE_CONTEXTSUBSCRIBER
    , topEdgeProperty("Screen.TopEdge")
    , isCoveredProperty("Screen.IsCovered")
#endif
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    QObject::connect(&topEdgeProperty, SIGNAL(valueChanged()),
            q, SLOT(_q_updateOrientationAngle()));
    QObject::connect(&isCoveredProperty, SIGNAL(valueChanged()),
            q, SLOT(_q_isCoveredChanged()));
//    connect(MKeyboardStateTracker::instance(), SIGNAL(stateChanged()),
//            this, SLOT(updateOrientationAngle()));
#endif

    initContextSubscriber();
}

void MDeclarativeWindowPrivate::initContextSubscriber()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    //waiting for properties to synchronize
    topEdgeProperty.waitForSubscription();
    isCoveredProperty.waitForSubscription();

    //initiating the variables to current orientation
    _q_updateOrientationAngle();
    _q_isCoveredChanged();
#endif
}

void MDeclarativeWindowPrivate::_q_isCoveredChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    bool covered = isCoveredProperty.value().toBool();

    if (isCovered != covered) {
        qDebug() << "MDeclarativeWindowPrivate" << "Covered:" << covered;

        isCovered = covered;
        emit q->coveredChanged();
    }
#endif
}

void MDeclarativeWindowPrivate::_q_updateOrientationAngle()
{
    if (orientationLocked)
        return;

#ifdef HAVE_CONTEXTSUBSCRIBER
    MDeclarativeWindow::Orientation newOrientation = MDeclarativeWindow::Landscape;
    QString edge = topEdgeProperty.value().toString();
    bool isKeyboardOpen = false; //### MKeyboardStateTracker::instance()->isOpen();

    if (edge == "top") {// && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle0, isKeyboardOpen))) {
        newOrientation = MDeclarativeWindow::Landscape;
    } else if (edge == "left") {// && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle270, isKeyboardOpen))) {
        newOrientation = MDeclarativeWindow::Portrait;
    } else if (edge == "right") { // && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle90, isKeyboardOpen))) {
        newOrientation = MDeclarativeWindow::PortraitInverted;
    } else if (edge == "bottom") { // && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle180, isKeyboardOpen))) {
        newOrientation = MDeclarativeWindow::LandscapeInverted;
    } else {
#if 0
        //it seems that orientation does not match allowed for current kybrd state.
        //check if the previous one was ok:
        if ((orientation == Landscape && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle0, isKeyboardOpen))) ||
            (orientation == PortraitInverted && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle90, isKeyboardOpen))) ||
            (orientation == LandscapeInverted && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle180, isKeyboardOpen))) ||
            (orientation == Portrait && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle270, isKeyboardOpen)))
            ){
            //it was: let's just use an old angle
            newOrientation = orientation;
        } else {
            //it was not: let's use first allowed:
            if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle0, isKeyboardOpen))
                newOrientation = Landscape;
            else if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle270, isKeyboardOpen))
                newOrientation = Portrait;
            else if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle90, isKeyboardOpen))
                newOrientation = PortraitInverted;
            else if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle180, isKeyboardOpen))
                newOrientation = LandscapeInverted;
            else
                qFatal("MDeclarativeWindowPrivate::updateOrientationAngle() - current keyboard state seems not to be covered in device.conf file");
        }
#endif
    }

    if (newOrientation != orientation) {
        orientation = newOrientation;
        emit q->orientationChanged();
    }
#endif
}





MDeclarativeWindow::MDeclarativeWindow(QDeclarativeItem *parent)
        : QDeclarativeItem(parent),
        d(new MDeclarativeWindowPrivate(this))
{
}

MDeclarativeWindow::~MDeclarativeWindow()
{
    delete d;
}

void MDeclarativeWindow::setOrientation(Orientation o)
{
    d->orientation = o;
    emit orientationChanged();
}

MDeclarativeWindow::Orientation MDeclarativeWindow::orientation() const
{
    return d->orientation;
}

bool MDeclarativeWindow::isOrientationLocked() const
{
    return d->orientationLocked;
}

void MDeclarativeWindow::setOrientationLocked(bool locked)
{
    d->orientationLocked = locked;
    emit orientationLockedChanged();
}

#include "moc_mdeclarativewindow.cpp"
