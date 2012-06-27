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

#ifndef SDECLARATIVESCREEN_P_SENSOR_H
#define SDECLARATIVESCREEN_P_SENSOR_H

#include "sdeclarativescreen.h"
#include "sdeclarativescreen_p.h"
#include <QDeclarativeView>

#ifdef Q_OS_SYMBIAN
#include <QCoreApplication>
#endif

QT_FORWARD_DECLARE_CLASS(QDeclarativeEngine)


#ifdef Q_OS_SYMBIAN
class OrientationListener : public QObject
{
    Q_OBJECT

public:
    static OrientationListener *getCountedInstance();
    static void deleteCountedInstance();

private:
    OrientationListener();
    ~OrientationListener();
    static bool symbianEventFilter(void *message, long *result);

Q_SIGNALS:
    void orientationEvent();

private:
    // Data
    static int userCount;
    static OrientationListener *instance;
    static QCoreApplication::EventFilter previousEventFilter;
};
#endif

class SDeclarativeScreenPrivateSensor : public SDeclarativeScreenPrivate
{

    Q_OBJECT
    Q_DECLARE_PUBLIC(SDeclarativeScreen)

public:

    SDeclarativeScreenPrivateSensor(SDeclarativeScreen *qq, QDeclarativeEngine *engine, QDeclarativeView *view);
    ~SDeclarativeScreenPrivateSensor();

    void setAllowedOrientations(SDeclarativeScreen::Orientations orientations);
    void privateSetOrientation(int orientation);


public Q_SLOTS:
    void switchGeometry();
    void viewStatusChanged(QDeclarativeView::Status status);

#ifdef Q_OS_SYMBIAN
    void syncOrientationWithSystem();
#endif

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    int m_animate : 1;
    int m_hasWindow : 1;

#ifdef Q_OS_SYMBIAN
    SDeclarativeScreen::Orientation systemOrientation();
#endif
};

#endif // SDECLARATIVESCREEN_P_H
