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

#include <QWidget>
#include <QApplication>
#include "mwindowstate.h"
#include "mwindowstate_p.h"

MWindowStatePrivate * MWindowStatePrivate::instance = NULL;

bool (*MWindowStatePrivate::origEventFilter)(void*, long int*) = NULL;

MWindowStatePrivate::MWindowStatePrivate()
    : viewMode(MWindowState::Fullsize)
    , focus(MWindowStatePrivate::FENone)
    , visible(false)
    , active(false)
    , fullyObscured(false)
    , visibleInSwitcherPropertySet(false)
    , animating(false)
    , eventMaskSet(false)
{
    MWindowStatePrivate::instance = this;
}

MWindowState::~MWindowState()
{
    delete d_ptr;
}

MWindowStatePrivate::~MWindowStatePrivate()
{
#ifdef Q_WS_X11
    if (qApp) {
        qApp->setEventFilter(origEventFilter);
    }
#endif
}

#ifdef Q_WS_X11

namespace
{
    const unsigned int VisibleChangedDelay = 1000;
    const char * MeeGoTouchWindowManagerName = "MCompositor";
}

static int handleXError(Display *, XErrorEvent *)
{
    return 0;
}

Window MWindowStatePrivate::effectiveWinId(Window winIdFromEvent)
{
    QWidget * window = QWidget::find(winIdFromEvent);
    if (window) {
        return window->effectiveWinId();
    } else {
        return 0;
    }
}

void MWindowStatePrivate::initVisibilityWatcher()
{
    origEventFilter = QApplication::instance()->setEventFilter(eventFilter);
}

void MWindowStatePrivate::initVisibleChangedTimer()
{
    Q_Q(MWindowState);

    visibleChangedTimer.connect(&visibleChangedTimer, SIGNAL(timeout()),
                                q, SLOT(_q_doVisibleChangedNotVisible()));
    visibleChangedTimer.setInterval(VisibleChangedDelay);
    visibleChangedTimer.setSingleShot(true);
}

void MWindowStatePrivate::appendEventMask(Window win)
{
    XWindowAttributes existingAttributes;
    XSetWindowAttributes newAttributes;
    Status status;

    status = MX11Wrapper::XGetWindowAttributes(QX11Info::display(), win, &existingAttributes);
    if (status == 0) {
        qFatal("MWindow: XGetWindowAttributes() failed!");
    }

    newAttributes.event_mask = existingAttributes.your_event_mask |
                               VisibilityChangeMask |
                               PropertyChangeMask |
                               FocusChangeMask;

    MX11Wrapper::XChangeWindowAttributes(QX11Info::display(), win, CWEventMask, &newAttributes);
}

bool MWindowStatePrivate::eventFilter(void *message, long int *result)
{
    Q_UNUSED(result);

    XEvent *event = reinterpret_cast<XEvent*>(message);
    if (event->type == VisibilityNotify) {
        XVisibilityEvent *xevent = reinterpret_cast<XVisibilityEvent *>(event);
        instance->handleXVisibilityEvent(xevent);
        return true;

    } else if (event->type == PropertyNotify) {
        XPropertyEvent *xevent = reinterpret_cast<XPropertyEvent *>(event);
        instance->handleXPropertyEvent(xevent);
        return false;

    } else if (event->type == FocusIn || event->type == FocusOut) {
        XFocusChangeEvent *xevent = reinterpret_cast<XFocusChangeEvent *>(event);
        instance->handleXFocusChangeEvent(xevent);
        return false;
    }

    if (origEventFilter) {
        return origEventFilter(message, result);
    }
    else {
        return false;
    }
}

void MWindowStatePrivate::handleXVisibilityEvent(XVisibilityEvent *xevent)
{
    // Check if MCompositor is running. This is done only once.
    static const bool wmRunning = isMeeGoWindowManagerRunning();

    // Listen only to synthetic events if MeeGo window
    // manager is running. Note that VisibilityFullyObscured
    // doesn't always mean that window is hidden. It can be
    // "logically" visible if its thumbnail is seen in the
    // switcher. This is why we set the fullyObscured flag and combine
    // it later with information from Home Screen.

    if (xevent->send_event || !wmRunning) {
        Window winId = effectiveWinId(xevent->window);
        if (xevent->window == winId) {
            switch (xevent->state) {
            case VisibilityFullyObscured:
                fullyObscured = true;
                doVisibleChanged(false);
                break;
            case VisibilityUnobscured:
            case VisibilityPartiallyObscured:
                fullyObscured = false;
                doViewModeChanged(MWindowState::Fullsize);
                doVisibleChanged(true);
                break;
            default:
                break;
            }
        }
    }
}

void MWindowStatePrivate::handleXPropertyEvent(XPropertyEvent *xevent)
{
    // _MEEGOTOUCH_VISIBLE_IN_SWITCHER is set by Home Screen for
    // windows that are in the switcher and visible. Set/unset the
    // flag because we need to combine this information with X11's
    // visibility events.

    if (xevent->state == PropertyNewValue) {

        Atom           type;
        int            format;
        unsigned long  nItems;
        unsigned long  bytesAfter;
        unsigned char *data = NULL;

        static Atom switcherAtom = MX11Wrapper::XInternAtom(QX11Info::display(),
                                                           "_MEEGOTOUCH_VISIBLE_IN_SWITCHER", True);

        if (xevent->atom == switcherAtom) {
            // Read value of the property. Should be 1 or 0.
            if (MX11Wrapper::XGetWindowProperty(QX11Info::display(), xevent->window, switcherAtom,
                                               0, 1, False, XA_CARDINAL, &type, &format, &nItems,
                                               &bytesAfter, &data) == Success && data) {

                const bool visibleInSwitcher = *data;
                visibleInSwitcherPropertySet = visibleInSwitcher;

                // Visible in switcher property was added
                // => window is visible as a thumbnail
                if (visibleInSwitcher) {
                    doViewModeChanged(MWindowState::Thumbnail);
                    doVisibleChanged(true);

                // Visible in switcher property was removed and
                // fullyObscured is already set => window is not
                // visible
                } else if (fullyObscured) {
                    doVisibleChanged(false);

                // Visible in switcher property was removed and
                // fullyObscured is not set => window visible and fullsize
                } else {
                    doViewModeChanged(MWindowState::Fullsize);
                    doVisibleChanged(true);
                }

                MX11Wrapper::XFree(data);
            }
        }
    }
}

void MWindowStatePrivate::handleXFocusChangeEvent(XFocusChangeEvent *xevent)
{
    static bool firstFocusChange = true;

    Window winId = effectiveWinId(xevent->window);

    if (xevent->window == winId) {
        if (xevent->mode == NotifyNormal) {
            if (xevent->type == FocusIn) {
                focus = FEFocusIn;

                // The X event filter is installed at a too late stage to catch
                // the first VisibilityNotify. This causes the window to be
                // invisible from MWindowState's point of view until the
                // next VisibilityNotify.
                // 
                // To correct this we explicitly set the window to be visible
                // and fullsize when the first FocusIn event arrives. To keep
                // the functionality as transparent as possible,
                // doActiveChanged() is called _after_ the visibility is
                // changed.
                //
                // We also append the event mask here in order to get the
                // correct window id.

                if (firstFocusChange && !fullyObscured) {
                    firstFocusChange = false;
                    appendEventMask(winId);
                    doViewModeChanged(MWindowState::Fullsize);
                    doVisibleChanged(true);
                }

                doActiveChanged(true);
            } else {
                focus = FEFocusOut;
                doActiveChanged(false);
            }
        }
    }
}

void MWindowStatePrivate::doActiveChanged(bool newActive)
{
    Q_Q(MWindowState);

    if (active != newActive) {
        active = newActive;
        emit q->activeChanged();
    }
}

bool MWindowStatePrivate::isMeeGoWindowManagerRunning()
{
    bool retValue = false;

    Display       *dpy = QX11Info::display();
    Window         rootw = RootWindow(dpy, XDefaultScreen(dpy));
    Atom           wmSupportAtom = MX11Wrapper::XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False);
    Atom           type;
    int            format;
    unsigned long  numItems;
    unsigned long  bytesAfter;
    unsigned char *data = 0;

    if (MX11Wrapper::XGetWindowProperty(dpy, rootw, wmSupportAtom, 0, 1, False, XA_WINDOW,
                                       &type, &format, &numItems, &bytesAfter, &data) == Success) {
        if (data) {

            Window wid = *(reinterpret_cast<Window *>(data));
            MX11Wrapper::XFree(data);
            data = 0;

            Atom wmNameAtom = MX11Wrapper::XInternAtom(dpy, "WM_NAME", False);

            // Set error handler because window wid we got might not exist and
            // the following name query would fail.
            int (*previousHandler)(Display *, XErrorEvent *) = MX11Wrapper::XSetErrorHandler(handleXError);

            if (MX11Wrapper::XGetWindowProperty(dpy, wid, wmNameAtom, 0, 16, False, XA_STRING,
                                               &type, &format, &numItems, &bytesAfter, &data) == Success) {
                if (data) {
                    if (strcmp(reinterpret_cast<const char *>(data), MeeGoTouchWindowManagerName) == 0) {
                        retValue = true;
                    }

                    XFree(data);
                    data = 0;
                }
            }

            MX11Wrapper::XSetErrorHandler(previousHandler);
        }
    }

    return retValue;
}

void MWindowStatePrivate::_q_doVisibleChangedNotVisible()
{
    Q_Q(MWindowState);

    // Check that window is fully obscured and thumbnail is
    // not being seen in the switcher
    if (!visibleInSwitcherPropertySet && fullyObscured) {
        visible = false;
        emit q->visibleChanged();
    }
}

void MWindowStatePrivate::doViewModeChanged(MWindowState::ViewMode newViewMode)
{
    Q_Q(MWindowState);

    if (viewMode != newViewMode) {
        viewMode = newViewMode;
        emit q->viewModeChanged();
    }
}

void MWindowStatePrivate::doVisibleChanged(bool newVisible)
{
    Q_Q(MWindowState);

    // Emit visibleChanged() immediately if we are becaming
    // visible. Otherwise we need to wait a bit due to possible race
    // between _MEEGOTOUCH_VISIBLE_IN_SWITCHER and X11's visibility
    // events (that race condition causes oscillation).

    if (visible != newVisible) {
        if (newVisible) {
            visibleChangedTimer.stop();
            visible = true;
            emit q->visibleChanged();
            // Set the window active if it was before
            if (focus == FEFocusIn) {
                doActiveChanged(true);
            }
        } else {
            visibleChangedTimer.start();
            // Set the window not active
            doActiveChanged(false);
        }
    }
}

void MWindowState::_q_doVisibleChangedNotVisible()
{
    Q_D(MWindowState);
    d->_q_doVisibleChangedNotVisible();
}

#endif // Q_WS_X11

static MWindowState *self = 0;

MWindowState* MWindowState::instance()
{
    if (!self)
        self = new MWindowState();
    return self;
}

MWindowState::MWindowState(QObject *parent) :
    QObject(parent),
    d_ptr(new MWindowStatePrivate)
{
    Q_D(MWindowState);
    d->q_ptr = this;

#ifdef Q_WS_X11
    d->initVisibilityWatcher();
    d->initVisibleChangedTimer();
#endif // Q_WS_X11
}

MWindowState::ViewMode MWindowState::viewMode() const
{
    Q_D(const MWindowState);
    return d->viewMode;
}

QString MWindowState::viewModeString() const
{
    Q_D(const MWindowState);

    const char *s = 0;
    switch (d->viewMode) {
    case Fullsize:
        s = "Fullsize";
        break;
    case Thumbnail:
        s = "Thumbnail";
        break;
    }

    return QString::fromLatin1(s);
}

bool MWindowState::visible() const
{
    Q_D(const MWindowState);
    return d->visible;
}

bool MWindowState::active() const
{
    Q_D(const MWindowState);
    return d->active;
}

bool MWindowState::animating() const
{
    Q_D(const MWindowState);
    return d->animating;
}

void MWindowState::setAnimating(bool animatingStatus)
{
    Q_D(MWindowState);
    if (animatingStatus != d->animating) {
        d->animating = animatingStatus;
        emit animatingChanged();
    }
}
