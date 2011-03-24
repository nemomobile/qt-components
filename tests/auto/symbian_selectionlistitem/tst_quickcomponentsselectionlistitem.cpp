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

class tst_quickcomponentsselectionlistitem : public QObject
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

void tst_quickcomponentsselectionlistitem::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsselectionlistitem.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));
}

void tst_quickcomponentsselectionlistitem::validateProperties()
{
    // style - from ListItem
    // TODO: Property deprecated. Remove unit test when the property is removed.
    QVERIFY(m_componentObject->property("style").isValid());

    // mode - from ListItem
    QVariant mode = m_componentObject->property("mode");
    QVERIFY(mode.isValid());
    QVERIFY(mode.canConvert(QVariant::String));
    QCOMPARE(mode.toString(), QString("normal"));

    // padding - from ListItem
    // TODO: Property deprecated. Remove unit test when the property is removed.
    QVERIFY(m_componentObject->property("padding").isValid());

    // paddingItem
    QVERIFY(m_componentObject->property("paddingItem").isValid());

    // verticalSpacing - from ListItem
    // TODO: Property deprecated. Remove unit test when the property is removed.
    QVariant verticalSpacing = m_componentObject->property("verticalSpacing");
    QVERIFY(verticalSpacing.isValid());
    QVERIFY(verticalSpacing.canConvert(QVariant::Double));
    QVERIFY(verticalSpacing.toReal() >= 0);

    // horizontalSpacing - from ListItem
    // TODO: Property deprecated. Remove unit test when the property is removed.
    QVariant horizontalSpacing = m_componentObject->property("horizontalSpacing");
    QVERIFY(horizontalSpacing.isValid());
    QVERIFY(horizontalSpacing.canConvert(QVariant::Double));
    QVERIFY(horizontalSpacing.toReal() >= 0);

    // enabled - from ListItem
    QVariant enabled = m_componentObject->property("enabled");
    QVERIFY(enabled.isValid());
    QVERIFY(enabled.canConvert(QVariant::Bool));
    QCOMPARE(enabled.toBool(), true);
    m_componentObject->setProperty("enabled", false);
    enabled = m_componentObject->property("enabled");
    QCOMPARE(enabled.toBool(), false);

    // title
    QVariant title = m_componentObject->property("title");
    QVERIFY(title.isValid());
    QVERIFY(title.canConvert(QVariant::String));
    QVERIFY(title.toString().isEmpty());
    m_componentObject->setProperty("title", "MyTitle");
    title = m_componentObject->property("title");
    QCOMPARE(title.toString(), QString("MyTitle"));

    // subTitle
    QVariant subTitle = m_componentObject->property("subTitle");
    QVERIFY(subTitle.isValid());
    QVERIFY(subTitle.canConvert(QVariant::String));
    QVERIFY(subTitle.toString().isEmpty());
    m_componentObject->setProperty("subTitle", "MySubTitle");
    subTitle = m_componentObject->property("subTitle");
    QCOMPARE(subTitle.toString(), QString("MySubTitle"));
}

void tst_quickcomponentsselectionlistitem::testClickedSignal()
{
    QSignalSpy spy(m_componentObject, SIGNAL(clicked()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(m_componentObject, "clicked", Qt::DirectConnection);
    QCOMPARE(spy.count(), 1);
}

void tst_quickcomponentsselectionlistitem::testPressAndHoldSignal()
{
    QSignalSpy spy(m_componentObject, SIGNAL(pressAndHold()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(m_componentObject, "pressAndHold", Qt::DirectConnection);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(tst_quickcomponentsselectionlistitem)

#include "tst_quickcomponentsselectionlistitem.moc"
