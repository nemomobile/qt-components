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


#include <qtest.h>
#include "tst_quickcomponentstest.h"

class tst_quickcomponentslistitemtext : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void validateProperties();

private:
    QObject *m_componentObject;
};

void tst_quickcomponentslistitemtext::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentslistitemtext.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));
}

void tst_quickcomponentslistitemtext::validateProperties()
{
    // style - TODO: Property deprecated. Remove unit test when the property is removed.
    QVERIFY(m_componentObject->property("style").isValid());

    // mode
    QVariant mode = m_componentObject->property("mode");
    QVERIFY(mode.isValid());
    QVERIFY(mode.canConvert(QVariant::String));
    QCOMPARE(mode.toString(), QString("normal"));
    m_componentObject->setProperty("mode", "pressed");
    mode = m_componentObject->property("mode");
    QCOMPARE(mode.toString(), QString("pressed"));

    // role
    QVariant role = m_componentObject->property("role");
    QVERIFY(role.isValid());
    QVERIFY(role.canConvert(QVariant::String));
    QCOMPARE(role.toString(), QString("Title"));
    m_componentObject->setProperty("role", "SubTitle");
    role = m_componentObject->property("role");
    QCOMPARE(role.toString(), QString("SubTitle"));
    m_componentObject->setProperty("role", "Heading");
    role = m_componentObject->property("role");
    QCOMPARE(role.toString(), QString("Heading"));

    // text
    QVariant text = m_componentObject->property("text");
    QVERIFY(text.isValid());
    QVERIFY(text.canConvert(QVariant::String));
    QVERIFY(text.toString().isEmpty());
    m_componentObject->setProperty("text", "My text");
    text = m_componentObject->property("text");
    QCOMPARE(text.toString(), QString("My text"));
}

QTEST_MAIN(tst_quickcomponentslistitemtext)
#include "tst_quickcomponentslistitemtext.moc"
