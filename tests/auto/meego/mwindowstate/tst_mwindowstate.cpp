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

#define UNIT_TEST 

#include <QWidget>
#include <QtTest/QtTest>
#include <QSignalSpy>
#include "mwindowstate.h"
#include "mwindowstate_p.h"

#define ATOM_SWITCHER 1
#define ATOM_DUMMY    2

bool visibleInSwitcherProperty = true;

#ifdef Q_WS_X11

Atom MX11Wrapper::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    Q_UNUSED(only_if_exists);
    Q_UNUSED(display);

    if (strcmp(atom_name, "_MEEGOTOUCH_VISIBLE_IN_SWITCHER") == 0) {
        return ATOM_SWITCHER;
    }
    return 0;
}

int MX11Wrapper::XFree(void *data)
{
    delete [](unsigned char *)data;
    return 0;
}

int MX11Wrapper::XDefaultScreen(Display * display)
{
    return ::XDefaultScreen(display);
}

int MX11Wrapper::XGetWindowProperty(Display *display, Window w, Atom property,
                                    long long_offset, long long_length, Bool del,
                                    Atom req_type, Atom *actual_type_return,
                                    int *actual_format_return, unsigned long *nitems_return,
                                    unsigned long *bytes_after_return,
                                    unsigned char **prop_return)
{
    Q_UNUSED(display);
    Q_UNUSED(w);
    Q_UNUSED(long_offset);
    Q_UNUSED(long_length);
    Q_UNUSED(del);
    Q_UNUSED(req_type);
    Q_UNUSED(actual_type_return);
    Q_UNUSED(actual_format_return);
    Q_UNUSED(bytes_after_return);

    if (property == ATOM_SWITCHER) {
        *nitems_return = 1;
        *prop_return = new unsigned char[sizeof(bool)];
        *prop_return[0] = visibleInSwitcherProperty;
        return Success;
    }
    return BadAtom;
}

XErrorHandler MX11Wrapper::XSetErrorHandler(XErrorHandler handler)
{
    return ::XSetErrorHandler(handler);
}

Status MX11Wrapper::XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return)
{
    return ::XGetWindowAttributes(display, w, window_attributes_return);
}

int MX11Wrapper::XChangeWindowAttributes(Display *display, Window w, unsigned long valuemask, XSetWindowAttributes *attributes)
{
    return ::XChangeWindowAttributes(display, w, valuemask, attributes);
}

#endif //Q_WS_X11

class tst_MWindowState : public QObject
{
    Q_OBJECT

public:
    tst_MWindowState();
    virtual ~tst_MWindowState();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
#ifdef Q_WS_X11
    void testVisibilityUnobscured();
    void testVisibilityFullyObscured();
    void testVisibilityPartiallyObscured();
    void testFullsizeVisibleToThumbnailVisible();
    void testThumbnailVisibleToThumbnailHidden();
    void testFullsizeHiddenToFullsizeVisible();
    void testFullsizeVisibleToFullsizeHidden();
    void testThumbnailHiddenToThumbnailVisible();
    void testThumbnailVisibleToFullsizeVisible();
    void testFullsizeHiddenToThumbnailVisible();
    void testFullsizeVisibleToThumbnailHidden();
    void testThumbnailHiddenToFullsizeVisible();
    void testWindowActive();
#endif //Q_WS_X11

private:
#ifdef Q_WS_X11
    void setListenedWindowId();
    void sendVisibilityEvent(int state);
    void sendPropertyEvent(Atom atom);
    void sendFocusChangeEvent(int type);
    void sleepAndProcessEvents();
    // Real window is necessary for the unit test
    // Otherwise X would say "Bad window"
    QWidget win;
    Window winId;
#endif //Q_WS_X11
    MWindowState * testObject;
};

tst_MWindowState::tst_MWindowState() :
        testObject(0)
{
#ifdef Q_WS_X11
    winId = win.winId();
#endif //Q_WS_X11
}

tst_MWindowState::~tst_MWindowState()
{
}

void tst_MWindowState::initTestCase()
{
}

void tst_MWindowState::cleanupTestCase()
{
}

void tst_MWindowState::init()
{
#ifdef Q_WS_X11
    testObject = MWindowState::instance();
    setListenedWindowId();
#endif //Q_WS_X11
}

void tst_MWindowState::cleanup()
{
#ifdef Q_WS_X11
    delete testObject;
#endif //Q_WS_X11
}

#ifdef Q_WS_X11

void tst_MWindowState::sleepAndProcessEvents()
{
    sleep(1.2);
    QApplication::processEvents();
}

void tst_MWindowState::sendPropertyEvent(Atom atom)
{
    XPropertyEvent event;
    event.type = PropertyNotify;
    event.window = winId;
    event.atom = atom;
    event.state = PropertyNewValue;
    testObject->d_ptr->handleXPropertyEvent(&event);
}

void tst_MWindowState::sendVisibilityEvent(int state)
{
    XVisibilityEvent event;
    event.type = VisibilityNotify;
    event.window = winId;
    event.state = state;
    event.send_event = 1;
    testObject->d_ptr->handleXVisibilityEvent(&event);
}

void tst_MWindowState::sendFocusChangeEvent(int type)
{
    XFocusChangeEvent event;
    event.type = type;
    event.mode = NotifyNormal;
    event.window = winId;
    testObject->d_ptr->handleXFocusChangeEvent(&event);
}

void tst_MWindowState::setListenedWindowId()
{
    // Set the window id which the tested class is listening.
    // MWindowState gets the window id from the first
    // XPropertyChangeEvent.
    sendPropertyEvent(ATOM_DUMMY);
}

void tst_MWindowState::testVisibilityUnobscured()
{
    sendVisibilityEvent(VisibilityFullyObscured);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QCOMPARE(testObject->visible(), false);
    sendVisibilityEvent(VisibilityUnobscured);
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(visibilitySpy.count(), 1);
}

void tst_MWindowState::testVisibilityFullyObscured()
{
    sendVisibilityEvent(VisibilityUnobscured);
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QCOMPARE(testObject->visible(), true);
    sendVisibilityEvent(VisibilityFullyObscured);
    sleepAndProcessEvents();
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(visibilitySpy.count(), 1);
}

void tst_MWindowState::testVisibilityPartiallyObscured()
{
    sendVisibilityEvent(VisibilityFullyObscured);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QCOMPARE(testObject->visible(), false);
    sendVisibilityEvent(VisibilityPartiallyObscured);
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(visibilitySpy.count(), 1);
}

// Fullsize application -> switcher and switcher visible
void tst_MWindowState::testFullsizeVisibleToThumbnailVisible()
{
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sendVisibilityEvent(VisibilityUnobscured);
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    sendVisibilityEvent(VisibilityFullyObscured);
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    QCOMPARE(visibilitySpy.count(), 0);
    QCOMPARE(viewModeSpy.count(), 1);
}

// Application visible in switcher -> switcher not shown anymore
void tst_MWindowState::testThumbnailVisibleToThumbnailHidden()
{
    sendVisibilityEvent(VisibilityFullyObscured);
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    QCOMPARE(visibilitySpy.count(), 1);
    QCOMPARE(viewModeSpy.count(), 0);
}

// Window on top of this one is closed
void tst_MWindowState::testFullsizeHiddenToFullsizeVisible()
{
    sendVisibilityEvent(VisibilityFullyObscured);
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    sendVisibilityEvent(VisibilityUnobscured);
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    QCOMPARE(visibilitySpy.count(), 1);
    QCOMPARE(viewModeSpy.count(), 0);
}

// Another window comes on top of this one
void tst_MWindowState::testFullsizeVisibleToFullsizeHidden()
{
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sendVisibilityEvent(VisibilityUnobscured);
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    sendVisibilityEvent(VisibilityFullyObscured);
    sleepAndProcessEvents();
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    QCOMPARE(visibilitySpy.count(), 1);
    QCOMPARE(viewModeSpy.count(), 0);
}

// Application in switcher but switcher not visible -> switcher visible
void tst_MWindowState::testThumbnailHiddenToThumbnailVisible()
{
    sendVisibilityEvent(VisibilityFullyObscured);
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    QCOMPARE(visibilitySpy.count(), 1);
    QCOMPARE(viewModeSpy.count(), 0);
}

// Application maximized from switcher
void tst_MWindowState::testThumbnailVisibleToFullsizeVisible()
{
    sendVisibilityEvent(VisibilityFullyObscured);
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    sendVisibilityEvent(VisibilityUnobscured);
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    QCOMPARE(visibilitySpy.count(), 0);
    QCOMPARE(viewModeSpy.count(), 1);
}

// Hidden application moved to switcher which is visible
void tst_MWindowState::testFullsizeHiddenToThumbnailVisible()
{
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sendVisibilityEvent(VisibilityFullyObscured);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    QCOMPARE(visibilitySpy.count(), 1);
    QCOMPARE(viewModeSpy.count(), 1);
}

// Visible application moved to switcher which is not visible visible
void tst_MWindowState::testFullsizeVisibleToThumbnailHidden()
{
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sendVisibilityEvent(VisibilityUnobscured);
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    sendVisibilityEvent(VisibilityFullyObscured);
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    QCOMPARE(visibilitySpy.count(), 1);
    QCOMPARE(viewModeSpy.count(), 1);
}

// Application hidden & in switcher maximized
void tst_MWindowState::testThumbnailHiddenToFullsizeVisible()
{
    sendVisibilityEvent(VisibilityFullyObscured);
    visibleInSwitcherProperty = true;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sleepAndProcessEvents();
    QSignalSpy visibilitySpy(testObject, SIGNAL(visibleChanged()));
    QSignalSpy viewModeSpy(testObject, SIGNAL(viewModeChanged()));
    QCOMPARE(testObject->visible(), false);
    QCOMPARE(testObject->viewMode(), MWindowState::Thumbnail);
    QCOMPARE(testObject->viewModeString(), QString("Thumbnail"));
    visibleInSwitcherProperty = false;
    sendPropertyEvent(ATOM_SWITCHER);
    sendVisibilityEvent(VisibilityUnobscured);
    QCOMPARE(testObject->visible(), true);
    QCOMPARE(testObject->viewMode(), MWindowState::Fullsize);
    QCOMPARE(testObject->viewModeString(), QString("Fullsize"));
    QCOMPARE(visibilitySpy.count(), 1);
    QCOMPARE(viewModeSpy.count(), 1);
}

void tst_MWindowState::testWindowActive()
{
    QSignalSpy activeSpy(testObject, SIGNAL(activeChanged()));
    sendFocusChangeEvent(FocusIn);
    QCOMPARE(testObject->active(), true);
    sendFocusChangeEvent(FocusOut);
    QCOMPARE(testObject->active(), false);
    sendFocusChangeEvent(FocusIn);
    QCOMPARE(testObject->active(), true);
    QCOMPARE(activeSpy.count(), 3);
}
#endif //Q_WS_X11

QTEST_MAIN(tst_MWindowState)
#include "tst_mwindowstate.moc"
