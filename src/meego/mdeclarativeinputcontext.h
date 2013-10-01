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

#ifndef MDECLARATIVEINPUTCONTEXT_H
#define MDECLARATIVEINPUTCONTEXT_H

#include <QQuickItem>
#include <QEvent>

#include "mdeclarativescreen.h"

class MDeclarativeInputContextPrivate;

class MDeclarativeInputContext : public QObject
{
    Q_OBJECT

public:
    MDeclarativeInputContext(QQuickItem *parent = 0);
    virtual ~MDeclarativeInputContext();

    Q_PROPERTY(bool softwareInputPanelVisible READ softwareInputPanelVisible NOTIFY softwareInputPanelVisibleChanged FINAL)
    Q_PROPERTY(QRect softwareInputPanelRect READ softwareInputPanelRect NOTIFY softwareInputPanelRectChanged FINAL)
    Q_PROPERTY(QRectF microFocus READ microFocus NOTIFY microFocusChanged) //TODO: will be removed on w13 - remove "NOTIFY microFocusChanged"
    Q_PROPERTY(QVariant softwareInputPanelEvent READ softwareInputPanelEvent WRITE setSoftwareInputPanelEvent NOTIFY softwareInputPanelEventChanged)
    Q_PROPERTY(QQmlComponent* customSoftwareInputPanelComponent READ customSoftwareInputPanelComponent WRITE setCustomSoftwareInputPanelComponent NOTIFY customSoftwareInputPanelComponentChanged)
    Q_PROPERTY(QQuickItem* customSoftwareInputPanelTextField WRITE setCustomSoftwareInputPanelTextField NOTIFY customSoftwareInputPanelTextFieldChanged)
    Q_PROPERTY(bool customSoftwareInputPanelVisible READ customSoftwareInputPanelVisible WRITE setCustomSoftwareInputPanelVisible NOTIFY customSoftwareInputPanelVisibleChanged)
    QRectF microFocus() const;

    Q_INVOKABLE void updateMicroFocus();

    Q_INVOKABLE void reset();
    Q_INVOKABLE void update();
    Q_INVOKABLE bool setPreeditText(const QString &newPreedit, int eventCursorPosition, int replacementStart, int replacementLength);

    Q_INVOKABLE QQuickItem * targetInputFor(QQmlComponent *customSoftwareInputPanelComponent);
    Q_INVOKABLE void simulateSipOpen();
    Q_INVOKABLE void simulateSipClose();

    Q_INVOKABLE void clearClipboard();

    bool softwareInputPanelVisible() const;
    QRect softwareInputPanelRect() const;

    QVariant softwareInputPanelEvent() const;
    void setSoftwareInputPanelEvent(const QVariant& event);

    QQmlComponent *customSoftwareInputPanelComponent() const;
    void setCustomSoftwareInputPanelComponent(QQmlComponent *component);

    static void setKeyboardOrientation(MDeclarativeScreen::Orientation orientation);

    bool customSoftwareInputPanelVisible() const;
    void setCustomSoftwareInputPanelVisible(bool visible);

    QQuickItem *customSoftwareInputPanelTextField() const;
    void setCustomSoftwareInputPanelTextField(QQuickItem *item);

Q_SIGNALS:
    void softwareInputPanelVisibleChanged();
    void softwareInputPanelRectChanged();
    void minimizedChanged();
    void microFocusChanged();  //TODO: will be removed on w13
    void softwareInputPanelEventChanged();
    void customSoftwareInputPanelComponentChanged();
    void customSoftwareInputPanelVisibleChanged();
    void customSoftwareInputPanelTextFieldChanged();

private:
    Q_DISABLE_COPY(MDeclarativeInputContext)

    Q_PRIVATE_SLOT(d, void _q_updateKeyboardRectangle())
    Q_PRIVATE_SLOT(d, void _q_sipChanged(const QRect &))

    friend class MDeclarativeInputContextPrivate;
    MDeclarativeInputContextPrivate *d;
};

QML_DECLARE_TYPE(MDeclarativeInputContext)
#endif
