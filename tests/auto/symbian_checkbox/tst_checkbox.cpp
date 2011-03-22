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

#include <QTest>
#include <QDeclarativeItem>
#include <QDeclarativeComponent>
#include <QDebug>
#include "tst_quickcomponentstest.h"

class tst_checkbox : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testValid();
    void testProperties();
    void testImplicitSize();

private:
    QObject *componentObject;
};


void tst_checkbox::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_checkbox.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_checkbox::cleanupTestCase()
{
}

void tst_checkbox::init()
{
}

void tst_checkbox::cleanup()
{
}

void tst_checkbox::testValid()
{
    QVERIFY(componentObject->property("checked").isValid());
    QVERIFY(componentObject->property("pressed").isValid());
    QVERIFY(componentObject->property("text").isValid());
}

void tst_checkbox::testProperties()
{
    const QMetaObject *metaInfo = componentObject->metaObject();
    int metaPropertyCount = 0;
    int propertyCount = 0;

    if (metaInfo) {
        metaPropertyCount = metaInfo->propertyCount();
        for (int i = 0; i < metaPropertyCount; i++) {
            QMetaProperty property = metaInfo->property(i);

            if (property.name() == QString("checked")) {
                //increase the property count
                propertyCount++;
                //check for default value
                QCOMPARE(property.read(componentObject).toBool(), false);
                //set the property and compare
                property.write(componentObject,true);
                QCOMPARE(property.read(componentObject).toBool(), true);
            }

            if (property.name() == QString("text")) {
                //increase the property count
                propertyCount++;
                //check for default value
                QVERIFY(property.read(componentObject).toString().isNull());
                //set the property and compare
                property.write(componentObject,"checkbox_text");
                QCOMPARE(property.read(componentObject).toString() , QString("checkbox_text"));
            }
        }
    }
    QCOMPARE(propertyCount, 2);
}

void tst_checkbox::testImplicitSize()
{
    QDeclarativeItem *checkBox = qobject_cast<QDeclarativeItem*>(componentObject);
    QVERIFY(checkBox);

    // no text
    checkBox->setProperty("text", "");
    const qreal noTextImplicitWidth = checkBox->implicitWidth();
    const qreal noTextImplicitHeight = checkBox->implicitHeight();
    QVERIFY(noTextImplicitWidth > 5); // needs to be something for the tool
    QVERIFY(noTextImplicitHeight > 5); // needs to be something for the tool

    // short text
    checkBox->setProperty("text", "short");
    const qreal shortTextImplicitWidth = checkBox->implicitWidth();
    const qreal shortTextImplicitHeight = checkBox->implicitHeight();
    QVERIFY(shortTextImplicitWidth > noTextImplicitWidth);
    QVERIFY(shortTextImplicitHeight >= noTextImplicitHeight);

    // long text
    checkBox->setProperty("text", "longer text than just 'short'");
    const qreal longTextImplicitWidth = checkBox->implicitWidth();
    const qreal longTextImplicitHeight = checkBox->implicitHeight();
    QVERIFY(longTextImplicitWidth > shortTextImplicitWidth);
    QVERIFY(longTextImplicitHeight >= shortTextImplicitHeight);

    // long text
    checkBox->setProperty("text", "longer text than just 'short'");
    checkBox->setProperty("checked", false);
    QCOMPARE(longTextImplicitWidth, checkBox->implicitWidth());
    QCOMPARE(longTextImplicitHeight, checkBox->implicitHeight());
    checkBox->setProperty("checked", true);
    QCOMPARE(longTextImplicitWidth, checkBox->implicitWidth());
    QCOMPARE(longTextImplicitHeight, checkBox->implicitHeight());
}

QTEST_MAIN(tst_checkbox)
#include "tst_checkbox.moc"
