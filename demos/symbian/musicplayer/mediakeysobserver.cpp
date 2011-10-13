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

#include "mediakeysobserver.h"

MediaKeysObserver::MediaKeysObserver(QObject *parent) : QObject(parent)
{
    interfaceSelector = CRemConInterfaceSelector::NewL();
    coreTarget = CRemConCoreApiTarget::NewL(*interfaceSelector, *this);
    interfaceSelector->OpenTargetL();
}

MediaKeysObserver::~MediaKeysObserver()
{
    delete interfaceSelector;
}

void MediaKeysObserver::MrccatoCommand(TRemConCoreApiOperationId operationId,
                                       TRemConCoreApiButtonAction buttonAct)
{
    switch (operationId) {
    case ERemConCoreApiPausePlayFunction:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EPlayPauseKey);
        break;

    case ERemConCoreApiStop:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EStopKey);
        break;

    case ERemConCoreApiRewind:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EFastRewindKey);
        break;

    case ERemConCoreApiForward:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EForwardKey);
        break;

    case ERemConCoreApiVolumeUp:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EVolIncKey);
        else if (buttonAct == ERemConCoreApiButtonPress)
            emit mediaKeyPressed(MediaKeysObserver::EVolIncKey);
        else if (buttonAct == ERemConCoreApiButtonRelease)
            emit mediaKeyReleased(MediaKeysObserver::EVolIncKey);
        break;

    case ERemConCoreApiVolumeDown:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EVolDecKey);
        else if (buttonAct == ERemConCoreApiButtonPress)
            emit mediaKeyPressed(MediaKeysObserver::EVolDecKey);
        else if (buttonAct == ERemConCoreApiButtonRelease)
            emit mediaKeyReleased(MediaKeysObserver::EVolDecKey);
        break;

    case ERemConCoreApiFastForward:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EFastForwardKey);
        else if (buttonAct == ERemConCoreApiButtonPress)
            emit mediaKeyPressed(MediaKeysObserver::EFastForwardKey);
        else if (buttonAct == ERemConCoreApiButtonRelease)
            emit mediaKeyReleased(MediaKeysObserver::EFastForwardKey);
        break;

    case ERemConCoreApiBackward:
        if (buttonAct == ERemConCoreApiButtonClick)
            emit mediaKeyClicked(MediaKeysObserver::EBackwardKey);
        else if (buttonAct == ERemConCoreApiButtonPress)
            emit mediaKeyPressed(MediaKeysObserver::EBackwardKey);
        else if (buttonAct == ERemConCoreApiButtonRelease)
            emit mediaKeyReleased(MediaKeysObserver::EBackwardKey);
        break;

    default:
        break;
    }
}
