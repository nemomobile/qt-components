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
    void pushAndPopTwoPages();
    void clearStack();
    void findPage();
    void busyChanged();
    void unwind();
    void properties();
    void initialPage();
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

void tst_quickcomponentspagestack::pushAndPopTwoPages()
{
    QMetaObject::invokeMethod(componentObject.data(), "pushPage", Qt::DirectConnection);
    QMetaObject::invokeMethod(componentObject.data(), "pushPage", Qt::DirectConnection);
    QTRY_COMPARE(componentObject.data()->property("busy").toBool(), false);
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection);
    QMetaObject::invokeMethod(componentObject.data(), "popPage", Qt::DirectConnection);
    QTRY_COMPARE(componentObject.data()->property("busy").toBool(), false);
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

    QMetaObject::invokeMethod(componentObject.data(), "pushPageStaticWithProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject.data(), "checkForPageStaticProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == true);
}

void tst_quickcomponentspagestack::initialPage()
{
    QDeclarativeItem *defaultStack = componentObject->findChild<QDeclarativeItem *>("pageStack");
    QVERIFY(defaultStack);
    QVERIFY(!defaultStack->property("initialPage").isValid());
    QCOMPARE(defaultStack->property("depth").toInt(), 0);

    QDeclarativeItem *pageStackInitComponent = componentObject->findChild<QDeclarativeItem *>("pageStackInitComponent");
    QVERIFY(pageStackInitComponent);
    QVERIFY(pageStackInitComponent->property("initialPage").isValid());
    QCOMPARE(pageStackInitComponent->property("depth").toInt(), 1);
    QCOMPARE(pageStackInitComponent->property("currentPage").value<QDeclarativeItem *>()->objectName(), QLatin1String("dynamicpage"));

    QDeclarativeItem *pageStackInitStatic = componentObject->findChild<QDeclarativeItem *>("pageStackInitStatic");
    QVERIFY(pageStackInitStatic);
    QVERIFY(pageStackInitStatic->property("initialPage").isValid());
    QCOMPARE(pageStackInitStatic->property("depth").toInt(), 1);
    QCOMPARE(pageStackInitStatic->property("currentPage").value<QDeclarativeItem *>()->objectName(), QLatin1String("page1"));
}

QTEST_MAIN(tst_quickcomponentspagestack)

#include "tst_quickcomponentspagestack.moc"
