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

#include "sdeclarativescreen.h"
#include "sdeclarativescreen_p.h"
#include "sdeclarativescreen_p_resize.h"
#include <QApplication>
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <qmath.h>
#include <qnamespace.h>

#ifdef Q_DEBUG_SCREEN
#include <QDebug>
#endif // Q_DEBUG_SCREEN

SDeclarativeScreenPrivateResize::SDeclarativeScreenPrivateResize( SDeclarativeScreen *qq, QDeclarativeEngine *engine, QDeclarativeView *view)
    : SDeclarativeScreenPrivate(qq, engine, view)
{
    if (m_view) {
        m_view->installEventFilter(this);

        bool landscapeLock = false;
#ifdef Q_OS_SYMBIAN
        landscapeLock = deviceSupportsOnlyLandscape();
#endif

        //In case the orientation lock was set in the cpp side
        if (m_view->testAttribute(Qt::WA_LockLandscapeOrientation) || landscapeLock) {
#ifdef Q_DEBUG_SCREEN
            qDebug() << "SDeclarativeScreenPrivateResize - LockingLandscapeOrientation";
#endif
            setAllowedOrientations(SDeclarativeScreen::Landscape);
        } else if (m_view->testAttribute(Qt::WA_LockPortraitOrientation)) {
#ifdef Q_DEBUG_SCREEN
            qDebug() << "SDeclarativeScreenPrivateResize - LockingPortraitOrientation";
#endif
            setAllowedOrientations(SDeclarativeScreen::Portrait);
        }
    }
}

SDeclarativeScreenPrivateResize::~SDeclarativeScreenPrivateResize()
{
}

void SDeclarativeScreenPrivateResize::setScreenSize(QSize size)
{
    Q_Q(SDeclarativeScreen);

    // Fallback
    if (size.isEmpty())
        size = enforceFixedOrientation(systemScreenSize());

    if (size.isEmpty() || size == m_screenSize)
        return;

#ifndef Q_OS_SYMBIAN
    QSize sizeTransposed = size;
    sizeTransposed.transpose();
    if (m_displaySize != size && m_displaySize != sizeTransposed) {
        m_displaySize = size;
        q->emit displayChanged();
    }
#endif

    if ((m_screenSize.width() <= m_screenSize.height() && size.width() > size.height())
     || (m_screenSize.width() > m_screenSize.height() && size.width() <= size.height()))
        q->emit privateAboutToChangeOrientation(0, false);

    SDeclarativeScreenPrivate::setScreenSize(size);
    setCurrentOrientation(portraitScreen() ? SDeclarativeScreen::Portrait : SDeclarativeScreen::Landscape);
}


void SDeclarativeScreenPrivateResize::setAllowedOrientations(SDeclarativeScreen::Orientations orientations)
{
#ifdef Q_DEBUG_SCREEN
    qDebug() << "SDeclarativeScreenPrivateResize::setAllowedOrientations" << orientations;
#endif

#if defined(Q_OS_SYMBIAN)
    if((orientations != SDeclarativeScreen::Landscape) && deviceSupportsOnlyLandscape())
        return;
#endif

    SDeclarativeScreenPrivate::setAllowedOrientations(orientations);

    if (!m_initialized)
        return;

#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    if (portraitAllowed() && landscapeAllowed() && m_view)
        m_view->setAttribute(Qt::WA_AutoOrientation, true);
    else if (portraitAllowed() && !landscapeAllowed() && m_view)
        m_view->setAttribute(Qt::WA_LockPortraitOrientation, true);
    else if (!portraitAllowed() && landscapeAllowed() && m_view)
        m_view->setAttribute(Qt::WA_LockLandscapeOrientation, true);

    setScreenSize(enforceFixedOrientation(systemScreenSize()));
#else

    QSize newScreenSize = m_displaySize;
    if (portraitAllowed() && landscapeAllowed() && !portraitDisplay())
        newScreenSize.transpose();
    else if (portraitAllowed() && !landscapeAllowed() && !portraitDisplay())
        newScreenSize.transpose();
    else if (!portraitAllowed() && landscapeAllowed() && portraitDisplay())
        newScreenSize.transpose();

    if (m_view)
        m_view->resize(newScreenSize);
    else
        setScreenSize(newScreenSize);
#endif
}

bool SDeclarativeScreenPrivateResize::portraitScreen() const
{
    return m_screenSize.height() > m_screenSize.width();
}


QSize SDeclarativeScreenPrivateResize::systemScreenSize()
{
#if defined(Q_OS_SYMBIAN)
    TPixelsTwipsAndRotation params = screenParams();
    return QSize(params.iPixelSize.iWidth, params.iPixelSize.iHeight);
#elif defined(Q_WS_SIMULATOR)
    return QApplication::desktop()->screenGeometry().size();
#else
    return (m_screenSize.isEmpty()) ? m_displaySize : m_screenSize;
#endif
}

QSize SDeclarativeScreenPrivateResize::enforceFixedOrientation(QSize size)
{
    if ((size.width() <= size.height() && !portraitAllowed()) 
        || (size.width() > size.height() && !landscapeAllowed())) {
        size.transpose();
    }
    return size;
}

#ifndef Q_OS_SYMBIAN
void SDeclarativeScreenPrivateResize::privateSetOrientation(int orientation)
{
    if (orientation == SDeclarativeScreen::All
     || orientation == SDeclarativeScreen::LandscapeInverted
     || orientation == SDeclarativeScreen::PortraitInverted)
        return;

    if (m_allowedOrientations != SDeclarativeScreen::Default && !(m_allowedOrientations & orientation))
        return;

    QSize newScreenSize = m_displaySize;

    if(orientation == SDeclarativeScreen::Default)
        orientation = portraitDisplay() ? SDeclarativeScreen::Portrait : SDeclarativeScreen::Landscape;

    if (orientation == SDeclarativeScreen::Portrait && !portraitDisplay())
        newScreenSize.transpose();
    else if (orientation == SDeclarativeScreen::Landscape && portraitDisplay())
        newScreenSize.transpose();

    if (m_view)
        m_view->resize(newScreenSize);
    else
        setScreenSize(newScreenSize);
}
#endif

bool SDeclarativeScreenPrivateResize::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);

    if (m_initialized && event->type() == QEvent::Resize) {
        QSize size = static_cast<QResizeEvent*>(event)->size();
        setScreenSize(size);
    }

    return QObject::eventFilter(obj, event);
}
