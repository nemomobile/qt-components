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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <qdeclarativecontext.h>
#include <qdeclarativecomponent.h>
#include <qdeclarativeview.h>
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
