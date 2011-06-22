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

#include "sdeclarativeimplicitsizeitem.h"
#include "tst_quickcomponentstest.h"
#include <QtTest/QtTest>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>

class TestImplicitSizeItem : public SDeclarativeImplicitSizeItem
{
    Q_OBJECT
public:
    TestImplicitSizeItem(QDeclarativeItem *parent = 0)
        : SDeclarativeImplicitSizeItem(parent)
    {
    }

    Q_INVOKABLE void multiplyImplicitWidth(qreal a, qreal b)
    {
        setImplicitWidthNotify(a * b);
    }

};

class tst_SDeclarativeImplicitSizeItem : public QObject
{
    Q_OBJECT

public slots:
    void widthChanged() {
        timesWidthChanged++;
    }
    void heightChanged() {
        timesHeightChanged++;
    }

private slots:
    void testDirectQmlUsage();
    void testCodeUsage();
    void testCodeQMLUsage();

private:
    int timesWidthChanged;
    int timesHeightChanged;
};



void tst_SDeclarativeImplicitSizeItem::testDirectQmlUsage()
{
    QString text("import QtQuick 1.1\n"
        "import com.nokia.symbian 1.1\n"
        "ImplicitSizeItem {\n"
        "}\n");

    QString errors;
    QObject *rootObject = tst_quickcomponentstest::createComponentFromString(text, &errors);
    QVERIFY(errors.isEmpty());

    QVERIFY(rootObject->property("implicitWidth").isValid());
    QVERIFY(rootObject->property("implicitHeight").isValid());

    connect(rootObject, SIGNAL(implicitWidthChanged()), this, SLOT(widthChanged()));
    connect(rootObject, SIGNAL(implicitHeightChanged()), this, SLOT(heightChanged()));

    const qreal newWidthValue = 23.6;
    int oldTimes = timesWidthChanged;
    rootObject->setProperty("implicitWidth", QVariant(newWidthValue));
    QCOMPARE(timesWidthChanged,  oldTimes + 1);
    QCOMPARE(rootObject->property("implicitWidth").value<qreal>(),  newWidthValue);

    const qreal newHeightValue = 234345;
    oldTimes = timesHeightChanged;
    rootObject->setProperty("implicitHeight", QVariant(newHeightValue));
    QCOMPARE(timesHeightChanged,  oldTimes + 1);
    QCOMPARE(rootObject->property("implicitHeight").value<qreal>(),  newHeightValue);
    delete rootObject;
}

void tst_SDeclarativeImplicitSizeItem::testCodeUsage()
{
    TestImplicitSizeItem *element = new TestImplicitSizeItem;

    connect(element, SIGNAL(implicitWidthChanged()), this, SLOT(widthChanged()));
    const qreal a = 345.623;
    const qreal b = 43590;
    const qreal newWidthValue = a * b;
    int oldTimes = timesWidthChanged;
    element->multiplyImplicitWidth(a, b);
    QCOMPARE(timesWidthChanged,  oldTimes + 1);
    QCOMPARE(element->implicitWidth(), newWidthValue);

    delete element;
}

void tst_SDeclarativeImplicitSizeItem::testCodeQMLUsage()
{
    qmlRegisterType<TestImplicitSizeItem>("myexport", 1, 0, "ImplicitSizeItem");

    QString text("import QtQuick 1.1\n"
                 "import myexport 1.0\n"
                 "ImplicitSizeItem {\n"
                 "}\n");

    QString errors;
    QObject *rootObject = tst_quickcomponentstest::createComponentFromString(text, &errors);
    QVERIFY(errors.isEmpty());

    connect(rootObject, SIGNAL(implicitWidthChanged()), this, SLOT(widthChanged()));
    const qreal a = 345.623;
    const qreal b = 43590;
    const qreal newWidthValue = a * b;

    int oldTimes = timesWidthChanged;
    QMetaObject::invokeMethod(rootObject, "multiplyImplicitWidth", Q_ARG(qreal, a), Q_ARG(qreal, b));
    QCOMPARE(timesWidthChanged,  oldTimes + 1);
    QCOMPARE(rootObject->property("implicitWidth").value<qreal>(),  newWidthValue);
    delete rootObject;
}

QTEST_MAIN(tst_SDeclarativeImplicitSizeItem)

#include "tst_sdeclarativeimplicitsizeitem.moc"
