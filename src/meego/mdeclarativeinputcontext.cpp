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
#include <QInputMethodEvent>
#include <QTimer>
#include <QRect>
#include <QClipboard>
#include <QDebug>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QInputContext>
#endif

#ifdef HAVE_MALIIT
#include <maliit/inputmethod.h>
#include <maliit/preeditinjectionevent.h>
#elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QInputMethod>
#include <QGuiApplication>
#endif

#include "mdeclarativeinputcontext.h"

class MDeclarativeInputContextPrivate
{
public:
    MDeclarativeInputContextPrivate(MDeclarativeInputContext *qq);
    ~MDeclarativeInputContextPrivate();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    void _q_updateKeyboardRectangle();
#endif
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
    QQmlComponent *sipVkbComponent;
    QQuickItem *sipVkbTextField;
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
#elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    simulateSip = false;
    QObject::connect(qApp->inputMethod(), SIGNAL(keyboardRectangleChanged()),
                     q, SLOT(_q_updateKeyboardRectangle()));
#endif
}

MDeclarativeInputContextPrivate::~MDeclarativeInputContextPrivate()
{
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void MDeclarativeInputContextPrivate::_q_updateKeyboardRectangle()
{
    _q_sipChanged(qApp->inputMethod()->keyboardRectangle().toRect());
}
#endif

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
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    d->microFocus = qApp->inputMethod()->cursorRectangle();
#else
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
#endif
}

MDeclarativeInputContext::MDeclarativeInputContext(QQuickItem *parent)
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
#if defined(HAVE_MALIIT) || QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    return d->sipRect;
#else
    return QRect(d->sipSimulationRect);
#endif
}

void MDeclarativeInputContext::reset()
{
#if defined(HAVE_MALIIT) || QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    qApp->inputMethod()->reset();
#else
    QInputContext *ic = qApp->inputContext();
    if (ic) ic->reset();
#endif
}

void MDeclarativeInputContext::update()
{
#if defined(HAVE_MALIIT) || QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    qApp->inputMethod()->update(Qt::ImQueryAll);
#else
    QInputContext *ic = qApp->inputContext();
    if (ic) ic->update();
#endif
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

QQmlComponent *MDeclarativeInputContext::customSoftwareInputPanelComponent() const
{
    return d->sipVkbComponent;
}

void MDeclarativeInputContext::setCustomSoftwareInputPanelComponent(QQmlComponent * component)
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

QQuickItem *MDeclarativeInputContext::customSoftwareInputPanelTextField() const
{
    return d->sipVkbTextField;
}

#if QT_VERSION >= 0x050000
void MDeclarativeInputContext::setCustomSoftwareInputPanelTextField(QQuickItem *item)
#else
void MDeclarativeInputContext::setCustomSoftwareInputPanelTextField(QQuickItem *item)
#endif
{
    if(d->sipVkbTextField != item) {
#if QT_VERSION >= 0x050000
        d->sipVkbTextField = static_cast<QQuickItem*>(item);
#else
        d->sipVkbTextField = item;
#endif
        emit customSoftwareInputPanelTextFieldChanged();
    }
}

QQuickItem *MDeclarativeInputContext::targetInputFor(QQmlComponent *sipVkbComponent)
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
