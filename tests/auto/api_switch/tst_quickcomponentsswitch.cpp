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

#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsswitch : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void checked();
    void pressed();
    void clicked();

private:
    QObject *componentObject;
};

void tst_quickcomponentsswitch::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsswitch.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsswitch::checked()
{
    QVERIFY(componentObject->setProperty("checked",false));
    QCOMPARE(componentObject->property("checked").toBool(),false);
    QVERIFY(componentObject->setProperty("checked",true));
    QCOMPARE(componentObject->property("checked").toBool(),true);
}

void tst_quickcomponentsswitch::pressed()
{
    QVERIFY(componentObject->setProperty("pressed",false));
    QCOMPARE(componentObject->property("pressed").toBool(),false);
    QVERIFY(componentObject->setProperty("pressed",true));
    QCOMPARE(componentObject->property("pressed").toBool(),true);
}

void tst_quickcomponentsswitch::clicked()
{
    QSignalSpy spy(componentObject, SIGNAL(clicked()));
    QVERIFY(spy.isValid());
    QMetaObject::invokeMethod(componentObject,"clicked",Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
}

QTEST_MAIN(tst_quickcomponentsswitch)

#include "tst_quickcomponentsswitch.moc"
