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
#include <QtDeclarative/qdeclarativeitem.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentstoolbar : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void setToolsA();
    void setToolsB();
    void setToolsToNull();
    void toolbarlayout();

private:
    static bool isVisible(QObject*);
    QObject *componentObject;
};

void tst_quickcomponentstoolbar::initTestCase()
{
    QString errors;

    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstoolbar.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentstoolbar::setToolsA()
{
    QVariant retPage;

    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("undefined"));
    QMetaObject::invokeMethod(componentObject, "setToolsA", Q_RETURN_ARG(QVariant, retPage));

    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("toolsA"));
    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsA")));
    QVERIFY(retPage.isValid());

    QObject *layout = qvariant_cast<QObject*>(retPage);
    QCOMPARE(layout->objectName(), QString("toolsA"));
    QVERIFY(isVisible(layout));

    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsB")));
    QVERIFY(retPage.isValid());

    layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(!isVisible(layout));
}

void tst_quickcomponentstoolbar::setToolsB()
{
    QVariant retPage;
    QMetaObject::invokeMethod(componentObject, "setToolsB", Q_RETURN_ARG(QVariant, retPage));
    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("toolsB"));
    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsA")));

    QVERIFY(retPage.isValid());
    QObject *layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(!isVisible(layout ));

    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsB")));
    QVERIFY(retPage.isValid());
    layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(isVisible(layout));
}

void tst_quickcomponentstoolbar::setToolsToNull()
{
    QVariant toolsNull;
    QMetaObject::invokeMethod(componentObject, "setToolsToNull", Q_RETURN_ARG(QVariant, toolsNull));
    QVERIFY(toolsNull.toBool());
    QCOMPARE(componentObject->property("currentToolsName").toString(),QString("undefined"));
}

void tst_quickcomponentstoolbar::toolbarlayout()
{
    QVariant retPage;
    QMetaObject::invokeMethod(componentObject, "setToolsB");
    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsB")));

    QVERIFY(retPage.isValid());
    QObject *layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(isVisible(layout ));

    layout->setProperty("backButton", QVariant(false));
    QVERIFY(!layout->property("backButton").toBool());

    layout->setProperty("backButton", QVariant(true));
    QVERIFY(layout->property("backButton").toBool());
}

bool tst_quickcomponentstoolbar::isVisible(QObject* obj)
{
    if (!obj)
        return false;
    int idx = obj->metaObject()->indexOfProperty("visible");
    if (idx < 0)
        return false;
    QMetaProperty prop = obj->metaObject()->property(idx);
    return prop.read(obj).toBool();
}

QTEST_MAIN(tst_quickcomponentstoolbar)

#include "tst_quickcomponentstoolbar.moc"
