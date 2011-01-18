/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "apicheckbase.h"
#include <QtDeclarative/qdeclarativeengine.h>
#include <QtDeclarative/qdeclarativecomponent.h>


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
    const QString &block = QString("import Qt 4.7\nimport %1\n%2 { %3 }")
        .arg(m_module).arg(name).arg(body);

    QDeclarativeComponent *component = new QDeclarativeComponent(m_engine);
    component->setData(block.toLatin1(), QUrl());

    m_name = name;
    m_object = component->create();

    QVERIFY2(m_object, qPrintable(QString("Could not create %1").arg(name)));
}

void ApiCheckBase::validateProperty(const QString &name, const QString &typeName) const
{
    const QMetaObject *meta = m_object->metaObject();
    const int propertyIndex = meta->indexOfProperty(name.toLatin1().data());

    QVERIFY2(propertyIndex != -1,
             qPrintable(QString("property '%1.%2' does not exist").arg(m_name, name)));

    const QMetaProperty &metaProperty = meta->property(propertyIndex);

    QVERIFY2(metaProperty.typeName() == typeName,
             qPrintable(QString("property '%1.%2' has invalid type (expected: %3)")
                        .arg(m_name, name, typeName)));
}

void ApiCheckBase::validateProperty(const QString &name, QVariant::Type type, const QVariant &value) const
{
    const QMetaObject *meta = m_object->metaObject();
    const int propertyIndex = meta->indexOfProperty(name.toLatin1().data());

    QVERIFY2(propertyIndex != -1,
             qPrintable(QString("property '%1.%2' does not exist").arg(m_name, name)));

    const QMetaProperty &metaProperty = meta->property(propertyIndex);

    QVERIFY2(metaProperty.type() == type,
             qPrintable(QString("property '%1.%2' has invalid type (expected: %3)")
                        .arg(m_name, name, QVariant::typeToName(type))));

    if (value.isValid()) {
        const QVariant &variant = metaProperty.read(m_object);

        QVERIFY2(variant == value,
                 qPrintable(QString("property '%1.%2' has wrong default value (expected: %3)")
                            .arg(m_name, name, value.toString())));
    }
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
