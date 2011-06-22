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

#include "apicheckbase.h"
#include <QtDeclarative/qdeclarativeengine.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtDeclarative/qdeclarativecontext.h>

ApiCheckBase::ApiCheckBase(QDeclarativeEngine *engine, const QString &module)
    : QObject(),
      m_module(module),
      m_object(0),
      m_engine(engine)
{

}

ApiCheckBase::~ApiCheckBase()
{
    if (m_object)
        delete m_object;
}

void ApiCheckBase::init(const QString &name, const QString &body)
{
    const QString block = QString("import QtQuick 1.1\nimport %1\n%2 { %3 }")
        .arg(m_module).arg(name).arg(body);

    QDeclarativeComponent *component = new QDeclarativeComponent(m_engine);
    component->setData(block.toLatin1(), QUrl());

    m_name = name;
    m_object = component->create();

    QVERIFY2(m_object, qPrintable(QString("Could not create %1").arg(name)));
}

void ApiCheckBase::initContextProperty(const QString &name)
{
    const QString block = QString("import QtQuick 1.1\nimport %1\nItem{}").arg(m_module);

    // Make sure the root context is initialized properly
    QDeclarativeComponent *component = new QDeclarativeComponent(m_engine);
    component->setData(block.toLatin1(), QUrl());

    m_name = name;
    m_object = qVariantValue<QObject *>(m_engine->rootContext()->contextProperty(name));

    QVERIFY2(m_object, qPrintable(QString("Could not find %1").arg(name)));
}

void ApiCheckBase::validateProperty(const QString &name, const QString &typeName) const
{
    QMetaProperty property = metaProperty(name);

    QVERIFY2(property.isValid(),
             qPrintable(QString("property '%1.%2' does not exist").arg(m_name, name)));

    QVERIFY2(property.typeName() == typeName,
             qPrintable(QString("property '%1.%2' has invalid type (expected: %3, had: %4)")
                        .arg(m_name, name, typeName, property.typeName())));
}

void ApiCheckBase::validateProperty(const QString &name, QVariant::Type type, const QVariant &value) const
{
    QMetaProperty property = metaProperty(name);

    QVERIFY2(property.isValid(),
             qPrintable(QString("property '%1.%2' does not exist").arg(m_name, name)));

    QVERIFY2(property.type() == type,
             qPrintable(QString("property '%1.%2' has invalid type (expected: %3, had: %4)")
                        .arg(m_name, name, QVariant::typeToName(type), property.typeName())));

    if (value.isValid()) {
        QVariant variant = property.read(m_object);

        QVERIFY2(variant == value,
                 qPrintable(QString("property '%1.%2' has wrong default value (expected: %3, had: %4)")
                            .arg(m_name, name, value.toString(), variant.toString())));
    }
}

void ApiCheckBase::validateDeclarativeProperty(const QString &name, const QString &typeName) const
{
    QMetaProperty property = metaProperty(name);

    QVERIFY2(property.isValid(),
             qPrintable(QString("property '%1.%2' does not exist").arg(m_name, name)));

    QVERIFY2(QString(property.typeName()).contains(typeName),
             qPrintable(QString("property '%1.%2' has invalid type (expected: %3, had: %4)")
                        .arg(m_name, name, typeName, property.typeName())));
}

void ApiCheckBase::validateEnumProperty(const QString &propertyName, const QVariant &value) const
{
    validateProperty(propertyName, QVariant::Int, value);
    QMetaProperty property = metaProperty(propertyName);
    QVERIFY2(property.isEnumType(),
             qPrintable(QString("property '%1.%2' is not enum type as expected").arg(m_name, propertyName)));
}

void ApiCheckBase::validateFlagProperty(const QString &propertyName, const QVariant &value) const
{
    validateEnumProperty(propertyName, value);
    QMetaProperty property = metaProperty(propertyName);
    QVERIFY2(property.isFlagType(),
             qPrintable(QString("property '%1.%2' is not flag type as expected").arg(m_name, propertyName)));
}

void ApiCheckBase::validateSignal(const char *signalName) const
{
    const QMetaObject *meta = m_object->metaObject();

    QVERIFY2(meta->indexOfSignal(signalName) != -1,
             qPrintable(QString("signal '%1.%2' not found").arg(m_name).arg(signalName)));
}

void ApiCheckBase::validateMethod(const char *methodName) const
{
    const QMetaObject *meta = m_object->metaObject();

    QVERIFY2(meta->indexOfMethod(methodName) != -1,
             qPrintable(QString("method '%1.%2' not found").arg(m_name).arg(methodName)));
}

QMetaProperty ApiCheckBase::metaProperty(const QString &name) const
{
    const QMetaObject *meta = m_object->metaObject();
    const int propertyIndex = meta->indexOfProperty(name.toLatin1().data());
    return meta->property(propertyIndex);
}
