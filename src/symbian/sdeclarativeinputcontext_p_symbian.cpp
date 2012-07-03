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

#include "sdeclarativeinputcontext.h"
#include "sdeclarativeinputcontext_p_symbian.h"
#include "sdeclarativetouchinput.h"
#include "sdeclarativescreen.h"

SDeclarativeInputContextPrivate::SDeclarativeInputContextPrivate(SDeclarativeInputContext *qq, SDeclarativeScreen *screen)
    : q_ptr(qq)
    , m_screen(screen)
    , m_touchInput(NULL)
{
    Q_Q(SDeclarativeInputContext);
    QT_TRAP_THROWING(m_touchInput = CTouchInput::NewL(*this));
    QObject::connect(screen, SIGNAL(currentOrientationChanged()), q, SIGNAL(heightChanged()));
}

SDeclarativeInputContextPrivate::~SDeclarativeInputContextPrivate()
{
    delete m_touchInput;
}

qreal SDeclarativeInputContextPrivate::height() const
{
    qreal tmpHeight = 0;
    if (m_screen->height() >= m_screen->width())
        tmpHeight = m_touchInput->PortraitHeight();
    else
        tmpHeight = m_touchInput->LandscapeHeight();

    if (tmpHeight != KErrNotReady)
        return tmpHeight;

    return m_screen->height() * 1/2;
}

bool SDeclarativeInputContextPrivate::visible() const
{
    return m_touchInput->Visible();
}

void SDeclarativeInputContextPrivate::VisibleChanged()
{
    Q_Q(SDeclarativeInputContext);
    q->emit visibleChanged();
}

void SDeclarativeInputContextPrivate::HeightChanged()
{
    Q_Q(SDeclarativeInputContext);
    q->emit heightChanged();
}
