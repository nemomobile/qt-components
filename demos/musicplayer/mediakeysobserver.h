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

#ifndef MEDIAKEYSOBSERVER_H
#define MEDIAKEYSOBSERVER_H

#include <QObject>

#ifdef Q_OS_SYMBIAN

#include <remconcoreapitarget.h>
#include <remconcoreapitargetobserver.h>
#include <remconinterfaceselector.h>

class MediaKeysObserver : public QObject, public MRemConCoreApiTargetObserver
{
    Q_OBJECT
    Q_ENUMS(MediaKey)

public:
    enum MediaKey {
        EVolIncKey,
        EVolDecKey,
        EPlayPauseKey,
        EStopKey,
        EBackwardKey,
        EForwardKey,
        EFastForwardKey,
        EFastRewindKey
    };

    MediaKeysObserver(QObject *parent = 0);
    virtual ~MediaKeysObserver();

protected:
    void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);

signals:
    void mediaKeyClicked(int key);
    void mediaKeyPressed(int key);
    void mediaKeyReleased(int key);

private:
    CRemConInterfaceSelector *interfaceSelector;
    CRemConCoreApiTarget *coreTarget;
};

#else // Q_OS_SYMBIAN

// Dummy implementation for non-symbian platforms
class MediaKeysObserver : public QObject
{
    Q_OBJECT
    Q_ENUMS(MediaKey)

public:
    enum MediaKey {
        EVolIncKey,
        EVolDecKey,
        EPlayPauseKey,
        EStopKey,
        EBackwardKey,
        EForwardKey,
        EFastForwardKey,
        EFastRewindKey
    };

signals:
    void mediaKeyClicked(int key);
    void mediaKeyPressed(int key);
    void mediaKeyReleased(int key);
};

#endif // Q_OS_SYMBIAN

#endif // MEDIAKEYSOBSERVER_H

