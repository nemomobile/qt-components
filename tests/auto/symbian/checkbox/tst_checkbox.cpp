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
    QVERIFY(componentObject->property("platformInverted").isValid());
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

            if (property.name() == QString("platformInverted")) {
                //increase the property count
                propertyCount++;
                //check for default value
                QCOMPARE(property.read(componentObject).toBool(), false);
                //set the property and compare
                property.write(componentObject,true);
                QCOMPARE(property.read(componentObject).toBool(), true);
            }
        }
    }
    QCOMPARE(propertyCount, 3);
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
