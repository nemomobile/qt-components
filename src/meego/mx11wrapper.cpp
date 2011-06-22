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

#include "mx11wrapper.h"

#ifdef Q_WS_X11

Atom MX11Wrapper::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    return ::XInternAtom(display, atom_name, only_if_exists);
}

Status MX11Wrapper::XGetWindowAttributes(Display *display, Window w,
                                         XWindowAttributes *window_attributes_return)
{
    return ::XGetWindowAttributes(display, w, window_attributes_return);
}

int MX11Wrapper::XGetWindowProperty(Display *display, Window w, Atom property,
                       long long_offset, long long_length, Bool del,
                       Atom req_type, Atom *actual_type_return,
                       int *actual_format_return,
                       unsigned long *nitems_return, unsigned long *bytes_after_return,
                       unsigned char **prop_return)
{
    return ::XGetWindowProperty(display, w, property, long_offset, long_length, del,
                                req_type, actual_type_return,
                                actual_format_return,
                                nitems_return, bytes_after_return,
                                prop_return);
}

int MX11Wrapper::XFree(void *data)
{
    return ::XFree(data);
}

XErrorHandler MX11Wrapper::XSetErrorHandler(XErrorHandler handler)
{
    return ::XSetErrorHandler(handler);
}

int MX11Wrapper::XDefaultScreen(Display *display)
{
    return ::XDefaultScreen(display);
}

int MX11Wrapper::XChangeWindowAttributes(Display *display, Window w, unsigned long valuemask,
                                         XSetWindowAttributes *attributes)
{
    return ::XChangeWindowAttributes(display, w, valuemask, attributes);
}

#endif // Q_WS_X11
