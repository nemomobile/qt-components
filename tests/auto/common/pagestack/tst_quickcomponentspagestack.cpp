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
#include <qdeclarativeitem.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentspagestack : public QObject

{
    Q_OBJECT

private slots:
    void initTestCase();
    void pushPage();
    void pushPageStatic();
    void pushPageArray();
    void replacePage();
    void popPage();
    void clear();

private:
    QObject *componentObject;
};

void tst_quickcomponentspagestack::initTestCase()
{
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
    QCOMPARE(retVal, QString("2"));
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("page1"));
}

void tst_quickcomponentspagestack::pushPageArray()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "pushPageArray", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QCOMPARE(retVal, QString("5"));
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
    QCOMPARE(retVal, QString("2"));
}

void tst_quickcomponentspagestack::popPage()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QVERIFY(retVal == QString("1"));

    QMetaObject::invokeMethod(componentObject, "popPage", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QVERIFY(retVal != QString("0"));

    // Always one page is left to stack (first page pushed into stack was "dynamicpage")
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("dynamicpage"));
}

void tst_quickcomponentspagestack::clear()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "clearStack", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    QVERIFY(retVal == QString("0"));
}


QTEST_MAIN(tst_quickcomponentspagestack)

#include "tst_quickcomponentspagestack.moc"
