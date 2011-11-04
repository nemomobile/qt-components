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

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QStringList>

class FileAccess : public QObject
{
    Q_OBJECT

public:
    explicit FileAccess(QObject *parent = 0);

    Q_INVOKABLE QStringList qmlFileNames(const QString &path) const;
    Q_INVOKABLE QStringList qmlFilePaths(const QString &path) const;
    Q_INVOKABLE QStringList qmlPaths() const;
};

class Settings : public QObject
{
    Q_OBJECT
    Q_ENUMS(IndicatorIds)
public:

    enum IndicatorIds {
        IndicatorEMail = 2,
        IndicatorSecuredConnection = 6,
        IndicatorBluetooth = 12,
        IndicatorUsb = 28
    };

    explicit Settings(QObject *parent = 0);
    ~Settings();

    Q_INVOKABLE void clearClipboard() const;
    Q_INVOKABLE void setOrientation(int orientation);
    Q_INVOKABLE int orientation() const;
    Q_INVOKABLE void setIndicatorState(int indicatorId, bool on) const;
    Q_INVOKABLE void setTime(int hours, int minutes);
    // 1 - resize-based, 0 - sensors/transform-based
    Q_INVOKABLE void setOrientationMethod(int orientationMethod);
    Q_INVOKABLE int orientationMethod() const;
};

class LayoutDirectionSetter : public QObject
{
    Q_OBJECT

public:
    explicit LayoutDirectionSetter(QObject *parent = 0);

    Q_INVOKABLE void setLayoutDirection(int direction);
};


#endif // UTILS_H
