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

#include <QApplication>
#include <QMouseEvent>
#include <QInputContext>
#include <QInputMethodEvent>
#include <QTimer>
#include <QRect>
#include <QClipboard>
#include <QDebug>

#ifdef HAVE_MALIIT
#include <maliit/inputmethod.h>
#include <maliit/preeditinjectionevent.h>
#endif

#include "mdeclarativeinputcontext.h"

class MDeclarativeInputContextPrivate
{
public:
    MDeclarativeInputContextPrivate(MDeclarativeInputContext *qq);
    ~MDeclarativeInputContextPrivate();

    void _q_sipChanged(const QRect &);
    void _q_checkMicroFocusHint();

    MDeclarativeInputContext *q;

    bool sipVisible;

    QRect sipRect;

    bool simulateSip;
    bool customSoftwareInputPanelVisible;
    QRect sipSimulationRect;
    QRect sipDefaultSimulationRect;

    QRectF microFocus;
    QVariant sipEvent;
    QDeclarativeComponent *sipVkbComponent;
    QDeclarativeItem *sipVkbTextField;
};

MDeclarativeInputContextPrivate::MDeclarativeInputContextPrivate(MDeclarativeInputContext *qq)
    : q(qq)
    , sipVisible(false)
    , sipRect(QRect())
    , simulateSip(true)
    , customSoftwareInputPanelVisible(false)
    , sipSimulationRect(QRect())
    , sipDefaultSimulationRect(QRect(0, 0, 240, 240))
    , sipVkbComponent(NULL)
    , sipVkbTextField(NULL)
{
#ifdef HAVE_MALIIT
    simulateSip = false;
    QObject::connect(Maliit::InputMethod::instance(), SIGNAL(areaChanged(const QRect &)),
                     q, SLOT(_q_sipChanged(const QRect &)));
#endif
}

MDeclarativeInputContextPrivate::~MDeclarativeInputContextPrivate()
{
}

void MDeclarativeInputContextPrivate::_q_sipChanged(const QRect &rect)
{
    bool visible = !rect.isEmpty();

    if (sipRect != rect) {
        sipRect = rect;
        q->update();
        emit q->softwareInputPanelRectChanged();
    }

    if (visible != sipVisible) {
        sipVisible = visible;
        q->updateMicroFocus();
        emit q->softwareInputPanelVisibleChanged();
    }
}

void MDeclarativeInputContext::updateMicroFocus()
{
    if (QWidget *widget = QApplication::focusWidget()) {
        QVariant v = widget->inputMethodQuery(Qt::ImMicroFocus);
        if (!v.toRectF().isValid()) {
            d->microFocus = QRectF(-1, -1, -1, -1);
            return;
        }
        QRectF mf = v.toRectF();
        if (mf != d->microFocus) {
            d->microFocus = mf;
        }
    } else {
        d->microFocus = QRectF(-1, -1, -1, -1);
    }
}

MDeclarativeInputContext::MDeclarativeInputContext(QDeclarativeItem *parent)
    : QObject(parent),
      d(new MDeclarativeInputContextPrivate(this))
{
}

MDeclarativeInputContext::~MDeclarativeInputContext()
{
    delete d;
}

void MDeclarativeInputContext::setKeyboardOrientation(MDeclarativeScreen::Orientation orientation)
{
#ifdef HAVE_MALIIT
    if (Maliit::InputMethod::instance()->area().isEmpty()) {
        Maliit::OrientationAngle angle = Maliit::Angle0;
        switch(orientation) {
        case MDeclarativeScreen::Landscape:
            angle = Maliit::Angle0;
            break;
        case MDeclarativeScreen::PortraitInverted:
            angle = Maliit::Angle90;
            break;
        case MDeclarativeScreen::LandscapeInverted:
            angle = Maliit::Angle180;
            break;
        case MDeclarativeScreen::Portrait:
            angle = Maliit::Angle270;
            break;
        }

        Maliit::InputMethod::instance()->setOrientationAngle(angle);
    }
#else
    Q_UNUSED(orientation);
#endif
}

QRectF MDeclarativeInputContext::microFocus() const
{
    return d->microFocus;
}

bool MDeclarativeInputContext::softwareInputPanelVisible() const
{
    return d->sipVisible;
}

QRect MDeclarativeInputContext::softwareInputPanelRect() const
{
#ifdef HAVE_MALIIT
    return d->sipRect;
#else
    return QRect(d->sipSimulationRect);
#endif
}

void MDeclarativeInputContext::reset()
{
    QInputContext *ic = qApp->inputContext();
    if (ic) ic->reset();
}

void MDeclarativeInputContext::update()
{
    QInputContext *ic = qApp->inputContext();
    if (ic) ic->update();
}


QVariant MDeclarativeInputContext::softwareInputPanelEvent() const
{
    return d->sipEvent;
}

void MDeclarativeInputContext::setSoftwareInputPanelEvent(const QVariant& event)
{
    d->sipEvent = event;
    emit softwareInputPanelEventChanged();
}

QDeclarativeComponent *MDeclarativeInputContext::customSoftwareInputPanelComponent() const
{
    return d->sipVkbComponent;
}

void MDeclarativeInputContext::setCustomSoftwareInputPanelComponent(QDeclarativeComponent * component)
{
    if(d->sipVkbComponent != component) {
        d->sipVkbComponent = component;
        emit customSoftwareInputPanelComponentChanged();
    }
}

bool MDeclarativeInputContext::customSoftwareInputPanelVisible() const
{
    return d->customSoftwareInputPanelVisible;
}

void MDeclarativeInputContext::setCustomSoftwareInputPanelVisible(bool visible)
{
    if(d->customSoftwareInputPanelVisible != visible) {
        d->customSoftwareInputPanelVisible = visible;
        emit customSoftwareInputPanelVisibleChanged();
    }
}

QDeclarativeItem * MDeclarativeInputContext::customSoftwareInputPanelTextField() const
{
    return d->sipVkbTextField;
}

void MDeclarativeInputContext::setCustomSoftwareInputPanelTextField(QDeclarativeItem * item)
{
    if(d->sipVkbTextField != item) {
        d->sipVkbTextField = item;
        emit customSoftwareInputPanelTextFieldChanged();
    }
}

QDeclarativeItem * MDeclarativeInputContext::targetInputFor(QDeclarativeComponent *sipVkbComponent)
{
    if(sipVkbComponent)
        return customSoftwareInputPanelTextField();

    return NULL;
}

void MDeclarativeInputContext::simulateSipOpen()
{
    if(d->simulateSip) {
        d->sipSimulationRect = d->sipDefaultSimulationRect;
        d->_q_sipChanged(d->sipSimulationRect);
    }
}

void MDeclarativeInputContext::simulateSipClose()
{
    if(d->simulateSip) {
        d->sipSimulationRect = QRect();
        d->_q_sipChanged(d->sipSimulationRect);
    }
}

bool MDeclarativeInputContext::setPreeditText(const QString &newPreedit, int eventCursorPosition, int replacementStart, int replacementLength)
{
#ifdef HAVE_MALIIT
    QInputContext *ic = qApp->inputContext();
    if (ic) {
        Maliit::PreeditInjectionEvent event(newPreedit, eventCursorPosition);
        event.setReplacement(replacementStart, replacementLength);
        return ic->filterEvent(&event);
    }
#else
    Q_UNUSED(newPreedit)
    Q_UNUSED(eventCursorPosition)
    Q_UNUSED(replacementStart)
    Q_UNUSED(replacementLength)
#endif
    return false;
}

void MDeclarativeInputContext::clearClipboard()
{
    if (qApp->clipboard())
        qApp->clipboard()->clear();
}

#include "moc_mdeclarativeinputcontext.cpp"
