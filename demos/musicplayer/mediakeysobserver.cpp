/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
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
