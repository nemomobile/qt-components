/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <qdeclarativewindow.h>

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
}

void tst_quickcomponentspagestack::clear()
{
    QString retVal;
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "clearStack", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));
    retVal = componentObject->property("depth").toString();
    // Always one page is left to stack (first page pushed into stack was "dynamicpage")
    QCOMPARE(componentObject->property("currentPageName").toString(), QString("dynamicpage"));
    QVERIFY(retVal != QString("0"));
}


QTEST_MAIN(tst_quickcomponentspagestack)

#include "tst_quickcomponentspagestack.moc"
