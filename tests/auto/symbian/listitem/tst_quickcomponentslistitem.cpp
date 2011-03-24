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

class tst_quickcomponentslistitem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void validateProperties();
    void testClickedSignal();
    void testPressAndHoldSignal();

private:
    QObject *m_componentObject;
};

void tst_quickcomponentslistitem::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentslistitem.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));
}

void tst_quickcomponentslistitem::validateProperties()
{
    // style - TODO: Property deprecated. Remove unit test when the property is removed.
    QVERIFY(m_componentObject->property("style").isValid());

    // mode
    QVariant mode = m_componentObject->property("mode");
    QVERIFY(mode.isValid());
    QVERIFY(mode.canConvert(QVariant::String));
    QCOMPARE(mode.toString(), QString("normal"));

    // padding - TODO: Property deprecated. Remove unit test when the property is removed.
    QVERIFY(m_componentObject->property("padding").isValid());

    // paddingItem
    QVERIFY(m_componentObject->property("paddingItem").isValid());

    // verticalSpacing- TODO: Property deprecated. Remove unit test when the property is removed.
    QVariant verticalSpacing = m_componentObject->property("verticalSpacing");
    QVERIFY(verticalSpacing.isValid());
    QVERIFY(verticalSpacing.canConvert(QVariant::Double));
    QVERIFY(verticalSpacing.toReal() >= 0);

    // horizontalSpacing- TODO: Property deprecated. Remove unit test when the property is removed.
    QVariant horizontalSpacing = m_componentObject->property("horizontalSpacing");
    QVERIFY(horizontalSpacing.isValid());
    QVERIFY(horizontalSpacing.canConvert(QVariant::Double));
    QVERIFY(horizontalSpacing.toReal() >= 0);

    // enabled
    QVariant enabled = m_componentObject->property("enabled");
    QVERIFY(enabled.isValid());
    QVERIFY(enabled.canConvert(QVariant::Bool));
    QCOMPARE(enabled.toBool(), true);
    m_componentObject->setProperty("enabled", false);
    enabled = m_componentObject->property("enabled");
    QCOMPARE(enabled.toBool(), false);

    // subItemIndicator
    QVariant indicator = m_componentObject->property("subItemIndicator");
    QVERIFY(indicator.isValid());
    QVERIFY(indicator.canConvert(QVariant::Bool));
    QCOMPARE(indicator.toBool(), false);
    m_componentObject->setProperty("subItemIndicator", true);
    indicator = m_componentObject->property("subItemIndicator");
    QCOMPARE(indicator.toBool(), true);
}

void tst_quickcomponentslistitem::testClickedSignal()
{
    QSignalSpy spy(m_componentObject, SIGNAL(clicked()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(m_componentObject, "clicked", Qt::DirectConnection);
    QCOMPARE(spy.count(), 1);
}

void tst_quickcomponentslistitem::testPressAndHoldSignal()
{
    QSignalSpy spy(m_componentObject, SIGNAL(pressAndHold()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(m_componentObject, "pressAndHold", Qt::DirectConnection);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(tst_quickcomponentslistitem)

#include "tst_quickcomponentslistitem.moc"
