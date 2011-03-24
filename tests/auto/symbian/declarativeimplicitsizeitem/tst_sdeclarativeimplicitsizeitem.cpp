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
    QString text("import Qt 4.7\n"
        "import com.nokia.symbian 1.0\n"
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

    QString text("import Qt 4.7\n"
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
