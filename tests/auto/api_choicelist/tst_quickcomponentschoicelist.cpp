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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtDeclarative/qdeclarativeview.h>
#include <QDeclarativeItem>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentschoicelist : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void currentIndex();
    void model();

private:
    QObject *componentObject;
};

void tst_quickcomponentschoicelist::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentschoicelist.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentschoicelist::currentIndex()
{
    // make sure property is present and set initial value
    QVERIFY(componentObject->setProperty("currentIndex", 1));

    // test property has right initial value
    QCOMPARE(componentObject->property("currentIndex").toInt(), 1);

    // change value and test again
    QVERIFY(componentObject->setProperty("currentIndex", 2));
    QCOMPARE(componentObject->property("currentIndex").toInt(), 2);
}

// This test can fail, the common API has not defined the final type for the model property
// For now we will test this as a QVariant
void tst_quickcomponentschoicelist::model()
{
    QObject *modelObject_1 = new QObject();
    modelObject_1->setObjectName("model_1");

    QVariant modelVariant = qVariantFromValue(modelObject_1);
    // make sure property is present and set initial value
    QVERIFY(componentObject->setProperty("model", modelVariant));

    // get property
    QVariant tmpVariant = componentObject->property("model");
    QObject *tmpObject = tmpVariant.value<QObject*>();

    // check if it is the one that was previously set
    QVERIFY2(modelObject_1->objectName() == tmpObject->objectName(), "Error, the returned object is not the one thar was previously set");

    QObject *modelObject_2 = new QObject();
    modelObject_2->setObjectName("model_2");

    modelVariant = qVariantFromValue(modelObject_2);
    // set property with a new value
    QVERIFY(componentObject->setProperty("model", modelVariant));

    tmpVariant = componentObject->property("model");
    tmpObject = tmpVariant.value<QObject*>();

    // check it
    QVERIFY2(modelObject_2->objectName() == tmpObject->objectName(), "Error, the returned object is not the one thar was previously set");

    delete modelObject_1;
    delete modelObject_2;
}

QTEST_MAIN(tst_quickcomponentschoicelist)

#include "tst_quickcomponentschoicelist.moc"
