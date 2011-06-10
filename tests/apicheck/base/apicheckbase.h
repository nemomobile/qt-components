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

#ifndef APICHECKBASE_H
#define APICHECKBASE_H

#include <QtTest/QtTest>
#include <QtCore/QObject>
#include <QtCore/QVariant>


class QDeclarativeEngine;

class ApiCheckBase : public QObject
{
    Q_OBJECT

public:
    ApiCheckBase(QDeclarativeEngine *engine, const QString &module);
    virtual ~ApiCheckBase();

protected:
    void init(const QString &name, const QString &body = QString());
    void initContextProperty(const QString &name);
    void validateProperty(const QString &propertyName, const QString &typeName) const;
    void validateProperty(const QString &propertyName, QVariant::Type propertyType,
                          const QVariant &value = QVariant()) const;
    void validateDeclarativeProperty(const QString &name, const QString &typeName) const;
    void validateEnumProperty(const QString &propertyName, const QVariant &value = QVariant()) const;
    void validateFlagProperty(const QString &propertyName, const QVariant &value = QVariant()) const;
    void validateSignal(const char *signalName) const;
    void validateMethod(const char *methodName) const;

private:
    QMetaProperty metaProperty(const QString &name) const;

    QString m_name;
    QString m_module;
    QObject *m_object;
    QDeclarativeEngine *m_engine;
};

#endif
