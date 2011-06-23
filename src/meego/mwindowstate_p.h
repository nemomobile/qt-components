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

#ifndef MWINDOWSTATE_P_H
#define MWINDOWSTATE_P_H

#ifdef Q_WS_X11
#include <QX11Info>
#include "mx11wrapper.h"
#endif

#include <QTimer>

class MWindowStatePrivate
{
public:

    MWindowStatePrivate();
    virtual ~MWindowStatePrivate();

#ifdef Q_WS_X11
    void initVisibilityWatcher();
    void initVisibleChangedTimer();
    void handleXVisibilityEvent(XVisibilityEvent *xevent);
    void handleXPropertyEvent(XPropertyEvent *xevent);
    void handleXFocusChangeEvent(XFocusChangeEvent *xevent);
    void appendEventMask(Window win);
    static bool eventFilter(void *message, long int *result);
    static bool isMeeGoWindowManagerRunning();
    void doActiveChanged(bool newActive);
    void _q_doVisibleChangedNotVisible();
    void doVisibleChanged(bool newVisible);
    void doViewModeChanged(MWindowState::ViewMode newViewMode);
    Window effectiveWinId(Window winIdFromEvent);
#endif // Q_WS_X11

protected:

    MWindowState * q_ptr;

private:

    // Represents the focus information received from X11
    enum FocusEvent { FENone, FEFocusIn, FEFocusOut };

    static bool (*origEventFilter)(void*, long int*);
    static MWindowStatePrivate *instance;

    MWindowState::ViewMode viewMode;

    // Latest FocusChangeEvent received from X11
    FocusEvent focus;

    // True, if window or thumbnail is visible
    bool visible;

    // True, if the window has focus
    bool active;

    // True, if VisibilityFullyObscured was the most recent
    // visibility event received from X11
    bool fullyObscured;

    // True, if _MEEGOTOUCH_VISIBLE_IN_SWITCHER is set
    bool visibleInSwitcherPropertySet;

    // Timer used to delay transitions from visible to hidden.
    // (Solves race condition between _MEEGOTOUCH_VISIBLE_IN_SWITCHER
    //  and fullyObscured).
    QTimer visibleChangedTimer;

    bool animating;

    bool eventMaskSet;

    Q_DECLARE_PUBLIC(MWindowState)
};

#endif // MWINDOWSTATE_P_H
