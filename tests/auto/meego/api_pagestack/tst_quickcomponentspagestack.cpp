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
#include <QDeclarativeEngine>
#include <qdeclarativecontext.h>
#include <qdeclarativecomponent.h>
#include <qdeclarativeview.h>
#include <qdeclarativeitem.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentspagestack : public QObject
{
    Q_OBJECT

private slots:
    void init();
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
    QObject *componentObject;
};

void tst_quickcomponentspagestack::init()
{
    // Initialize test
    QString errors; 
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentspagestack.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));             
}

void tst_quickcomponentspagestack::pushPage()
{
    QString retVal;
    QVariant retPage;

    QCOMPARE(componentObject->property("currentPageName").toString(), QString("0"));
    QMetaObject::invokeMethod(componentObject, "pushPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));

    retVal = componentObject->property("depth").toString();
    QCOMPARE(retVal, QString("1"));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("dynamicpage"));
}

void tst_quickcomponentspagestack::pushPageStatic()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "pushPageStatic", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QCOMPARE(retVal, QString("1"));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page1"));
}

void tst_quickcomponentspagestack::pushPageArray()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "pushPageArray", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QCOMPARE(retVal, QString("3"));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page3"));
    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page2"));
    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page1"));
    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
}

void tst_quickcomponentspagestack::replacePage()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "replacePage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page3"));
    QCOMPARE(retVal, QString("1"));
}

void tst_quickcomponentspagestack::popPage()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "pushPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QVERIFY(retVal == QString("1"));

    // pop doesn't clear the stack. One page is always left to the stack.
    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QVERIFY(retVal != QString("0"));
}

void tst_quickcomponentspagestack::clearStack()
{
    QString retVal;
    QVariant retPage;

    // Stack depth should be 0 after clearing the stack
    QMetaObject::invokeMethod(componentObject, "pushPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject, "clearStack", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QVERIFY(retVal == QString("0"));
}

void tst_quickcomponentspagestack::findPage()
{
    QVariant retPage;

    // find page which exists in stack
    QMetaObject::invokeMethod(componentObject, "pushPagesForFind", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject, "findPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == true);

    // try to find page which doesn't exist
    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject, "findPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == false);
}

void tst_quickcomponentspagestack::busyChanged()
{
    QVariant retPage;

    // Make initial push, busy shouldn't change
    QMetaObject::invokeMethod(componentObject, "pushPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));

    QVERIFY(componentObject->property("busyChangedTriggered").toBool()==false);

    // Make second push, busy SHOULD change
    QMetaObject::invokeMethod(componentObject, "pushPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(componentObject->property("busyChangedTriggered").toBool()==true);
}

void tst_quickcomponentspagestack::unwind()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "pushPageArray", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QCOMPARE(retVal, QString("3"));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page3"));
    QMetaObject::invokeMethod(componentObject, "unwind", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page1"));

    // Reinitialize test
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentspagestack.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));

    QMetaObject::invokeMethod(componentObject, "pushPageArray", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QCOMPARE(retVal, QString("3"));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page3"));
    QMetaObject::invokeMethod(componentObject, "unwindToBottom", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page1"));
}

void tst_quickcomponentspagestack::properties()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "pushPageWithProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject, "checkForPageProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == true);

    QMetaObject::invokeMethod(componentObject, "pushPageStaticWithProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QMetaObject::invokeMethod(componentObject, "checkForPageStaticProperties", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    QVERIFY(retPage.toBool() == true);
}

QTEST_MAIN(tst_quickcomponentspagestack)

#include "tst_quickcomponentspagestack.moc"
