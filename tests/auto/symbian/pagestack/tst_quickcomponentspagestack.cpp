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

#include "tst_quickcomponentstest.h"
#include <QTest>
#include <QDeclarativeItem>

class tst_quickcomponentspagestack : public QObject
{
    Q_OBJECT

private slots:
    void cleanup();
    void initTestCase();
    void pushPage();
    void pushPageStatic();
    void pushPageArray();
    void replacePage();
    void popPage();
    void clearStack();
    void findPage();
    void busyChanged();
    void unwind();
    void properties();
private:
    QScopedPointer<QObject> componentObject;
};

void tst_quickcomponentspagestack::cleanup()
{
    QMetaObject::invokeMethod(componentObject.data(), "clearStack", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("0"));
}

void tst_quickcomponentspagestack::initTestCase()
{
    // Initialize test
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentspagestack.qml", &errors));
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentspagestack::pushPage()
{
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("0"));
    QMetaObject::invokeMethod(componentObject.data(), "pushPage", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("1"));
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("dynamicpage"));
}

void tst_quickcomponentspagestack::pushPageStatic()
{
    QMetaObject::invokeMethod(componentObject.data(), "pushPageStatic", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("1"));
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page1"));
}

void tst_quickcomponentspagestack::pushPageArray()
{
    QMetaObject::invokeMethod(componentObject.data(), "pushPageArray", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("3"));
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page3"));
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page2"));
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page1"));
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection);
}

void tst_quickcomponentspagestack::replacePage()
{
    QMetaObject::invokeMethod(componentObject.data(), "replacePage", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page3"));
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("1"));
}

void tst_quickcomponentspagestack::popPage()
{
    QMetaObject::invokeMethod(componentObject.data(), "pushPage", Qt::DirectConnection);
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("1"));

    // pop doesn't clear the stack. One page is always left to the stack.
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection);
    QVERIFY(componentObject.data()->property("depth").toString() != QString("0"));
}

void tst_quickcomponentspagestack::clearStack()
{
    // Stack depth should be 0 after clearing the stack
    QMetaObject::invokeMethod(componentObject.data(), "pushPage", Qt::DirectConnection);
    QMetaObject::invokeMethod(componentObject.data(), "clearStack", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("0"));
}

void tst_quickcomponentspagestack::findPage()
{
    QVariant retPage;
    // find page which exists in stack
    QMetaObject::invokeMethod(componentObject.data(), "pushPagesForFind", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject.data(), "findPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == true);

    // try to find page which doesn't exist
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject.data(), "findPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == false);
}

void tst_quickcomponentspagestack::busyChanged()
{
    // Reinitialize test
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentspagestack.qml", &errors));
    QVERIFY2(componentObject, qPrintable(errors));

    // Make initial push, busy shouldn't change
    QMetaObject::invokeMethod(componentObject.data(), "pushPage", Qt::DirectConnection);
    QVERIFY(componentObject.data()->property("busyChangedTriggered").toBool() == false);

    // Make second push, busy SHOULD change
    QMetaObject::invokeMethod(componentObject.data(), "pushPage", Qt::DirectConnection);
    QVERIFY(componentObject.data()->property("busyChangedTriggered").toBool() == true);
}

void tst_quickcomponentspagestack::unwind()
{
    QMetaObject::invokeMethod(componentObject.data(), "pushPageArray", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("3"));
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page3"));
    QMetaObject::invokeMethod(componentObject.data(), "unwind", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page1"));

    // Reinitialize test
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentspagestack.qml", &errors));
    QVERIFY2(componentObject, qPrintable(errors));

    QMetaObject::invokeMethod(componentObject.data(), "pushPageArray", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("depth").toString(), QString("3"));
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page3"));
    QMetaObject::invokeMethod(componentObject.data(), "unwindToBottom", Qt::DirectConnection);
    QCOMPARE(componentObject.data()->property("currentPageName").toString(), QString("page1"));
}

void tst_quickcomponentspagestack::properties()
{
    QVariant retPage;
    QMetaObject::invokeMethod(componentObject.data(), "pushPageWithProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject.data(), "checkForPageProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == true);
}

QTEST_MAIN(tst_quickcomponentspagestack)

#include "tst_quickcomponentspagestack.moc"
